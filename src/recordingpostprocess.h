#ifndef RECORDINGPOSTPROCESS_H
#define RECORDINGPOSTPROCESS_H

#include <QByteArray>

// Normalises captured raw bytes to Int16 PCM, infers the true capture sample rate
// from the amount of audio actually processed (not wall-clock time), then converts
// channels and resamples to the project format.  Extracted for unit testing and
// to keep MainWindow readable.

struct RecordingPostProcessParams
{
    QByteArray rawBytes;
    /// QAudioFormat::SampleFormat enum value (e.g. 2 = Int16)
    int sampleFormat = 2;
    int reportedChannels = 2;
    int reportedSampleRate = 44100;
    /// Prefer QAudioSource::processedUSecs() after stop(); -1 = unknown
    qint64 processedMicroseconds = -1;
    /// Fallback only when processedMicroseconds is unavailable (less reliable)
    double wallClockSeconds = 0;
    int projectSampleRate = 44100;
    int projectChannelCount = 2;
};

struct RecordingPostProcessResult
{
    QByteArray int16Pcm;
    int sampleRate = 44100;
    int channelCount = 2;
    /// Effective capture rate inferred before resampling (for logging / tests)
    double inferredCaptureRate = 0;
    /// measuredByteRate / expectedByteRate for reported rate; 0 = not computed
    double throughputVsReported = 0;
};

class RecordingPostProcess
{
public:
    [[nodiscard]] static RecordingPostProcessResult process(const RecordingPostProcessParams& p);
};

#endif
