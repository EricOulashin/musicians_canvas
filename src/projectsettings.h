#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QString>
#include <QByteArray>

// Per-project audio/MIDI configuration stored inside the project JSON file.
// These settings take precedence over the global app defaults for the loaded project.
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
};

#endif // PROJECTSETTINGS_H
