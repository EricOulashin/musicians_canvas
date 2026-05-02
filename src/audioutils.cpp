#include "audioutils.h"
#include "effectdsp.h"
#include "midisynth.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>
#include <QTemporaryFile>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <QJsonArray>

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
            if (synth.renderMidiToWav(track.midiNotes, track.midiControlChanges, track.lengthSeconds,
 tmpPath, sampleRate, soundFontPath))
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


bool AudioUtils::writeInt16PcmToAudioFile(const QByteArray& int16Data,
                                           int sampleRate,
                                           int channelCount,
                                           const QString& path)
{
    const QString ext = QStringLiteral(".") + QFileInfo(path).suffix().toLower();
    if (ext == QStringLiteral(".wav"))
        return writeWavInt16Pcm(path, int16Data, sampleRate, channelCount);

    QTemporaryFile tmpWav(QDir::temp().filePath("mc_enc_XXXXXX.wav"));
    tmpWav.setAutoRemove(true);
    if (!tmpWav.open())
        return false;
    const QString tmpWavPath = tmpWav.fileName();
    tmpWav.close();

    if (!writeWavInt16Pcm(tmpWavPath, int16Data, sampleRate, channelCount))
        return false;

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

    auto outFile = EOUtils::createAudioFileObjForNewFile(path.toStdString().c_str());
    if (!outFile)
    {
        wavFile->close();
        QFile::remove(tmpWavPath);
        return false;
    }

    outFile->setAudioFileInfo(wavInfo);

    if (!outFile->open(EOUtils::AUDIO_FILE_WRITE))
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
        outFile->writeSample_int64(sample);
    }

    wavFile->close();
    outFile->close();
    QFile::remove(tmpWavPath);
    return true;
}

bool AudioUtils::writeAudioDataToFlac(const QByteArray& int16Data,
                                       int sampleRate,
                                       int channelCount,
                                       const QString& flacPath)
{
    return writeInt16PcmToAudioFile(int16Data, sampleRate, channelCount, flacPath);
}

bool AudioUtils::readEncodedAudioFile(const QString& path,
                                    QByteArray& audioData,
                                    int& sampleRate,
                                    int& channelCount)
{
    auto audioFile = EOUtils::createAudioFileObjForExistingFile(path.toStdString().c_str());
    if (!audioFile)
        return false;

    if (!audioFile->open(EOUtils::AUDIO_FILE_READ))
        return false;

    const EOUtils::AudioFileInfo info = audioFile->getAudioFileInfo();
    sampleRate   = info.SampleRateHz();
    channelCount = info.NumChannels();

    const int16_t bitsPerSample = info.BitsPerSample();
    const size_t numSamples = audioFile->numSamples();
    audioData.resize(static_cast<int>(numSamples * sizeof(qint16)));
    qint16* dst = reinterpret_cast<qint16*>(audioData.data());

    for (size_t i = 0; i < numSamples; ++i)
    {
        int64_t sample = 0;
        if (!audioFile->getNextSample_int64(sample))
            break;
        if (bitsPerSample > 16)
            sample >>= (bitsPerSample - 16);
        else if (bitsPerSample > 0 && bitsPerSample < 16)
            sample <<= (16 - bitsPerSample);
        dst[i] = static_cast<qint16>(
            std::max(static_cast<int64_t>(-32768),
                     std::min(static_cast<int64_t>(32767), sample)));
    }

    audioFile->close();
    return true;
}

bool AudioUtils::readFlacAudioData(const QString& path,
                                    QByteArray& audioData,
                                    int& sampleRate,
                                    int& channelCount)
{
    return readEncodedAudioFile(path, audioData, sampleRate, channelCount);
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

namespace
{
void applyGainPanToStereoInt16(QByteArray& stereoInterleaved, float gainDb, float pan)
{
    if (stereoInterleaved.isEmpty())
        return;
    const float lin = std::pow(10.0f, gainDb / 20.0f);
    const float p = std::clamp(pan, -1.f, 1.f);
    const float ang = (p + 1.f) * (float(M_PI) * 0.25f);
    const float gL = std::cos(ang) * lin;
    const float gR = std::sin(ang) * lin;
    auto* s = reinterpret_cast<qint16*>(stereoInterleaved.data());
    const int nFrames = stereoInterleaved.size() / 4;
    for (int i = 0; i < nFrames; ++i)
    {
        const float L = s[i * 2] / 32768.0f;
        const float R = s[i * 2 + 1] / 32768.0f;
        s[i * 2] = static_cast<qint16>(
            std::clamp(L * gL, -1.0f, 1.0f) * 32767.0f);
        s[i * 2 + 1] = static_cast<qint16>(
            std::clamp(R * gR, -1.0f, 1.0f) * 32767.0f);
    }
}

/// Trim, resample to mixRate, stereo, gain/pan. Empty if no audible content.
QByteArray prepareTrackForMix(const TrackData& track, int mixSampleRate,
                              const QString& soundFontPath, bool renderMidiToAudio,
                              double midiGainMultiplier, const QString& projectPath,
                              bool writeMidiFlacAsset)
{
    QByteArray stereoPcm;
    int workRate = 44100;

    if (track.type == TrackType::Audio && !track.audioData.isEmpty())
    {
        QByteArray pcm = track.audioData;
        int ch = track.channelCount > 0 ? track.channelCount : 2;
        workRate = track.sampleRate > 0 ? track.sampleRate : mixSampleRate;
        const int bpf = ch * static_cast<int>(sizeof(qint16));
        int frames = pcm.size() / bpf;
        const int trim0 = static_cast<int>(
            std::llround(std::max(0.0, track.trimStartSec) * workRate));
        const int trim1 = static_cast<int>(
            std::llround(std::max(0.0, track.trimEndSec) * workRate));
        const int startF = std::clamp(trim0, 0, frames);
        const int endF = std::clamp(frames - trim1, 0, frames);
        if (endF <= startF)
            return {};
        pcm = pcm.mid(startF * bpf, (endF - startF) * bpf);
        frames = endF - startF;

        if (workRate != mixSampleRate)
            pcm = AudioUtils::resampleInt16(pcm, workRate, mixSampleRate, ch);
        if (ch != 2)
            pcm = AudioUtils::convertChannels(pcm, ch, 2);
        stereoPcm = pcm;
    }
    else if (renderMidiToAudio && track.type == TrackType::MIDI
             && (!track.midiNotes.isEmpty() || !track.midiControlChanges.isEmpty()))
    {
        QTemporaryFile tmpWav(QDir::temp().filePath("mc_midi_XXXXXX.wav"));
        tmpWav.setAutoRemove(false);
        if (!tmpWav.open())
            return {};
        const QString tmpWavPath = tmpWav.fileName();
        tmpWav.close();

        MidiSynth synth;
        if (!synth.renderMidiToWav(track.midiNotes, track.midiControlChanges, track.lengthSeconds,
                                   tmpWavPath, mixSampleRate, soundFontPath, midiGainMultiplier))
        {
            QFile::remove(tmpWavPath);
            return {};
        }

        if (writeMidiFlacAsset && !projectPath.isEmpty())
        {
            QDir().mkpath(projectPath);
            QString baseName = AudioUtils::sanitizedTrackFilesystemName(track.name);
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
                            if (!wavAudio->getNextSample_int64(s))
                                break;
                            flacAudio->writeSample_int64(s);
                        }
                        flacAudio->close();
                    }
                }
                wavAudio->close();
            }
        }

        int fileSr = mixSampleRate;
        int fileCh = 2;
        if (!AudioUtils::readWavInt16Pcm(tmpWavPath, stereoPcm, fileSr, fileCh))
        {
            QFile::remove(tmpWavPath);
            return {};
        }
        QFile::remove(tmpWavPath);

        const int bpf = fileCh * static_cast<int>(sizeof(qint16));
        int frames = stereoPcm.size() / bpf;
        const int trim0 = static_cast<int>(
            std::llround(std::max(0.0, track.trimStartSec) * fileSr));
        const int trim1 = static_cast<int>(
            std::llround(std::max(0.0, track.trimEndSec) * fileSr));
        const int startF = std::clamp(trim0, 0, frames);
        const int endF = std::clamp(frames - trim1, 0, frames);
        if (endF <= startF)
            return {};
        stereoPcm = stereoPcm.mid(startF * bpf, (endF - startF) * bpf);
        if (fileCh != 2)
            stereoPcm = AudioUtils::convertChannels(stereoPcm, fileCh, 2);
    }
    else
        return {};

    applyGainPanToStereoInt16(stereoPcm, track.gainDb, track.pan);
    return stereoPcm;
}

void addInt16StereoToFloatMix(const QByteArray& pcm, QVector<float>& L, QVector<float>& R)
{
    const auto* s = reinterpret_cast<const qint16*>(pcm.constData());
    const int n = qMin(static_cast<int>(pcm.size() / 4), L.size());
    for (int i = 0; i < n; ++i)
    {
        L[i] += s[i * 2] / 32768.0f;
        R[i] += s[i * 2 + 1] / 32768.0f;
    }
}

void addSendToAux(const QByteArray& pcm, float send, QVector<float>& auxL, QVector<float>& auxR,
                  int maxFrames)
{
    if (send <= 0.f || pcm.isEmpty())
        return;
    const auto* s = reinterpret_cast<const qint16*>(pcm.constData());
    const int n = pcm.size() / 4;
    const int lim = qMin(maxFrames, n);
    for (int i = 0; i < lim; ++i)
    {
        auxL[i] += (s[i * 2] / 32768.0f) * send;
        auxR[i] += (s[i * 2 + 1] / 32768.0f) * send;
    }
}

QByteArray floatStereoToInt16Pcm(const QVector<float>& L, const QVector<float>& R)
{
    QByteArray out;
    out.resize(L.size() * 4);
    auto* d = reinterpret_cast<qint16*>(out.data());
    for (int i = 0; i < L.size(); ++i)
    {
        d[i * 2] = static_cast<qint16>(std::clamp(L[i], -1.0f, 1.0f) * 32767.0f);
        d[i * 2 + 1] = static_cast<qint16>(std::clamp(R[i], -1.0f, 1.0f) * 32767.0f);
    }
    return out;
}

void int16StereoToFloat(const QByteArray& pcm, QVector<float>& L, QVector<float>& R)
{
    const auto* s = reinterpret_cast<const qint16*>(pcm.constData());
    const int n = pcm.size() / 4;
    L.resize(n);
    R.resize(n);
    for (int i = 0; i < n; ++i)
    {
        L[i] = s[i * 2] / 32768.0f;
        R[i] = s[i * 2 + 1] / 32768.0f;
    }
}

}  // namespace

bool AudioUtils::mixTracksToFile(const QVector<TrackData>& tracks,
                                  const QString& outputPath,
                                  const QString& projectPath,
                                  int sampleRate,
                                  const QString& soundFontPath,
                                  bool renderMidiToAudio,
                                  double midiGainMultiplier,
                                  const QJsonArray& auxEffectChain)
{
    int mixSampleRate = sampleRate;
    for (const auto& track : tracks)
    {
        if (track.enabled && track.type == TrackType::Audio && !track.audioData.isEmpty())
        {
            mixSampleRate = track.sampleRate > 0 ? track.sampleRate : sampleRate;
            break;
        }
    }

    QVector<QByteArray> perTrackPcm;
    perTrackPcm.reserve(tracks.size());
    int maxFrames = 0;

    bool anyAuxSend = false;
    for (const TrackData& t : tracks)
    {
        if (trackShouldMix(t, tracks) && t.auxSend > 0.f)
        {
            anyAuxSend = true;
            break;
        }
    }
    const bool useAux = !auxEffectChain.isEmpty() && anyAuxSend;

    for (const auto& track : tracks)
    {
        if (!trackShouldMix(track, tracks))
        {
            perTrackPcm.append(QByteArray());
            continue;
        }
        QByteArray pcm =
            prepareTrackForMix(track, mixSampleRate, soundFontPath, renderMidiToAudio,
                                 midiGainMultiplier, projectPath, /*writeMidiFlacAsset=*/true);
        perTrackPcm.append(pcm);
        maxFrames = qMax(maxFrames, pcm.size() / 4);
    }

    if (maxFrames <= 0)
        return false;

    QVector<float> dryL(maxFrames, 0.f);
    QVector<float> dryR(maxFrames, 0.f);
    QVector<float> auxL;
    QVector<float> auxR;
    if (useAux)
    {
        auxL = QVector<float>(maxFrames, 0.f);
        auxR = QVector<float>(maxFrames, 0.f);
    }

    for (int ti = 0; ti < tracks.size(); ++ti)
    {
        const TrackData& track = tracks[ti];
        const QByteArray& pcm = perTrackPcm[ti];
        if (pcm.isEmpty())
            continue;
        addInt16StereoToFloatMix(pcm, dryL, dryR);
        if (useAux)
            addSendToAux(pcm, track.auxSend, auxL, auxR, maxFrames);
    }

    if (useAux)
    {
        QByteArray auxPcm = floatStereoToInt16Pcm(auxL, auxR);
        applyAudioEffectChain(auxPcm, 2, mixSampleRate, auxEffectChain);
        QVector<float> wetL, wetR;
        int16StereoToFloat(auxPcm, wetL, wetR);
        const int n = qMin(maxFrames, wetL.size());
        for (int i = 0; i < n; ++i)
        {
            dryL[i] += wetL[i];
            dryR[i] += wetR[i];
        }
    }

    float peak = 0.f;
    for (int i = 0; i < maxFrames; ++i)
    {
        peak = qMax(peak, qMax(std::fabs(dryL[i]), std::fabs(dryR[i])));
    }
    const float norm = (peak > 0.9f) ? (0.9f / peak) : 1.f;

    const QString outExt = QFileInfo(outputPath).suffix().toLower();
    const bool outWav = (outExt == QLatin1String("wav"));

    QTemporaryFile tmpWav(QDir::temp().filePath("mc_mixout_XXXXXX.wav"));
    tmpWav.setAutoRemove(false);
    if (!tmpWav.open())
        return false;
    const QString wavOutPath = tmpWav.fileName();
    tmpWav.close();

    QFile file(wavOutPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    writeWavHeader(file, static_cast<quint32>(mixSampleRate),
                   static_cast<quint32>(maxFrames));
    constexpr float scale = 32767.0f;
    constexpr float minV = -32768.0f;
    for (int i = 0; i < maxFrames; ++i)
    {
        const qint16 l = static_cast<qint16>(
            std::clamp(dryL[i] * norm * scale, minV, scale));
        const qint16 r = static_cast<qint16>(
            std::clamp(dryR[i] * norm * scale, minV, scale));
        file.write(reinterpret_cast<const char*>(&l), 2);
        file.write(reinterpret_cast<const char*>(&r), 2);
    }
    file.close();

    if (outWav)
    {
        if (QFile::exists(outputPath))
            QFile::remove(outputPath);
        if (!QFile::rename(wavOutPath, outputPath))
        {
            if (!QFile::copy(wavOutPath, outputPath))
                return false;
            QFile::remove(wavOutPath);
        }
    }
    else
    {
        QByteArray pcmData;
        int sr = mixSampleRate;
        int ch = 2;
        if (!readWavInt16Pcm(wavOutPath, pcmData, sr, ch))
        {
            QFile::remove(wavOutPath);
            return false;
        }
        if (!writeInt16PcmToAudioFile(pcmData, sr, ch, outputPath))
        {
            QFile::remove(wavOutPath);
            return false;
        }
        QFile::remove(wavOutPath);
    }

    return true;
}

bool AudioUtils::exportStemsToDirectory(const QVector<TrackData>& tracks,
                                          const QString& outputDirectory,
                                          const QString& projectPath,
                                          int sampleRate,
                                          const QString& soundFontPath,
                                          bool renderMidiToAudio,
                                          double midiGainMultiplier,
                                          const QString& outputExtension)
{
    if (outputDirectory.isEmpty() || !QDir(outputDirectory).exists())
        return false;

    int mixSampleRate = sampleRate;
    for (const auto& track : tracks)
    {
        if (track.type == TrackType::Audio && !track.audioData.isEmpty())
        {
            mixSampleRate = track.sampleRate > 0 ? track.sampleRate : sampleRate;
            break;
        }
    }

    for (const TrackData& track : tracks)
    {
        const bool hasAudio = track.type == TrackType::Audio && !track.audioData.isEmpty();
        const bool hasMidi = track.type == TrackType::MIDI
                             && (!track.midiNotes.isEmpty() || !track.midiControlChanges.isEmpty());
        if (!hasAudio && !hasMidi)
            continue;

        TrackData one = track;
        one.mute = false;
        one.solo = false;
        QByteArray pcm =
            prepareTrackForMix(one, mixSampleRate, soundFontPath, renderMidiToAudio,
                                 midiGainMultiplier, QString(), /*writeMidiFlacAsset=*/false);
        if (pcm.isEmpty())
            continue;

        QString base = sanitizedTrackFilesystemName(track.name);
        if (base.isEmpty())
            base = QStringLiteral("track_%1").arg(track.id);
        const QString ext =
            outputExtension.startsWith(QLatin1Char('.')) ? outputExtension
                                                          : QLatin1Char('.') + outputExtension;
        const QString outPath = outputDirectory + QDir::separator() + base
                                + QStringLiteral("_stem") + ext;
        if (!writeInt16PcmToAudioFile(pcm, mixSampleRate, 2, outPath))
            return false;
    }
    return true;
}

bool AudioUtils::readWavInt16Pcm(const QString& path, QByteArray& audioData,
                                  int& sampleRate, int& channelCount)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    const QByteArray riff = f.read(12);
    if (riff.size() < 12
        || riff.left(4) != QByteArray("RIFF", 4)
        || riff.mid(8, 4) != QByteArray("WAVE", 4))
    {
        return false;
    }

    bool foundFmt  = false;
    bool foundData = false;

    while (!f.atEnd())
    {
        const QByteArray chunkHdr = f.read(8);
        if (chunkHdr.size() < 8)
            break;

        const QByteArray chunkId = chunkHdr.left(4);
        const quint32 chunkSize =
            static_cast<quint32>(static_cast<unsigned char>(chunkHdr[4]))
            | (static_cast<quint32>(static_cast<unsigned char>(chunkHdr[5])) << 8)
            | (static_cast<quint32>(static_cast<unsigned char>(chunkHdr[6])) << 16)
            | (static_cast<quint32>(static_cast<unsigned char>(chunkHdr[7])) << 24);

        if (chunkId == QByteArray("fmt ", 4))
        {
            if (chunkSize < 16)
                return false;
            const QByteArray fmt = f.read(chunkSize);
            if (static_cast<quint32>(fmt.size()) < chunkSize)
                return false;
            channelCount =
                static_cast<int>(static_cast<unsigned char>(fmt[2]))
                | (static_cast<int>(static_cast<unsigned char>(fmt[3])) << 8);
            sampleRate =
                static_cast<int>(static_cast<unsigned char>(fmt[4]))
                | (static_cast<int>(static_cast<unsigned char>(fmt[5])) << 8)
                | (static_cast<int>(static_cast<unsigned char>(fmt[6])) << 16)
                | (static_cast<int>(static_cast<unsigned char>(fmt[7])) << 24);
            foundFmt = true;
        }
        else if (chunkId == QByteArray("data", 4))
        {
            audioData = f.read(chunkSize);
            foundData = true;
            break;
        }
        else
        {
            qint64 skipBytes = static_cast<qint64>(chunkSize);
            if (skipBytes % 2 != 0)
                skipBytes++;
            f.seek(f.pos() + skipBytes);
        }
    }

    return foundFmt && foundData;
}

bool AudioUtils::writeWavInt16Pcm(const QString& path, const QByteArray& int16Interleaved,
                                   int sampleRate, int channelCount)
{
    if (channelCount <= 0 || sampleRate <= 0)
        return false;
    const int frameBytes = channelCount * static_cast<int>(sizeof(qint16));
    if (int16Interleaved.isEmpty() || (int16Interleaved.size() % frameBytes != 0))
        return false;

    const quint32 numFrames =
        static_cast<quint32>(int16Interleaved.size() / frameBytes);
    const quint16 ch = static_cast<quint16>(channelCount);
    const quint32 dataSize = static_cast<quint32>(int16Interleaved.size());
    const quint16 blockAlign = static_cast<quint16>(frameBytes);
    const quint32 byteRate = static_cast<quint32>(sampleRate) * blockAlign;
    const quint32 fileSize = 36 + dataSize;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&fileSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    quint32 fmtSize = 16;
    file.write(reinterpret_cast<const char*>(&fmtSize), 4);
    quint16 audioFormat = 1;
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    file.write(reinterpret_cast<const char*>(&ch), 2);
    quint32 sr = static_cast<quint32>(sampleRate);
    file.write(reinterpret_cast<const char*>(&sr), 4);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    quint16 bitsPerSample = 16;
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);
    file.write(int16Interleaved);
    file.close();
    return true;
}

bool AudioUtils::applyMixEffectChainToAudioFile(const QString& path, const QJsonArray& chain)
{
    if (chain.isEmpty())
        return true;

    const QString ext = QStringLiteral(".") + QFileInfo(path).suffix().toLower();
    QByteArray pcm;
    int sampleRate = 44100;
    int channelCount = 2;

    if (ext == QStringLiteral(".wav"))
    {
        if (!readWavInt16Pcm(path, pcm, sampleRate, channelCount))
            return false;
    }
    else
    {
        if (!readEncodedAudioFile(path, pcm, sampleRate, channelCount))
            return false;
    }

    applyAudioEffectChain(pcm, channelCount, sampleRate, chain);

    return writeInt16PcmToAudioFile(pcm, sampleRate, channelCount, path);
}
