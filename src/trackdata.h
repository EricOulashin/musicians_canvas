#ifndef TRACKDATA_H
#define TRACKDATA_H

#include <QString>
#include <QVector>
#include <QByteArray>
#include <QJsonArray>

enum class TrackType
{
    Audio,
    MIDI
};

struct MidiNote
{
    int note;       // MIDI note number (0-127)
    int velocity;
    double startTime;  // in seconds
    double duration;  // in seconds
    // 0-based MIDI channel (0..15).  Channel 9 is the General MIDI standard
    // drum channel — notes captured from a drum controller arrive on this
    // channel and must be rendered there for the percussion bank to be
    // selected.  Default 0 (= MIDI channel 1) for legacy data that was
    // recorded before the channel field existed.
    int channel = 0;
};

struct TrackData
{
    int id = -1;
    QString name;
    TrackType type = TrackType::Audio;
    bool enabled = true;
    bool armed = false;

    // For audio tracks: raw PCM data (16-bit stereo, 44100 Hz)
    QByteArray audioData;
    int sampleRate = 44100;
    int channelCount = 2;

    // Ordered chain of real-time recording effects (see EffectsDialog / EffectWidget).
    // Each element: { "type": "reverb"|"chorus", "params": { ... } }
    QJsonArray audioEffectChain;

    // For MIDI tracks: note events
    QVector<MidiNote> midiNotes;
    double lengthSeconds = 0;  // Track length in seconds

    [[nodiscard]] double length() const
    {
        if (type == TrackType::Audio)
        {
            int sampleCount = audioData.size() / (channelCount * 2);  // 16-bit = 2 bytes
            return sampleCount / (double)sampleRate;
        }
        else
        {
            return lengthSeconds;
        }
    }
};

#endif // TRACKDATA_H
