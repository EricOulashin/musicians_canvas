#include "audioutils.h"
#include "midisynth.h"
#include <QFile>
#include <QDir>
#include <QRegularExpression>
#include <QTemporaryFile>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "AudioFileTools.h"
#include "AudioFileInfo.h"
#include "FLACFileInfo.h"

QString AudioUtils::sanitizedTrackFilesystemName(const QString& trackName)
{
    QString result = trackName.trimmed();
    static const QRegularExpression invalid(QStringLiteral("[/\\\\:*?\"<>|\\r\\n]"));
    result.replace(invalid, QStringLiteral("_"));
    return result;
}

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

// ── Windowed sinc resampler ──────────────────────────────────────────────
// This implements the same algorithm family that libsoxr / Audacity use for
// high-quality sample-rate conversion: a FIR filter built from a sinc kernel
// multiplied by a Kaiser window.  The kernel width (SINC_HALF_LEN taps on
// each side) controls quality vs. cost; 16 taps provides ~96 dB stopband
// attenuation, comparable to soxr's "High Quality" setting.
//
// When downsampling, the filter is scaled by the conversion ratio to act as
// a proper anti-aliasing low-pass filter (Audacity/soxr do the same).
// When upsampling, the sinc kernel reconstructs the waveform from the
// Nyquist-limited source, producing smooth output without imaging artifacts.
//
// This approach preserves pitch and duration exactly.

static constexpr int    SINC_HALF_LEN = 16;    // taps on each side
static constexpr double KAISER_BETA   = 9.0;   // ~96 dB attenuation

// Modified Bessel function of the first kind, order 0 (I0).
static double besselI0(double x)
{
    double sum = 1.0;
    double term = 1.0;
    for (int k = 1; k < 25; ++k)
    {
        term *= (x * x) / (4.0 * k * k);
        sum += term;
        if (term < 1e-12 * sum)
            break;
    }
    return sum;
}

// Kaiser window: w(n) = I0(beta * sqrt(1 - (n/N)^2)) / I0(beta)
static double kaiserWindow(double n, double N, double beta)
{
    const double r = n / N;
    if (std::abs(r) >= 1.0)
        return 0.0;
    return besselI0(beta * std::sqrt(1.0 - r * r)) / besselI0(beta);
}

QByteArray AudioUtils::resampleInt16(const QByteArray& int16Data,
                                      int srcRate,
                                      int dstRate,
                                      int channelCount)
{
    if (srcRate == dstRate || srcRate <= 0 || dstRate <= 0 || channelCount <= 0)
        return int16Data;

    const qint16* src = reinterpret_cast<const qint16*>(int16Data.constData());
    const int srcFrames = int16Data.size() / (channelCount * static_cast<int>(sizeof(qint16)));
    if (srcFrames <= 0)
        return int16Data;

    const double ratio = static_cast<double>(srcRate) / static_cast<double>(dstRate);
    const int dstFrames = static_cast<int>(std::ceil(srcFrames / ratio));

    // When downsampling (ratio > 1), scale the sinc kernel to act as a
    // low-pass filter at the output Nyquist frequency (anti-aliasing).
    // When upsampling (ratio < 1), use the full bandwidth.
    const double cutoff   = (ratio > 1.0) ? 1.0 / ratio : 1.0;
    const int    halfLen  = (ratio > 1.0)
                                ? static_cast<int>(std::ceil(SINC_HALF_LEN * ratio))
                                : SINC_HALF_LEN;
    const double winHalf  = static_cast<double>(halfLen);

    QByteArray result(dstFrames * channelCount * static_cast<int>(sizeof(qint16)), 0);
    qint16* dst = reinterpret_cast<qint16*>(result.data());

    for (int i = 0; i < dstFrames; ++i)
    {
        const double srcCenter = i * ratio;
        const int    iStart    = std::max(0, static_cast<int>(std::floor(srcCenter - winHalf)));
        const int    iEnd      = std::min(srcFrames - 1, static_cast<int>(std::ceil(srcCenter + winHalf)));

        for (int c = 0; c < channelCount; ++c)
        {
            double accum = 0.0;
            double wSum  = 0.0;
            for (int j = iStart; j <= iEnd; ++j)
            {
                const double d = j - srcCenter;          // distance in source samples
                // sinc(x) = sin(pi*x) / (pi*x), with sinc(0) = 1
                double sincVal;
                const double dx = d * cutoff;
                if (std::abs(dx) < 1e-9)
                    sincVal = 1.0;
                else
                    sincVal = std::sin(M_PI * dx) / (M_PI * dx);

                const double w = kaiserWindow(d, winHalf, KAISER_BETA) * sincVal * cutoff;
                accum += src[j * channelCount + c] * w;
                wSum  += w;
            }
            if (wSum != 0.0)
                accum /= wSum;
            dst[i * channelCount + c] = static_cast<qint16>(
                std::max(-32768.0, std::min(32767.0, std::round(accum))));
        }
    }
    return result;
}

QByteArray AudioUtils::convertChannels(const QByteArray& int16Data,
                                        int srcChannels,
                                        int dstChannels)
{
    if (srcChannels == dstChannels || srcChannels <= 0 || dstChannels <= 0)
        return int16Data;

    const qint16* src = reinterpret_cast<const qint16*>(int16Data.constData());
    const int srcFrames = int16Data.size() / (srcChannels * static_cast<int>(sizeof(qint16)));
    if (srcFrames <= 0)
        return int16Data;

    QByteArray result(srcFrames * dstChannels * static_cast<int>(sizeof(qint16)), 0);
    qint16* dst = reinterpret_cast<qint16*>(result.data());

    if (srcChannels == 1 && dstChannels == 2)
    {
        // Mono → Stereo: duplicate the channel (same as Audacity)
        for (int i = 0; i < srcFrames; ++i)
        {
            dst[i * 2]     = src[i];
            dst[i * 2 + 1] = src[i];
        }
    }
    else if (srcChannels == 2 && dstChannels == 1)
    {
        // Stereo → Mono: average L and R (same as Audacity)
        for (int i = 0; i < srcFrames; ++i)
        {
            const int32_t L = src[i * 2];
            const int32_t R = src[i * 2 + 1];
            dst[i] = static_cast<qint16>((L + R) / 2);
        }
    }
    else
    {
        // General case: copy as many channels as we can, zero-fill the rest
        const int copyChannels = std::min(srcChannels, dstChannels);
        for (int i = 0; i < srcFrames; ++i)
        {
            for (int c = 0; c < copyChannels; ++c)
                dst[i * dstChannels + c] = src[i * srcChannels + c];
        }
    }
    return result;
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
                    QString baseName = sanitizedTrackFilesystemName(track.name);
                    if (baseName.isEmpty())
                        baseName = QStringLiteral("track_%1").arg(track.id);
                    const QString flacPath =
                        projectPath + QDir::separator() + baseName + QStringLiteral(".flac");
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
