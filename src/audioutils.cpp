#include "audioutils.h"
#include "midisynth.h"
#include <QFile>
#include <QDir>
#include <QTemporaryFile>
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include "AudioFileTools.h"
#include "AudioFileInfo.h"
#include "FLACFileInfo.h"

static void writeWavHeader(QFile& file, quint32 sampleRate, quint32 numSamples)
{
    quint32 byteRate = sampleRate * 4;
    quint32 dataSize = numSamples * 4;
    quint32 fileSize = 36 + dataSize;

    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&fileSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    quint32 fmtSize = 16;
    file.write(reinterpret_cast<const char*>(&fmtSize), 4);
    quint16 audioFormat = 1;
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    quint16 numChannels = 2;
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    quint16 blockAlign = 4;
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    quint16 bitsPerSample = 16;
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);
}

bool AudioUtils::mixTracksToWav(const QVector<TrackData>& tracks,
                                const QString& outputPath,
                                int sampleRate,
                                const QString& soundFontPath)
{
    double maxLength = 0;
    for (const auto& t : tracks)
    {
        if (!t.enabled) continue;
        double len = t.length();
        if (len > maxLength) maxLength = len;
    }
    if (maxLength <= 0) maxLength = 1.0;

    const int totalFrames = static_cast<int>(maxLength * sampleRate);
    const int totalSamples = totalFrames * 2;
    QVector<float> mixLeft(totalFrames, 0.0f);
    QVector<float> mixRight(totalFrames, 0.0f);

    for (const auto& track : tracks)
    {
        if (!track.enabled) continue;

        if (track.type == TrackType::Audio)
        {
            int trackFrames = track.audioData.size() / (track.channelCount * 2);
            const qint16* src = reinterpret_cast<const qint16*>(track.audioData.constData());
            float scale = track.sampleRate == sampleRate ? 1.0f : 1.0f;
            for (int i = 0; i < qMin(trackFrames, totalFrames); ++i)
            {
                float l = src[i * track.channelCount] / 32768.0f;
                float r = track.channelCount > 1 ? src[i * track.channelCount + 1] / 32768.0f : l;
                mixLeft[i] += l * scale;
                mixRight[i] += r * scale;
            }
        }
        else if (track.type == TrackType::MIDI && !track.midiNotes.isEmpty())
        {
            QTemporaryFile tmpWav(QDir::temp().filePath("mc_mix_XXXXXX.wav"));
            tmpWav.setAutoRemove(true);
            if (!tmpWav.open()) continue;
            const QString tmpPath = tmpWav.fileName();
            tmpWav.close();

            MidiSynth synth;
            if (synth.renderMidiToWav(track.midiNotes, track.lengthSeconds, tmpPath, sampleRate, soundFontPath))
            {
                if (QFile f(tmpPath); f.open(QIODevice::ReadOnly))
                {
                    f.seek(44);
                    const QByteArray data = f.readAll();
                    const qint16* src = reinterpret_cast<const qint16*>(data.constData());
                    const int frames = data.size() / 4;
                    for (int i = 0; i < qMin(frames, totalFrames); ++i)
                    {
                        mixLeft[i] += src[i * 2] / 32768.0f;
                        mixRight[i] += src[i * 2 + 1] / 32768.0f;
                    }
                }
            }
        }
    }

    float maxVal = 0;
    for (int i = 0; i < totalFrames; ++i)
    {
        float m = qMax(qAbs(mixLeft[i]), qAbs(mixRight[i]));
        if (m > maxVal) maxVal = m;
    }
    float gain = (maxVal > 0.9f) ? (0.9f / maxVal) : 1.0f;

    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    writeWavHeader(file, sampleRate, totalSamples / 2);
    for (int i = 0; i < totalFrames; ++i)
    {
        constexpr float scale = 32767.0f;
        constexpr float minVal = -32768.0f;
        qint16 l = static_cast<qint16>(std::clamp(mixLeft[i] * gain * scale, minVal, scale));
        qint16 r = static_cast<qint16>(std::clamp(mixRight[i] * gain * scale, minVal, scale));
        file.write(reinterpret_cast<const char*>(&l), 2);
        file.write(reinterpret_cast<const char*>(&r), 2);
    }
    file.close();
    return true;
}

// Writes a raw audio track's PCM data to a temporary WAV file.
// Returns the path, or empty string on failure.  Caller must delete the file.
static QString writeAudioTrackToTempWav(const TrackData& track)
{
    QTemporaryFile tmp(QDir::temp().filePath("mc_audio_XXXXXX.wav"));
    tmp.setAutoRemove(false);
    if (!tmp.open()) return {};
    const QString path = tmp.fileName();

    const quint32 sr = static_cast<quint32>(track.sampleRate > 0 ? track.sampleRate : 44100);
    const int frames = track.audioData.size() / (track.channelCount * 2);
    writeWavHeader(tmp, sr, static_cast<quint32>(frames));
    // Write audio data; if mono, duplicate to stereo
    if (track.channelCount == 2)
    {
        tmp.write(track.audioData);
    }
    else
    {
        const qint16* src = reinterpret_cast<const qint16*>(track.audioData.constData());
        for (int i = 0; i < frames; ++i)
        {
            tmp.write(reinterpret_cast<const char*>(&src[i]), 2);
            tmp.write(reinterpret_cast<const char*>(&src[i]), 2);
        }
    }
    tmp.close();
    return path;
}


bool AudioUtils::writeAudioDataToFlac(const QByteArray& int16Data,
                                       int sampleRate,
                                       int channelCount,
                                       const QString& flacPath)
{
    // Write int16Data to a temporary WAV file
    QTemporaryFile tmpWav(QDir::temp().filePath("mc_rec_XXXXXX.wav"));
    tmpWav.setAutoRemove(false);
    if (!tmpWav.open())
        return false;
    const QString tmpWavPath = tmpWav.fileName();

    const quint32 sr  = static_cast<quint32>(sampleRate > 0 ? sampleRate : 44100);
    const quint16 ch  = static_cast<quint16>(channelCount > 0 ? channelCount : 2);
    const quint16 bps = 16;
    const quint16 blk = ch * (bps / 8);
    const quint32 byteRate = sr * blk;
    const quint32 dataSize = static_cast<quint32>(int16Data.size());
    const quint32 fileSize = 36 + dataSize;
    const quint16 wavFmt   = 1;  // PCM
    quint32 fmtSize = 16;

    tmpWav.write("RIFF", 4);
    tmpWav.write(reinterpret_cast<const char*>(&fileSize), 4);
    tmpWav.write("WAVE", 4);
    tmpWav.write("fmt ", 4);
    tmpWav.write(reinterpret_cast<const char*>(&fmtSize),   4);
    tmpWav.write(reinterpret_cast<const char*>(&wavFmt),    2);
    tmpWav.write(reinterpret_cast<const char*>(&ch),        2);
    tmpWav.write(reinterpret_cast<const char*>(&sr),        4);
    tmpWav.write(reinterpret_cast<const char*>(&byteRate),  4);
    tmpWav.write(reinterpret_cast<const char*>(&blk),       2);
    tmpWav.write(reinterpret_cast<const char*>(&bps),       2);
    tmpWav.write("data", 4);
    tmpWav.write(reinterpret_cast<const char*>(&dataSize),  4);
    tmpWav.write(int16Data);
    tmpWav.close();

    // Open the temp WAV via EOUtils and encode to FLAC
    auto wavFile = EOUtils::createAudioFileObjForExistingFile(tmpWavPath.toStdString().c_str());
    if (!wavFile)
    {
        QFile::remove(tmpWavPath);
        return false;
    }

    if (!wavFile->open(EOUtils::AUDIO_FILE_READ))
    {
        QFile::remove(tmpWavPath);
        return false;
    }

    const EOUtils::AudioFileInfo wavInfo = wavFile->getAudioFileInfo();

    auto flacFile = EOUtils::createAudioFileObjForNewFile(flacPath.toStdString().c_str());
    if (!flacFile)
    {
        wavFile->close();
        QFile::remove(tmpWavPath);
        return false;
    }

    flacFile->setAudioFileInfo(wavInfo);

    if (!flacFile->open(EOUtils::AUDIO_FILE_WRITE))
    {
        wavFile->close();
        QFile::remove(tmpWavPath);
        return false;
    }

    const size_t numSamples = wavFile->numSamples();
    for (size_t i = 0; i < numSamples; ++i)
    {
        int64_t sample = 0;
        if (!wavFile->getNextSample_int64(sample))
            break;
        flacFile->writeSample_int64(sample);
    }

    wavFile->close();
    flacFile->close();
    QFile::remove(tmpWavPath);
    return true;
}

bool AudioUtils::readFlacAudioData(const QString& path,
                                    QByteArray& audioData,
                                    int& sampleRate,
                                    int& channelCount)
{
    auto flacFile = EOUtils::createAudioFileObjForExistingFile(path.toStdString().c_str());
    if (!flacFile)
        return false;

    if (!flacFile->open(EOUtils::AUDIO_FILE_READ))
        return false;

    const EOUtils::AudioFileInfo info = flacFile->getAudioFileInfo();
    sampleRate   = info.SampleRateHz();
    channelCount = info.NumChannels();

    const int16_t bitsPerSample = info.BitsPerSample();
    const size_t numSamples = flacFile->numSamples();
    audioData.resize(static_cast<int>(numSamples * sizeof(qint16)));
    qint16* dst = reinterpret_cast<qint16*>(audioData.data());

    for (size_t i = 0; i < numSamples; ++i)
    {
        int64_t sample = 0;
        if (!flacFile->getNextSample_int64(sample))
            break;
        // Scale to 16-bit if the source has a different bit depth
        if (bitsPerSample > 16)
            sample >>= (bitsPerSample - 16);
        else if (bitsPerSample > 0 && bitsPerSample < 16)
            sample <<= (16 - bitsPerSample);
        dst[i] = static_cast<qint16>(
            std::max(static_cast<int64_t>(-32768),
                     std::min(static_cast<int64_t>(32767), sample)));
    }

    flacFile->close();
    return true;
}

bool AudioUtils::mixTracksToFile(const QVector<TrackData>& tracks,
                                  const QString& outputPath,
                                  const QString& projectPath,
                                  int sampleRate,
                                  const QString& soundFontPath)
{
    std::vector<std::string> inputFiles;
    std::vector<std::string> tempFiles;  // files to clean up afterward

    // Determine the sample rate to use for the mix.  Audio tracks are recorded at
    // the device's native rate (e.g. 48000 Hz on a PipeWire system), which may
    // differ from the project sample-rate setting.  EOUtils requires all input
    // files to share the same sample rate, so we derive the mix rate from the
    // first enabled audio track that has data, and fall back to the project
    // sampleRate only when there are no audio tracks (MIDI-only mix).
    int mixSampleRate = sampleRate;
    for (const auto& track : tracks)
    {
        if (track.enabled && track.type == TrackType::Audio && !track.audioData.isEmpty())
        {
            mixSampleRate = track.sampleRate > 0 ? track.sampleRate : sampleRate;
            break;
        }
    }

    for (const auto& track : tracks)
    {
        if (!track.enabled) continue;

        if (track.type == TrackType::Audio && !track.audioData.isEmpty())
        {
            const QString wavPath = writeAudioTrackToTempWav(track);
            if (!wavPath.isEmpty())
            {
                inputFiles.push_back(wavPath.toStdString());
                tempFiles.push_back(wavPath.toStdString());
            }
        }
        else if (track.type == TrackType::MIDI && !track.midiNotes.isEmpty())
        {
            // Render MIDI to a temp WAV for mixing.  Using WAV at this step avoids
            // depending on the FLAC encoder succeeding; if encoding fails the track
            // would otherwise be silently dropped from the mix.
            QTemporaryFile tmpWav(QDir::temp().filePath("mc_midi_XXXXXX.wav"));
            tmpWav.setAutoRemove(false);
            if (!tmpWav.open())
                continue;
            const QString tmpWavPath = tmpWav.fileName();
            tmpWav.close();

            MidiSynth synth;
            if (synth.renderMidiToWav(track.midiNotes, track.lengthSeconds,
                                       tmpWavPath, mixSampleRate, soundFontPath))
            {
                inputFiles.push_back(tmpWavPath.toStdString());
                tempFiles.push_back(tmpWavPath.toStdString());

                // Best-effort: also save a persistent FLAC copy as a project asset.
                // This does not affect mixing — failure is silently ignored.
                if (!projectPath.isEmpty())
                {
                    QDir().mkpath(projectPath);
                    const QString flacPath = projectPath + QDir::separator() +
                                             QStringLiteral("track_%1_midi.flac").arg(track.id);
                    auto wavAudio = EOUtils::createAudioFileObjForExistingFile(
                        tmpWavPath.toStdString().c_str());
                    if (wavAudio && wavAudio->open(EOUtils::AUDIO_FILE_READ))
                    {
                        const EOUtils::AudioFileInfo info = wavAudio->getAudioFileInfo();
                        auto flacAudio = EOUtils::createAudioFileObjForNewFile(
                            flacPath.toStdString().c_str());
                        if (flacAudio)
                        {
                            flacAudio->setAudioFileInfo(info);
                            if (flacAudio->open(EOUtils::AUDIO_FILE_WRITE))
                            {
                                const size_t n = wavAudio->numSamples();
                                for (size_t si = 0; si < n; ++si)
                                {
                                    int64_t s = 0;
                                    if (!wavAudio->getNextSample_int64(s)) break;
                                    flacAudio->writeSample_int64(s);
                                }
                                flacAudio->close();
                            }
                        }
                        wavAudio->close();
                    }
                }
            }
            else
            {
                QFile::remove(tmpWavPath);
            }
        }
    }

    bool success = false;
    if (!inputFiles.empty())
    {
        const auto result = EOUtils::mixAudioFiles(inputFiles, outputPath.toStdString());
        success = static_cast<bool>(result);
    }

    for (const auto& f : tempFiles)
        QFile::remove(QString::fromStdString(f));

    return success;
}
