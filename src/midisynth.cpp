#include "midisynth.h"
#include "appsettings.h"
#include <fluidsynth.h>
#include <memory>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <algorithm>
#include <cstring>

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
                                const QString& soundFontPath)
{
    if (notes.isEmpty() && lengthSeconds <= 0) return true;

    double maxTime = lengthSeconds;
    for (const auto& n : notes)
    {
        double end = n.startTime + n.duration;
        if (end > maxTime) maxTime = end;
    }
    if (maxTime < 0.01) maxTime = 1.0;

    fluid_settings_t* settings = new_fluid_settings();
    fluid_settings_setint(settings, "synth.sample-rate", sampleRate);
    fluid_settings_setint(settings, "synth.lock-memory", 0);

    fluid_synth_t* synth = new_fluid_synth(settings);
    // Use project-specific SoundFont if provided; fall back to app default
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

    const int blockSize = 64;
    const int totalSamples = static_cast<int>(maxTime * sampleRate * 2);
    QVector<float> leftBuf(blockSize), rightBuf(blockSize);
    QVector<qint16> outputBuffer;

    auto noteCompare = [](const MidiNote& a, const MidiNote& b) {
        return a.startTime < b.startTime;
    };
    QVector<MidiNote> sortedNotes = notes;
    std::sort(sortedNotes.begin(), sortedNotes.end(), noteCompare);

    struct NoteEvent
    {
        double time;
        int note;
        int vel;
        bool on;
    };
    QVector<NoteEvent> events;
    for (const auto& n : sortedNotes)
    {
        events.append({n.startTime, n.note, n.velocity, true});
        events.append({n.startTime + n.duration, n.note, 0, false});
    }
    std::sort(events.begin(), events.end(), [](const NoteEvent& a, const NoteEvent& b)
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
            if (e.on)
            {
                fluid_synth_noteon(synth, 0, e.note, e.vel);
            }
            else
            {
                fluid_synth_noteoff(synth, 0, e.note);
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
    fluid_settings_setint(settings, "synth.sample-rate", sampleRate);
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
