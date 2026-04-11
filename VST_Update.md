# VST support investigation for musicians_canvas

This document summarizes what it would take to enable support for VST instruments and plugins in **musicians_canvas**.

## What VST plugins are (brief)

A **VST** (Virtual Studio Technology) plugin is a software audio module that integrates into a **Digital Audio Workstation (DAW)** to emulate hardware instruments or sound effects. Developed by Steinberg, these plugins allow producers to add virtual synthesizers, samplers, and processors like EQs and compressors to a DAW.

Key aspects:

- **VST Instruments (VSTi)**: Generate sound — synthesizers, samplers, drum machines.
- **VST Effects**: Process incoming audio — reverbs, delays, compressors, EQs, etc.
- **VST MIDI Effects**: Process MIDI (e.g. arpeggiators, transposition).
- **Workflow**: Plugins run **inside host software** (Ableton, FL Studio, Logic, etc.), not as standalone programs in the typical DAW workflow (though some vendors offer standalone wrappers).
- **Compatibility**: **VST3** is the modern standard; it is generally more CPU-efficient than legacy VST2.

Useful references:

- [What is a VST plugin? — Native Instruments Blog](https://blog.native-instruments.com/vst-plugin/)
- [Virtual Studio Technology — Wikipedia](https://en.wikipedia.org/wiki/Virtual_Studio_Technology)
- [VST 2.0 spec (PDF, historical)](https://jvstwrapper.sourceforge.net/vst20spec.pdf)
- [Plugin formats explained — Plugin Boutique](https://help.pluginboutique.com/hc/en-us/articles/6233336161428-Plugin-Formats-Explained-VST-AU-AAX-etc)
- [What Is a VST? — Careers in Music](https://www.careersinmusic.com/what-is-a-vst/)
- [YouTube: overview of VST plugins](https://www.youtube.com/watch?v=k2YHVT-zzB4)

---

## What musicians_canvas is today

- **Stack**: Qt6/C++17 desktop app targeting **Linux, Windows, and macOS**.
- **Effects**: Custom insert chains stored as JSON on each audio track (`TrackData::audioEffectChain`), applied in DSP code (`applyAudioEffectChain` in `effectdsp`) to **16-bit PCM after capture** — not a live VST-style processing graph.

```cpp
// From trackdata.h — effect chain is a fixed set of built-in types, not VST
// Each element: { "type": "reverb"|"chorus"|"flanger"|"overdrive_distortion"|"amp_cabinet", "params": { ... } }
QJsonArray audioEffectChain;
```

- **Playback / mix**: Tracks are mixed (via `audio_mixer_cpp` / `AudioUtils::mixTracksToFile`) to a **temporary file**, then **`QMediaPlayer`** plays that file — again, not a continuous real-time processing callback.
- **MIDI**: **FluidSynth + SoundFont** only (`MidiSynth`); there is **no pluggable instrument** path.

There are **no references to VST, CLAP, or JUCE** in the codebase; built-in effects and synthesis only.

---

## What “VST support” actually requires

VST (typically **VST3** for new work) defines a **host–plugin contract**: the application must act as a **VST host**. The host must:

- Load native plugin binaries (`.vst3` bundles / DLLs per platform).
- Negotiate **sample rate**, **block size**, and bus layouts.
- Call the plugin’s **process** path on a suitable **real-time** audio thread.
- Route **audio** and **MIDI** as required (instruments need MIDI in; effects need audio in/out).
- Expose **parameters** and optionally automation.
- Show or **embed** the plugin **editor** (platform-specific window parenting).
- **Serialize state** for project save/load (presets, vendor chunks).

So “adding VST support” means **implementing or integrating a host layer**, not merely linking a single audio library.

---

## Gap vs current architecture (main engineering effort)

| Area | Today | Needed for VST |
|------|--------|----------------|
| **Audio graph** | Mix/export + file playback | Real-time **callback-driven** engine (stable buffers, timing) |
| **Insert effects** | Offline chain on recorded buffer | Host **audio processing** in real time, or a correct **offline** pass with latency/tail handling |
| **MIDI instruments** | FluidSynth render-to-WAV | MIDI → **VSTi** → audio in the same engine as monitoring/overdub |
| **UI** | Qt-only custom panels | **Embed** VST editor (HWND / NSView / X11 / Wayland considerations per OS) |
| **State** | JSON for built-in params | Plugin **state blobs** + preset UX |

The largest cost is usually **moving from “render file → play file” to “audio thread + plugin graph”** for behaviors users expect: low-latency monitoring, VSTi on MIDI tracks, and reorderable plugin chains during recording.

---

## Practical implementation approaches

1. **Integrate a mature host stack (e.g. JUCE as a library)**  
   - **Pros**: VST3 hosting, editor embedding, many edge cases already handled.  
   - **Cons**: Large dependency, build complexity; careful integration with Qt (often a dedicated audio module/thread alongside the Qt GUI).

2. **Use the Steinberg VST3 SDK directly**  
   - **Pros**: No third-party framework; full control.  
   - **Cons**: High effort — lifecycle, threading, scanning, reference counting, bus layouts, sidechains, etc. New development should target **VST3** and current Steinberg documentation.

3. **External process / bridge**  
   - Run plugins in a **separate helper process** for crash isolation.  
   - **Pros**: A crash in a plugin does not take down the main app.  
   - **Cons**: IPC, latency, and complexity; the helper still needs a host implementation.

4. **Other formats**  
   - **CLAP**, **AU** (macOS), **AAX** (Pro Tools) are separate. Supporting “all plugins” often means multiple backends. A reasonable first scope for this project is **VST3 only** on all three desktop platforms where vendors ship builds.

---

## Non-code considerations

- **Distribution**: Plugin scan paths, crash blacklists, and user expectations around rescanning.
- **macOS**: Code signing / notarization and security attributes can affect loading third-party plugin bundles.
- **Legal / licensing**: Follow Steinberg’s **VST3 SDK** terms; the ecosystem has moved toward VST3 and updated SDK licensing (see Wikipedia and Steinberg’s developer portal for current terms).

---

## Rough effort summary

- **VST3 effects on recorded audio only (offline bounce through a plugin)**: Smaller than full DAW hosting, but still requires host loading, state, buffer processing, and tail/latency handling — on the order of **weeks** for a solid approach if reusing something like JUCE.
- **Full parity** (MIDI → VSTi, live monitoring, VST chains while recording, project save with plugin state): **Major feature** — expect a **multi-month** refactor of the audio engine, not a thin layer on top of `effectdsp.cpp`.

---

## Suggested next scoping questions

The minimum engine design depends on which milestone matters first:

1. **(a)** VST3 **effects** only, applied after recording (closest to today’s post-process model, but still needs a real host).
2. **(b)** **Real-time** VST inserts on the **capture** path (monitoring + printed audio).
3. **(c)** **VST instruments** for MIDI tracks (largest change vs FluidSynth-only rendering).

Pick one primary goal; each implies different threading, buffering, and UI work.
