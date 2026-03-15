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

static void writeWavHeader(QFile& file, quint32 sampleRate, quint32 numSamples) {
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
                                int sampleRate) {
    double maxLength = 0;
    for (const auto& t : tracks) {
        if (!t.enabled) continue;
        double len = t.length();
        if (len > maxLength) maxLength = len;
    }
    if (maxLength <= 0) maxLength = 1.0;

    const int totalFrames = static_cast<int>(maxLength * sampleRate);
    const int totalSamples = totalFrames * 2;
    QVector<float> mixLeft(totalFrames, 0.0f);
    QVector<float> mixRight(totalFrames, 0.0f);

    for (const auto& track : tracks) {
        if (!track.enabled) continue;

        if (track.type == TrackType::Audio) {
            int trackFrames = track.audioData.size() / (track.channelCount * 2);
            const qint16* src = reinterpret_cast<const qint16*>(track.audioData.constData());
            float scale = track.sampleRate == sampleRate ? 1.0f : 1.0f;
            for (int i = 0; i < qMin(trackFrames, totalFrames); ++i) {
                float l = src[i * track.channelCount] / 32768.0f;
                float r = track.channelCount > 1 ? src[i * track.channelCount + 1] / 32768.0f : l;
                mixLeft[i] += l * scale;
                mixRight[i] += r * scale;
            }
        } else if (track.type == TrackType::MIDI && !track.midiNotes.isEmpty()) {
            QTemporaryFile tmpWav(QDir::temp().filePath("mc_mix_XXXXXX.wav"));
            tmpWav.setAutoRemove(true);
            if (!tmpWav.open()) continue;
            const QString tmpPath = tmpWav.fileName();
            tmpWav.close();

            MidiSynth synth;
            if (synth.renderMidiToWav(track.midiNotes, track.lengthSeconds, tmpPath, sampleRate)) {
                if (QFile f(tmpPath); f.open(QIODevice::ReadOnly)) {
                    f.seek(44);
                    const QByteArray data = f.readAll();
                    const qint16* src = reinterpret_cast<const qint16*>(data.constData());
                    const int frames = data.size() / 4;
                    for (int i = 0; i < qMin(frames, totalFrames); ++i) {
                        mixLeft[i] += src[i * 2] / 32768.0f;
                        mixRight[i] += src[i * 2 + 1] / 32768.0f;
                    }
                }
            }
        }
    }

    float maxVal = 0;
    for (int i = 0; i < totalFrames; ++i) {
        float m = qMax(qAbs(mixLeft[i]), qAbs(mixRight[i]));
        if (m > maxVal) maxVal = m;
    }
    float gain = (maxVal > 0.9f) ? (0.9f / maxVal) : 1.0f;

    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    writeWavHeader(file, sampleRate, totalSamples / 2);
    for (int i = 0; i < totalFrames; ++i) {
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
static QString writeAudioTrackToTempWav(const TrackData& track) {
    QTemporaryFile tmp(QDir::temp().filePath("mc_audio_XXXXXX.wav"));
    tmp.setAutoRemove(false);
    if (!tmp.open()) return {};
    const QString path = tmp.fileName();

    const quint32 sr = static_cast<quint32>(track.sampleRate > 0 ? track.sampleRate : 44100);
    const int frames = track.audioData.size() / (track.channelCount * 2);
    writeWavHeader(tmp, sr, static_cast<quint32>(frames));
    // Write audio data; if mono, duplicate to stereo
    if (track.channelCount == 2) {
        tmp.write(track.audioData);
    } else {
        const qint16* src = reinterpret_cast<const qint16*>(track.audioData.constData());
        for (int i = 0; i < frames; ++i) {
            tmp.write(reinterpret_cast<const char*>(&src[i]), 2);
            tmp.write(reinterpret_cast<const char*>(&src[i]), 2);
        }
    }
    tmp.close();
    return path;
}

// Renders a MIDI track to a FLAC file in projectPath (or system temp if projectPath is empty).
// Returns the path to the FLAC file, or empty string on failure.
static QString renderMidiTrackToFlac(const TrackData& track,
                                     const QString& projectPath,
                                     int sampleRate) {
    // Step 1: render MIDI to a temporary WAV using MidiSynth
    QTemporaryFile tmpWav(QDir::temp().filePath("mc_midi_XXXXXX.wav"));
    tmpWav.setAutoRemove(false);
    if (!tmpWav.open()) return {};
    const QString tmpWavPath = tmpWav.fileName();
    tmpWav.close();

    MidiSynth synth;
    if (!synth.renderMidiToWav(track.midiNotes, track.lengthSeconds, tmpWavPath, sampleRate)) {
        QFile::remove(tmpWavPath);
        return {};
    }

    // Step 2: determine output FLAC path
    const QString dir = projectPath.isEmpty() ? QDir::tempPath() : projectPath;
    QDir().mkpath(dir);
    const QString flacPath = dir + QDir::separator() +
                             QString("track_%1_midi.flac").arg(track.id);

    // Step 3: open the temp WAV via audio_mixer_cpp
    auto wavFile = EOUtils::createAudioFileObjForExistingFile(tmpWavPath.toStdString().c_str());
    if (!wavFile) { QFile::remove(tmpWavPath); return {}; }

    auto openResult = wavFile->open(EOUtils::AUDIO_FILE_READ);
    if (!openResult) { QFile::remove(tmpWavPath); return {}; }

    const EOUtils::AudioFileInfo wavInfo = wavFile->getAudioFileInfo();

    // Step 4: create output FLAC file and configure it with the same audio format
    auto flacFile = EOUtils::createAudioFileObjForNewFile(flacPath.toStdString().c_str());
    if (!flacFile) { wavFile->close(); QFile::remove(tmpWavPath); return {}; }

    flacFile->setAudioFileInfo(wavInfo);

    auto writeResult = flacFile->open(EOUtils::AUDIO_FILE_WRITE);
    if (!writeResult) { wavFile->close(); QFile::remove(tmpWavPath); return {}; }

    // Step 5: copy all samples from WAV → FLAC
    const size_t numSamples = wavFile->numSamples();
    for (size_t i = 0; i < numSamples; ++i) {
        int64_t sample = 0;
        if (!wavFile->getNextSample_int64(sample)) break;
        flacFile->writeSample_int64(sample);
    }

    wavFile->close();
    flacFile->close();
    QFile::remove(tmpWavPath);

    return flacPath;
}

bool AudioUtils::mixTracksToFile(const QVector<TrackData>& tracks,
                                  const QString& outputPath,
                                  const QString& projectPath,
                                  int sampleRate) {
    std::vector<std::string> inputFiles;
    std::vector<std::string> tempFiles;  // files to clean up afterward

    for (const auto& track : tracks) {
        if (!track.enabled) continue;

        if (track.type == TrackType::Audio && !track.audioData.isEmpty()) {
            const QString wavPath = writeAudioTrackToTempWav(track);
            if (!wavPath.isEmpty()) {
                inputFiles.push_back(wavPath.toStdString());
                tempFiles.push_back(wavPath.toStdString());
            }
        } else if (track.type == TrackType::MIDI && !track.midiNotes.isEmpty()) {
            const QString flacPath = renderMidiTrackToFlac(track, projectPath, sampleRate);
            if (!flacPath.isEmpty()) {
                inputFiles.push_back(flacPath.toStdString());
                // FLAC files in project dir are intentionally kept, not added to tempFiles
            }
        }
    }

    bool success = false;
    if (!inputFiles.empty()) {
        const auto result = EOUtils::mixAudioFiles(inputFiles, outputPath.toStdString());
        success = static_cast<bool>(result);
    }

    for (const auto& f : tempFiles)
        QFile::remove(QString::fromStdString(f));

    return success;
}
