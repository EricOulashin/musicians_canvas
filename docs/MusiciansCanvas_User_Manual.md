---
title: "Musician's Canvas User Manual"
subtitle: "Multi-Track Music Recording Application"
version: "0.1.2"
date: "2026-04-03"
author: "Eric Oulashin"
---

# Musician's Canvas User Manual

## Introduction

Musician's Canvas is a multi-track music recording application for desktop PCs. It supports
audio recording from microphones and line-in devices, MIDI recording from keyboards and
controllers, and mixing all tracks to a single audio file (WAV, FLAC, MP3, Ogg Vorbis, or AIFF). A companion application,
Virtual MIDI Keyboard, provides a software piano keyboard for sending MIDI notes.

Musician's Canvas is designed for ease of use while providing features commonly found in
digital audio workstations (DAWs):

- Multi-track audio and MIDI recording
- Overdub recording with synchronized playback of existing tracks
- Built-in FluidSynth MIDI synthesizer with SoundFont support
- High-quality sample-rate conversion for recording at any project sample rate
- Automatic mono/stereo device detection
- Project-based settings with per-project overrides
- Mix to WAV, FLAC, MP3, Ogg Vorbis, or AIFF
- Dark and light themes
- Localized in 18 languages including English, German, Spanish, French, Japanese, Portuguese, Chinese, Russian, Swedish, Finnish, Danish, Norwegian, Polish, Greek, Irish, Welsh, and Pirate
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

### Button Bar

Just below the menu bar there is a toolbar with quick-access buttons:

![Button bar](../screenshots/ButtonBar.png)

- **Open Project**: Same as **File > Open Project** — opens a previously saved project.
- **Save Project**: Same as **File > Save Project** — saves the current project. This
  button is only enabled when a project directory is set.
- **Project Settings**: Same as **Project > Project Settings** — opens the project
  settings dialog. This button is only enabled when a project directory is set.
- **Configuration**: Same as **Settings > Configuration** — opens the global application
  settings dialog.
- **Metronome Settings**: Opens the metronome settings dialog (see the
  [Metronome](#metronome) section below).

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

### Adding Tracks by Drag and Drop

While a project is open, you can drag one or more supported audio files from your
file manager (Windows Explorer, macOS Finder, Linux file manager, etc.) directly
onto the Musician's Canvas window to add them as new audio tracks.

- **Supported formats:** `.wav`, `.flac`, `.mp3`, `.ogg`, `.aiff`, `.aif`, and `.aifc`. Files in any other format are
  silently skipped, and a dialog at the end lists which files were skipped.
- **File copying:** If the dropped file is not already in the project directory,
  it is copied there automatically. If a file with the same name already exists
  in the project directory, you are asked whether to replace it.
- **Track name:** The file's base name (without the extension) is used as the
  new track name. For example, dropping `Bass Line.wav` creates an audio track
  named "Bass Line".
- **Multiple files at once:** Several files may be dragged together; each
  supported file becomes its own track in a single drop.
- **When the drop is rejected:** Drops are only accepted while a project is
  open and Musician's Canvas is **not** currently playing or recording. Stop
  playback or recording first if you want to drag in additional tracks.

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

### Recording insert effects (audio tracks only)

Audio tracks have an **Effects** button directly under **Options**. It opens the **Track
effects** dialog, where you can build an **ordered chain** of insert effects for recordings
on that track:

![Track effects dialog](../screenshots/Track_Effects_Dialog.png)

You can add **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion**, **Amp & cabinet** (amplifier and speaker modeling: amp type, cabinet, gain, bass/mid/treble, air/presence, and wet/dry mix), and **Vibrato (Tremolo)**.

**Vibrato (Tremolo)** is the classic Fender-style “Vibrato” found on many amplifiers: an **oscillating change in volume over time**. Use **Speed** to set how fast the volume pulses and **Intensity** (Depth) to set how strong the pulsing is (from subtle shimmer to full chop).

The following screenshots show the **Amp & cabinet** effect:

![Amp & cabinet modeling (1)](../screenshots/Amp_And_Cabinet_Model_1.png)

![Amp & cabinet modeling (2)](../screenshots/Amp_And_Cabinet_Model_2.png)

- Click **Add effect…** and choose an effect type. You can add several instances and remove one with the red **✕** in the effect’s
  header.
- Drag the **≡** grip on a row to **reorder** the chain. The effect at the **top** runs
  **first** on the captured audio; the next runs on its output, and so on.
- Adjust sliders and read-outs for each effect (pre-delay, decay, mix, modulation, chorus
  rate/depth, flanger manual/rate/depth/feedback, overdrive/distortion color and drive, amp/cabinet type and EQ, etc.). Units are in milliseconds and hertz so they stay meaningful after
  the app converts your recording to the **project sample rate**. Both **mono** and **stereo**
  tracks are supported: mono is processed internally as a dual-mono path and mixed back to one
  channel; stereo keeps separate dry paths where the algorithms allow.
- **OK** keeps your edits (they are written to the project). **Cancel** puts the chain back
  to how it was when you opened the dialog.

Effects are applied to audio **when you stop recording**—after the normal capture and
sample-rate conversion pipeline—so what you hear from the effect controls matches what gets
baked into the FLAC for that take. The effect configuration is stored in `project.json`
under that track’s `audioEffectChain` entry.

### Mix effects (full project)

**Project → Project Settings → Mix Effects** lets you build the same kind of ordered effect chain as **Track effects** (**Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion**, **Amp & cabinet**, **Vibrato (Tremolo)**), but applied to the **entire mixed program**: when you press **Play** to hear all enabled tracks together, and when you export with **Mix tracks to file** (toolbar or **Tools** menu). The chain is saved in `project.json` under `projectSettings` → `mixEffectChain`.

**Project → Project Settings → Aux / Send Bus** configures a **shared effect chain** fed by each track’s **Aux** send slider (on the track row). The dry mix of all tracks is summed, each track’s post-gain/post-pan signal is scaled by its **Aux** level and sent through this bus, then the **wet aux** output is added back to the dry sum **before** **Mix Effects** run. Use it for a single shared reverb/delay while keeping per-track insert effects independent.

To reduce harsh [digital clipping](https://en.wikipedia.org/wiki/Clipping_%28audio%29) when processing pushes peaks toward full scale, the effect engine applies a **soft limiter** to normalized float samples immediately before conversion to 16-bit PCM. The **EffectWidget** base class documents `guardFloatSampleForInt16Pcm()` and `softLimitFloatSampleForInt16Pcm()` for any new real-time code that writes to 16-bit audio.

### Per-track mixer, mute/solo, trim, and MIDI details

Each track row includes a compact **mixer strip**:

- **Gain**: Per-track level in decibels (the slider uses tenths of a dB; 0 dB = unity gain).
- **Pan**: Stereo placement (-100 = full left, +100 = full right).
- **Aux**: Send amount (0–100%) into the project **Aux / Send Bus** (see above).
- **Mute**: Silences the track in the mix without disabling it in the arrange view.
- **Solo**: If **any** track has **Solo** enabled, **only** soloed tracks are heard (unless they are also muted).

**Options → Track Configuration** also offers **Clip trim (non-destructive)**: **Trim start** and **Trim end** skip that many seconds from the beginning and end of the clip for **playback, mix, and export** without deleting the underlying recording.

MIDI tracks can carry **control change (CC)** automation stored in the project and in exported `.mid` files; offline playback and mix use these events when rendering MIDI to audio.

**Edit → Undo** / **Redo** (standard shortcuts) apply to mixer and trim changes made on tracks.

### Monitor while recording

Next to the main **time display**, the **Monitor audio while recording** checkbox controls
whether you hear **live input** through the **project audio output** while the transport is
recording:

- **Audio tracks**: The sound from the input device is sent to the output in real time (the
  usual capture and WAV/FLAC pipeline is unchanged). This is layered on top of **overdub**
  playback if other tracks are playing along.
- **MIDI tracks**: When the project is set to **render MIDI to audio for playback** and a
  **SoundFont** is configured, incoming MIDI is played through the same software synthesizer
  route so you can hear notes as you record. If you use **external MIDI output** instead, use
  your instrument’s own monitoring; this checkbox mainly affects the on-screen synth path.

The choice is **stored in the project** (field `monitorWhileRecording` in `project.json`).
Turn monitoring **off** if you do not want to hear the input in your speakers or headphones
(for example to reduce feedback when using a microphone).

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

## Metronome

Musician's Canvas includes a built-in metronome that can be used during recording to
help keep time. Click the metronome button on the button bar (below the menu bar) to
open the metronome settings dialog:

![Metronome settings](../screenshots/MetronomeSettings.png)

The dialog provides:

- **Enable metronome during recording**: When checked, the metronome plays a tick sound
  while recording is active. The tick is played through the system audio and is **not**
  captured into the recorded track.
- **Beats per minute**: A numeric input for the tempo, in beats per minute (BPM). The
  range is 20–300 BPM. When **Project → Tempo map** defines tempo changes, the metronome
  follows those markers during recording (the BPM field still sets the initial tempo when
  no marker applies yet).

When the metronome is enabled, it begins ticking once recording actually starts (after
the 3-second countdown completes), and stops when recording ends.

## Playback

Click the **Play** button to mix and play back all enabled tracks. The button tooltip
changes to indicate whether it will play or record based on whether a track is armed.
Disabled tracks (unchecked) are excluded from playback.

During playback, audio tracks are decoded from their audio files on disk (typically FLAC in the project folder) and MIDI tracks are
rendered to audio using the built-in FluidSynth synthesizer. All tracks are mixed
together and played through the system's audio output device.

Click the **Stop** button to end playback at any time.

## Mixing to a File

Use **Tools > Mix tracks to file** (Ctrl+M) to export all enabled tracks to a single
audio file. A dialog lets you choose the output path and format:

- **Output file**: Browse to select the destination file path.
- **Format**: Choose the file extension when you browse — **FLAC** (lossless compression),
  **WAV** (uncompressed PCM), **MP3**, **Ogg Vorbis**, or **AIFF**. Lossy formats (MP3, Ogg)
  produce smaller files; WAV, FLAC, and AIFF can be lossless depending on source content.

The mix uses the project's configured sample rate. MIDI tracks are rendered using the
configured SoundFont. Per-track **gain**, **pan**, **mute/solo**, **aux send**, and
**trim** are applied the same way as during playback.

**Tools → Export stems to folder** opens a dialog where you pick the output folder and **file format**
(WAV, FLAC, MP3, Ogg Vorbis, AIFF, etc.). One stem file is written per track (enabled tracks that
participate in the mix). Each stem reflects that track’s mixer settings and trim; master
**Mix Effects** on the full program are **not** applied to individual stems.

## Settings

### Global Settings

Use **Settings > Configuration** (Ctrl+,) to set global defaults that apply to all
projects:

![General settings](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### General Tab

- **Theme**: Choose between Dark and Light themes.
- **Write recording debug log**: When enabled, Musician's Canvas writes detailed
  recording diagnostic information to a file called `recording_debug.txt` in the
  project directory. This includes the capture backend, device name, sample rate,
  channel count, and timing data. This is disabled by default and is primarily useful
  for troubleshooting recording issues.

#### Display Tab

- **Numeric display LED color**: Choose the color used for the numeric LED time display
  shown on the main window toolbar. The active digit segments are drawn in the chosen
  color, and inactive segments are drawn as a dim version of the same color. Available
  colors are Light Red, Dark Red, Light Green, Dark Green, Light Blue, Dark Blue,
  Yellow, Orange, Light Cyan, and Dark Cyan. The default is Light Green.

![Display settings](../screenshots/MusiciansCanvas_DisplaySettings.png)

#### Language Tab

- **Language**: Choose the display language for the application. The default is "System
  Default", which uses the operating system's language setting. Available languages are
  English, Deutsch (German), Español (Spanish), Français (French), Japanese, Português
  (Brazilian Portuguese), Chinese (Traditional), Chinese (Simplified), Dansk (Danish),
  Ελληνικά (Greek), Gaeilge (Irish), Norsk (Norwegian), Polski (Polish),
  Русский (Russian), Suomi (Finnish), Svenska (Swedish), Cymraeg (Welsh), and Pirate.
  The interface updates immediately when you change the language.

![Language settings](../screenshots/MusiciansCanvas_LanguageSettings.png)

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

#### Mix Effects tab

The **Mix Effects** tab is a scrollable list with the same controls as **Track effects** (**Add effect…**, drag **≡** to reorder, **✕** to remove). Processing order is **top to bottom** on the **combined** mix of all enabled tracks. These effects run during **whole-project playback** and when **mixing to a single audio file** (same formats as export); they are **not** baked into individual track files on disk. An empty list leaves the mixed signal unchanged aside from the mixer’s own level handling.

#### Aux / Send Bus tab

Configure the **shared aux effect chain** (same effect types as track inserts). Each track’s **Aux** slider on the track row controls how much of that track’s signal is sent through this bus; the wet aux return is summed with the dry mix **before** **Mix Effects** are applied.

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
| Project Settings                    | Ctrl+P   | Configure project-specific settings (includes **Aux / Send Bus**) |
| Tempo map                           |          | Edit tempo changes (seconds vs BPM) used for metronome timing hints and MIDI quantization |
| Add Demo Data to Selected Track     |          | Add sample MIDI notes for demonstration  |

### Settings Menu

| Menu Item     | Shortcut | Description                       |
|---------------|----------|-----------------------------------|
| Configuration | Ctrl+,   | Open global application settings  |

### Edit Menu

| Menu Item | Shortcut | Description |
|-----------|----------|-------------|
| Undo      | Ctrl+Z   | Undo recent mixer/trim edits on tracks |
| Redo      | Ctrl+Shift+Z | Redo |

### Tools Menu

| Menu Item             | Shortcut | Description                              |
|-----------------------|----------|------------------------------------------|
| Mix tracks to file    | Ctrl+M   | Export all enabled tracks to a file      |
| Export stems to folder |         | Write one stem file per track; choose format in the dialog (with mixer gain/pan/trim; no master Mix Effects) |
| Recording options     |          | **Punch-in** region for audio retakes; **loop playback** region for whole-project play |
| Quantize MIDI         |          | Snap MIDI note starts to a grid (all MIDI tracks or armed track only) |
| Add drum track        | D        | Add a MIDI drum track and write a `.mid` groove (see below) |
| Virtual MIDI Keyboard |          | Launch the companion keyboard app        |

### Help Menu

| Menu Item | Shortcut | Description |
|----------|----------|-------------|
| Manual   | Alt+M    | Open the PDF user manual in the currently selected language |
| About    |          | Show version and application information |

### Add drum track

**Tools → Add drum track** (shortcut **D**) creates a new **MIDI** track intended for percussion: notes are on **MIDI channel 10** (General MIDI drums; internally channel index 9). The track is given a default name such as **Drums** (with a numeric suffix if that name is already taken).

A **Standard MIDI File** (`.mid`) is written into the **project directory** immediately, containing a **two-bar 4/4** pattern with kick, snare, and closed hi-hat at a tempo chosen as follows:

- If **Metronome Settings** (toolbar **metronome** button) has **Enable metronome during recording** **enabled**, the pattern uses the **Beats per minute** value from that dialog.
- If the metronome is **disabled**, the application **estimates BPM** from a **mono mix** of all **enabled** **audio** tracks that contain recorded audio. If there is no usable audio or estimation fails, **120 BPM** is used.

Save the project (**File → Save Project**) so `project.json` references the new `.mid` file like any other MIDI track.

**Note:** BPM detection is a simple onset-autocorrelation helper; it works best on rhythmic material and may pick half/double time on some sources—adjust the metronome and re-add the track, or edit the MIDI file in another tool if needed.

**Further reading (external):** [Audient — How to program realistic MIDI drum tracks](https://audient.com/tutorial/how-to-program-realistic-midi-drum-tracks/), [MeldaProduction — MDrummer](https://www.meldaproduction.com/MDrummer) (see also Melda’s [MDrummer overview video](https://www.youtube.com/watch?v=qfXuNcfDuIA)), [Reddit — generating MIDI drum patterns from audio](https://www.reddit.com/r/ableton/comments/1e51a7g/generating_midi_drum_patterns_based_on_audio_input/), the overview paper *[Automated Music Track Generation](https://cs229.stanford.edu/proj2014/Louis%20Eugene,%20Guillaume%20Rostaing,%20Automated%20Music%20Track%20Generation.pdf)* (Stanford CS229), and an informal video on [playing drum-like MIDI from a keyboard controller](https://www.youtube.com/watch?v=jFVMKf8-IXk).

## Keyboard Shortcuts

| Shortcut        | Action                         |
|-----------------|--------------------------------|
| Ctrl+S          | Save project                   |
| Ctrl+O          | Open project                   |
| Ctrl+Z          | Undo (mixer/trim)              |
| Ctrl+Shift+Z    | Redo                           |
| Ctrl+M          | Mix tracks to file             |
| D               | Add drum track (Tools menu)    |
| Ctrl+P          | Project Settings               |
| Ctrl+,          | Settings / Configuration       |
| Alt+M           | Open PDF manual (Help)         |
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
- **MIDI Channel**: A numeric input (1--16) selects the MIDI channel that notes,
  program changes, and control changes are sent on. The default is channel 1. The
  selected channel takes effect immediately when changed. Picking a SoundFont
  preset while a particular channel is selected applies that bank/program to that
  channel, so you can play different sounds on different channels.
- **Drums**: A checkbox just above the MIDI channel selector. When unticked
  (the default), the MIDI channel is whatever the user has chosen. When ticked,
  the MIDI channel is automatically switched to channel 10 — the General MIDI
  standard drum channel — so anything played sounds as percussion. Unticking
  the box restores the channel that was selected before Drums was enabled.
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

## FAQ (Frequently Asked Questions)

### How do I record a MIDI track?

1. Set a **project directory** (required for saving).
2. Add or select a track and open **Options** (or click the track type icon).
3. Set the track type to **MIDI**, then close the dialog.
4. In **Settings > Configuration > MIDI**, choose your **MIDI input** (hardware port or virtual cable) and a **SoundFont** (`.sf2`) for playback later.
5. **Arm** that track (only one track can be armed at a time).
6. Click **Record**, wait for the countdown, play your controller, then click **Stop**.

Notes are shown in the piano roll. Use **File > Save Project** so a `.mid` file (and `project.json`) is written in the project folder.

### Why is my MIDI track silent when I play it back?

Playback uses the **built-in FluidSynth** synthesizer with the **SoundFont** from your settings. Check **Settings > Configuration > MIDI** (or **Project > Project Settings** if you override per project): a valid `.sf2` path must be set. On Linux a system SoundFont may be found automatically; on Windows and macOS you usually need to browse to a SoundFont file manually.

### How does Virtual MIDI Keyboard relate to Musician's Canvas?

They are **separate programs**. Launch Virtual MIDI Keyboard from **Tools > Virtual MIDI Keyboard** (or run it on its own). To send notes from the on-screen keyboard **into** Musician's Canvas while recording a MIDI track, your operating system must route the keyboard’s **MIDI output** to an input port that Musician's Canvas is using—often via a virtual MIDI cable (e.g. loopback driver) or by selecting matching ports in both apps. The apps do not auto-connect to each other.

### What is the difference between Configuration and Project Settings?

**Settings > Configuration** sets **global defaults** (theme, language, default MIDI/audio devices, SoundFont, etc.) stored in the application’s own settings. **Project > Project Settings** overrides some of those choices **for the current project only** and is stored inside `project.json`. If a field is left at the project default, the global Configuration value is used.

### Why won’t drag-and-drop add audio files?

Dropping files is only accepted when a **project directory is set**, and Musician's Canvas is **not** playing or recording. Supported extensions include **`.wav`**, **`.flac`**, **`.mp3`**, **`.ogg`**, **`.aiff`**, **`.aif`**, and **`.aifc`**; other extensions are skipped and listed in a dialog. Each dropped file becomes a new **audio** track named from the file’s base name.

### Where are my recordings stored?

Audio recordings are saved in the **project directory** as **`<track_name>.flac`** (with characters like `/ \ : * ? " < > |` turned into underscores). The project file is **`project.json`** in the same folder. MIDI tracks are saved as **`<track_name>.mid`** when you save the project (alongside note data in `project.json`).

### What file name is used when a MIDI track is rendered for mixing?

When you **mix** or **play back**, MIDI is synthesized to a temporary WAV internally. If the project path is known, Musician's Canvas also writes a **cached FLAC** in the project folder named **`<sanitized_track_name>.flac`** (same sanitization as other track files). This helps reuse renders; the important rule is that it matches the **track name**, not an internal ID.

### Can I record two tracks at once?

No. Only **one** track can be **armed** at a time; that track receives the next recording. You can build a song by recording tracks one after another (**overdub** plays existing tracks while you record a new one).

### Is the metronome recorded into my track?

No. When enabled, the metronome plays through **system audio** for your timing only. It is **not** mixed into the recorded file.

### Why does Musician's Canvas say I need ASIO on Windows?

On Windows the main application expects an **ASIO** driver for reliable low-latency audio. Install a driver such as **ASIO4ALL** or one from your interface manufacturer if you see startup or audio errors.

### On macOS, where is Virtual MIDI Keyboard installed?

When you use the **`.app` bundle**, the Virtual MIDI Keyboard executable is **copied inside** `Musician's Canvas.app` (under **Contents/MacOS/**) so you can distribute a single app folder. You can still run it from **Tools > Virtual MIDI Keyboard** in the menu.

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
