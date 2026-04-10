#include "midirecordingmonitor.h"

#include <fluidsynth.h>

#include <QtGlobal>
#include <QFile>

#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QAudioFormat>
#include <QAudioSink>
#include <QTimer>
#include <vector>
#endif

#include <cmath>
#include <cstring>

static void suppressFluidWarn(int /*level*/, const char* /*message*/, void* /*data*/)
{
}

MidiRecordingMonitor::MidiRecordingMonitor(QObject* parent)
    : QObject(parent)
{
}

MidiRecordingMonitor::~MidiRecordingMonitor()
{
    stop();
}

#ifdef QT_MULTIMEDIA_AVAILABLE
bool MidiRecordingMonitor::start(const QString& soundFontPath, int sampleRate,
                                 const QAudioDevice& outputDevice, double gainMultiplier)
{
    stop();

    if (soundFontPath.isEmpty() || !QFile::exists(soundFontPath)
        || outputDevice.isNull() || sampleRate <= 0)
        return false;

    m_sampleRate = sampleRate;

    fluid_set_log_function(FLUID_WARN, suppressFluidWarn, nullptr);

    m_settings = new_fluid_settings();
    fluid_settings_setint(m_settings, "audio.realtime-prio", 0);
    fluid_settings_setint(m_settings, "synth.lock-memory", 0);
    fluid_settings_setnum(m_settings, "synth.sample-rate", static_cast<double>(sampleRate));
    fluid_settings_setnum(m_settings, "synth.gain", std::clamp(0.8 * gainMultiplier, 0.01, 4.0));

    m_synth = new_fluid_synth(m_settings);
    if (fluid_synth_sfload(m_synth, soundFontPath.toUtf8().constData(), 1) == FLUID_FAILED)
    {
        teardownSynth();
        return false;
    }

    QAudioFormat fmt;
    fmt.setSampleRate(sampleRate);
    fmt.setChannelCount(2);
    fmt.setSampleFormat(QAudioFormat::Int16);

    if (!outputDevice.isFormatSupported(fmt))
    {
        teardownSynth();
        return false;
    }

    m_sink = new QAudioSink(outputDevice, fmt, this);
    m_sink->setVolume(1.0f);
    m_sink->setBufferSize(std::max(4096, (sampleRate * 4) / 8));
    m_sinkDevice = m_sink->start();
    if (m_sinkDevice == nullptr)
    {
        m_sink->deleteLater();
        m_sink = nullptr;
        teardownSynth();
        return false;
    }

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MidiRecordingMonitor::tick);
    m_timer->start(15);
    return true;
}
#endif

void MidiRecordingMonitor::stop()
{
    if (m_timer)
    {
        m_timer->stop();
        m_timer->deleteLater();
        m_timer = nullptr;
    }
#ifdef QT_MULTIMEDIA_AVAILABLE
    m_sinkDevice = nullptr;
    if (m_sink)
    {
        m_sink->stop();
        m_sink->deleteLater();
        m_sink = nullptr;
    }
#endif
    {
        QMutexLocker lock(&m_mutex);
        m_messageQueue.clear();
    }
    teardownSynth();
}

void MidiRecordingMonitor::teardownSynth()
{
    if (m_synth)
    {
        for (int ch = 0; ch < 16; ++ch)
            fluid_synth_all_sounds_off(m_synth, ch);
        delete_fluid_synth(m_synth);
        m_synth = nullptr;
    }
    if (m_settings)
    {
        delete_fluid_settings(m_settings);
        m_settings = nullptr;
    }
}

void MidiRecordingMonitor::postMidiBytes(const std::vector<unsigned char>& msg)
{
    if (msg.empty() || m_synth == nullptr)
        return;
    QByteArray copy(static_cast<int>(msg.size()), Qt::Uninitialized);
    std::memcpy(copy.data(), msg.data(), msg.size());
    QMutexLocker lock(&m_mutex);
    m_messageQueue.push_back(std::move(copy));
}

void MidiRecordingMonitor::processMidiBytes(const unsigned char* d, int n)
{
    if (n < 1 || m_synth == nullptr)
        return;
    const unsigned char status = d[0];
    // System realtime / common — ignore for note paths
    if (status == 0xF8 || status == 0xFA || status == 0xFB || status == 0xFC || status == 0xFE)
        return;

    const int kind = status & 0xF0;
    const int channel = status & 0x0F;

    if (kind == 0x90 && n >= 3)
    {
        const int note = d[1] & 0x7F;
        const int vel = d[2] & 0x7F;
        if (vel > 0)
            fluid_synth_noteon(m_synth, channel, note, vel);
        else
            fluid_synth_noteoff(m_synth, channel, note);
    }
    else if (kind == 0x80 && n >= 3)
    {
        fluid_synth_noteoff(m_synth, channel, d[1] & 0x7F);
    }
    else if (kind == 0xB0 && n >= 3)
    {
        fluid_synth_cc(m_synth, channel, d[1] & 0x7F, d[2] & 0x7F);
    }
    else if (kind == 0xC0 && n >= 2)
    {
        fluid_synth_program_change(m_synth, channel, d[1] & 0x7F);
    }
    else if (kind == 0xE0 && n >= 3)
    {
        const int value = ((d[2] & 0x7F) << 7) | (d[1] & 0x7F);
        fluid_synth_pitch_bend(m_synth, channel, value);
    }
}

void MidiRecordingMonitor::tick()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    if (m_synth == nullptr || m_sink == nullptr || m_sinkDevice == nullptr)
        return;

    {
        QMutexLocker lock(&m_mutex);
        for (const QByteArray& msg : m_messageQueue)
            processMidiBytes(reinterpret_cast<const unsigned char*>(msg.constData()), msg.size());
        m_messageQueue.clear();
    }

    const qint64 room = static_cast<qint64>(m_sink->bytesFree());
    if (room < 8)
        return;

    int frames = static_cast<int>(qMin(qint64(512), room / 4LL));
    if (frames <= 0)
        return;

    std::vector<qint16> interleaved(static_cast<size_t>(frames) * 2u);
    fluid_synth_write_s16(m_synth, frames, interleaved.data(), 0, 2, interleaved.data(), 1, 2);
    m_sinkDevice->write(reinterpret_cast<const char*>(interleaved.data()),
                          static_cast<qint64>(frames) * 4);
#endif
}
