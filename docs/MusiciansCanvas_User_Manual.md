---
title: "Musician's Canvas User Manual"
subtitle: "Multi-Track Music Recording Application"
version: "0.1.0"
date: "2026-04-03"
author: "Eric Oulashin"
---

# Musician's Canvas User Manual

## Introduction

Musician's Canvas is a multi-track music recording application for desktop PCs. It supports
audio recording from microphones and line-in devices, MIDI recording from keyboards and
controllers, and mixing all tracks to a single WAV or FLAC file. A companion application,
Virtual MIDI Keyboard, provides a software piano keyboard for sending MIDI notes.

Musician's Canvas is designed for ease of use while providing features commonly found in
digital audio workstations (DAWs):

- Multi-track audio and MIDI recording
- Overdub recording with synchronized playback of existing tracks
- Built-in FluidSynth MIDI synthesizer with SoundFont support
- High-quality sample-rate conversion for recording at any project sample rate
- Automatic mono/stereo device detection
- Project-based settings with per-project overrides
- Mix to WAV or FLAC
- Dark and light themes
- Localized in multiple languages (English, German, Spanish, French, Japanese, Portuguese, Chinese, and Pirate)
- Companion Virtual MIDI Keyboard application

## Getting Started

### Launching the Application

Run the `musicians_canvas` executable from the build directory or your installation
location:

```
./musicians_canvas
```

On first launch, the application opens with an empty project. You will need to set a
project directory before recording.

On startup, the application applies the saved theme (dark or light) and, if a project
directory was previously used and contains a `project.json` file, the project is
automatically loaded.

### Setting Up a Project

1. **Set the project directory**: Enter or browse to a folder in the "Project Location"
   field at the top of the window. This is where recordings and the project file will
   be stored.

2. **Add a track**: Click the **+ Add Track** button. A new track appears with default
   settings. If it is the only track in the project and has not been recorded yet, it
   is automatically armed for recording.

3. **Name the track**: Type a name in the text field next to the "Options" button. This
   name is used as the filename for the recorded audio file.

![Main window](../screenshots/MusiciansCanvas_1_MainWin.png)

### Saving and Opening Projects

- **Save**: Use **File > Save Project** (Ctrl+S) to save the current project as a JSON
  file in the project directory.
- **Open**: Use **File > Open Project** (Ctrl+O) to load a previously saved project.

The project file (`project.json`) stores track names, types, MIDI notes, audio file
references, and all project-specific settings. Audio files are stored in the same
directory as `project.json` and are named after their tracks (e.g., `My_Track.flac`).

If you close the application with unsaved changes, a confirmation dialog asks whether
to save before exiting.

## Track Management

### Adding and Removing Tracks

- Click **+ Add Track** to add a new track to the arrangement.
- Click the **x** button on the right side of a track row to remove it.
- Click **Clear Tracks** (the red button in the toolbar) to remove all tracks. A
  confirmation dialog is shown before proceeding.

### Configuring Track Type

Each track can be configured as either **Audio** (for microphone/line-in recording) or
**MIDI** (for keyboard/controller recording).

To change the track type:

- Click the **Options** button on the track, or
- Click the **track type icon** (between "Options" and the name field)

This opens the Track Configuration dialog where you can select the input source.

![Track configuration](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Track Controls

Each track row provides the following controls:

- **Enable checkbox**: Toggles whether the track is included in playback and mixing.
  Disabling a track also automatically disarms it if it was armed.
- **Arm radio button**: Selects this track as the recording target. Only one track can
  be armed at a time; arming a new track automatically disarms any previously armed
  track.
- **Name field**: Editable text field for the track name. This name is used as the
  audio filename (with invalid filesystem characters replaced by underscores).
- **Options button**: Opens the Track Configuration dialog.
- **Type icon**: Shows a speaker icon for audio tracks or a piano icon for MIDI tracks.
  Clicking it opens the Track Configuration dialog.
- **Remove button (x)**: Removes the track from the project.

### Auto-Arming

When a project has exactly one track and that track has not been recorded yet, it is
automatically armed for recording. This applies both when adding the first track to a
new project and when opening an existing project that has a single empty track.

### Track Visualization

- **Audio tracks** display a waveform visualization of the recorded audio. When no audio
  has been recorded, the area shows "No audio recorded".
- **MIDI tracks** display a piano roll visualization showing recorded notes on a grid
  spanning A0 to C8. Notes are colored by velocity. When no MIDI data has been recorded,
  the area shows "No MIDI data recorded".

## Recording

### Audio Recording

1. Ensure the project directory is set.
2. Arm the target track (check the "Arm" radio button).
3. Click the **Record** button (red circle).
4. A 3-second countdown appears on the track ("Get ready... 3", "2", "1"), then
   recording begins.
5. During recording, a live level meter is displayed in the track's waveform area,
   showing the current amplitude as a gradient bar (green to yellow to red) with a
   "Recording" label.
6. Click the **Stop** button to end recording.

The recorded audio is saved as a FLAC file in the project directory, named after the
track.

During recording and playback, all interactive controls (track buttons, settings, etc.)
are disabled to prevent accidental changes.

#### Overdub Recording

When recording a new track while other enabled tracks already contain audio or MIDI data,
Musician's Canvas performs overdub recording: the existing tracks are mixed together and
played back in real time while the new track is being recorded. This allows you to hear
previously recorded parts while laying down a new one.

The mix of existing tracks is prepared before capture begins, so recording and playback
start at approximately the same instant, keeping all tracks synchronized.

#### Recording Backends

Musician's Canvas supports two audio capture backends:

- **PortAudio** (default when available): Provides reliable, low-latency recording
  and is the recommended backend.
- **Qt Multimedia**: A fallback backend using Qt's built-in audio capture. Used when
  PortAudio is not available or when explicitly selected in Project Settings.

The recording backend can be configured per-project in **Project > Project Settings >
Audio**.

#### Sample Rate and Device Handling

Musician's Canvas records at the audio input device's native sample rate and then
automatically converts to the project's configured sample rate using high-quality
resampling. This means you can set any project sample rate (e.g., 44100 Hz or 48000 Hz)
regardless of the device's native rate. The conversion preserves pitch and duration
exactly.

#### Mono Device Detection

Some audio devices (e.g., USB webcam microphones) are physically mono but are advertised
as stereo by the operating system. Musician's Canvas automatically detects this and
adjusts the channel count accordingly. If the project is configured for stereo, the mono
signal is duplicated to both channels.

### MIDI Recording

1. Set the track type to **MIDI** via the Options button.
2. Ensure a MIDI input device is configured in **Settings > Configuration > MIDI**.
3. Arm the track and click Record.
4. Play notes on your MIDI controller.
5. Click Stop to end recording.

MIDI notes are displayed in a piano roll visualization on the track.

## Playback

Click the **Play** button to mix and play back all enabled tracks. The button tooltip
changes to indicate whether it will play or record based on whether a track is armed.
Disabled tracks (unchecked) are excluded from playback.

During playback, audio tracks are decoded from their FLAC files and MIDI tracks are
rendered to audio using the built-in FluidSynth synthesizer. All tracks are mixed
together and played through the system's audio output device.

Click the **Stop** button to end playback at any time.

## Mixing to a File

Use **Tools > Mix tracks to file** (Ctrl+M) to export all enabled tracks to a single
audio file. A dialog lets you choose the output path and format:

- **Output file**: Browse to select the destination file path.
- **Format**: Choose between FLAC (lossless compression, smaller files) or WAV
  (uncompressed).

The mix uses the project's configured sample rate. MIDI tracks are rendered using the
configured SoundFont.

## Settings

### Global Settings

Use **Settings > Configuration** (Ctrl+,) to set global defaults that apply to all
projects:

![General settings](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### General Tab

- **Theme**: Choose between Dark and Light themes.

#### Language Tab

- **Language**: Choose the display language for the application. The default is "System
  Default", which uses the operating system's language setting. Available languages are
  English, Deutsch (German), Español (Spanish), Français (French), Japanese, Português
  (Brazilian Portuguese), Chinese (Traditional), Chinese (Simplified), and Pirate.
  The interface updates immediately when you change the language.

#### MIDI Tab

- **MIDI Output Device**: Select the built-in FluidSynth synthesizer or an external
  MIDI device. Use the **Refresh** button to rescan for available MIDI devices.
- **SoundFont**: Browse to a `.sf2` SoundFont file for MIDI synthesis. On Linux, a
  system SoundFont may be auto-detected if the `fluid-soundfont-gm` package is
  installed. On Windows and macOS, you must configure the SoundFont path manually.

![MIDI settings](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Audio Tab

- **Audio Input Device**: Select the microphone or line-in device for recording.
- **Audio Output Device**: Select the speaker or headphone device for playback.

![Audio settings](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Project Settings

Use **Project > Project Settings** (Ctrl+P) to override global defaults for the current
project only. This is useful for projects that need a specific sample rate, SoundFont,
or audio device. Project-specific settings are saved inside the `project.json` file.

![Project MIDI settings](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### MIDI Tab

- **MIDI Device**: Select a MIDI device for this project, or leave at the default to use
  the global setting.
- **SoundFont**: Select a SoundFont file for this project.
- **Refresh**: Rescan for available MIDI devices.

#### Audio Tab

- **Audio Input Device**: Select the recording device for this project.
- **Recording capture backend** (when PortAudio is available):
  - **PortAudio (native input)**: Recommended. Uses the same audio library as Audacity.
  - **Qt Multimedia**: Fallback option using Qt's built-in audio capture.
- **PortAudio Input Device**: When using the PortAudio backend, select the specific
  PortAudio input device.
- **Audio Output Device**: Select the playback device for this project.

##### Audio Format Settings

- **Sample rate**: Choose from standard rates (8000 Hz to 192000 Hz). The device's native
  rate is labeled "(native)". Rates that require resampling are labeled "(resampled)".
  You can select any rate regardless of device capability; Musician's Canvas will
  automatically resample as needed.
- **Channels**: Mono or Stereo. If the device only supports mono, the Stereo option
  is disabled.

![Project audio settings](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

## Menus

### File Menu

| Menu Item     | Shortcut | Description                     |
|---------------|----------|---------------------------------|
| Save Project  | Ctrl+S   | Save current project to disk    |
| Open Project  | Ctrl+O   | Open an existing project file   |
| Close         | Ctrl+Q   | Quit the application            |

### Project Menu

| Menu Item                           | Shortcut | Description                              |
|-------------------------------------|----------|------------------------------------------|
| Project Settings                    | Ctrl+P   | Configure project-specific settings      |
| Add Demo Data to Selected Track     |          | Add sample MIDI notes for demonstration  |

### Settings Menu

| Menu Item     | Shortcut | Description                       |
|---------------|----------|-----------------------------------|
| Configuration | Ctrl+,   | Open global application settings  |

### Tools Menu

| Menu Item             | Shortcut | Description                              |
|-----------------------|----------|------------------------------------------|
| Mix tracks to file    | Ctrl+M   | Export all enabled tracks to a file      |
| Virtual MIDI Keyboard |          | Launch the companion keyboard app        |

## Keyboard Shortcuts

| Shortcut        | Action                         |
|-----------------|--------------------------------|
| Ctrl+S          | Save project                   |
| Ctrl+O          | Open project                   |
| Ctrl+M          | Mix tracks to file             |
| Ctrl+P          | Project Settings               |
| Ctrl+,          | Settings / Configuration       |
| Ctrl+Q / Alt+F4 | Quit                          |

## Virtual MIDI Keyboard

The Virtual MIDI Keyboard is a companion application (`virtual_midi_keyboard`) that
provides an on-screen piano keyboard for sending MIDI notes. It can be launched from the
**Tools > Virtual MIDI Keyboard** menu in the main application, or run independently.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Features

- Click keys on the on-screen piano to play notes
- Use the computer keyboard as a piano keyboard (see key mapping below)
- Connect to external MIDI output devices or use the built-in FluidSynth synthesizer
- Connect to a MIDI input device to display incoming notes on the keyboard
- Adjustable synthesizer volume (master gain, 10%--200%)
- SoundFont selection for the built-in synthesizer
- MIDI instrument/program selection (General MIDI or SoundFont presets)
- Chorus/effect control knob
- Octave shifting (-3 to +5)
- MIDI volume control (CC#7, 0--127)

### Toolbar Controls

- **Volume**: MIDI volume (CC#7), adjustable from 0 to 127 via the spin box.
- **Octave**: Shift the keyboard octave using the **<** and **>** buttons or the spin
  box. Range is -3 to +5.
- **Chorus/Effect**: A rotary knob and text field (1--127) to set the chorus/effect
  level (MIDI CC#93). Click **Apply** to send the value.
- **Instrument selector**: Choose a MIDI instrument. When using the built-in FluidSynth
  synthesizer, this shows SoundFont presets (Bank:Program Name). When connected to an
  external MIDI device, this lists the 128 General MIDI instruments.

### Playing with the Computer Keyboard

The computer keyboard is mapped to piano keys across two octaves:

**Lower octave (starting at the current octave):**

| Key | Note  |
|-----|-------|
| Z   | C     |
| S   | C#/Db |
| X   | D     |
| D   | D#/Eb |
| C   | E     |
| V   | F     |
| G   | F#/Gb |
| B   | G     |
| H   | G#/Ab |
| N   | A     |
| J   | A#/Bb |
| M   | B     |

**Upper octave (one octave higher):**

| Key | Note  |
|-----|-------|
| Q   | C     |
| 2   | C#/Db |
| W   | D     |
| 3   | D#/Eb |
| E   | E     |
| R   | F     |
| 5   | F#/Gb |
| T   | G     |
| 6   | G#/Ab |
| Y   | A     |
| 7   | A#/Bb |
| U   | B     |
| I   | C (next octave) |
| 9   | C#/Db |
| O   | D     |
| 0   | D#/Eb |
| P   | E     |

Keys light up visually when pressed (white keys turn light blue, black keys darken).

### Configuration

Open the Configuration dialog (**Settings > Configuration**, Ctrl+,) to set up MIDI and
audio devices:

![Virtual MIDI Keyboard configuration](../screenshots/VMIDIKeyboard2.png)

#### MIDI Tab

- **MIDI Output Device**: Select an external device or leave blank for the built-in
  FluidSynth synthesizer.
- **MIDI Input Device**: Select a controller to forward notes to the keyboard display.
- **Synthesizer Volume (Master Gain)**: Adjust the output level of the built-in
  synthesizer (10%--200%).
- **SoundFont**: Browse to a `.sf2` SoundFont file for the built-in synthesizer.
- **Refresh**: Rescan for available MIDI devices.

#### Audio Tab

- **Audio Output Device**: Select the output device for the built-in synthesizer.

#### Language Tab

- **Language**: Choose the display language. The same languages as Musician's Canvas
  are available. The interface updates immediately when you change the language.

### Virtual MIDI Keyboard Shortcuts

| Shortcut | Action                 |
|----------|------------------------|
| Ctrl+,   | Configuration dialog   |
| Ctrl+U   | Help / Usage info      |
| Ctrl+Q   | Close                  |

## Troubleshooting

### No Audio Output

- Check that the correct audio output device is selected in Settings > Configuration >
  Audio.
- On Linux, verify that PipeWire or PulseAudio is running and that the output is not
  muted. Use `amixer` or your desktop's sound settings to check volume levels.
- On Windows, ensure an ASIO audio driver is installed (e.g.,
  [ASIO4ALL](https://asio4all.org/) or a manufacturer-provided ASIO driver for your
  audio interface). Musician's Canvas requires ASIO for low-latency audio on Windows.

### MIDI Tracks Are Silent

- Ensure a SoundFont (`.sf2` file) is configured in Settings > Configuration > MIDI.
- On Linux, a system SoundFont may be auto-detected if the `fluid-soundfont-gm` package
  is installed.
- On Windows and macOS, you must configure the SoundFont path manually.

### Recording Sounds Distorted or Has Wrong Pitch

- This can happen when the audio input device's native sample rate differs from the
  project's configured rate. Musician's Canvas automatically handles this via
  resampling, but if issues persist, try setting the project sample rate to match the
  device's native rate.
- USB webcam microphones often have unusual native rates (e.g., 32000 Hz). The
  application detects these automatically.
- If using the Qt Multimedia backend and experiencing issues, try switching to the
  PortAudio backend in Project Settings > Audio.

### Virtual MIDI Keyboard Has No Audio

- On Linux with PipeWire, ensure the `libpipewire-0.3-dev` package is installed (needed
  for PipeWire integration with the FluidSynth synthesizer).
- Check that a SoundFont is loaded (see the MIDI tab in the configuration dialog).
- Verify the audio output device is selected and the system volume is not muted.

## Building from Source

See the [README](../README.md) for complete build instructions for Linux, macOS, and
Windows, including all required dependencies.
