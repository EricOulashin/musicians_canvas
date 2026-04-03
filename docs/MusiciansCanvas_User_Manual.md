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
- Built-in FluidSynth MIDI synthesizer with SoundFont support
- High-quality sample-rate conversion (windowed sinc interpolation)
- Automatic mono/stereo device detection
- Project-based settings with per-project overrides
- Mix to WAV or FLAC
- Dark and light themes

## Getting Started

### Launching the Application

Run the `musicians_canvas` executable from the build directory or your installation
location:

```
./musicians_canvas
```

On first launch, the application opens with an empty project. You will need to set a
project directory before recording.

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

The project file stores track names, types, MIDI notes, audio file references, and all
project-specific settings.

## Track Management

### Adding and Removing Tracks

- Click **+ Add Track** to add a new track to the arrangement.
- Click the **x** button on the right side of a track row to remove it.

### Configuring Track Type

Each track can be configured as either **Audio** (for microphone/line-in recording) or
**MIDI** (for keyboard/controller recording).

To change the track type:

- Click the **Options** button on the track, or
- Click the **track type icon** (between "Options" and the name field)

This opens the Track Configuration dialog where you can select the input source.

![Track configuration](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Arming a Track for Recording

To record, you must first "arm" the target track by checking the **Arm** checkbox on
that track. Only one track can be armed at a time; arming a new track automatically
disarms any previously armed track.

## Recording

### Audio Recording

1. Arm the target track (check the "Arm" checkbox).
2. Click the **Record** button (red circle).
3. A 3-second countdown appears on the track, then recording begins.
4. Click the **Stop** button to end recording.

The recorded audio is saved as a FLAC file in the project directory, named after the
track.

#### Sample Rate and Device Handling

Musician's Canvas records at the audio input device's native sample rate and then
converts to the project's configured sample rate using high-quality windowed sinc
interpolation (Kaiser window, ~96 dB stopband attenuation). This is the same algorithm
family used by Audacity and libsoxr.

This means you can set any project sample rate (e.g., 44100 Hz or 48000 Hz) regardless
of the device's native rate. The conversion preserves pitch and duration exactly.

#### Mono Device Detection

Some audio devices (e.g., USB webcam microphones) are physically mono but are advertised
as stereo by the operating system. Musician's Canvas automatically detects this by
comparing the actual data rate against the expected rate and adjusts the channel count
accordingly. If the project is configured for stereo, the mono signal is duplicated to
both channels.

### MIDI Recording

1. Set the track type to **MIDI** via the Options button.
2. Ensure a MIDI input device is configured in **Settings > Configuration > MIDI**.
3. Arm the track and click Record.
4. Play notes on your MIDI controller.
5. Click Stop to end recording.

MIDI notes are displayed in a piano roll visualization on the track.

## Playback

Click the **Play** button to mix and play back all enabled tracks. Disabled tracks
(unchecked) are excluded from playback.

During playback, audio tracks are decoded from their FLAC files and MIDI tracks are
rendered to audio using the built-in FluidSynth synthesizer. All tracks are mixed
together and played through the system's audio output device.

## Mixing to a File

Use **Tools > Mix tracks to file** (Ctrl+M) to export all enabled tracks to a single
WAV or FLAC file. A dialog lets you choose the output path and format.

The mix uses the project's configured sample rate. MIDI tracks are rendered using the
configured SoundFont.

## Settings

### Global Settings

Use **Settings > Configuration** (Ctrl+,) to set global defaults that apply to all
projects:

![General settings](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### General Tab

- **Theme**: Choose between Dark and Light themes.

#### MIDI Tab

- **MIDI Output Device**: Select the built-in FluidSynth synthesizer or an external
  MIDI device.
- **SoundFont**: Browse to a `.sf2` SoundFont file for MIDI synthesis.

![MIDI settings](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Audio Tab

- **Audio Input Device**: Select the microphone or line-in device for recording.
- **Audio Output Device**: Select the speaker or headphone device for playback.

![Audio settings](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Project Settings

Use **Project > Project Settings** (Ctrl+P) to override global defaults for the current
project only. This is useful for projects that need a specific sample rate, SoundFont,
or audio device.

![Project MIDI settings](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### Audio Format Settings

- **Sample rate**: Choose from standard rates (8000 Hz to 96000 Hz). Rates above the
  device's native rate are labeled "(resampled)" and will be converted automatically.
  The device's native rate is labeled "(native)".
- **Channels**: Mono or Stereo. If the device only supports mono, the Stereo option
  may be disabled.

![Project audio settings](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

## Virtual MIDI Keyboard

The Virtual MIDI Keyboard is a companion application (`virtual_midi_keyboard`) that
provides an on-screen piano keyboard for sending MIDI notes.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Features

- Click keys on the on-screen piano to play notes
- Connect to external MIDI output devices or use the built-in FluidSynth synthesizer
- Connect to a MIDI input device to display incoming notes
- Adjustable synthesizer volume (master gain, 10%--200%)
- SoundFont selection for the built-in synthesizer

### Configuration

Open the Configuration dialog to set up MIDI and audio devices:

![Virtual MIDI Keyboard configuration](../screenshots/VMIDIKeyboard2.png)

#### MIDI Tab

- **MIDI Output Device**: Select an external device or leave blank for the built-in
  FluidSynth synthesizer.
- **MIDI Input Device**: Select a controller to forward notes to the keyboard display.
- **Synthesizer Volume (Master Gain)**: Adjust the output level of the built-in
  synthesizer (10%--200%).

#### Audio Tab

- **Audio Output Device**: Select the output device for the built-in synthesizer.
- **SoundFont**: Browse to a `.sf2` SoundFont file.

## Keyboard Shortcuts

| Shortcut        | Action                  |
|-----------------|-------------------------|
| Ctrl+S          | Save project            |
| Ctrl+O          | Open project            |
| Ctrl+M          | Mix tracks to file      |
| Ctrl+P          | Project Settings        |
| Ctrl+,          | Settings / Configuration|
| Ctrl+Q / Alt+F4 | Quit                   |

## Troubleshooting

### No Audio Output

- Check that the correct audio output device is selected in Settings > Configuration >
  Audio.
- On Linux, verify that PipeWire or PulseAudio is running and that the output is not
  muted. Use `amixer` or your desktop's sound settings to check volume levels.
- On Windows, ensure an ASIO driver is installed (required for musicians_canvas).

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

### Virtual MIDI Keyboard Has No Audio

- On Linux with PipeWire, ensure the `libpipewire-0.3-dev` package is installed (needed
  for `pw_init()` before FluidSynth creates audio resources).
- Check that a SoundFont is loaded (see the Audio tab in the configuration dialog).
- Verify the audio output device is selected and the system volume is not muted.

## Building from Source

See the [README](../README.md) for complete build instructions for Linux, macOS, and
Windows, including all required dependencies.
