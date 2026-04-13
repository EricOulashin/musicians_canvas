---
title: "Musician's Canvas Captain's Log & User Manual"
subtitle: "A Multi-Shanty Music Plunderin' Application fer Scallywags an' Sea Dogs Alike"
version: "0.1.1"
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
- Localized in 18 tongues includin' English, Deutsch, Español, Français, Japanese, Português, Chinese, Русский, Svenska, Suomi, Dansk, Norsk, Polski, Ελληνικά, Gaeilge, Cymraeg, an' Pirate -- the finest language on the seven seas!
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

![Main window](../screenshots/i18n/pirate/MusiciansCanvas_1_MainWin.png)

### Button Bar (The Quick-Draw Belt)

Just below the menu bar, ye'll find a row o' trusty buttons for quick plunderin':

![Button bar](../screenshots/i18n/pirate/ButtonBar.png)

- **Open Project**: Same as **File > Open Project** — hoists up a shanty ye stashed away earlier.
- **Save Project**: Same as **File > Save Project** — buries the current shanty in yer chest.
  This button be only lively when ye've marked a project directory on yer map.
- **Project Settings**: Same as **Project > Project Settings** — opens the shanty settings
  parchment. This button be only lively when ye've marked a project directory on yer map.
- **Configuration**: Same as **Settings > Configuration** — opens the global ship's settings parchment.
- **Metronome Settings**: Opens the metronome settings parchment (see the Metronome section below, matey).

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

### Haulin' Shanties Aboard by Drag an' Drop

While ye have a project open, ye can drag one or more seaworthy audio files from
yer file manager (Windows Explorer, macOS Finder, Linux file manager, or whate'er
chart ye use) directly onto the Musician's Canvas windo' to haul 'em aboard as
fresh audio shanties.

- **Seaworthy formats:** `.wav` and `.flac`. Files in any other format be quietly
  tossed overboard, an' a dialog at the end lists which files were cast off.
- **Copyin' the booty:** If the dropped file ain't already stashed in the
  project's hold, it be copied there automatic-like. If a file with the same
  name already be in the hold, ye'll be asked whether to replace the old one --
  no accidental keelhaulin' of yer treasures.
- **Shanty name:** The file's base name (without the extension) be used as the
  new shanty name. Fer example, droppin' `Bass Line.wav` creates an audio shanty
  christened "Bass Line".
- **Many files in one haul:** Several files can be dragged together; each
  seaworthy file becomes its own shanty in a single drop, savin' ye the trouble
  o' hoistin' 'em one at a time.
- **When the drop be refused:** Drops only be accepted while a project be open
  an' Musician's Canvas be **not** currently playin' or plunderin'. Belay the
  playback or recordin' first if ye wish to drag more shanties aboard.

### Configurin' Shanty Type

Each shanty can be configured as either **Audio** (fer microphone/line-in plunderin') or **MIDI** (fer keyboard/controller plunderin').

To change the shanty type:

- Click the **Options** button on the shanty, or
- Click the **track type icon** (between "Options" and the name field)

This opens the Track Configuration dialog where ye can select the input source, arr.

![Track configuration](../screenshots/i18n/pirate/MusiciansCanvas_2_TrackConfig.png)

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

### Insert effects (audio shanties only)

Audio shanties have an **Effects** button just below **Options**. It opens **Track effects**, where ye build a **ordered chain** o' plunder-time insert effects fer this shanty:

![Track effects dialog](../screenshots/i18n/pirate/Track_Effects_Dialog.png)

Ye can haul aboard **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion**, **Amp & cabinet** (modelin' amp an' speaker box: amp voice, cabinet, gain, bass/middle/treble, air, an' wet/dry), an' **Vibrato (Tremolo)**.

**Vibrato (Tremolo)** be that classic Fender “Vibrato” (aye, a tremolo): an **oscillatin' change in volume over time**. Set **Speed** fer how fast she pulses, an' **Intensity** (Depth) fer how deep she dips (from a wee shimmer to a full-on chop).

Spyglass on **Amp & cabinet**:

![Amp & cabinet modelin' (1)](../screenshots/i18n/pirate/Amp_And_Cabinet_Model_1.png)

![Amp & cabinet modelin' (2)](../screenshots/i18n/pirate/Amp_And_Cabinet_Model_2.png)

- **Add effect…** an' pick a type o' effect. Ye may stack more than one; the red **✕** in the header scuttles an effect.
- Drag **≡** to **reorder**. The **topmost** effect runs **first**.
- ms an' Hz stay meanin'ful after convertin' to the **project sample rate**. **Mono** an' **stereo** both be supported (mono runs dual-mono through the chain an' mixes back ta one channel).
- **OK** saves ta the project; **Cancel** restores the chain as when ye opened the dialog.

Effects be applied **when ye click Stop**, after the usual capture an' resample. The rig be stored in `project.json` under `audioEffectChain`.

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

### Hear yer racket live

Beside the **time glass**, **Hear yer racket live while recordin'** sends **live plunder** to the **project's speakin' tubes** while ye capture:

- **Audio shanties**: ye hear the mic or line as it happens (the hold be still written to treasure). Stacks atop **overdub** if other shanties be playin'.
- **MIDI shanties**: when **Render MIDI to sound for playin' back** be set and a **SoundFont** be loaded, notes come through the soft synth. With **outside MIDI ports**—trust yer keyboard's own listenin' line.

The flag be **kept in the chart** (`monitorWhileRecording` in `project.json`). Strike it if ye fear the mic pickin' up the speakers.

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

## Metronome (The Ship's Drummer)

Musician's Canvas carries a trusty built-in metronome, a steady drummer ye can call upon
durin' recordin' to keep yer crew in time. Click the metronome button on the button bar
(below the menu bar) to open the metronome settings parchment:

![Metronome settings](../screenshots/i18n/pirate/MetronomeSettings.png)

The parchment offers:

- **Enable metronome during recording**: When ye check this box, the metronome beats a tick
  while recordin' be underway. The tick sails out through the ship's speakers and be **not**
  captured into the recorded shanty.
- **Beats per minute**: A numeric input fer the tempo, in beats per minute (BPM). The range
  be 20–300 BPM, matey.

When the metronome be enabled, it starts tickin' once recordin' actually begins (after the
3-second countdown be done) and stops when recordin' ends. Yarrr!

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

![General settings](../screenshots/i18n/pirate/MusiciansCanvas_3_GeneralSettings.png)

#### General Tab

- **Theme**: Choose between Dark an' Light themes. Dark fer night watches, Light fer sunny sailin'!
- **Write recording debug log**: When enabled, Musician's Canvas writes detailed
  plunderin' diagnostic information to a file called `recording_debug.txt` in the project
  directory. This includes the capture backend, device name, sample rate, channel count,
  an' timin' data. This be disabled by default an' be primarily useful fer troubleshootin'
  plunderin' issues, savvy?

#### Display Tab

- **Numeric display LED color**: Choose the color o' the numeric LED time display that
  glimmers on the main window toolbar, matey! The active digit segments be drawn in the
  chosen color, an' the lazy inactive segments be drawn as a dim, ghostly version o' the
  same hue. Available colors be Light Red, Dark Red, Light Green, Dark Green, Light Blue,
  Dark Blue, Yellow, Orange, Light Cyan, an' Dark Cyan. The default be Light Green, bright
  as a sea serpent's eye.

![Display settin's](../screenshots/i18n/pirate/MusiciansCanvas_DisplaySettings.png)

#### Language Tab

- **Language**: Choose the display language fer the application, ye worldly pirate! The
  default be "System Default", which uses the operatin' system's language settin'. Available
  tongues be English, Deutsch (German), Español (Spanish), Français (French), Japanese,
  Português (Brazilian Portuguese), Chinese (Traditional), Chinese (Simplified), Dansk
  (Danish), Ελληνικά (Greek), Gaeilge (Irish), Norsk (Norwegian), Polski (Polish),
  Русский (Russian), Suomi (Finnish), Svenska (Swedish), Cymraeg (Welsh), an' Pirate.
  The interface updates immediately when ye change the language -- no need to restart the ship!

![Language settin's](../screenshots/i18n/pirate/MusiciansCanvas_LanguageSettings.png)

#### MIDI Tab

- **MIDI Output Device**: Select the built-in FluidSynth synthesizer or an external MIDI device. Use the **Refresh** button to rescan fer available MIDI devices on the horizon.
- **SoundFont**: Browse to a `.sf2` SoundFont file fer MIDI synthesis. On Linux, a system SoundFont may be auto-detected if the `fluid-soundfont-gm` package be installed. On Windows an' macOS, ye must configure the SoundFont path manually, ye landlubber.

![MIDI settings](../screenshots/i18n/pirate/MusiciansCanvas_4_MIDISettings.png)

#### Audio Tab

- **Audio Input Device**: Select the microphone or line-in device fer plunderin' sounds.
- **Audio Output Device**: Select the speaker or headphone device fer replayin' shanties.

![Audio settings](../screenshots/i18n/pirate/MusiciansCanvas_5_AudioSettings.png)

### Project Settings

Use **Project > Project Settings** (Ctrl+P) to override global defaults fer the current project only. This be useful fer projects that need a specific sample rate, SoundFont, or audio device. Project-specific settings be saved inside the `project.json` file, safe in the treasure chest.

![Project MIDI settings](../screenshots/i18n/pirate/MusiciansCanvas_6_ProjectMIDISettings.png)

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

![Project audio settings](../screenshots/i18n/pirate/MusiciansCanvas_7_ProjectAudioSettings.png)

#### Mix Effects tab

The **Mix Effects** tab is a scrollable list with the same controls as **Track effects** (**Add effect…**, drag **≡** to reorder, **✕** to remove). Processing order is **top to bottom** on the **combined** mix of all enabled tracks. These effects run during **whole-project playback** and when **mixing to a single WAV or FLAC file**; they are **not** baked into individual track files on disk. An empty list leaves the mixed signal unchanged aside from the mixer's own level handling.

#### Aux / Send Bus tab

Configure the **shared aux effect chain** (same effect types as track inserts). Each track’s **Aux** slider on the track row controls how much of that track’s signal is sent through this bus; the wet aux return is summed with the dry mix **before** **Mix Effects** are applied.

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
| Project Settings                    | Ctrl+P   | Configure project-specific settings (includes **Aux / Send Bus**) |
| Tempo map                           |          | Edit tempo changes (seconds vs BPM) for metronome and MIDI quantization |
| Add Demo Data to Selected Track     |          | Add sample MIDI notes fer demonstration purposes  |

### Edit Menu

| Menu Item | Shortcut | Description |
|-----------|----------|-------------|
| Undo      | Ctrl+Z   | Undo recent mixer/trim edits on tracks |
| Redo      | Ctrl+Shift+Z | Redo |

### Settings Menu

| Menu Item     | Shortcut | Description                              |
|---------------|----------|------------------------------------------|
| Configuration | Ctrl+,   | Open global application settings, matey  |

### Tools Menu

| Menu Item             | Shortcut | Description                                      |
|-----------------------|----------|--------------------------------------------------|
| Mix tracks to file    | Ctrl+M   | Export all enabled shanties to a single file      |
| Export stems to folder |          | One WAV stem per track (gain/pan/trim; no master Mix Effects) |
| Recording options     |          | **Punch-in** region for audio; **loop playback** for the whole project |
| Quantize MIDI         |          | Snap MIDI note starts to a grid (all MIDI tracks or armed track only) |
| Add drum track        | D        | Add a MIDI drum deck an' a `.mid` scroll (see below) |
| Virtual MIDI Keyboard |          | Launch the companion keyboard vessel              |

### Help Menu

| Menu Item | Shortcut | Description |
|----------|----------|-------------|
| Manual   | Alt+M    | Open the PDF manual in the tongue ye selected |
| About    |          | Show version an' app info |

## Keyboard Shortcuts

| Shortcut        | Action                                  |
|-----------------|-----------------------------------------|
| Ctrl+S          | Save project (stow yer treasure!)       |
| Ctrl+Z          | Undo (mixer/trim)              |
| Ctrl+Shift+Z    | Redo                           |
| Ctrl+O          | Open project                            |
| Ctrl+M          | Mix shanties to file                    |
| D               | Add drum deck (Tools menu)            |
| Ctrl+P          | Project Settings                        |
| Ctrl+,          | Settings / Configuration                |
| Alt+M           | Open PDF manual (Help)         |
| Ctrl+Q / Alt+F4 | Quit (abandon ship)                    |


### Add a drum deck

**Tools → Add drum track** (hotkey **D**) adds a **MIDI** deck on **channel 10** General MIDI (index 9). Default name **Drums** (numbered if taken).

A **`.mid`** be writ **straight** into the **project chest**: two bars o' 4/4 kick, snare, an' hat. Tempo:

- If **Enable metronome durin' recordin'** be set in the **metronome** dialog, that **BPM** be used.
- Else **BPM be guessed** from **enabled** **audio** tracks; if that fails, **120 BPM**.

**Charts ashore:** [Audient](https://audient.com/tutorial/how-to-program-realistic-midi-drum-tracks/), [MDrummer](https://www.meldaproduction.com/MDrummer), [Reddit](https://www.reddit.com/r/ableton/comments/1e51a7g/generating_midi_drum_patterns_based_on_audio_input/), [CS229 PDF](https://cs229.stanford.edu/proj2014/Louis%20Eugene,%20Guillaume%20Rostaing,%20Automated%20Music%20Track%20Generation.pdf).

## Virtual MIDI Keyboard

The Virtual MIDI Keyboard be a companion vessel (`virtual_midi_keyboard`) that provides an on-screen pianer keyboard fer sendin' MIDI notes across the musical seas. It can be launched from the **Tools > Virtual MIDI Keyboard** menu in the main application, or run independently as a lone privateer.

![Virtual MIDI Keyboard](../screenshots/i18n/pirate/VMIDIKeyboard1.png)

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
- **MIDI Channel**: A numeric input (1--16) to pick which o' the sixteen MIDI channels yer notes, program changes, an' control changes be dispatched upon, matey. The default be channel 1. The chosen channel takes effect the moment ye change it. Pickin' a SoundFont preset while a particular channel be selected applies that bank/program to that very channel, so ye can play different sounds on different channels -- a whole crew o' instruments at yer command!
- **Drum Plunder**: A checkbox perched just above the MIDI channel selector. When unticked (the default), the MIDI channel be whatever ye chose yerself. When ticked, the MIDI channel be automatically hoisted to channel 10 — the General MIDI standard drum channel — so anythin' ye play booms out as percussion, like a barrel o' cannons on the deck. Unticking the box restores the channel ye had selected afore the Drum Plunder was unleashed.
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

![Virtual MIDI Keyboard configuration](../screenshots/i18n/pirate/VMIDIKeyboard2.png)

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

## FAQ (Questions Sailed Often)

### How do I record a MIDI shanty — er, *track*?

1. Set yer **project directory** (ye can't stash booty without a hold).
2. Add or pick a track and open **Options** (or click the track-type icon, savvy?).
3. Set the type to **MIDI** and batten the dialog.
4. Under **Settings > Configuration > MIDI**, pick yer **MIDI input** (hardware port or a virtual cable across the bilge) and a **SoundFont** (`.sf2`) fer playin' back later.
5. **Arm** that track (only one may be armed at a time — no double powder charges).
6. Click **Record**, endure the countdown, play yer controller, then **Stop**.

Notes show in the piano roll. **File > Save Project** writes a `.mid` and `project.json` into the project folder.

### Why be me MIDI track silent on playback?

Playback uses **FluidSynth** with the **SoundFont** from settings. Eye **Settings > Configuration > MIDI** (or **Project > Project Settings** if ye overrode per voyage): a proper `.swab` — beg pardon, `.sf2` — path be required. On Linux a system SoundFont may be found without map; on Windows and macOS ye often pick the file by hand.

### How be Virtual MIDI Keyboard tied to Musician's Canvas?

They be **two separate vessels**. Launch Virtual MIDI Keyboard from **Tools > Virtual MIDI Keyboard** (or sail solo). To pipe the on-screen keys **into** Musician's Canvas while recordin' MIDI, the OS must route the keyboard's **MIDI out** to an **in** that Musician's Canvas listens on — often a virtual MIDI cable or matched ports in both apps. They don't board each other automatic-like.

### What differ Configuration from Project Settings?

**Settings > Configuration** sets **global defaults** (theme, tongue, MIDI/audio gear, SoundFont, and such). **Project > Project Settings** overrides fer **this project only** and lives in `project.json`. If a field stays at project default, the global Configuration value holds the wheel.

### Why won't drag-and-drop take me audio files?

Files be accepted only with a **project directory** set and when Musician's Canvas **ain't** playin' nor recordin'. **`.wav`** and **`.flac`** be welcome; other extensions walk the plank and get listed in a dialog. Each file becomes a new **audio** track named from the file's stem.

### Where be me recordings stowed?

Audio goes in the **project directory** as **`<track_name>.flac`** (characters like `/ \ : * ? " < > |` be replaced with underscores). The voyage log be **`project.json`** in the same folder. MIDI tracks save as **`<track_name>.mid`** when ye save the project (notes also in `project.json`).

### What filename when MIDI be rendered fer mixin'?

On **mix** or **playback**, MIDI be rendered to a temporary WAV below decks first. If the project path be known, Musician's Canvas also writes a **cache FLAC** in the project folder: **`<sanitized_track_name>.flac`** (same rules as other track booty). The name follows the **track name**, not a secret hold number.

### Can I record two tracks at once?

Nay. Only **one** track can be **armed** at a time; it gets the next recording. Build yer chart **one layer at a time** (**overdub** plays existin' tracks while ye lay down a new one).

### Does the metronome get pressed into the recording?

Nay. When enabled, the metronome ticks through **system audio** fer yer ears only. It **ain't mixed** into the captured file.

### Why does Musician's Canvas demand ASIO on Windows?

On Windows the main app expects an **ASIO** driver fer steady low-latency sound. Install **ASIO4ALL** or a maker's driver if ye hit errors.

### Where be Virtual MIDI Keyboard on macOS?

In the **`.app` bundle**, the Virtual MIDI Keyboard executable be **copied inside** `Musician's Canvas.app` (**Contents/MacOS/**) so one app folder can be handed ashore. Still launch it from **Tools > Virtual MIDI Keyboard**.

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
