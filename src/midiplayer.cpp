#include "midiplayer.h"
#include "trackdata.h"

#include <RtMidi.h>
#include <QTimer>
#include <algorithm>
#include <cmath>

MidiPlayer::MidiPlayer(QObject* parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &MidiPlayer::onTick);

    m_finishTimer = new QTimer(this);
    m_finishTimer->setSingleShot(true);
    m_finishTimer->setTimerType(Qt::PreciseTimer);
    connect(m_finishTimer, &QTimer::timeout, this, &MidiPlayer::onFinishTimeout);
}

MidiPlayer::~MidiPlayer()
{
    stop();
}

static inline int clampChannel(int ch)
{
    return (ch >= 0 && ch <= 15) ? ch : 0;
}

bool MidiPlayer::start(int midiDeviceIndex, const QVector<TrackData>& tracks, int volumePercent)
{
    stop();

    if (midiDeviceIndex <= 0)
        return false; // built-in synth not supported for realtime MIDI-out here

    // Build event list
    m_events.clear();
    m_events.reserve(1024);

    double lastEventTime = 0.0;
    for (const auto& t : tracks)
    {
        if (!trackShouldMix(t, tracks)) continue;
        if (t.type != TrackType::MIDI) continue;
        for (const auto& n : t.midiNotes)
        {
            const int ch = clampChannel(n.channel);
            const double onT  = std::max(0.0, n.startTime);
            const double offT = std::max(onT, n.startTime + std::max(0.0, n.duration));

            Event on;
            on.timeSec = onT;
            on.msg = { static_cast<unsigned char>(0x90 | ch),
                       static_cast<unsigned char>(n.note & 0x7F),
                       static_cast<unsigned char>(n.velocity & 0x7F) };
            m_events.push_back(on);

            Event off;
            off.timeSec = offT;
            off.msg = { static_cast<unsigned char>(0x80 | ch),
                        static_cast<unsigned char>(n.note & 0x7F),
                        static_cast<unsigned char>(0) };
            m_events.push_back(off);

            lastEventTime = std::max(lastEventTime, offT);
        }
    }

    if (m_events.isEmpty())
        return false;

    std::stable_sort(m_events.begin(), m_events.end(),
        [](const Event& a, const Event& b) { return a.timeSec < b.timeSec; });

    // Open MIDI output
    try
    {
        auto out = std::make_shared<RtMidiOut>();
        const unsigned int port = static_cast<unsigned int>(midiDeviceIndex - 1);
        if (port >= out->getPortCount())
            return false;
        out->openPort(port);
        m_midiOut = out;
    }
    catch (...)
    {
        m_midiOut.reset();
        return false;
    }

    sendInitialVolume(volumePercent);

    m_eventIndex = 0;
    m_running = true;
    m_clock.restart();

    // Drive with a small tick; we also send with a small lookahead window.
    m_timer->start(5);

    // Fire finished a bit after the last event time.
    const int finishMs = static_cast<int>(std::clamp(lastEventTime * 1000.0 + 250.0, 0.0, 24.0 * 60.0 * 60.0 * 1000.0));
    m_finishTimer->start(finishMs);
    return true;
}

void MidiPlayer::sendInitialVolume(int volumePercent)
{
    if (!m_midiOut) return;
    const int pct = std::clamp(volumePercent, 0, 200);
    const int v127 = std::clamp(static_cast<int>(std::lround((pct / 100.0) * 127.0)), 0, 127);
    try
    {
        // Best-effort: reset controllers then set Channel Volume (CC7) on all channels.
        for (int ch = 0; ch < 16; ++ch)
        {
            std::vector<unsigned char> reset = {
                static_cast<unsigned char>(0xB0 | ch),
                static_cast<unsigned char>(121), // Reset All Controllers
                static_cast<unsigned char>(0)
            };
            m_midiOut->sendMessage(&reset);

            std::vector<unsigned char> vol = {
                static_cast<unsigned char>(0xB0 | ch),
                static_cast<unsigned char>(7), // Channel Volume (MSB)
                static_cast<unsigned char>(v127)
            };
            m_midiOut->sendMessage(&vol);
        }
    }
    catch (...)
    {
        // If this fails, playback can still proceed.
    }
}

void MidiPlayer::stop()
{
    if (m_timer) m_timer->stop();
    if (m_finishTimer) m_finishTimer->stop();

    sendAllNotesOff();

    if (m_midiOut)
    {
        try { m_midiOut->closePort(); } catch (...) {}
        m_midiOut.reset();
    }

    m_events.clear();
    m_eventIndex = 0;
    m_running = false;
}

bool MidiPlayer::isRunning() const
{
    return m_running;
}

void MidiPlayer::sendAllNotesOff()
{
    if (!m_midiOut) return;
    try
    {
        // CC 123 (All Notes Off) on all channels.
        for (int ch = 0; ch < 16; ++ch)
        {
            std::vector<unsigned char> msg = {
                static_cast<unsigned char>(0xB0 | ch),
                static_cast<unsigned char>(123),
                static_cast<unsigned char>(0)
            };
            m_midiOut->sendMessage(&msg);
        }
    }
    catch (...)
    {
        // Best-effort cleanup only
    }
}

void MidiPlayer::onTick()
{
    if (!m_midiOut) return;
    if (m_eventIndex >= m_events.size()) return;

    const double nowSec = m_clock.elapsed() / 1000.0;
    constexpr double lookaheadSec = 0.010; // 10ms

    try
    {
        while (m_eventIndex < m_events.size() && m_events[m_eventIndex].timeSec <= nowSec + lookaheadSec)
        {
            auto& e = m_events[m_eventIndex];
            m_midiOut->sendMessage(&e.msg);
            ++m_eventIndex;
        }
    }
    catch (...)
    {
        // If MIDI output fails mid-playback, stop scheduling.
        stop();
    }
}

void MidiPlayer::onFinishTimeout()
{
    m_running = false;
    emit finished();
}

