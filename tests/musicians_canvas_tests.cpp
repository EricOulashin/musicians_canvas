// Headless unit tests for Musician's Canvas (add new test cases here as needed).
// Run: ./musicians_canvas_tests

#include "recordingpostprocess.h"

#include <QtGlobal>
#include <cmath>
#include <cstdlib>

static void fail(const char* msg)
{
    qWarning("%s", msg);
    std::abort();
}

static QByteArray makeStereoSine32000OneSecond()
{
    // 32000 Hz stereo Int16: 1 second
    const int frames = 32000;
    QByteArray raw;
    raw.resize(frames * 2 * static_cast<int>(sizeof(qint16)));
    auto* p = reinterpret_cast<qint16*>(raw.data());
    for (int i = 0; i < frames; ++i)
    {
        const qint16 s = static_cast<qint16>(20000.0 * std::sin(2.0 * 3.141592653589793 * 440.0 * i / 32000.0));
        p[i * 2]     = s;
        p[i * 2 + 1] = s;
    }
    return raw;
}

static QByteArray makeStereoSilence(int numFrames)
{
    QByteArray raw;
    raw.resize(numFrames * 2 * static_cast<int>(sizeof(qint16)));
    raw.fill(char{0});
    return raw;
}

static void test_recording_postprocess()
{
    // True capture: 1 s of stereo @ 32000 Hz; Qt reports processed duration = 1e6 µs
    {
        RecordingPostProcessParams p;
        p.rawBytes = makeStereoSine32000OneSecond();
        p.sampleFormat = 2;  // Int16
        p.reportedChannels = 2;
        p.reportedSampleRate = 32000;
        p.processedMicroseconds = 1000000;
        p.wallClockSeconds = 1.05;  // wall clock slightly off — should not dominate
        p.projectSampleRate = 44100;
        p.projectChannelCount = 2;

        const RecordingPostProcessResult r = RecordingPostProcess::process(p);
        const int outFrames = r.int16Pcm.size() / (r.channelCount * 2);
        if (outFrames != 44100)
            fail("expected 44100 output frames for 1s @ 32000 -> 44100 stereo");
        if (r.sampleRate != 44100 || r.channelCount != 2)
            fail("expected project rate/channels in result");
        if (std::abs(r.inferredCaptureRate - 32000.0) > 50.0)
            fail("inferred capture rate should be ~32000");
    }

    // No processedUSecs but throughput matches reported (wall consistent with 32 kHz stereo)
    {
        RecordingPostProcessParams p;
        p.rawBytes = makeStereoSine32000OneSecond();
        p.sampleFormat = 2;
        p.reportedChannels = 2;
        p.reportedSampleRate = 32000;
        p.processedMicroseconds = -1;
        p.wallClockSeconds = 1.0;
        p.projectSampleRate = 44100;
        p.projectChannelCount = 2;

        const RecordingPostProcessResult r = RecordingPostProcess::process(p);
        const int outFrames = r.int16Pcm.size() / (r.channelCount * 2);
        if (outFrames != 44100)
            fail("when byte rate matches reported rate, expect 44100 output frames");
        if (r.throughputVsReported < 0.99 || r.throughputVsReported > 1.01)
            fail("throughput ratio should be ~1 for full-rate capture");
    }

    // Regression: C920-style capture — API says 32 kHz stereo but stream delivers ~half the
    // bytes/sec (matches user recording_debug: ~456704 bytes / ~7.426 s wall).
    {
        constexpr int   kFrames   = 114176;
        constexpr double kWallSec = 7.426;
        RecordingPostProcessParams p;
        p.rawBytes = makeStereoSilence(kFrames);
        p.sampleFormat = 2;
        p.reportedChannels = 2;
        p.reportedSampleRate = 32000;
        p.processedMicroseconds = -1;
        p.wallClockSeconds = kWallSec;
        p.projectSampleRate = 44100;
        p.projectChannelCount = 2;

        if (p.rawBytes.size() != 456704)
            fail("sanity: regression buffer size");

        const RecordingPostProcessResult r = RecordingPostProcess::process(p);
        const int   outFrames = r.int16Pcm.size() / (r.channelCount * 2);
        const double outDur   = static_cast<double>(outFrames) / 44100.0;
        if (outDur < 6.9 || outDur > 7.85)
            fail("C920-style half-throughput capture should yield ~wall-clock duration at 44100");
        if (r.throughputVsReported >= 0.72)
            fail("regression: throughput ratio should be below slow threshold");
    }
}

int main()
{
    test_recording_postprocess();
    return 0;
}
