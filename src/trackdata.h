#ifndef TRACKDATA_H
#define TRACKDATA_H

#include <QString>
#include <QVector>
#include <QByteArray>

enum class TrackType {
    Audio,
    MIDI
};

struct MidiNote {
    int note;       // MIDI note number (0-127)
    int velocity;
    double startTime;  // in seconds
    double duration;  // in seconds
};

struct TrackData {
    int id = -1;
    QString name;
    TrackType type = TrackType::Audio;
    bool enabled = true;
    bool armed = false;

    // For audio tracks: raw PCM data (16-bit stereo, 44100 Hz)
    QByteArray audioData;
    int sampleRate = 44100;
    int channelCount = 2;

    // For MIDI tracks: note events
    QVector<MidiNote> midiNotes;
    double lengthSeconds = 0;  // Track length in seconds

    [[nodiscard]] double length() const {
        if (type == TrackType::Audio) {
            int sampleCount = audioData.size() / (channelCount * 2);  // 16-bit = 2 bytes
            return sampleCount / (double)sampleRate;
        } else {
            return lengthSeconds;
        }
    }
};

#endif // TRACKDATA_H
