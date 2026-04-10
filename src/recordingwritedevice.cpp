#include "recordingwritedevice.h"
#include "recordingaudiomonitor.h"

#include <QtGlobal>

RecordingWriteDevice::RecordingWriteDevice(QObject* parent)
    : QIODevice(parent)
{
}

bool RecordingWriteDevice::open(OpenMode mode)
{
    if (!(mode & QIODevice::WriteOnly))
        return false;
    m_storage.clear();
    return QIODevice::open(mode | QIODevice::Unbuffered);
}

qint64 RecordingWriteDevice::readData(char* /*data*/, qint64 /*maxlen*/)
{
    return -1;
}

qint64 RecordingWriteDevice::writeData(const char* data, qint64 len)
{
    if (len <= 0)
        return 0;
    m_storage.append(data, static_cast<int>(len));
    if (m_monitor)
        m_monitor->enqueuePcm(data, static_cast<int>(len));
    return len;
}
