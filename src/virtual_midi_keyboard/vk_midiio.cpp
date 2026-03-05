#include "vk_midiio.h"
#include "vk_settings.h"
#include <RtMidi.h>
#include <fluidsynth.h>
#include <QMetaObject>
#include <algorithm>
#include <cstring>
#include <memory>

using std::make_shared;
using std::shared_ptr;

static void suppressFluidWarn(int, const char*, void*) {}

class VkMidiIo::Impl {
public:
    shared_ptr<RtMidiOut> midiOut;
    shared_ptr<RtMidiIn> midiIn;

    fluid_settings_t* fluidSettings = nullptr;
    fluid_synth_t* fluidSynth = nullptr;
    fluid_audio_driver_t* fluidDriver = nullptr;
    bool useFluidSynth = false;

    ~Impl() {
        cleanup();
    }

    void cleanup() {
        if (fluidDriver) {
            delete_fluid_audio_driver(fluidDriver);
            fluidDriver = nullptr;
        }
        if (fluidSynth) {
            delete_fluid_synth(fluidSynth);
            fluidSynth = nullptr;
        }
        if (fluidSettings) {
            delete_fluid_settings(fluidSettings);
            fluidSettings = nullptr;
        }
        if (midiOut) {
            try { midiOut->closePort(); } catch (...) {}
            midiOut.reset();
        }
        if (midiIn) {
            try { midiIn->closePort(); } catch (...) {}
            midiIn.reset();
        }
    }
};

static void midiCallback(double /*deltatime*/, std::vector<unsigned char>* message, void* userData) {
    auto* io = static_cast<VkMidiIo*>(userData);
    if (message->size() >= 2) {
        const unsigned char status = (*message)[0];
        const int note = (*message)[1];
        const int vel = message->size() > 2 ? (*message)[2] : 0;
        if ((status & 0xF0) == 0x90 && vel > 0)
            QMetaObject::invokeMethod(io, "forwardMidiNoteOn", Qt::QueuedConnection,
                Q_ARG(int, note), Q_ARG(int, vel));
        else if ((status & 0xF0) == 0x80 || ((status & 0xF0) == 0x90 && vel == 0))
            QMetaObject::invokeMethod(io, "forwardMidiNoteOff", Qt::QueuedConnection, Q_ARG(int, note));
    }
}

void VkMidiIo::forwardMidiNoteOn(int note, int velocity) {
    emit midiNoteOn(note, velocity);
}

void VkMidiIo::forwardMidiNoteOff(int note) {
    emit midiNoteOff(note);
}

VkMidiIo::VkMidiIo(QObject* parent) : QObject(parent) {
    fluid_set_log_function(FLUID_WARN, suppressFluidWarn, nullptr);
    m_impl = make_shared<Impl>();
}

VkMidiIo::~VkMidiIo() = default;

void VkMidiIo::setOutputDevice(int index) {
    m_impl->cleanup();
    m_outputOpen = false;

    if (index <= 0) {
        m_impl->fluidSettings = new_fluid_settings();
#if defined(_WIN32)
        fluid_settings_setstr(m_impl->fluidSettings, "audio.driver", "dsound");
#elif defined(__APPLE__)
        fluid_settings_setstr(m_impl->fluidSettings, "audio.driver", "coreaudio");
#else
        fluid_settings_setstr(m_impl->fluidSettings, "audio.driver", "pulseaudio");
#endif
        fluid_settings_setint(m_impl->fluidSettings, "audio.realtime-prio", 0);
        m_impl->fluidSynth = new_fluid_synth(m_impl->fluidSettings);
        const QString sfPath = VkSettings::instance().soundFontPath();
        if (!sfPath.isEmpty())
            fluid_synth_sfload(m_impl->fluidSynth, sfPath.toUtf8().constData(), 1);
        m_impl->fluidDriver = new_fluid_audio_driver(m_impl->fluidSettings, m_impl->fluidSynth);
        m_outputOpen = (m_impl->fluidDriver != nullptr);
    } else {
        try {
            auto out = make_shared<RtMidiOut>();
            if (const unsigned int portIndex = static_cast<unsigned>(index - 1);
                portIndex < out->getPortCount()) {
                out->openPort(portIndex);
                m_impl->midiOut = out;
                m_outputOpen = true;
            }
        } catch (...) {}
    }
    emit soundListChanged();
}

void VkMidiIo::setInputDevice(int index) {
    if (m_impl->midiIn) {
        try { m_impl->midiIn->closePort(); } catch (...) {}
        m_impl->midiIn.reset();
    }
    if (index < 0) return;
    try {
        auto in = make_shared<RtMidiIn>();
        if (static_cast<unsigned>(index) < in->getPortCount()) {
            in->openPort(index);
            in->setCallback(&midiCallback, this);
            m_impl->midiIn = in;
        }
    } catch (...) {}
}

void VkMidiIo::setSoundFont(const QString& path) {
    if (m_impl->fluidSynth && !path.isEmpty()) {
        fluid_synth_sfload(m_impl->fluidSynth, path.toUtf8().constData(), 1);
        emit soundListChanged();
    }
}

void VkMidiIo::setAudioOutputDevice(const QByteArray& /*deviceId*/) {
}

bool VkMidiIo::isUsingFluidSynth() const {
    return m_impl && m_impl->fluidSynth != nullptr;
}

QList<VkMidiIo::SoundPreset> VkMidiIo::getPresets() const {
    QList<SoundPreset> presets;
    if (!m_impl->fluidSynth) return presets;
    fluid_sfont_t* sf = fluid_synth_get_sfont(m_impl->fluidSynth, 0);
    if (!sf) return presets;
    fluid_sfont_iteration_start(sf);
    fluid_preset_t* preset;
    while ((preset = fluid_sfont_iteration_next(sf)) != nullptr) {
        SoundPreset p;
        p.bank    = fluid_preset_get_banknum(preset);
        p.program = fluid_preset_get_num(preset);
        p.name    = QString::fromUtf8(fluid_preset_get_name(preset));
        presets.append(p);
    }
    std::sort(presets.begin(), presets.end(), [](const SoundPreset& a, const SoundPreset& b) {
        return a.bank != b.bank ? a.bank < b.bank : a.program < b.program;
    });
    return presets;
}

void VkMidiIo::selectPreset(int bank, int program) {
    if (m_impl->fluidSynth) {
        fluid_synth_bank_select(m_impl->fluidSynth, 0, static_cast<unsigned int>(bank));
        fluid_synth_program_change(m_impl->fluidSynth, 0, program);
    } else if (m_impl->midiOut) {
        std::vector<unsigned char> msg;
        msg = { 0xB0, 0x00, static_cast<unsigned char>((bank >> 7) & 0x7F) };
        m_impl->midiOut->sendMessage(&msg);
        msg = { 0xB0, 0x20, static_cast<unsigned char>(bank & 0x7F) };
        m_impl->midiOut->sendMessage(&msg);
        msg = { 0xC0, static_cast<unsigned char>(program & 0x7F) };
        m_impl->midiOut->sendMessage(&msg);
    }
}

void VkMidiIo::sendNoteOn(int note, int velocity) {
    if (m_impl->midiOut) {
        std::vector<unsigned char> msg = { 0x90, static_cast<unsigned char>(note & 0x7F),
            static_cast<unsigned char>(velocity & 0x7F) };
        m_impl->midiOut->sendMessage(&msg);
    } else if (m_impl->fluidSynth) {
        fluid_synth_noteon(m_impl->fluidSynth, 0, note, velocity);
    }
}

void VkMidiIo::sendNoteOff(int note) {
    if (m_impl->midiOut) {
        std::vector<unsigned char> msg = { 0x80, static_cast<unsigned char>(note & 0x7F), 0 };
        m_impl->midiOut->sendMessage(&msg);
    } else if (m_impl->fluidSynth) {
        fluid_synth_noteoff(m_impl->fluidSynth, 0, note);
    }
}

void VkMidiIo::sendProgramChange(int program) {
    if (m_impl->midiOut) {
        std::vector<unsigned char> msg = { 0xC0, static_cast<unsigned char>(program & 0x7F) };
        m_impl->midiOut->sendMessage(&msg);
    } else if (m_impl->fluidSynth) {
        fluid_synth_program_change(m_impl->fluidSynth, 0, program);
    }
}

void VkMidiIo::sendControlChange(int controller, int value) {
    if (m_impl->midiOut) {
        std::vector<unsigned char> msg = { 0xB0, static_cast<unsigned char>(controller & 0x7F),
            static_cast<unsigned char>(value & 0x7F) };
        m_impl->midiOut->sendMessage(&msg);
    } else if (m_impl->fluidSynth) {
        fluid_synth_cc(m_impl->fluidSynth, 0, controller, value);
    }
}
