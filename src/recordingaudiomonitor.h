#ifndef RECORDINGAUDIOMONITOR_H
#define RECORDINGAUDIOMONITOR_H

#include <QAudioFormat>
#include <QByteArray>
#include <QObject>
#include <QMutex>

class QAudioDevice;
class QTimer;

#ifdef QT_MULTIMEDIA_AVAILABLE
class QAudioSink;
class QIODevice;
#endif

// Low-latency pass-through of captured PCM to the project audio output while recording.
class RecordingAudioMonitor : public QObject
{
    Q_OBJECT
public:
    explicit RecordingAudioMonitor(QObject* parent = nullptr);
    ~RecordingAudioMonitor() override;

    /// Returns false if output cannot play \a captureFormat.
    bool start(const QAudioDevice& outputDevice, const QAudioFormat& captureFormat);
    void stop();

    /// Thread-safe; safe to call from PortAudio callback or QAudioSource writer thread.
    void enqueuePcm(const char* data, int len);

private slots:
    void drainOutput();

private:
    QMutex       m_mutex;
    QByteArray   m_queue;
    QTimer*      m_timer = nullptr;
    QAudioFormat m_format;

#ifdef QT_MULTIMEDIA_AVAILABLE
    QAudioSink* m_sink = nullptr;
    QIODevice*  m_sinkDevice = nullptr;
#endif

    static constexpr int kMaxQueuedBytes = 800000;  // ~0.25s stereo int16 @ 48k × safety
};

#endif
