#ifndef RECORDINGWRITEDEVICE_H
#define RECORDINGWRITEDEVICE_H

#include <QByteArray>
#include <QIODevice>

class RecordingAudioMonitor;

// Write-only QIODevice that stores captured PCM for disk finalize and optionally
// forwards it to a real-time monitor.
class RecordingWriteDevice : public QIODevice
{
public:
    explicit RecordingWriteDevice(QObject* parent = nullptr);

    void setAudioMonitor(RecordingAudioMonitor* monitor) { m_monitor = monitor; }

    const QByteArray& recordedData() const { return m_storage; }

    bool open(OpenMode mode) override;

protected:
    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 len) override;

private:
    QByteArray m_storage;
    RecordingAudioMonitor* m_monitor = nullptr;
};

#endif
