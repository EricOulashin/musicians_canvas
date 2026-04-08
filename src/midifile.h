#ifndef MIDIFILE_H
#define MIDIFILE_H

#include <QString>
#include <QVector>
#include "trackdata.h"

// Read and write Standard MIDI File (SMF) format 0 files (.mid).
//
// Musician's Canvas uses a fixed timing convention so the conversion
// between seconds and ticks is trivial:
//
//   division = 1000 ticks per quarter note
//   tempo    = 1,000,000 microseconds per quarter note  (=> 60 BPM)
//
// At 60 BPM with 1000 PPQ, one quarter note is exactly one second and one
// tick is exactly one millisecond.  Notes captured by MidiRecorder (which
// stores start time / duration in seconds with millisecond precision) round
// trip without any loss.
//
// The writer preserves each note's MIDI channel (0..15) so drum tracks
// recorded on channel 10 (= channel index 9) are saved correctly and play
// back as drums when re-rendered.
namespace MidiFile {

// Write `notes` as a Standard MIDI File at `path`.  Returns true on success.
// `lengthSeconds` is the total length of the track in seconds; an
// "End of Track" meta event is written at this position so the file's
// duration matches the original recording even if the last note ends earlier.
bool write(const QString& path,
           const QVector<MidiNote>& notes,
           double lengthSeconds);

// Read a Standard MIDI File from `path` and return its note events.  Both
// format 0 and format 1 files are supported (events from all tracks are
// merged).  On success, `outLengthSeconds` (if non-null) receives the
// duration computed from the End-of-Track meta events, or from the last
// note's end time if no end-of-track marker is present.
QVector<MidiNote> read(const QString& path, double* outLengthSeconds = nullptr);

} // namespace MidiFile

#endif // MIDIFILE_H
