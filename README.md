# Musician's Canvas

This is a multi-track music recording application for desktop PCs. This is written in C++ and uses Qt for
the GUI library. This application is meant to be built for Linux, Windows, and Mac OS. Ease of use is a
main design consideration.  This project started as an experiment with Cursor (AI coding tool) to see what
it would be able to create as far as an application like this, with continued use of AI development as well
as traditional software development.

## Features/Planned Features

- **Multi-track arrangement**: Add multiple tracks to compose songs
- **Track types**: Configure each track for audio recording (from microphone/line-in) or MIDI recording (from keyboard/controller)
- **Visual feedback**: Audio waveform display for audio tracks, MIDI piano roll for MIDI tracks
- **Built-in MIDI synthesizer**: Renders MIDI tracks to audio using FluidSynth with configurable SoundFont
- **Mix to single audio file**: Export all enabled tracks to a single mixed audio file (for this, I plan to use code I've already written in my <a href='https://github.com/EricOulashin/audio_mixer_cpp' target='_blank'>C++ Audio Mixer project</a>)
- **Configuration**: Select audio input device, MIDI device, and SoundFont file

## Screenshots
These are some screenshots:
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

### Ubuntu / Debian
```bash
sudo apt install builad-essential cmake \
  qt6-base-dev qt6-multimedia-dev \
  libfluidsynth-dev librtmidi-dev
```

### Fedora
```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel
```

### macOS
```bash
brew install qt cmake fluid-synth rtmidi
```

### Windows
Download & install Msys2 here:
<a href='https://www.msys2.org' target='_blank'>https://www.msys2.org</a>.
After installing, add its bin path to your system path in the environment variables.  Assuming you
left it with the default path of C:\msys64, add C:\msys64\mingw64\bin to your system path.
Then install the fluidsynth & rtmidi packages:
```cmd
pacman -S mingw-w64-x86_64-fluidsynth
pacman -S mingw-w64-ucrt-x86_64-rtmidi
```
The following pages are information about the fluidsynth & rtmidi packages:
<a href='https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth' target='_blank'>https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth</a>
<a href='https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi' target='_blank'>https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi</a>

## Building

```bash
make
```

Or manually:
```bash
mkdir build && cd build
cmake ..
make
```

In Windows, when the build\Release directory exists:
```cmd
cd build\Release
cmake --build . --config Release
```
And if you've installed Msys2 and the requirements, even if you add C:\msys64\bin to the system path,
for some reason it might not always find the required DLLs; in that situation, you can run the
copyRequiredWinDLLs.bat from the build\Release directory if you haven't already:
```cmd
..\..\copyRequiredWinDLLs.bat
```

## Running

```bash
make run
# or
./build/musicians_canvas
```

## Usage

1. **Add tracks**: Click "+ Add Track" to add a new track
2. **Configure track**: Click "Options" on a track to choose Audio or MIDI input
3. **Settings**: Use Settings → Configuration to select:
   - MIDI tab: MIDI input device and SoundFont file (.sf2) for synthesis
   - Audio tab: Audio input device for recording
4. **Mix**: Use Project → Mix to export all enabled tracks to a WAV file

## Menu Shortcuts

- **Alt+F**: File menu
- **Alt+P**: Project menu  
- **Alt+S**: Settings menu
- **Alt+C**: Configuration (when Settings menu is open)
- **Alt+M**: Mix (when Project menu is open)
- **Ctrl+Q**: Quit

## License

This project is provided as-is for educational and personal use.
