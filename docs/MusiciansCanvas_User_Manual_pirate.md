---
title: "Musician's Canvas Captain's Log & User Manual"
subtitle: "A Multi-Shanty Music Plunderin' Application fer Scallywags an' Sea Dogs Alike"
version: "0.1.0"
date: "2026-04-03"
author: "Cap'n Eric Oulashin"
lang: "en-pirate"
---

# Musician's Canvas User Manual -- Pirate Edition

## Introduction

Ahoy, matey! Welcome aboard Musician's Canvas, a mighty multi-shanty music plunderin' application fer desktop vessels. She supports plunderin' sounds from microphones an' line-in devices, capturin' MIDI treasure from keyboards an' controllers, an' mixin' all yer shanties down to a single WAV or FLAC file. A trusty companion ship, Virtual MIDI Keyboard, provides a software pianer keyboard fer sendin' MIDI notes across the briny deep.

Musician's Canvas be designed fer ease o' use while providin' features commonly found in digital audio workstations (DAWs), savvy?

- Multi-shanty audio an' MIDI plunderin'
- Overdub plunderin' with synchronized replayin' o' existing shanties
- Built-in FluidSynth MIDI synthesizer with SoundFont support
- High-quality sample-rate conversion fer plunderin' at any project sample rate
- Automatic mono/stereo device detection -- she figures it out on 'er own!
- Project-based settings with per-project overrides
- Mix to WAV or FLAC
- Dark an' light themes (fer sailin' by night or by day)
- Localized in multiple tongues (English, Deutsch, Español, Français, Japanese, Português, Chinese, an' Pirate -- the finest language on the seven seas!)
- Companion Virtual MIDI Keyboard application

## Gettin' Started

### Launchin' the Ship

Run the `musicians_canvas` executable from the build directory or yer installation berth:

```
./musicians_canvas
```

On first launch, the application opens with an empty project, like a blank treasure map. Ye'll need to set a project directory before ye can start plunderin' sounds.

On startup, the application applies the saved theme (dark or light) an', if a project directory was previously charted an' contains a `project.json` file, the project be automatically loaded fer ye.

### Settin' Up a Project

1. **Set the project directory**: Enter or browse to a folder in the "Project Location" field at the top o' the window. This be where yer plundered recordings an' the project file will be stowed.

2. **Add a shanty**: Click the **+ Add Track** button. A new shanty appears with default settings. If it be the only shanty in the project an' has not been recorded yet, it be automatically armed fer plunderin'.

3. **Name the shanty**: Type a name in the text field next to the "Options" button. This name be used as the filename fer the recorded audio file, so choose wisely, ye scallywag!

![Main window](../screenshots/MusiciansCanvas_1_MainWin.png)

### Savin' and Openin' Projects

- **Save**: Use **File > Save Project** (Ctrl+S) to save yer current project as a JSON file in the project directory. Don't forget to save, lest ye lose yer treasure!
- **Open**: Use **File > Open Project** (Ctrl+O) to load a previously saved project from the depths.

The project file (`project.json`) stores shanty names, types, MIDI notes, audio file references, an' all project-specific settings. Audio files be stored in the same directory as `project.json` an' be named after their shanties (e.g., `My_Track.flac`).

If ye close the application with unsaved changes, a confirmation dialog asks whether to save before abandonin' ship.

## Shanty Management

### Addin' an' Removin' Shanties

- Click **+ Add Track** to add a new shanty to the arrangement.
- Click the **x** button on the starboard side of a shanty row to remove it -- walk the plank, shanty!
- Click **Clear Tracks** (the red button in the toolbar) to scuttle all shanties. A confirmation dialog be shown before proceedin', so ye don't accidentally sink yer own fleet.

### Configurin' Shanty Type

Each shanty can be configured as either **Audio** (fer microphone/line-in plunderin') or **MIDI** (fer keyboard/controller plunderin').

To change the shanty type:

- Click the **Options** button on the shanty, or
- Click the **track type icon** (between "Options" and the name field)

This opens the Track Configuration dialog where ye can select the input source, arr.

![Track configuration](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Shanty Controls

Each shanty row provides the followin' controls:

- **Enable checkbox**: Toggles whether the shanty be included in replayin' an' mixin'. Disablin' a shanty also automatically disarms it if it was armed -- can't fire a cannon that ain't loaded!
- **Arm radio button**: Selects this shanty as the plunderin' target. Only one shanty can be armed at a time; armin' a new shanty automatically disarms any previously armed one.
- **Name field**: Editable text field fer the shanty name. This name be used as the audio filename (with invalid filesystem characters replaced by underscores).
- **Options button**: Opens the Track Configuration dialog.
- **Type icon**: Shows a speaker icon fer audio shanties or a piano icon fer MIDI shanties. Clickin' it opens the Track Configuration dialog.
- **Remove button (x)**: Sends the shanty to Davy Jones' locker.

### Auto-Armin'

When a project has exactly one shanty an' that shanty has not been recorded yet, it be automatically armed fer plunderin'. This applies both when addin' the first shanty to a new project an' when openin' an existing project that has a single empty shanty. Convenient, that be!

### Shanty Visualization

- **Audio shanties** display a waveform visualization of the plundered audio. When no audio has been plundered, the area shows "No audio recorded".
- **MIDI shanties** display a piano roll visualization showin' recorded notes on a grid spannin' A0 to C8. Notes be colored by velocity -- the harder ye strike, the bolder the color! When no MIDI data has been plundered, the area shows "No MIDI data recorded".

## Plunderin' Sounds (Recording)

### Audio Plunderin'

1. Ensure the project directory be set.
2. Arm the target shanty (check the "Arm" radio button).
3. Click the **Record** button (red circle) -- fire the cannons!
4. A 3-second countdown appears on the shanty ("Get ready... 3", "2", "1"), then plunderin' begins.
5. Durin' plunderin', a live level meter be displayed in the shanty's waveform area, showin' the current amplitude as a gradient bar (green to yellow to red) with a "Recording" label.
6. Click the **Stop** button to end plunderin'.

The plundered audio be saved as a FLAC file in the project directory, named after the shanty.

Durin' plunderin' an' replayin', all interactive controls (shanty buttons, settings, etc.) be disabled to prevent accidental changes. Keep yer hands off the riggin' while we're recordin'!

#### Overdub Plunderin'

When plunderin' a new shanty while other enabled shanties already contain audio or MIDI data, Musician's Canvas performs overdub plunderin': the existing shanties be mixed together an' played back in real time while the new shanty be recorded. This lets ye hear previously plundered parts while layin' down a new one, like singin' harmony with yer crewmates!

The mix o' existing shanties be prepared before capture begins, so plunderin' an' replayin' start at approximately the same instant, keepin' all shanties synchronized like a well-drilled crew.

#### Plunderin' Backends

Musician's Canvas supports two audio capture backends:

- **PortAudio** (default when available): Provides reliable, low-latency plunderin' an' be the recommended backend. Steady as she goes!
- **Qt Multimedia**: A fallback backend using Qt's built-in audio capture. Used when PortAudio be not available or when explicitly selected in Project Settings.

The plunderin' backend can be configured per-project in **Project > Project Settings > Audio**.

#### Sample Rate an' Device Handlin'

Musician's Canvas records at the audio input device's native sample rate an' then automatically converts to the project's configured sample rate using high-quality resamplin'. This means ye can set any project sample rate (e.g., 44100 Hz or 48000 Hz) regardless o' the device's native rate, arr. The conversion preserves pitch an' duration exactly -- no cursed pitch-shiftin' here!

#### Mono Device Detection

Some audio devices (e.g., USB webcam microphones) be physically mono but are advertised as stereo by the operatin' system -- sneaky bilge rats! Musician's Canvas automatically detects this an' adjusts the channel count accordingly. If the project be configured fer stereo, the mono signal be duplicated to both channels.

### MIDI Plunderin'

1. Set the shanty type to **MIDI** via the Options button.
2. Ensure a MIDI input device be configured in **Settings > Configuration > MIDI**.
3. Arm the shanty an' click Record.
4. Play notes on yer MIDI controller -- let the music flow like rum!
5. Click Stop to end plunderin'.

MIDI notes be displayed in a piano roll visualization on the shanty.

## Replayin' the Shanty (Playback)

Click the **Play** button to mix an' replay all enabled shanties. The button tooltip changes to indicate whether it will play or record based on whether a shanty be armed. Disabled shanties (unchecked) be excluded from replayin' -- they stay below deck.

Durin' replayin', audio shanties be decoded from their FLAC files an' MIDI shanties be rendered to audio using the built-in FluidSynth synthesizer. All shanties be mixed together an' played through the system's audio output device. 'Tis a grand sound, if ye do say so yerself!

Click the **Stop** button to end replayin' at any time.

## Mixin' to a File

Use **Tools > Mix tracks to file** (Ctrl+M) to export all enabled shanties to a single audio file. A dialog lets ye choose the output path an' format:

- **Output file**: Browse to select the destination file path -- where shall we bury this treasure?
- **Format**: Choose between FLAC (lossless compression, smaller files) or WAV (uncompressed).

The mix uses the project's configured sample rate. MIDI shanties be rendered using the configured SoundFont.

## Settings

### Global Settings

Use **Settings > Configuration** (Ctrl+,) to set global defaults that apply to all projects across the seven seas:

![General settings](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### General Tab

- **Theme**: Choose between Dark an' Light themes. Dark fer night watches, Light fer sunny sailin'!

#### Language Tab

- **Language**: Choose the display language fer the application, ye worldly pirate! The
  default be "System Default", which uses the operatin' system's language settin'. Available
  tongues be English, Deutsch (German), Español (Spanish), Français (French), Japanese,
  Português (Brazilian Portuguese), Chinese (Traditional), Chinese (Simplified), an' Pirate.
  The interface updates immediately when ye change the language -- no need to restart the ship!

#### MIDI Tab

- **MIDI Output Device**: Select the built-in FluidSynth synthesizer or an external MIDI device. Use the **Refresh** button to rescan fer available MIDI devices on the horizon.
- **SoundFont**: Browse to a `.sf2` SoundFont file fer MIDI synthesis. On Linux, a system SoundFont may be auto-detected if the `fluid-soundfont-gm` package be installed. On Windows an' macOS, ye must configure the SoundFont path manually, ye landlubber.

![MIDI settings](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Audio Tab

- **Audio Input Device**: Select the microphone or line-in device fer plunderin' sounds.
- **Audio Output Device**: Select the speaker or headphone device fer replayin' shanties.

![Audio settings](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Project Settings

Use **Project > Project Settings** (Ctrl+P) to override global defaults fer the current project only. This be useful fer projects that need a specific sample rate, SoundFont, or audio device. Project-specific settings be saved inside the `project.json` file, safe in the treasure chest.

![Project MIDI settings](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### MIDI Tab

- **MIDI Device**: Select a MIDI device fer this project, or leave at the default to use the global settin'.
- **SoundFont**: Select a SoundFont file fer this project.
- **Refresh**: Rescan fer available MIDI devices -- look alive, swab the decks fer new gear!

#### Audio Tab

- **Audio Input Device**: Select the plunderin' device fer this project.
- **Recording capture backend** (when PortAudio be available):
  - **PortAudio (native input)**: Recommended. Uses the same audio library as Audacity. A fine vessel!
  - **Qt Multimedia**: Fallback option using Qt's built-in audio capture.
- **PortAudio Input Device**: When using the PortAudio backend, select the specific PortAudio input device.
- **Audio Output Device**: Select the replayin' device fer this project.

##### Audio Format Settings

- **Sample rate**: Choose from standard rates (8000 Hz to 192000 Hz). The device's native rate be labeled "(native)". Rates that require resamplin' be labeled "(resampled)". Ye can select any rate regardless of device capability; Musician's Canvas will automatically resample as needed -- no excuses, we sail at any speed!
- **Channels**: Mono or Stereo. If the device only supports mono, the Stereo option be disabled.

![Project audio settings](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

## Menus

### File Menu

| Menu Item     | Shortcut | Description                                    |
|---------------|----------|------------------------------------------------|
| Save Project  | Ctrl+S   | Save yer current project to disk               |
| Open Project  | Ctrl+O   | Open an existing project file from the depths   |
| Close         | Ctrl+Q   | Abandon ship (quit the application)            |

### Project Menu

| Menu Item                           | Shortcut | Description                                       |
|-------------------------------------|----------|---------------------------------------------------|
| Project Settings                    | Ctrl+P   | Configure project-specific settings               |
| Add Demo Data to Selected Track     |          | Add sample MIDI notes fer demonstration purposes  |

### Settings Menu

| Menu Item     | Shortcut | Description                              |
|---------------|----------|------------------------------------------|
| Configuration | Ctrl+,   | Open global application settings, matey  |

### Tools Menu

| Menu Item             | Shortcut | Description                                      |
|-----------------------|----------|--------------------------------------------------|
| Mix tracks to file    | Ctrl+M   | Export all enabled shanties to a single file      |
| Virtual MIDI Keyboard |          | Launch the companion keyboard vessel              |

## Keyboard Shortcuts

| Shortcut        | Action                                  |
|-----------------|-----------------------------------------|
| Ctrl+S          | Save project (stow yer treasure!)       |
| Ctrl+O          | Open project                            |
| Ctrl+M          | Mix shanties to file                    |
| Ctrl+P          | Project Settings                        |
| Ctrl+,          | Settings / Configuration                |
| Ctrl+Q / Alt+F4 | Quit (abandon ship)                    |

## Virtual MIDI Keyboard

The Virtual MIDI Keyboard be a companion vessel (`virtual_midi_keyboard`) that provides an on-screen pianer keyboard fer sendin' MIDI notes across the musical seas. It can be launched from the **Tools > Virtual MIDI Keyboard** menu in the main application, or run independently as a lone privateer.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Features

- Click keys on the on-screen pianer to play notes -- tickle them ivories!
- Use the computer keyboard as a pianer keyboard (see key mappin' below)
- Connect to external MIDI output devices or use the built-in FluidSynth synthesizer
- Connect to a MIDI input device to display incomin' notes on the keyboard
- Adjustable synthesizer volume (master gain, 10%--200%)
- SoundFont selection fer the built-in synthesizer
- MIDI instrument/program selection (General MIDI or SoundFont presets)
- Chorus/effect control knob -- add some sea shanty reverb!
- Octave shiftin' (-3 to +5)
- MIDI volume control (CC#7, 0--127)

### Toolbar Controls

- **Volume**: MIDI volume (CC#7), adjustable from 0 to 127 via the spin box. Crank it up, ye sea dog!
- **Octave**: Shift the keyboard octave using the **<** and **>** buttons or the spin box. Range be -3 to +5.
- **Chorus/Effect**: A rotary knob an' text field (1--127) to set the chorus/effect level (MIDI CC#93). Click **Apply** to send the value across the wire.
- **Instrument selector**: Choose a MIDI instrument. When using the built-in FluidSynth synthesizer, this shows SoundFont presets (Bank:Program Name). When connected to an external MIDI device, this lists the 128 General MIDI instruments.

### Playin' with the Computer Keyboard

The computer keyboard be mapped to pianer keys across two octaves. Here be yer treasure map o' keys:

**Lower octave (startin' at the current octave):**

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

Keys light up visually when pressed (white keys turn light blue like the Caribbean sea, black keys darken like a moonless night).

### Configuration

Open the Configuration dialog (**Settings > Configuration**, Ctrl+,) to set up MIDI an' audio devices:

![Virtual MIDI Keyboard configuration](../screenshots/VMIDIKeyboard2.png)

#### MIDI Tab

- **MIDI Output Device**: Select an external device or leave blank fer the built-in FluidSynth synthesizer.
- **MIDI Input Device**: Select a controller to forward notes to the keyboard display.
- **Synthesizer Volume (Master Gain)**: Adjust the output level o' the built-in synthesizer (10%--200%). From a whisper to a cannon blast!
- **SoundFont**: Browse to a `.sf2` SoundFont file fer the built-in synthesizer.
- **Refresh**: Rescan fer available MIDI devices.

#### Audio Tab

- **Audio Output Device**: Select the output device fer the built-in synthesizer.

#### Language Tab

- **Language**: Choose the display language. The same tongues as Musician's Canvas be
  available. The interface updates immediately when ye change the language, arr!

### Virtual MIDI Keyboard Shortcuts

| Shortcut | Action                       |
|----------|------------------------------|
| Ctrl+,   | Configuration dialog         |
| Ctrl+U   | Help / Usage info            |
| Ctrl+Q   | Close (abandon this vessel)  |

## Troubleshootin' (When Ye Hit Rough Seas)

### No Audio Output

- Check that the correct audio output device be selected in Settings > Configuration > Audio. Did ye plug in yer speakin' horns?
- On Linux, verify that PipeWire or PulseAudio be runnin' an' that the output be not muted. Use `amixer` or yer desktop's sound settings to check volume levels. Don't let silence be yer only shanty!
- On Windows, ensure an ASIO audio driver be installed (e.g., [ASIO4ALL](https://asio4all.org/) or a manufacturer-provided ASIO driver fer yer audio interface). Musician's Canvas requires ASIO fer low-latency audio on Windows.

### MIDI Shanties Be Silent

- Ensure a SoundFont (`.sf2` file) be configured in Settings > Configuration > MIDI. Without a SoundFont, the synth be mute as a dead parrot!
- On Linux, a system SoundFont may be auto-detected if the `fluid-soundfont-gm` package be installed.
- On Windows an' macOS, ye must configure the SoundFont path manually.

### Plundered Sounds Be Distorted or Have Wrong Pitch

- This can happen when the audio input device's native sample rate differs from the project's configured rate. Musician's Canvas automatically handles this via resamplin', but if issues persist, try settin' the project sample rate to match the device's native rate.
- USB webcam microphones often have unusual native rates (e.g., 32000 Hz). The application detects these automatically, the clever lass.
- If using the Qt Multimedia backend an' experiencin' issues, try switchin' to the PortAudio backend in Project Settings > Audio. Sometimes ye need a different ship fer different waters!

### Virtual MIDI Keyboard Has No Audio

- On Linux with PipeWire, ensure the `libpipewire-0.3-dev` package be installed (needed fer PipeWire integration with the FluidSynth synthesizer).
- Check that a SoundFont be loaded (see the MIDI tab in the configuration dialog). No SoundFont, no sound -- savvy?
- Verify the audio output device be selected an' the system volume be not muted. Arr, don't be a fool -- check the obvious first!

## Buildin' from Source

See the [README](../README.md) fer complete build instructions fer Linux, macOS, an' Windows, includin' all required dependencies. Fair winds an' smooth compilin' to ye!

---

*Arr, that be the end o' the manual, ye salty sea dog! Now go forth an' make glorious music on the high seas! May yer shanties be mighty an' yer recordings be free o' clippin'!* 🏴‍☠️
