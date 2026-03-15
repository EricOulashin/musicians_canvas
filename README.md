# Musician's Canvas

This is a multi-track music recording application for desktop PCs. This is written in C++ and uses Qt for
the GUI library. This application is meant to be built for Linux, Windows, and Mac OS. Ease of use is a
main design consideration.  This project started as an experiment with Cursor (AI coding tool) to see what
it would be able to create as far as an application like this, with continued use of AI development as well
as traditional software development.

## Features

- **Multi-track arrangement**: Add multiple tracks to compose songs
- **Named tracks**: Each track has an editable name used as the filename for recorded audio
- **Track types**: Configure each track for audio recording (from microphone/line-in) or MIDI recording (from keyboard/controller)
- **Visual feedback**: Audio waveform display for audio tracks, MIDI piano roll for MIDI tracks
- **Built-in MIDI synthesizer**: Renders MIDI tracks to audio using FluidSynth with a configurable SoundFont
- **Mix to single audio file**: Export all enabled tracks to a single mixed WAV or FLAC file using the [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) library
- **Save / Open project**: Serialize and restore the full project (tracks, names, types, MIDI notes, audio file references) to/from a JSON file
- **Low-latency audio**: On Windows, ASIO driver detection ensures low-latency audio; on Linux, process scheduling priority is raised for lower jitter with PipeWire / PulseAudio / ALSA
- **Virtual MIDI keyboard**: A companion application for sending MIDI notes via a software piano keyboard
- **Configuration**: Select audio input device, MIDI device, and SoundFont file
- **Dark / light theme**: Configurable via Settings → Configuration

## Screenshots

<p align="center">
	<a href="screenshots/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/MusiciansCanvas_1_MainWin.png" alt="Main window" width="800"></a>
	<a href="screenshots/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/MusiciansCanvas_2_TrackConfig.png" alt="Track configuration" width="800"></a>
	<a href="screenshots/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_3_GeneralSettings.png" alt="General settings" width="800"></a>
	<a href="screenshots/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_4_MIDISettings.png" alt="MIDI settings" width="800"></a>
	<a href="screenshots/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_5_AudioSettings.png" alt="Audio settings" width="800"></a>
	<a href="screenshots/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/VMIDIKeyboard1.png" alt="Virtual MIDI keyboard" width="800"></a>
	<a href="screenshots/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/VMIDIKeyboard2.png" alt="Virtual MIDI keyboard" width="800"></a>
</p>

## Dependencies

### audio_mixer_cpp (required on all platforms)

The mixing and FLAC-export functionality depends on the
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) library.
Clone it as a sibling directory of this repository before building:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

The CMake build expects to find it at `../audio_mixer_cpp` relative to this project's root.

---

### Linux — Ubuntu / Debian

```bash
sudo apt install build-essential cmake \
  qt6-base-dev qt6-multimedia-dev \
  libfluidsynth-dev librtmidi-dev libflac-dev
```

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel
```

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac
```

---

### Windows

Windows requires an **ASIO audio driver** for low-latency recording and playback.
Musician's Canvas will not start if no ASIO driver is detected.

**Installing an ASIO driver (choose one):**

| Driver | Notes |
|--------|-------|
| [ASIO4ALL](https://asio4all.org/) | Free, works with most built-in and USB audio hardware |
| Manufacturer driver | Best latency for dedicated audio interfaces (e.g. Focusrite, PreSonus, RME) |

**Toolchain — MSYS2 (recommended for MinGW builds):**

Download and install MSYS2 from <https://www.msys2.org>, then add
`C:\msys64\mingw64\bin` to your system `PATH`. Install the required packages:

```bash
pacman -S mingw-w64-x86_64-qt6-base
pacman -S mingw-w64-x86_64-qt6-multimedia
pacman -S mingw-w64-x86_64-fluidsynth
pacman -S mingw-w64-ucrt-x86_64-rtmidi
pacman -S mingw-w64-x86_64-flac
```

Package reference pages:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>

**Toolchain — Visual Studio 2022:**

Install Qt 6 via the [Qt Online Installer](https://www.qt.io/download) and obtain
FluidSynth, RtMidi, and libFLAC binaries (or build them from source).
The CMake build will locate them via `find_library` / `find_path` as long as the
appropriate directories are on `CMAKE_PREFIX_PATH`.

> **Note:** `advapi32` (Windows registry) and `winmm` (Windows Multimedia) are
> linked automatically by CMake; no additional installation is required for those.

---

## Building

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

**Windows (Visual Studio 2022):**

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

If required DLLs are not found at runtime, run the helper script from the build
output directory:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Running

```bash
./build/musicians_canvas
# or
./build/virtual_midi_keyboard   # companion virtual MIDI keyboard app
```

---

## Usage

1. **Set project directory**: Enter or browse to a folder where recordings and the project file will be stored
2. **Add tracks**: Click "+ Add Track"; name each track in the text field next to "Options"
3. **Configure track type**: Click "Options" on a track to choose Audio or MIDI
4. **Settings**: Use **Settings → Configuration** to select:
   - *MIDI* tab: MIDI input device and SoundFont (.sf2) for synthesis
   - *Audio* tab: Audio input device for recording
5. **Record**: Check "Arm" on the target track, then click the record button (red circle)
6. **Play**: Click the play button to mix and play back all enabled tracks
7. **Mix to file**: Use **Tools → Mix tracks to file** (Ctrl+M) to export to WAV or FLAC
8. **Save / Open**: Use **File → Save Project** (Ctrl+S) and **File → Open Project** (Ctrl+O)

---

## Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| Ctrl+S | Save project |
| Ctrl+O | Open project |
| Ctrl+M | Mix tracks to file |
| Ctrl+, | Settings / Configuration |
| Ctrl+Q / Alt+F4 | Quit |

---

## License

This project is provided as-is for educational and personal use.
