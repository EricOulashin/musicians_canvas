#ifndef DRUMTRACKGENERATOR_H
#define DRUMTRACKGENERATOR_H

#include <QByteArray>
#include <QVector>

#include "trackdata.h"

/// Utilities to estimate tempo from mixed audio and build a simple GM drum groove.
namespace DrumTrackGenerator
{

/// Estimate BPM from interleaved 16-bit PCM (40–220). Returns 0 if the buffer is too short
/// or no clear periodicity is found.
[[nodiscard]] int estimateBpmFromInt16Pcm(const QByteArray& pcm, int sampleRate, int channelCount);

/// Mix all enabled audio tracks to stereo int16 at \p targetSampleRate. Empty if no audio.
[[nodiscard]] QByteArray mixEnabledAudioTracksForAnalysis(const QVector<TrackData>& tracks,
                                                         int targetSampleRate);

/// Two-bar 4/4 rock-style groove on General MIDI drums (channel index 9). \p bpm is musical tempo.
[[nodiscard]] QVector<MidiNote> createGroovePattern(double bpm, int bars = 2);

/// Duration in seconds for the pattern (for SMF end time).
[[nodiscard]] double patternLengthSeconds(double bpm, int bars);

}  // namespace DrumTrackGenerator

#endif
