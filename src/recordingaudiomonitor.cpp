#include "recordingaudiomonitor.h"

#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QAudioDevice>
#include <QAudioFormat>
#include <QAudioSink>
#include <QTimer>
#endif

RecordingAudioMonitor::RecordingAudioMonitor(QObject* parent)
    : QObject(parent)
{
}

RecordingAudioMonitor::~RecordingAudioMonitor()
{
    stop();
}

bool RecordingAudioMonitor::start(const QAudioDevice& outputDevice, const QAudioFormat& captureFormat)
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    stop();

    if (outputDevice.isNull() || !captureFormat.isValid())
        return false;

    if (!outputDevice.isFormatSupported(captureFormat))
        return false;

    m_format = captureFormat;
    m_sink = new QAudioSink(outputDevice, captureFormat, this);
    m_sink->setVolume(1.0f);
    const int frameBytes = captureFormat.bytesPerFrame();
    if (frameBytes > 0 && captureFormat.sampleRate() > 0)
    {
        const int target = (captureFormat.sampleRate() * frameBytes) / 6;  // ~170 ms
        m_sink->setBufferSize(qBound(frameBytes * 256, target, frameBytes * 8192));
    }
    m_sinkDevice = m_sink->start();
    if (m_sinkDevice == nullptr)
    {
        m_sink->deleteLater();
        m_sink = nullptr;
        return false;
    }

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &RecordingAudioMonitor::drainOutput);
    m_timer->start(12);
    return true;
#else
    Q_UNUSED(outputDevice);
    Q_UNUSED(captureFormat);
    return false;
#endif
}

void RecordingAudioMonitor::stop()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    if (m_timer)
    {
        m_timer->stop();
        m_timer->deleteLater();
        m_timer = nullptr;
    }
    m_sinkDevice = nullptr;
    if (m_sink)
    {
        m_sink->stop();
        m_sink->deleteLater();
        m_sink = nullptr;
    }
#endif
    QMutexLocker lock(&m_mutex);
    m_queue.clear();
}

void RecordingAudioMonitor::enqueuePcm(const char* data, int len)
{
    if (len <= 0)
        return;
#ifdef QT_MULTIMEDIA_AVAILABLE
    if (m_sink == nullptr)
        return;
#endif
    QMutexLocker lock(&m_mutex);
    m_queue.append(data, len);
    while (m_queue.size() > kMaxQueuedBytes)
    {
        const int drop = m_queue.size() - kMaxQueuedBytes;
        m_queue.remove(0, drop);
    }
}

void RecordingAudioMonitor::drainOutput()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    if (m_sink == nullptr || m_sinkDevice == nullptr)
        return;

    QByteArray chunk;
    {
        QMutexLocker lock(&m_mutex);
        const qint64 roomRaw = static_cast<qint64>(m_sink->bytesFree());
        if (roomRaw <= 0)
            return;
        int room = static_cast<int>(qMin(qint64(kMaxQueuedBytes), roomRaw));
        const int frameBytes = m_format.bytesPerFrame();
        if (frameBytes > 0)
            room -= room % frameBytes;
        if (room <= 0)
            return;
        const int take = qMin(room, m_queue.size());
        if (take <= 0)
            return;
        chunk = QByteArray(m_queue.constData(), take);
        m_queue.remove(0, take);
    }

    if (!chunk.isEmpty())
        m_sinkDevice->write(chunk.constData(), chunk.size());
#else
    // no-op when Multimedia is disabled
#endif
}
