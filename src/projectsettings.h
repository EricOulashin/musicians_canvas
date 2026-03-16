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

    // Audio
    QByteArray audioInputDeviceId;   // empty = use app default
    QByteArray audioOutputDeviceId;  // empty = use app default
    int sampleRate   = 44100;        // recording and MIDI render sample rate
    int channelCount = 2;            // 1 = mono, 2 = stereo
};

#endif // PROJECTSETTINGS_H
