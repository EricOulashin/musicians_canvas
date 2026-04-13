#include "midisynth.h"
#include "appsettings.h"
#include <fluidsynth.h>
#include <memory>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
#include <algorithm>
#include <cstring>
#include <cmath>

// Common system SoundFont paths to try when neither the project nor the
// app default SoundFont is configured (or is unreadable).  Mirrors the
// list used by virtual_midi_keyboard's vk_midiio.cpp.
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
        // Windows (MSYS2 default install)
        "C:/msys64/mingw64/share/soundfonts/FluidR3_GM.sf2",
        "C:/Program Files/Musicians_Canvas/FluidR3_GM.sf2",
        nullptr
    };
    for (int i = 0; s_sfPaths[i] != nullptr; ++i)
    {
        if (QFile::exists(QString::fromUtf8(s_sfPaths[i])))
            return QString::fromUtf8(s_sfPaths[i]);
    }
    return {};
}

using std::make_shared;
using std::shared_ptr;

class MidiSynth::Impl
{
public:
    fluid_settings_t* settings = nullptr;
    fluid_synth_t* synth = nullptr;

    ~Impl()
    {
        if (synth)
        {
            delete_fluid_synth(synth);
            synth = nullptr;
        }
        if (settings)
        {
            delete_fluid_settings(settings);
            settings = nullptr;
        }
    }
};

static void writeWavHeader(QFile& file, quint32 sampleRate, quint32 numSamples)
{
    quint32 byteRate = sampleRate * 4;  // 2 channels * 2 bytes
    quint32 dataSize = numSamples * 4;
    quint32 fileSize = 36 + dataSize;

    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&fileSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    quint32 fmtSize = 16;
    file.write(reinterpret_cast<const char*>(&fmtSize), 4);
    quint16 audioFormat = 1;  // PCM
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    quint16 numChannels = 2;
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    quint16 blockAlign = 4;
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    quint16 bitsPerSample = 16;
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);
}

MidiSynth::MidiSynth()
{
    m_impl = make_shared<Impl>();
    m_impl->settings = new_fluid_settings();
}

MidiSynth::~MidiSynth() = default;

bool MidiSynth::loadSoundFont(const QString& path)
{
    if (!m_impl->synth)
    {
        m_impl->synth = new_fluid_synth(m_impl->settings);
    }
    return fluid_synth_sfload(m_impl->synth, path.toUtf8().constData(), 1) != FLUID_FAILED;
}

bool MidiSynth::renderMidiToWav(const QVector<MidiNote>& notes, double lengthSeconds,
                                const QString& outputPath, int sampleRate,
                                const QString& soundFontPath,
                                double gainMultiplier)
{
    return renderMidiToWav(notes, {}, lengthSeconds, outputPath, sampleRate, soundFontPath,
                           gainMultiplier);
}

bool MidiSynth::renderMidiToWav(const QVector<MidiNote>& notes,
                                const QVector<MidiControlChange>& controlChanges,
                                double lengthSeconds,
                                const QString& outputPath, int sampleRate,
                                const QString& soundFontPath,
                                double gainMultiplier)
{
    if (notes.isEmpty() && controlChanges.isEmpty() && lengthSeconds <= 0) return true;

    double maxTime = lengthSeconds;
    for (const auto& n : notes)
    {
        double end = n.startTime + n.duration;
        if (end > maxTime) maxTime = end;
    }
    for (const auto& c : controlChanges)
    {
        if (c.timeSec > maxTime) maxTime = c.timeSec;
    }
    if (maxTime < 0.01) maxTime = 1.0;

    fluid_settings_t* settings = new_fluid_settings();
    fluid_settings_setnum(settings, "synth.sample-rate", static_cast<double>(sampleRate));
    fluid_settings_setint(settings, "synth.lock-memory", 0);
    // Raise the master gain from FluidSynth's conservative default of 0.2.
    // This is further adjusted by gainMultiplier (used for playback-only MIDI volume).
    const double gm = std::clamp(gainMultiplier, 0.0, 2.0);
    fluid_settings_setnum(settings, "synth.gain", 0.8 * gm);

    fluid_synth_t* synth = new_fluid_synth(settings);

    // Resolve which SoundFont to load, in priority order:
    //   1. The project-specific SoundFont passed in
    //   2. The application default from AppSettings
    //   3. A system-installed SoundFont (Debian/Ubuntu/Fedora/Arch/Windows)
    //
    // If a configured path exists but fails to load, fall through to the
    // next option instead of giving up.  Even if all attempts fail we still
    // produce a valid (silent) output WAV so the MIDI track is preserved
    // in the timeline of the final mix; the previous behaviour was to
    // return false, which made mixTracksToFile silently DROP the entire
    // MIDI track from the mix.
    auto tryLoadSf = [synth](const QString& path) -> bool {
        if (path.isEmpty()) return false;
        if (!QFile::exists(path)) return false;
        return fluid_synth_sfload(synth, path.toUtf8().constData(), 1) != FLUID_FAILED;
    };

    bool sfLoaded = false;
    if (!soundFontPath.isEmpty())
        sfLoaded = tryLoadSf(soundFontPath);
    if (!sfLoaded)
        sfLoaded = tryLoadSf(AppSettings::instance().soundFontPath());
    if (!sfLoaded)
        sfLoaded = tryLoadSf(findSystemSoundFont());
    if (!sfLoaded)
    {
        // No SoundFont available — render silence so the MIDI track still
        // appears at the right place in the mix timeline.
        qWarning("MidiSynth::renderMidiToWav: no SoundFont could be loaded; "
                 "MIDI track will render as silence");
    }

    const int blockSize = 64;
    const int totalSamples = static_cast<int>(maxTime * sampleRate * 2);
    QVector<float> leftBuf(blockSize), rightBuf(blockSize);
    QVector<qint16> outputBuffer;

    auto noteCompare = [](const MidiNote& a, const MidiNote& b) {
        return a.startTime < b.startTime;
    };
    QVector<MidiNote> sortedNotes = notes;
    std::sort(sortedNotes.begin(), sortedNotes.end(), noteCompare);

    enum class EvKind { NoteOn, NoteOff, Cc };
    struct SynthEvent
    {
        double time;
        EvKind kind;
        int a = 0;
        int b = 0;
        int c = 0;
    };
    QVector<SynthEvent> events;
    for (const auto& n : sortedNotes)
    {
        events.append({n.startTime, EvKind::NoteOn, n.note, n.velocity, n.channel});
        events.append({n.startTime + n.duration, EvKind::NoteOff, n.note, 0, n.channel});
    }
    for (const auto& cc : controlChanges)
    {
        events.append({cc.timeSec, EvKind::Cc, cc.controller, cc.value, cc.channel});
    }
    std::sort(events.begin(), events.end(), [](const SynthEvent& a, const SynthEvent& b)
    {
        return a.time < b.time;
    });

    size_t eventIdx = 0;
    int samplesRendered = 0;
    int totalSamplesToRender = static_cast<int>(maxTime * sampleRate);

    while (samplesRendered < totalSamplesToRender)
    {
        double currentTime = samplesRendered / (double)sampleRate;
        double blockEndTime = currentTime + blockSize / (double)sampleRate;
        while (eventIdx < events.size() && events[eventIdx].time <= blockEndTime)
        {
            const auto& e = events[eventIdx];
            const int ch = (e.c >= 0 && e.c <= 15) ? e.c : 0;
            if (e.kind == EvKind::NoteOn)
            {
                fluid_synth_noteon(synth, ch, e.a, std::clamp(e.b, 0, 127));
            }
            else if (e.kind == EvKind::NoteOff)
            {
                fluid_synth_noteoff(synth, ch, e.a);
            }
            else if (e.kind == EvKind::Cc)
            {
                fluid_synth_cc(synth, ch, std::clamp(e.a, 0, 127), std::clamp(e.b, 0, 127));
            }
            eventIdx++;
        }
        fluid_synth_write_float(synth, blockSize, leftBuf.data(), 0, 1, rightBuf.data(), 0, 1);
        for (int i = 0; i < blockSize; ++i)
        {
            qint16 l = static_cast<qint16>(qBound(-32768.0f, leftBuf[i] * 32767.0f, 32767.0f));
            qint16 r = static_cast<qint16>(qBound(-32768.0f, rightBuf[i] * 32767.0f, 32767.0f));
            outputBuffer.append(l);
            outputBuffer.append(r);
        }
        samplesRendered += blockSize;
    }

    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly))
    {
        delete_fluid_synth(synth);
        delete_fluid_settings(settings);
        return false;
    }
    writeWavHeader(file, sampleRate, outputBuffer.size() / 2);
    file.write(reinterpret_cast<const char*>(outputBuffer.data()), outputBuffer.size() * 2);
    file.close();

    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    return true;
}

bool MidiSynth::renderMidiFileToWav(const QString& midiPath, const QString& outputPath,
                                    int sampleRate, const QString& soundFontPath)
{
    fluid_settings_t* settings = new_fluid_settings();
    fluid_settings_setstr(settings, "audio.file.name", outputPath.toUtf8().constData());
    fluid_settings_setstr(settings, "player.timing-source", "sample");
    fluid_settings_setnum(settings, "synth.sample-rate", static_cast<double>(sampleRate));
    fluid_settings_setint(settings, "synth.lock-memory", 0);

    fluid_synth_t* synth = new_fluid_synth(settings);
    QString sfPath = soundFontPath.isEmpty() ? AppSettings::instance().soundFontPath()
                                             : soundFontPath;
    if (!sfPath.isEmpty())
    {
        if (fluid_synth_sfload(synth, sfPath.toUtf8().constData(), 1) == FLUID_FAILED)
        {
            delete_fluid_synth(synth);
            delete_fluid_settings(settings);
            return false;
        }
    }

    fluid_player_t* player = new_fluid_player(synth);
    fluid_player_add(player, midiPath.toUtf8().constData());
    fluid_player_play(player);

    fluid_file_renderer_t* renderer = new_fluid_file_renderer(synth);

    while (fluid_player_get_status(player) == FLUID_PLAYER_PLAYING)
    {
        if (fluid_file_renderer_process_block(renderer) != FLUID_OK) break;
    }

    fluid_player_stop(player);
    fluid_player_join(player);

    delete_fluid_file_renderer(renderer);
    delete_fluid_player(player);
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    return true;
}
