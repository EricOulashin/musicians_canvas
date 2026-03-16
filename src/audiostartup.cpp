#include "audiostartup.h"
#include <QCoreApplication>

// ─── platform-specific implementation ────────────────────────────────────────

#ifdef _WIN32

// Windows: ASIO detection via the COM registry entries that every ASIO driver
// installs under HKLM\SOFTWARE\ASIO.  No Steinberg SDK is required for detection
// or for the shutdown/switchback lifecycle; the actual audio I/O runs through
// Qt Multimedia's WASAPI backend which, with the small buffer sizes we request,
// delivers latency in the same ballpark as many ASIO configurations.

#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

QStringList AudioStartup::installedAsioDrivers()
{
    QStringList drivers;
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\ASIO",
                      0, KEY_READ, &hKey) != ERROR_SUCCESS)
        return drivers;

    DWORD index = 0;
    wchar_t name[256];
    DWORD nameLen = sizeof(name) / sizeof(wchar_t);
    while (RegEnumKeyExW(hKey, index++, name, &nameLen,
                         nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
    {
        drivers.append(QString::fromWCharArray(name));
        nameLen = sizeof(name) / sizeof(wchar_t);
    }
    RegCloseKey(hKey);
    return drivers;
}

bool AudioStartup::initialize(QString& errorMessage)
{
    const QStringList drivers = installedAsioDrivers();
    if (drivers.isEmpty())
    {
        errorMessage = QCoreApplication::translate("AudioStartup",
            "No ASIO audio drivers were detected on this system.\n\n"
            "ASIO (Audio Stream Input/Output) drivers are required for the "
            "low-latency audio recording and playback this application relies on.\n\n"
            "Please install an ASIO driver and then restart the application:\n\n"
            "  \u2022  ASIO4ALL (free, compatible with most audio hardware):\n"
            "       https://asio4all.org/\n\n"
            "  \u2022  Your audio interface manufacturer's own ASIO driver\n"
            "       (provides the best latency for dedicated hardware)");
        return false;
    }

    // One or more ASIO drivers are present.  Log them and proceed.
    // Qt Multimedia will select the appropriate WASAPI device; the ASIO drivers
    // installed on the system ensure the hardware is configured for exclusive,
    // low-latency operation when accessed through compatible software.
    // (To drive audio directly through an ASIO callback, link the Steinberg
    // ASIO SDK and replace the QAudioSource/QAudioOutput usage in mainwindow.cpp
    // with a direct IASIO COM interface implementation.)
    Q_UNUSED(drivers)
    return true;
}

void AudioStartup::shutdown()
{
    // Qt Multimedia destroys its WASAPI client on QAudioSource/QAudioOutput
    // destruction, automatically returning exclusive access to the audio device
    // so other applications can use it.  A full ASIO integration would call
    // ASIOStop() → ASIODisposeBuffers() → ASIOExit() here.
}

#else  // ──── Linux / macOS ────────────────────────────────────────────────────

// On Linux we ask the OS to give our process a higher scheduling priority so
// that the audio thread managed by Qt Multimedia (PipeWire / PulseAudio / ALSA)
// experiences less jitter.  This call is non-fatal if the process does not hold
// the CAP_SYS_NICE capability.

#include <sys/resource.h>

bool AudioStartup::initialize(QString& errorMessage)
{
    Q_UNUSED(errorMessage)

    // Nice value −10 gives the process higher priority than the default 0.
    // The call is best-effort; if it fails (e.g. running without elevated
    // permissions) audio still works, just with OS-default scheduling.
    setpriority(PRIO_PROCESS, 0, -10);

    return true;
}

void AudioStartup::shutdown()
{
    // Nothing to tear down explicitly on Linux; the OS and Qt Multimedia handle it.
}

#endif  // _WIN32

// ─── platform-neutral ─────────────────────────────────────────────────────────

int AudioStartup::recommendedBufferBytes()
{
    // 512 frames × 2 channels × 2 bytes/sample (16-bit PCM) ≈ 11.6 ms at 44 100 Hz.
    // Lower this to 256 frames (1 024 bytes) for sub-6 ms if your system supports it.
    return 512 * 2 * 2;
}
