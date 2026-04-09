#include "midirecorder.h"
#include <RtMidi.h>
#include <QMutex>
#include <QMutexLocker>
#include <QStringList>
#include <vector>
#include <memory>
#include <climits>

class MidiRecorder::Impl
{
public:
    std::unique_ptr<RtMidiIn> midiIn;
    QElapsedTimer             timer;
    QMutex                    mutex;

    // In-flight notes: indexed by [channel * 128 + note]; value is the start
    // time (in seconds since timer.start()) of the most recent note-on for
    // that note on that channel.  -1 means "no active note".  Tracking
    // per-channel is essential because the same note number can be playing
    // simultaneously on different channels (e.g. drums on ch 10 and a piano
    // melody on ch 1).
    double activeNoteStart[16 * 128];
    int    activeVelocity[16 * 128];

    // Completed notes (note-on/off pairs).
    QVector<MidiNote> notes;

    bool recording = false;

    Impl()
    {
        for (int i = 0; i < 16 * 128; ++i)
        {
            activeNoteStart[i] = -1.0;
            activeVelocity[i]  = 0;
        }
    }
};

namespace {
void midiInCallback(double /*deltatime*/, std::vector<unsigned char>* message, void* userData)
{
    if (!message || message->size() < 2 || !userData)
        return;
    auto* impl = static_cast<MidiRecorder::Impl*>(userData);

    QMutexLocker lock(&impl->mutex);
    if (!impl->recording)
        return;

    const double now = impl->timer.isValid()
        ? impl->timer.elapsed() / 1000.0
        : 0.0;

    const unsigned char status = (*message)[0];
    const int channel = status & 0x0F;        // 0..15
    const int note    = (*message)[1] & 0x7F;
    const int vel     = message->size() > 2 ? ((*message)[2] & 0x7F) : 0;
    const unsigned char kind = status & 0xF0;
    const int slot = channel * 128 + note;

    if (kind == 0x90 && vel > 0)
    {
        // Note on.  If the same note is already on (on the same channel),
        // finalise the previous one — some controllers don't send note-off
        // for legato playing.
        if (impl->activeNoteStart[slot] >= 0.0)
        {
            MidiNote n;
            n.note      = note;
            n.velocity  = impl->activeVelocity[slot];
            n.channel   = channel;
            n.startTime = impl->activeNoteStart[slot];
            n.duration  = now - n.startTime;
            if (n.duration < 0.001) n.duration = 0.001;
            impl->notes.append(n);
        }
        impl->activeNoteStart[slot] = now;
        impl->activeVelocity[slot]  = vel;
    }
    else if (kind == 0x80 || (kind == 0x90 && vel == 0))
    {
        // Note off (or note-on with velocity 0, the running-status form).
        if (impl->activeNoteStart[slot] >= 0.0)
        {
            MidiNote n;
            n.note      = note;
            n.velocity  = impl->activeVelocity[slot];
            n.channel   = channel;
            n.startTime = impl->activeNoteStart[slot];
            n.duration  = now - n.startTime;
            if (n.duration < 0.001) n.duration = 0.001;
            impl->notes.append(n);
            impl->activeNoteStart[slot] = -1.0;
            impl->activeVelocity[slot]  = 0;
        }
    }
}
} // anonymous namespace

MidiRecorder::MidiRecorder()
    : m_impl(std::make_shared<Impl>())
{
}

MidiRecorder::~MidiRecorder()
{
    if (m_impl && m_impl->midiIn)
    {
        try { m_impl->midiIn->closePort(); } catch (...) {}
    }
}

bool MidiRecorder::start(const QString& portName, QString* errorMsg)
{
    try
    {
        auto in = std::make_unique<RtMidiIn>();
        const unsigned int n = in->getPortCount();
        unsigned int matched = UINT_MAX;
        for (unsigned int i = 0; i < n; ++i)
        {
            if (QString::fromStdString(in->getPortName(i)) == portName)
            {
                matched = i;
                break;
            }
        }
        if (matched == UINT_MAX)
        {
            if (errorMsg)
                *errorMsg = QStringLiteral("MIDI input port \"%1\" not found").arg(portName);
            return false;
        }
        in->openPort(matched);
        // Don't ignore active sensing or sysex by default; do ignore time-code
        // since we don't need it for recording note events.
        in->ignoreTypes(/*sysex*/ true, /*time*/ true, /*sense*/ true);
        in->setCallback(&midiInCallback, m_impl.get());
        {
            QMutexLocker lock(&m_impl->mutex);
            m_impl->notes.clear();
            for (int i = 0; i < 16 * 128; ++i)
            {
                m_impl->activeNoteStart[i] = -1.0;
                m_impl->activeVelocity[i]  = 0;
            }
            m_impl->timer.start();
            m_impl->recording = true;
        }
        m_impl->midiIn = std::move(in);
        return true;
    }
    catch (const std::exception& e)
    {
        if (errorMsg)
            *errorMsg = QString::fromUtf8(e.what());
        return false;
    }
    catch (...)
    {
        if (errorMsg)
            *errorMsg = QStringLiteral("Unknown RtMidi error");
        return false;
    }
}

QVector<MidiNote> MidiRecorder::stop(double* outLengthSeconds)
{
    QVector<MidiNote> result;
    double endTime = 0.0;
    {
        QMutexLocker lock(&m_impl->mutex);
        if (m_impl->recording && m_impl->timer.isValid())
            endTime = m_impl->timer.elapsed() / 1000.0;
        m_impl->recording = false;

        // Close any still-on notes at the end of recording (across every
        // possible channel × note combination).
        for (int slot = 0; slot < 16 * 128; ++slot)
        {
            if (m_impl->activeNoteStart[slot] >= 0.0)
            {
                MidiNote n;
                n.note      = slot % 128;
                n.channel   = slot / 128;
                n.velocity  = m_impl->activeVelocity[slot];
                n.startTime = m_impl->activeNoteStart[slot];
                n.duration  = endTime - n.startTime;
                if (n.duration < 0.001) n.duration = 0.001;
                m_impl->notes.append(n);
                m_impl->activeNoteStart[slot] = -1.0;
                m_impl->activeVelocity[slot]  = 0;
            }
        }
        result = m_impl->notes;
    }
    if (m_impl->midiIn)
    {
        try { m_impl->midiIn->cancelCallback(); } catch (...) {}
        try { m_impl->midiIn->closePort(); }      catch (...) {}
        m_impl->midiIn.reset();
    }
    if (outLengthSeconds)
        *outLengthSeconds = endTime;
    return result;
}

QVector<MidiNote> MidiRecorder::snapshot() const
{
    QMutexLocker lock(&m_impl->mutex);
    return m_impl->notes;
}

QStringList MidiRecorder::availableInputPorts()
{
    QStringList ports;
    try
    {
        RtMidiIn in;
        const unsigned int n = in.getPortCount();
        for (unsigned int i = 0; i < n; ++i)
            ports.append(QString::fromStdString(in.getPortName(i)));
    }
    catch (...) {}
    return ports;
}
