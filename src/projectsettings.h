#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QString>
#include <QByteArray>
#include <QJsonArray>
#include <QVector>

// Per-project audio/MIDI configuration stored inside the project JSON file.
// These settings take precedence over the global app defaults for the loaded project.

/// Tempo change on the project timeline (seconds from project start, BPM).
struct TempoMarker
{
    double timeSec = 0;
    double bpm = 120;
};

struct ProjectSettings
{
    // MIDI
    int     midiDeviceIndex  = -1;   // -1 = use app default
    QString soundFontPath;           // empty = use app default
    // MIDI INPUT port name (matched against RtMidiIn::getPortName).
    // Empty = no MIDI input — MIDI tracks cannot be recorded until set.
    QString midiInputPortName;
    // MIDI volume override for software synth rendering during playback.
    // -1 = use app default; otherwise percent (0-200).
    int midiVolumePercent = -1;
    // When true (default), MIDI tracks are rendered to audio for playback so they
    // can be heard along with audio tracks. When false, MIDI tracks are sent
    // in real time to the configured MIDI output device.
    bool renderMidiToAudioForPlayback = true;

    // Audio
    QByteArray audioInputDeviceId;   // empty = use app default (Qt Multimedia device id)
    QByteArray audioOutputDeviceId;  // empty = use app default
    int sampleRate   = 44100;        // recording and MIDI render sample rate
    int channelCount = 2;            // 1 = mono, 2 = stereo

    /// When false and the app is built with PortAudio, capture uses PortAudio (Audacity-style).
    /// When true, capture uses Qt Multimedia only.
    bool useQtAudioInput = false;
    /// PortAudio host input device index, or -1 for Pa_GetDefaultInputDevice().
    int portAudioInputDeviceIndex = -1;

    /// When true, route live input (audio and/or MIDI) to the project output while recording.
    bool monitorWhileRecording = false;

    /// Mix-bus effect chain (top to bottom). Applied when playing all tracks and when mixing
    /// the project to a file — same effect types and ordering as per-track effects.
    QJsonArray mixEffectChain;

    /// Shared aux/send bus: each track's auxSend taps post-fader stereo into this chain, then
    /// the wet bus is summed with the dry mix before the master mixEffectChain.
    QJsonArray auxEffectChain;

    /// Piecewise-constant tempo map for metronome flashes and MIDI quantization. When empty,
    /// global metronome BPM (Settings) is used.
    QVector<TempoMarker> tempoMarkers;

    /// Punch-in recording for audio: replace samples between punchInSec and punchOutSec in the
    /// existing take with the new recording. Ignored for MIDI or when punch is disabled.
    bool punchRecordingEnabled = false;
    double punchInSec = 0;
    double punchOutSec = 0;

    /// Loop playback region while playing all tracks (requires Qt Multimedia playback).
    bool loopPlaybackEnabled = false;
    double loopStartSec = 0;
    double loopEndSec = 0;
};

/// Effective BPM at time t (seconds) using markers; falls back to defaultBpm if no markers.
[[nodiscard]] double tempoBpmAtTime(const QVector<TempoMarker>& markers, double tSec,
                                    double defaultBpm);

#endif // PROJECTSETTINGS_H
