#include "recordingpostprocess.h"
#include "audioutils.h"

#include <QtGlobal>
#include <cmath>
#include <algorithm>

namespace
{
// Values match Qt 6 QAudioFormat::SampleFormat (so we need not link Qt Multimedia here).
constexpr int kFmtUInt8  = 1;
constexpr int kFmtInt16  = 2;
constexpr int kFmtInt32  = 3;
constexpr int kFmtFloat  = 4;

QByteArray rawToInt16(const QByteArray& rawData, int sampleFormat)
{
    if (sampleFormat == kFmtFloat)
    {
        const int n = rawData.size() / static_cast<int>(sizeof(float));
        QByteArray out(n * static_cast<int>(sizeof(qint16)), Qt::Uninitialized);
        const float* src = reinterpret_cast<const float*>(rawData.constData());
        qint16*      dst = reinterpret_cast<qint16*>(out.data());
        for (int i = 0; i < n; ++i)
            dst[i] = static_cast<qint16>(
                std::clamp(src[i] * 32767.0f, -32768.0f, 32767.0f));
        return out;
    }
    if (sampleFormat == kFmtInt32)
    {
        const int n = rawData.size() / static_cast<int>(sizeof(qint32));
        QByteArray out(n * static_cast<int>(sizeof(qint16)), Qt::Uninitialized);
        const qint32* src = reinterpret_cast<const qint32*>(rawData.constData());
        qint16*       dst = reinterpret_cast<qint16*>(out.data());
        for (int i = 0; i < n; ++i)
            dst[i] = static_cast<qint16>(src[i] >> 16);
        return out;
    }
    if (sampleFormat == kFmtUInt8)
    {
        const int n = rawData.size();
        QByteArray out(n * static_cast<int>(sizeof(qint16)), Qt::Uninitialized);
        const quint8* src = reinterpret_cast<const quint8*>(rawData.constData());
        qint16*       dst = reinterpret_cast<qint16*>(out.data());
        for (int i = 0; i < n; ++i)
            dst[i] = static_cast<qint16>((src[i] - 128) << 8);
        return out;
    }
    return rawData;
}

/// If measured is within `relTol` of a common studio rate, snap to that rate to reduce jitter.
int snapToStandardRate(double measured, double relTol = 0.012)
{
    static const int kStdRates[] = {
        8000, 11025, 12000, 16000, 22050, 24000, 32000,
        44100, 48000, 88200, 96000, 176400, 192000
    };
    if (measured <= 0.0 || !std::isfinite(measured))
        return 44100;
    int best = static_cast<int>(std::lround(measured));
    double bestRel = 1e9;
    for (int r : kStdRates)
    {
        const double rel = std::abs(measured - r) / static_cast<double>(r);
        if (rel < bestRel)
        {
            bestRel = rel;
            best = r;
        }
    }
    if (bestRel <= relTol)
        return best;
    return static_cast<int>(std::lround(std::clamp(measured, 4000.0, 384000.0)));
}
}  // namespace

RecordingPostProcessResult RecordingPostProcess::process(const RecordingPostProcessParams& p)
{
    RecordingPostProcessResult out;
    const int reportedCh = std::max(1, p.reportedChannels);

    QByteArray int16Data = rawToInt16(p.rawBytes, p.sampleFormat);
    if (int16Data.isEmpty())
    {
        out.sampleRate = p.projectSampleRate;
        out.channelCount = std::max(1, p.projectChannelCount);
        out.inferredCaptureRate = static_cast<double>(p.reportedSampleRate);
        return out;
    }

    const int frames = int16Data.size() / (reportedCh * static_cast<int>(sizeof(qint16)));
    if (frames <= 0)
    {
        out.sampleRate = p.projectSampleRate;
        out.channelCount = std::max(1, p.projectChannelCount);
        out.inferredCaptureRate = static_cast<double>(p.reportedSampleRate);
        return out;
    }

    // Derive the *true* capture sample rate (Audacity / PortAudio-style: match what the
    // stream actually delivers, not only what the API advertises).
    //
    // 1) QAudioSource::processedUSecs() when non-zero is the best duration signal.
    // 2) Otherwise compare *measured Int16 PCM byte rate* to *expected* for the reported rate
    //    (always in normalized PCM so float/32-bit captures are compared correctly).
    //    Many Linux + webcam + Qt stacks report stereo/32000 while the stream runs near
    //    half that effective frame rate (same bug class Audacity avoids by querying
    //    PortAudio-supported rates). If throughput is far below expected, infer rate
    //    from bytes ÷ wall time ÷ frame bytes.
    // 3) If measured throughput matches reported (within tolerance), trust reported —
    //    the wall clock is then consistent with the format (avoids mis-detecting rare
    //    timer quirks on short clips).
    constexpr double kMinWallForThroughput = 0.5;  // seconds
    constexpr double kSlowThroughputRatio  = 0.72; // meas < 72% of expected
    constexpr double kFastThroughputRatio    = 1.35; // meas > 135% of expected

    const double bytesPerFrame = static_cast<double>(reportedCh) * 2.0;  // Int16 interleaved
    const int    reportedRate = p.reportedSampleRate > 0 ? p.reportedSampleRate : 44100;
    double       inferredRate = static_cast<double>(reportedRate);
    int          captureRate  = reportedRate;

    if (p.processedMicroseconds > 0)
    {
        const double durationSec = static_cast<double>(p.processedMicroseconds) / 1e6;
        if (durationSec > 1e-6)
            inferredRate = static_cast<double>(frames) / durationSec;
        captureRate = snapToStandardRate(inferredRate, 0.015);
    }
    else if (p.wallClockSeconds >= kMinWallForThroughput && reportedRate > 0)
    {
        const double expByteRate  = static_cast<double>(reportedRate) * bytesPerFrame;
        const double measByteRate = static_cast<double>(int16Data.size()) / p.wallClockSeconds;
        const double ratio        = measByteRate / expByteRate;
        out.throughputVsReported  = ratio;

        if (ratio < kSlowThroughputRatio || ratio > kFastThroughputRatio)
        {
            inferredRate = measByteRate / bytesPerFrame;
            captureRate  = snapToStandardRate(inferredRate, 0.015);
        }
        else
        {
            inferredRate = static_cast<double>(reportedRate);
            captureRate  = snapToStandardRate(inferredRate, 0.015);
        }
    }
    else
        captureRate = snapToStandardRate(static_cast<double>(reportedRate), 0.015);

    out.inferredCaptureRate = inferredRate;

    int ch = reportedCh;
    const int projectRate = p.projectSampleRate;
    const int projectCh = std::max(1, p.projectChannelCount);

    if (ch != projectCh)
        int16Data = AudioUtils::convertChannels(int16Data, ch, projectCh);

    ch = projectCh;
    if (captureRate != projectRate && projectRate > 0)
        int16Data = AudioUtils::resampleInt16(int16Data, captureRate, projectRate, ch);

    out.int16Pcm = std::move(int16Data);
    out.sampleRate = projectRate;
    out.channelCount = ch;
    return out;
}
