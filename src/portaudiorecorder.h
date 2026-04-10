#ifndef PORTAUDIORECORDER_H
#define PORTAUDIORECORDER_H

#include <QByteArray>
#include <QString>

#include <functional>
#include <mutex>

#ifdef HAVE_PORTAUDIO
#include <portaudio.h>
#endif

// PortAudio-based capture (Audacity-style path). Implemented only when built with
// HAVE_PORTAUDIO; stubs return errors when disabled.

class PortAudioRecorder
{
public:
    PortAudioRecorder();
    ~PortAudioRecorder();

    static bool isCompiledWithPortAudio();

    /// Call once before any Pa_* use (e.g. from main()). Safe to call again (ref-counted).
    static bool initializeLibrary(QString* errorMessage = nullptr);
    /// Call once at process exit after static/device queries are finished.
    static void terminateLibrary();

    /// After initializeLibrary(): true if there is at least one input device.
    static bool hasInputDevice();

    static int defaultInputDeviceIndex();
    static int deviceCount();
    static QString deviceName(int paDeviceIndex);
    static int maxInputChannels(int paDeviceIndex);

    bool isActive() const { return m_stream != nullptr; }

    /// Opens a capture stream: Int16 interleaved PCM, rate negotiated (see implementation).
    /// \p paDeviceIndex Host API device index, or -1 for Pa_GetDefaultInputDevice().
    bool start(int paDeviceIndex, int channelCount, int preferredSampleRate, QString& errorMessage);

    void stop();

    /// Int16 little-endian interleaved PCM. Empty if not recording or no frames captured.
    QByteArray takeRecordedPcm();

    int streamSampleRate() const { return m_streamSampleRate; }
    int streamChannelCount() const { return m_streamChannels; }
    qint64 framesRecorded() const { return m_totalFrames; }

    /// Duration of captured audio in microseconds (for RecordingPostProcess).
    qint64 processedMicroseconds() const;

    /// Peak magnitude [0,1] from the most recent ~2048 frames per channel (meter).
    float recentPeak() const;

    /// Optional real-time tap of captured PCM (same layout as m_buffer). Thread-safe.
    void setMonitorTap(std::function<void(const char*, int)> fn);
    void clearMonitorTap();

private:
#ifdef HAVE_PORTAUDIO
    static int paCallback(const void* input, void* output, unsigned long frameCount,
                          const PaStreamCallbackTimeInfo* timeInfo,
                          PaStreamCallbackFlags statusFlags, void* userData);
#endif

    void*       m_stream = nullptr;  // PaStream*
    int         m_streamSampleRate = 0;
    int         m_streamChannels   = 0;
    qint64      m_totalFrames       = 0;
    mutable std::mutex m_mutex;
    QByteArray  m_buffer;
    int         m_bytesPerFrame = 0;

    std::mutex                           m_monitorMutex;
    std::function<void(const char*, int)> m_monitorTap;
};

#endif
