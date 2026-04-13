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

/// Channel control change (0xB0), stored with time in seconds for project/FluidSynth rendering.
struct MidiControlChange
{
    double timeSec = 0;
    int channel = 0; // 0..15
    int controller = 0;  // 0..127
    int value = 0;       // 0..127
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
    // Each element: { "type": "reverb"|"chorus"|"flanger"|"overdrive_distortion"|"amp_cabinet"|"vibrato", "params": { ... } }
    QJsonArray audioEffectChain;

    // For MIDI tracks: note events
    QVector<MidiNote> midiNotes;
    /// Expression, modulation, sustain, etc. — applied during offline render and saved to SMF.
    QVector<MidiControlChange> midiControlChanges;
    double lengthSeconds = 0;  // Track length in seconds

    /// Mixer gain in decibels (about -60 to +12), default 0.
    float gainDb = 0.f;
    /// Stereo pan: -1 = full left, 0 = center, +1 = full right.
    float pan = 0.f;
    /// When true, track is silent in the mix (track may still be enabled in the arrange view).
    bool mute = false;
    /// When any enabled track has solo, only soloed tracks are heard (mute still wins).
    bool solo = false;
    /// Send level (0-1) to the project aux effect bus (post-gain / post-pan tap).
    float auxSend = 0.f;

    /// Non-destructive trim: drop this many seconds from the start of the clip for playback/mix.
    double trimStartSec = 0;
    /// Non-destructive trim: drop this many seconds from the end of the clip for playback/mix.
    double trimEndSec = 0;

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

/// Enabled, not muted, and respects solo rules (any soloed enabled track gates others).
[[nodiscard]] bool trackShouldMix(const TrackData& t, const QVector<TrackData>& allTracks);

#endif // TRACKDATA_H
