#include "vk_midiio.h"
#include "vk_settings.h"
#include <RtMidi.h>
#include <fluidsynth.h>
#include <QMetaObject>
#include <QFile>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <vector>
#include <memory>

#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QAudioSink>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QIODevice>
#endif

using std::make_shared;
using std::shared_ptr;

static void suppressFluidWarn(int, const char*, void*) {}

// ---------------------------------------------------------------------------
// FluidSynthAudioDevice
//
// A QIODevice that pulls rendered audio from a fluid_synth_t on every
// readData() call.  This lets QAudioSink drive the audio loop directly
// rather than relying on FluidSynth's built-in audio-driver backends
// (which have been unreliable on PipeWire/PulseAudio systems).
// ---------------------------------------------------------------------------
#ifdef QT_MULTIMEDIA_AVAILABLE
class FluidSynthAudioDevice : public QIODevice
{
public:
    FluidSynthAudioDevice(fluid_synth_t* synth, int channels,
                          QAudioFormat::SampleFormat fmt, QObject* parent)
        : QIODevice(parent), m_synth(synth), m_channels(channels), m_sampleFormat(fmt)
    {}

    qint64 readData(char* data, qint64 maxSize) override
    {
        // Dispatch to the appropriate FluidSynth render function based on the
        // device's native sample format.  Using the wrong format causes Qt's
        // audio backend to receive bit patterns it misinterprets — the most
        // common symptom is audio that is nearly silent regardless of gain.
        const int bytesPerSample = sampleBytes();
        const int bytesPerFrame  = m_channels * bytesPerSample;
        const int numFrames      = static_cast<int>(maxSize) / bytesPerFrame;
        if (numFrames <= 0)
            return 0;

        if (m_sampleFormat == QAudioFormat::Float)
        {
            // Render directly into the output buffer as interleaved float32
            auto* out = reinterpret_cast<float*>(data);
            if (m_channels >= 2)
                fluid_synth_write_float(m_synth, numFrames, out, 0, 2, out, 1, 2);
            else
                fluid_synth_write_float(m_synth, numFrames, out, 0, 1, out, 0, 1);
        }
        else if (m_sampleFormat == QAudioFormat::Int16)
        {
            // Render as interleaved int16 — FluidSynth applies gain internally
            auto* out = reinterpret_cast<qint16*>(data);
            if (m_channels >= 2)
                fluid_synth_write_s16(m_synth, numFrames, out, 0, 2, out, 1, 2);
            else
                fluid_synth_write_s16(m_synth, numFrames, out, 0, 1, out, 0, 1);
        }
        else
        {
            // Int32 (S32LE) and any other format: render as float into a temp
            // buffer, then scale to full-range int32.  Hardware that reports
            // S32LE as its preferred format (e.g. Built-in Audio on many Linux
            // systems) needs this path; forcing Float32 on such a device causes
            // the Qt/FFmpeg backend to silently produce incorrect output.
            std::vector<float> tmp(static_cast<size_t>(numFrames * m_channels));
            float* lPtr = tmp.data();
            float* rPtr = (m_channels >= 2) ? tmp.data() + 1 : tmp.data();
            const int stride = (m_channels >= 2) ? 2 : 1;
            fluid_synth_write_float(m_synth, numFrames, lPtr, 0, stride, rPtr, 0, stride);

            auto* out = reinterpret_cast<qint32*>(data);
            for (int i = 0; i < numFrames * m_channels; ++i)
            {
                const float f = std::max(-1.0f, std::min(1.0f, tmp[static_cast<size_t>(i)]));
                out[i] = static_cast<qint32>(f * 2147483647.0f);
            }
        }
        return numFrames * bytesPerFrame;
    }

    qint64 writeData(const char* /*data*/, qint64 /*len*/) override { return 0; }
    bool   isSequential() const override { return true; }

    int sampleBytes() const
    {
        switch (m_sampleFormat)
        {
        case QAudioFormat::UInt8:  return 1;
        case QAudioFormat::Int16:  return 2;
        case QAudioFormat::Int32:  return 4;
        case QAudioFormat::Float:  return 4;
        default:                   return 4;
        }
    }

private:
    fluid_synth_t*             m_synth;
    int                        m_channels;
    QAudioFormat::SampleFormat m_sampleFormat;
};
#endif // QT_MULTIMEDIA_AVAILABLE

// ---------------------------------------------------------------------------

class VkMidiIo::Impl
{
public:
    shared_ptr<RtMidiOut> midiOut;
    shared_ptr<RtMidiIn>  midiIn;

    fluid_settings_t*    fluidSettings = nullptr;
    fluid_synth_t*       fluidSynth    = nullptr;
    fluid_audio_driver_t* fluidDriver  = nullptr;  // used only when Qt audio is unavailable

#ifdef QT_MULTIMEDIA_AVAILABLE
    QAudioSink*           audioSink    = nullptr;
    FluidSynthAudioDevice* audioDevice = nullptr;
#endif

    bool useFluidSynth = false;

    ~Impl() { cleanup(); }

    void cleanup()
    {
#ifdef QT_MULTIMEDIA_AVAILABLE
        if (audioSink)
        {
            audioSink->stop();
            delete audioSink;
            audioSink = nullptr;
        }
        if (audioDevice)
        {
            audioDevice->close();
            delete audioDevice;
            audioDevice = nullptr;
        }
#endif
        if (fluidDriver)
        {
            delete_fluid_audio_driver(fluidDriver);
            fluidDriver = nullptr;
        }
        if (fluidSynth)
        {
            delete_fluid_synth(fluidSynth);
            fluidSynth = nullptr;
        }
        if (fluidSettings)
        {
            delete_fluid_settings(fluidSettings);
            fluidSettings = nullptr;
        }
        if (midiOut)
        {
            try { midiOut->closePort(); } catch (...) {}
            midiOut.reset();
        }
        if (midiIn)
        {
            try { midiIn->closePort(); } catch (...) {}
            midiIn.reset();
        }
    }
};

static void midiCallback(double /*deltatime*/, std::vector<unsigned char>* message, void* userData)
{
    auto* io = static_cast<VkMidiIo*>(userData);
    if (message->size() >= 2)
    {
        const unsigned char status = (*message)[0];
        const int note = (*message)[1];
        const int vel  = message->size() > 2 ? (*message)[2] : 0;
        if ((status & 0xF0) == 0x90 && vel > 0)
            QMetaObject::invokeMethod(io, "forwardMidiNoteOn", Qt::QueuedConnection,
                Q_ARG(int, note), Q_ARG(int, vel));
        else if ((status & 0xF0) == 0x80 || ((status & 0xF0) == 0x90 && vel == 0))
            QMetaObject::invokeMethod(io, "forwardMidiNoteOff", Qt::QueuedConnection,
                Q_ARG(int, note));
    }
}

void VkMidiIo::forwardMidiNoteOn(int note, int velocity) { emit midiNoteOn(note, velocity); }
void VkMidiIo::forwardMidiNoteOff(int note)               { emit midiNoteOff(note); }

VkMidiIo::VkMidiIo(QObject* parent) : QObject(parent)
{
    fluid_set_log_function(FLUID_WARN, suppressFluidWarn, nullptr);
    m_impl = make_shared<Impl>();
}

VkMidiIo::~VkMidiIo() = default;

// Common system SoundFont paths to try when none is configured
static QString findSystemSoundFont()
{
    static const char* const s_sfPaths[] = {
        // Debian/Ubuntu
        "/usr/share/sounds/sf2/default-GM.sf2",
        "/usr/share/sounds/sf2/default.sf2",
        "/usr/share/sounds/sf2/FluidR3_GM.sf2",
        "/usr/share/sounds/sf2/TimGM6mb.sf2",
        // Fedora/RHEL
        "/usr/share/soundfonts/default.sf2",
        "/usr/share/soundfonts/FluidR3_GM.sf2",
        // Arch Linux
        "/usr/share/soundfonts/freepats-general-midi.sf2",
        nullptr
    };
    for (int i = 0; s_sfPaths[i] != nullptr; ++i)
    {
        if (QFile::exists(QString::fromUtf8(s_sfPaths[i])))
            return QString::fromUtf8(s_sfPaths[i]);
    }
    return {};
}

void VkMidiIo::setOutputDevice(const QString& portName)
{
    m_impl->cleanup();
    m_outputOpen = false;

    if (portName.isEmpty())
    {
        // ----------------------------------------------------------------
        // Built-in FluidSynth synthesiser
        // ----------------------------------------------------------------

#ifdef QT_MULTIMEDIA_AVAILABLE
        // Use the device's own preferred format so Qt's audio backend receives
        // the exact sample type it expects.  Forcing a different format (e.g.
        // Float32 on a device that reports S32LE) can cause the backend to
        // produce near-silent output due to incorrect format negotiation.
        const QAudioDevice qtOutDev = QMediaDevices::defaultAudioOutput();
        const QAudioFormat qtFmt    = qtOutDev.preferredFormat();
        const int synthSampleRate   = qtFmt.sampleRate() > 0 ? qtFmt.sampleRate() : 48000;
        const int synthChannels     = qtFmt.channelCount() > 0 ? qtFmt.channelCount() : 2;
        const QAudioFormat::SampleFormat synthSampleFmt = qtFmt.sampleFormat();
#else
        const int synthSampleRate = 44100;
        const int synthChannels   = 2;
        Q_UNUSED(synthChannels)
#endif

        m_impl->fluidSettings = new_fluid_settings();
        fluid_settings_setint(m_impl->fluidSettings, "audio.realtime-prio", 0);
        // Set sample rate before creating the synth
        fluid_settings_setnum(m_impl->fluidSettings, "synth.sample-rate",
                              static_cast<double>(synthSampleRate));
        // Raise the master gain from FluidSynth's conservative default of 0.2
        fluid_settings_setnum(m_impl->fluidSettings, "synth.gain",
                              static_cast<double>(VkSettings::instance().synthGain()));

        m_impl->fluidSynth = new_fluid_synth(m_impl->fluidSettings);

        // Load configured SoundFont, or search for a system one
        QString sfPath = VkSettings::instance().soundFontPath();
        if (sfPath.isEmpty())
            sfPath = findSystemSoundFont();
        if (!sfPath.isEmpty())
        {
            const int sfId = fluid_synth_sfload(
                m_impl->fluidSynth, sfPath.toUtf8().constData(), 1);
            if (sfId != FLUID_FAILED && VkSettings::instance().soundFontPath().isEmpty())
            {
                VkSettings::instance().setSoundFontPath(sfPath);
                VkSettings::instance().save();
            }
        }

#ifdef QT_MULTIMEDIA_AVAILABLE
        // ── Qt audio path ──────────────────────────────────────────────
        // Create a QIODevice that pulls PCM frames from FluidSynth.
        // QAudioSink calls readData() from its internal audio thread at
        // the rate needed to fill the hardware output buffer.
        m_impl->audioDevice = new FluidSynthAudioDevice(
            m_impl->fluidSynth, synthChannels, synthSampleFmt, nullptr);
        m_impl->audioDevice->open(QIODevice::ReadOnly);

        m_impl->audioSink = new QAudioSink(qtOutDev, qtFmt, nullptr);
        m_impl->audioSink->setVolume(1.0f);
        // ~100 ms of buffering — enough to avoid underruns without audible lag
        const int bytesPerSample = m_impl->audioDevice->sampleBytes();
        m_impl->audioSink->setBufferSize(
            synthSampleRate / 10 * synthChannels * bytesPerSample);
        m_impl->audioSink->start(m_impl->audioDevice);
#else
        // ── FluidSynth audio-driver fallback ───────────────────────────
        // Enumerate drivers this FluidSynth build actually supports so we
        // never request one that will fail (e.g. "pipewire-pulse").
        QStringList supported;
        fluid_settings_foreach_option(
            m_impl->fluidSettings, "audio.driver", &supported,
            [](void* data, const char* /*name*/, const char* option)
            {
                static_cast<QStringList*>(data)->append(QString::fromUtf8(option));
            });

        static const char* const s_preferred[] = {
#if defined(_WIN32)
            "wasapi", "dsound", "winmm",
#elif defined(__APPLE__)
            "coreaudio",
#else
            "pipewire", "pulseaudio", "alsa", "jack", "oss",
#endif
            nullptr
        };
        for (int di = 0; s_preferred[di] != nullptr; ++di)
        {
            if (!supported.contains(QString::fromUtf8(s_preferred[di])))
                continue;
            fluid_settings_setstr(m_impl->fluidSettings, "audio.driver", s_preferred[di]);
            m_impl->fluidDriver = new_fluid_audio_driver(
                m_impl->fluidSettings, m_impl->fluidSynth);
            if (m_impl->fluidDriver) break;
        }
#endif // QT_MULTIMEDIA_AVAILABLE

        m_outputOpen = (m_impl->fluidSynth != nullptr);
    }
    else
    {
        // ----------------------------------------------------------------
        // External MIDI output port — locate by name (stable across sessions)
        // ----------------------------------------------------------------
        try
        {
            auto out = make_shared<RtMidiOut>();
            const unsigned int n = out->getPortCount();
            for (unsigned int i = 0; i < n; ++i)
            {
                if (QString::fromStdString(out->getPortName(i)) == portName)
                {
                    out->openPort(i);
                    m_impl->midiOut = out;
                    m_outputOpen = true;
                    break;
                }
            }
        }
        catch (...) {}
    }
    emit soundListChanged();
}

void VkMidiIo::setInputDevice(const QString& portName)
{
    if (m_impl->midiIn)
    {
        try { m_impl->midiIn->closePort(); } catch (...) {}
        m_impl->midiIn.reset();
    }
    if (portName.isEmpty()) return;
    try
    {
        auto in = make_shared<RtMidiIn>();
        const unsigned int n = in->getPortCount();
        for (unsigned int i = 0; i < n; ++i)
        {
            if (QString::fromStdString(in->getPortName(i)) == portName)
            {
                in->openPort(i);
                in->setCallback(&midiCallback, this);
                m_impl->midiIn = in;
                break;
            }
        }
    }
    catch (...) {}
}

void VkMidiIo::setSoundFont(const QString& path)
{
    if (m_impl->fluidSynth && !path.isEmpty())
    {
        fluid_synth_sfload(m_impl->fluidSynth, path.toUtf8().constData(), 1);
        emit soundListChanged();
    }
}

void VkMidiIo::setAudioOutputDevice(const QByteArray& /*deviceId*/)
{
}

void VkMidiIo::setSynthGain(float gain)
{
    if (m_impl && m_impl->fluidSynth)
        fluid_synth_set_gain(m_impl->fluidSynth, gain);
}

bool VkMidiIo::isUsingFluidSynth() const
{
    return m_impl && m_impl->fluidSynth != nullptr;
}

bool VkMidiIo::isAudioDriverRunning() const
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    if (m_impl && m_impl->audioSink)
        return m_impl->audioSink->state() != QAudio::StoppedState;
#endif
    return m_impl && m_impl->fluidDriver != nullptr;
}

bool VkMidiIo::hasSoundFont() const
{
    if (!m_impl || !m_impl->fluidSynth) return false;
    return fluid_synth_get_sfont(m_impl->fluidSynth, 0) != nullptr;
}

QList<VkMidiIo::SoundPreset> VkMidiIo::getPresets() const
{
    QList<SoundPreset> presets;
    if (!m_impl->fluidSynth) return presets;
    fluid_sfont_t* sf = fluid_synth_get_sfont(m_impl->fluidSynth, 0);
    if (!sf) return presets;
    fluid_sfont_iteration_start(sf);
    fluid_preset_t* preset;
    while ((preset = fluid_sfont_iteration_next(sf)) != nullptr)
    {
        SoundPreset p;
        p.bank    = fluid_preset_get_banknum(preset);
        p.program = fluid_preset_get_num(preset);
        p.name    = QString::fromUtf8(fluid_preset_get_name(preset));
        presets.append(p);
    }
    std::sort(presets.begin(), presets.end(), [](const SoundPreset& a, const SoundPreset& b)
    {
        return a.bank != b.bank ? a.bank < b.bank : a.program < b.program;
    });
    return presets;
}

void VkMidiIo::selectPreset(int bank, int program)
{
    if (m_impl->fluidSynth)
    {
        fluid_synth_bank_select(m_impl->fluidSynth, 0,
                                static_cast<unsigned int>(bank));
        fluid_synth_program_change(m_impl->fluidSynth, 0, program);
    }
    else if (m_impl->midiOut)
    {
        std::vector<unsigned char> msg;
        msg = { 0xB0, 0x00, static_cast<unsigned char>((bank >> 7) & 0x7F) };
        m_impl->midiOut->sendMessage(&msg);
        msg = { 0xB0, 0x20, static_cast<unsigned char>(bank & 0x7F) };
        m_impl->midiOut->sendMessage(&msg);
        msg = { 0xC0, static_cast<unsigned char>(program & 0x7F) };
        m_impl->midiOut->sendMessage(&msg);
    }
}

void VkMidiIo::sendNoteOn(int note, int velocity)
{
    if (m_impl->midiOut)
    {
        std::vector<unsigned char> msg = {
            0x90,
            static_cast<unsigned char>(note     & 0x7F),
            static_cast<unsigned char>(velocity & 0x7F)
        };
        m_impl->midiOut->sendMessage(&msg);
    }
    else if (m_impl->fluidSynth)
    {
        fluid_synth_noteon(m_impl->fluidSynth, 0, note, velocity);
    }
}

void VkMidiIo::sendNoteOff(int note)
{
    if (m_impl->midiOut)
    {
        std::vector<unsigned char> msg = {
            0x80,
            static_cast<unsigned char>(note & 0x7F),
            0
        };
        m_impl->midiOut->sendMessage(&msg);
    }
    else if (m_impl->fluidSynth)
    {
        fluid_synth_noteoff(m_impl->fluidSynth, 0, note);
    }
}

void VkMidiIo::sendProgramChange(int program)
{
    if (m_impl->midiOut)
    {
        std::vector<unsigned char> msg = {
            0xC0,
            static_cast<unsigned char>(program & 0x7F)
        };
        m_impl->midiOut->sendMessage(&msg);
    }
    else if (m_impl->fluidSynth)
    {
        fluid_synth_program_change(m_impl->fluidSynth, 0, program);
    }
}

void VkMidiIo::sendControlChange(int controller, int value)
{
    if (m_impl->midiOut)
    {
        std::vector<unsigned char> msg = {
            0xB0,
            static_cast<unsigned char>(controller & 0x7F),
            static_cast<unsigned char>(value      & 0x7F)
        };
        m_impl->midiOut->sendMessage(&msg);
    }
    else if (m_impl->fluidSynth)
    {
        fluid_synth_cc(m_impl->fluidSynth, 0, controller, value);
    }
}
