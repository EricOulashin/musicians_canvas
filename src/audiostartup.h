#ifndef AUDIOSTARTUP_H
#define AUDIOSTARTUP_H

#include <QString>
#include <QStringList>

// Handles platform-specific audio system initialisation for low-latency operation.
//
// Windows:
//   Detects installed ASIO drivers via the Windows registry (HKLM\SOFTWARE\ASIO).
//   ASIO provides the lowest possible audio round-trip latency on Windows.
//   If no ASIO driver is found the application cannot run and must exit.
//   Full ASIO callback-level integration (buffer management, sample callbacks, etc.)
//   requires the Steinberg ASIO SDK; the detection and lifecycle management below
//   is implemented without it, using the COM registry presence check that all
//   ASIO drivers expose.  To obtain the SDK visit: https://www.steinberg.net/asiosdk
//
// Linux:
//   Raises the process scheduling priority so the audio and MIDI threads experience
//   less OS-induced jitter.  Qt Multimedia will automatically use PipeWire,
//   PulseAudio, or ALSA depending on what the system provides.
//
class AudioStartup
{
public:
    // Must be called before any QAudioSource / QAudioOutput instances are created.
    // Returns false (and fills errorMessage) when the required audio subsystem is
    // unavailable; the caller should display the message to the user and exit.
    static bool initialize(QString& errorMessage);

    // Call on application exit to cleanly release audio resources.
    static void shutdown();

    // Recommended QAudioSource buffer size in bytes for low-latency recording at
    // 44 100 Hz, stereo, 16-bit PCM.  Corresponds to ~512 frames (~11.6 ms).
    static int recommendedBufferBytes();

#ifdef _WIN32
    // Returns the display names of all ASIO drivers registered on this machine.
    static QStringList installedAsioDrivers();
#endif
};

#endif // AUDIOSTARTUP_H
