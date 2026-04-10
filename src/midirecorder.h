#ifndef MIDIRECORDER_H
#define MIDIRECORDER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QElapsedTimer>
#include <functional>
#include <memory>
#include <vector>
#include "trackdata.h"

// Captures MIDI events from an RtMidiIn port and converts them into
// a QVector<MidiNote> with start times and durations relative to the
// moment recording was started.  Note-on / note-off pairs are matched
// by note number; any unmatched note-on at stop time is closed out
// using the wall-clock end of recording.
//
// Usage:
//   MidiRecorder rec;
//   rec.start(portName);
//   ... user plays MIDI ...
//   QVector<MidiNote> notes = rec.stop(); // also closes the port
//
// All RtMidi callbacks happen on a background thread.  Pending events
// are protected with a mutex; stop() drains them.
class MidiRecorder
{
public:
    MidiRecorder();
    ~MidiRecorder();

    // Open the named RtMidiIn port (matching the same scheme used by
    // VkMidiIo and the project settings dialog) and begin recording.
    // Returns false if the port could not be opened.  errorMsg, if non-null,
    // receives a human-readable error description on failure.
    // Optional rawTap: invoked from RtMidi's thread for each incoming message.
    bool start(const QString& portName, QString* errorMsg = nullptr,
               std::function<void(const std::vector<unsigned char>&)> rawTap = {});

    /// Replace or clear the live tap while recording (thread-safe).
    void setRawTap(std::function<void(const std::vector<unsigned char>&)> rawTap);

    // Stop capture, close the port, and return the captured note events.
    // The returned length is the wall-clock duration in seconds at the
    // moment stop() was called.
    QVector<MidiNote> stop(double* outLengthSeconds = nullptr);

    // Get a copy of the events captured so far without stopping (used for
    // a live preview if needed).
    QVector<MidiNote> snapshot() const;

    // List of available RtMidiIn port names on this system, in stable order.
    static QStringList availableInputPorts();

    // Internal — exposed only so the RtMidi C callback can reach it.
    class Impl;

private:
    std::shared_ptr<Impl> m_impl;
};

#endif // MIDIRECORDER_H
