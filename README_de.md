# Musician's Canvas

Dies ist eine Mehrspuraufnahme-Anwendung für Desktop-PCs. Sie wurde in C++ geschrieben und verwendet Qt als
GUI-Bibliothek. Diese Anwendung ist für Linux, Windows und Mac OS konzipiert. Benutzerfreundlichkeit ist ein
wesentliches Designziel. Dieses Projekt begann als Experiment mit Cursor & Claude (KI-Programmierwerkzeuge), um zu sehen, was
es in der Lage wäre, als Anwendung dieser Art zu erstellen, mit fortgesetzter KI-Entwicklung sowie
traditioneller Softwareentwicklung.

Vor Jahren habe ich während des Studiums ein Mehrspuraufnahme-Programm erstellt (mit demselben Namen); dies ist ein
Versuch, etwas Besseres zu schaffen.

## Funktionen

- **Mehrspur-Arrangement**: Mehrere Spuren hinzufügen und entfernen, um Songs zu komponieren
- **Benannte Spuren**: Jede Spur hat einen bearbeitbaren Namen, der als Dateiname für aufgenommenes Audio verwendet wird
- **Spurtypen**: Jede Spur kann für Audioaufnahme (von Mikrofon/Line-In) oder MIDI-Aufnahme (von Keyboard/Controller) konfiguriert werden; klicken Sie auf das Spurtyp-Symbol, um die Eingangsquelle schnell zu ändern
- **Spur-Aufnahmebereitschaft**: Aktivieren Sie "Arm" bei einer Spur, um sie als Aufnahmeziel auszuwählen; nur eine Spur kann gleichzeitig aufnahmebereit sein. Eine einzelne nicht aufgenommene Spur wird automatisch aufnahmebereit gesetzt
- **Aufnahme-Countdown**: Ein 3-Sekunden visueller Countdown vor Beginn der Aufnahme, der dem Interpreten Zeit zur Vorbereitung gibt
- **Overdub-Aufnahme**: Beim Aufnehmen einer neuen Spur, während vorhandene Spuren aktiviert sind, werden die vorhandenen Spuren gemischt und in Echtzeit wiedergegeben, damit Sie sie beim Aufnehmen hören können. Wiedergabe und Aufnahme sind synchronisiert, um alle Spuren ausgerichtet zu halten
- **Visuelle Rückmeldung**: Audio-Wellenformanzeige für Audiospuren (mit Live-Pegelanzeige während der Aufnahme), MIDI-Pianorollenansicht für MIDI-Spuren
- **Integrierter MIDI-Synthesizer**: Rendert MIDI-Spuren zu Audio mittels FluidSynth mit konfigurierbarem SoundFont
- **Zu einzelner Audiodatei mischen**: Alle aktivierten Spuren in eine einzelne gemischte Datei (WAV, FLAC, MP3, Ogg Vorbis oder AIFF) exportieren — über **Tools → Mix tracks to file** — unter Verwendung der [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-Bibliothek und **libsndfile**. Ist das Ziel **MP3** oder **Ogg Vorbis**, zeigt der Dialog einen Bereich **Encoding** für Bitratenmodus und Qualität (u. a. MP3 CBR/ABR/VBR sowie Ogg nach Qualität oder nominaler Bitrate); bei **WAV**, **FLAC** und **AIFF** gibt es dort keine Bitratenauswahl.
- **Projekt speichern / öffnen**: Das vollständige Projekt (Spuren, Namen, Typen, MIDI-Noten, Audiodatei-Referenzen) in eine JSON-Datei serialisieren und daraus wiederherstellen, mit Erkennung nicht gespeicherter Änderungen beim Beenden
- **Projektspezifische Einstellungen**: Globale MIDI- und Audio-Standardwerte pro Projekt überschreiben (Abtastrate, SoundFont, MIDI-Gerät)
- **PortAudio-Aufnahme (optionaler Build)**: Wenn das Projekt mit PortAudio gebaut wird (`HAVE_PORTAUDIO`), kann die Aufnahme einen nativen PortAudio-Eingabepfad verwenden (ähnlich wie bei Audacity) anstelle von Qt Multimedia. **Project → Project Settings → Audio** ermöglicht die Wahl zwischen **PortAudio** oder **Qt Multimedia** und die Auswahl eines PortAudio-Eingabegeräts. Wenn PortAudio nicht installiert ist, wird der Build dennoch erfolgreich durchgeführt und die Aufnahme verwendet nur Qt Multimedia.
- **Hochwertige Abtastratenkonvertierung**: Nimmt mit der nativen Rate des Audiogeräts auf und konvertiert zur Projektrate mittels gefensterter Sinc-Interpolation (Kaiser-Fenster, ~96 dB Sperrdämpfung), derselbe Algorithmus-Familie, die von Audacity / libsoxr verwendet wird. Dies ermöglicht die Aufnahme mit jeder Projekt-Abtastrate (8000 Hz bis 192000 Hz) unabhängig von der nativen Geräterate, ohne Tonhöhen- oder Daueränderung.
- **Automatische Mono/Stereo-Verarbeitung**: Erkennt physisch mono Geräte, die als Stereo beworben werden (häufig bei USB-Webcam-Mikrofonen unter PipeWire), und konvertiert zwischen Mono und Stereo nach Bedarf (Verdopplung oder Mittelung), entsprechend dem Kanal-Routing-Ansatz von Audacity
- **Aufnahme-Insert-Effekte (Audiospuren)**: Über die Schaltfläche **Effects** (unter **Options** bei jeder Audiospur) öffnen Sie den Dialog **Track effects**. Fügen Sie **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion** und **Amp & cabinet** (Amp- und Boxen-Modellierung) hinzu und konfigurieren Sie sie, ziehen Sie den **≡**-Griff, um die Kette neu zu ordnen (oben läuft zuerst), und speichern Sie mit dem Projekt. Die Effekte werden auf die aufgenommene Aufnahme angewendet, wenn Sie die Aufnahme stoppen; die Parameter sind in realen Einheiten (ms, Hz), sodass sich das Verhalten bei Mono oder Stereo und typischen Projekt-Abtastraten (8 kHz–192 kHz) nach der Normalisierung der Aufnahme konsistent bleibt. Siehe das [Benutzerhandbuch](docs/MusiciansCanvas_User_Manual_de.md). Übersetzer können die Effektzeichenketten per `scripts/effect_i18n.tsv` (von `scripts/build_effect_tsv.py` erzeugt) und `scripts/fill_effect_i18n.py` nach `lupdate` stapelweise aktualisieren.
- **Mix-Effekte (Summenbus)**: Unter **Project → Project Settings → Mix Effects** legen Sie eine Effektkette für den **gesamten Mix** fest — dieselben Typen und dieselbe Reihenfolge wie bei Spureffekten. Sie wird bei **Wiedergabe aller Spuren** und beim **Mischen in eine Datei** (unterstützte Exportformate) angewendet. Die Einstellungen stehen pro Projekt in `project.json` unter `mixEffectChain`.
- **Anti-Clipping am Effektausgang**: Die integrierte DSP-Kette weicht Pegel vor der 16-Bit-PCM-Wandlung weich aus, um hartes digitales Clipping zu vermeiden. **EffectWidget** stellt `guardFloatSampleForInt16Pcm()` und `softLimitFloatSampleForInt16Pcm()` für neue Echtzeitverarbeitung bereit.
- **Mithören während der Aufnahme**: Das Kontrollkästchen **Ton während der Aufnahme mithören** (rechts neben der Zahlen-Uhr) leitet den Live-Eingang zum **im Projekt gewählten Audioausgang** während der Aufnahme. Bei **Audiospuren** ist das der gleiche Signalpfad wie die Aufnahme (zusätzlich zur Overdub-Wiedergabe). Bei **MIDI-Spuren** klingt das Spiel über FluidSynth, wenn **MIDI für Wiedergabe in Audio rendern** aktiv ist und ein SoundFont verfügbar ist. Die Einstellung wird im **Projekt** gespeichert (`monitorWhileRecording` in `project.json`). Deaktivieren Sie sie, um z. B. akustische Rückkopplung ins Mikrofon zu vermeiden.
- **Audio mit niedriger Latenz**: Unter Windows sorgt die ASIO-Treibererkennung für Audio mit niedriger Latenz; unter Linux wird die Prozessplanungspriorität für geringeren Jitter mit PipeWire / PulseAudio / ALSA erhöht
- **Schlagzeugspur hinzufügen**: **Tools → Schlagzeugspur hinzufügen** (Tastenkürzel **D**) legt eine MIDI-Spur auf dem General-MIDI-Kanal 10 (Schlagzeug) an und schreibt eine kurze Zwei-Takt-`.mid`-Datei (Kick, Snare, Hi-Hat) in den Projektordner. Ist im **Metronom** **Metronom während Aufnahme aktiv** eingeschaltet, wird das BPM von dort verwendet; sonst wird das Tempo aus den aktivierten Audiospuren geschätzt oder 120 BPM angenommen. Siehe das [Benutzerhandbuch](docs/MusiciansCanvas_User_Manual_de.md).
- **Virtual MIDI Keyboard**: Eine Begleitanwendung zum Senden von MIDI-Noten über eine Software-Klaviertastatur, mit integriertem FluidSynth-Synthesizer, einstellbarer Gesamtlautstärke, Computer-Tastatur-zu-Klavier-Zuordnung, Instrument-/Programmauswahl, Chorus-/Effektsteuerung und Oktavverschiebung
- **Konfiguration**: Auswahl von Audio-Eingabegerät, MIDI-Gerät und SoundFont-Datei (globale Standards und projektspezifische Überschreibungen)
- **Dunkles / helles Design**: Konfigurierbar über Settings → Configuration
- **Lokalisierung**: Beide Anwendungen sind in 18 Sprachen lokalisiert: Englisch, Deutsch, Spanisch, Französisch, Japanisch, Portugiesisch (Brasilien), Traditionelles Chinesisch, Vereinfachtes Chinesisch, Russisch, Schwedisch, Finnisch, Dänisch, Norwegisch, Polnisch, Griechisch, Irisch, Walisisch und Piratensprache. Die Sprache wird standardmäßig aus der Betriebssystem-Einstellung übernommen und kann in **Settings → Configuration → Language** geändert werden. Es wird das Standard-i18n-System von Qt (`QTranslator` / `.ts` / `.qm`) verwendet, und die Oberfläche wird sofort aktualisiert, wenn die Sprache geändert wird.
- **Benutzerhandbuch**: HTML- und PDF-Dokumentation, generiert aus Markdown-Quellen, verfügbar in allen unterstützten Sprachen (siehe [docs/](docs/))

## Bildschirmfotos

<p align="center">
	<a href="screenshots/i18n/de/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/i18n/de/MusiciansCanvas_1_MainWin.png" alt="Hauptfenster" width="800"></a>
	<a href="screenshots/i18n/de/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/i18n/de/MusiciansCanvas_2_TrackConfig.png" alt="Spurkonfiguration" width="800"></a>
	<a href="screenshots/i18n/de/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/i18n/de/MusiciansCanvas_3_GeneralSettings.png" alt="Allgemeine Einstellungen" width="800"></a>
	<a href="screenshots/i18n/de/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/i18n/de/MusiciansCanvas_4_MIDISettings.png" alt="Standard-/allgemeine MIDI-Einstellungen" width="800"></a>
	<a href="screenshots/i18n/de/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/i18n/de/MusiciansCanvas_5_AudioSettings.png" alt="Standard-/allgemeine Audio-Einstellungen" width="800"></a>
	<a href="screenshots/i18n/de/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/i18n/de/MusiciansCanvas_DisplaySettings.png" alt="Anzeige-Einstellungen" width="800"></a>
	<a href="screenshots/i18n/de/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/i18n/de/MusiciansCanvas_LanguageSettings.png" alt="Spracheinstellungen" width="800"></a>
	<a href="screenshots/i18n/de/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/i18n/de/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Projektspezifische MIDI-Einstellungen" width="800"></a>
	<a href="screenshots/i18n/de/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/i18n/de/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Projektspezifische Audio-Einstellungen" width="800"></a>
	<a href="screenshots/i18n/de/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/i18n/de/Track_Effects_Dialog.png" alt="Dialog Spureffekte" width="800"></a>
	<a href="screenshots/i18n/de/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/i18n/de/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/i18n/de/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/i18n/de/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/i18n/de/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/i18n/de/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/i18n/de/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/i18n/de/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Abhängigkeiten

### Compiler

Ein **C++17**-fähiger Compiler wird benötigt (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (auf allen Plattformen erforderlich)

Die Mischfunktion und Exportencoder (FLAC, WAV, MP3, Ogg Vorbis, AIFF) hängen von der
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-Bibliothek und **libsndfile** ab.
Klonen Sie sie als Geschwisterverzeichnis dieses Repositorys vor dem Erstellen:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

Der CMake-Build erwartet sie unter `../audio_mixer_cpp` relativ zum Stammverzeichnis dieses Projekts.

---

### libsndfile (erforderlich)

MP3-, Ogg-Vorbis- und AIFF-Unterstützung erfolgt über **libsndfile** (neben den bestehenden FLAC- und WAV-Pfad in audio_mixer_cpp). Installieren Sie das Entwicklungspaket (`libsndfile1-dev` unter Debian/Ubuntu, `libsndfile-devel` unter Fedora usw.).

---

### Qt6 Multimedia (optional, aber dringend empfohlen)

`qt6-multimedia` ist optional. Ohne es wird die Anwendung dennoch gebaut und ausgeführt, aber
**Audioaufnahme und -wiedergabe sind deaktiviert** — nur MIDI-Bearbeitung und Projektverwaltung
funktionieren. Installieren Sie es zusammen mit den Qt6-Kernbibliotheken mit den untenstehenden Plattformbefehlen.

---

### PortAudio (optional)

Wenn die PortAudio-Bibliothek und die Header installiert sind, aktiviert CMake **`HAVE_PORTAUDIO`** und
bindet es in **musicians_canvas** ein. Die Aufnahme verwendet dann standardmäßig den PortAudio-Pfad, es sei denn,
das Projekt ist auf **Qt Multimedia** in **Project → Project Settings → Audio** eingestellt. Wenn PortAudio
nicht gefunden wird, wird der Build ohne es fortgesetzt (Wrapper wird als Stubs kompiliert).

---

### SoundFont für MIDI-Synthese

Eine SoundFont-Datei (`.sf2`) ist erforderlich, damit MIDI-Spuren Audio erzeugen. Ohne eine solche
werden MIDI-Spuren als Stille gerendert. Unter **Linux** erkennt der integrierte FluidSynth-Synthesizer
automatisch einen SoundFont, wenn einer an einem Standard-Systempfad installiert ist
(siehe die Paketnamen unten). Unter **macOS und Windows** gibt es keinen Standard-Systempfad,
daher müssen Sie den SoundFont manuell in
**Settings → Configuration → MIDI** konfigurieren.

---

### Linux — Ubuntu / Debian

```bash
sudo apt install build-essential cmake \
  qt6-base-dev qt6-multimedia-dev \
  qt6-l10n-tools \
  libfluidsynth-dev librtmidi-dev libflac-dev libsndfile1-dev \
  libportaudio2 portaudio19-dev \
  libpipewire-0.3-dev \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` installiert `FluidR3_GM.sf2` nach `/usr/share/sounds/sf2/` und wird
> beim Start automatisch erkannt. `timgm6mb-soundfont` ist eine kleinere Alternative.
>
> `libpipewire-0.3-dev` wird auf PipeWire-basierten Systemen benötigt, damit die Virtual MIDI
> Keyboard `pw_init()` aufrufen kann, bevor FluidSynth seine Audio-Ressourcen erstellt.
> Der Build wird ohne es fortgesetzt; das `HAVE_PIPEWIRE`-Flag wird einfach nicht definiert.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel libsndfile-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` installiert `FluidR3_GM.sf2` nach `/usr/share/soundfonts/` und wird
> beim Start automatisch erkannt.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac libsndfile portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` installiert `FluidR3_GM.sf2` nach `/usr/share/soundfonts/` und wird
> beim Start automatisch erkannt. `pipewire` ist auf modernen Arch-Systemen typischerweise
> bereits installiert; die Entwicklungs-Header sind im Hauptpaket enthalten.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac libsndfile portaudio
```

> PipeWire ist ein reines Linux-Subsystem und wird auf macOS **nicht** benötigt. FluidSynth
> wird automatisch CoreAudio über das Qt Multimedia-Backend verwenden.
>
> Laden Sie einen General-MIDI-SoundFont herunter (z. B.
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) oder
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> und konfigurieren Sie seinen Pfad in **Settings → Configuration → MIDI**.

---

### Windows

**musicians_canvas** erfordert einen **ASIO-Audio-Treiber** für Aufnahme und Wiedergabe mit niedriger Latenz.
Die Anwendung startet nicht, wenn kein ASIO-Treiber erkannt wird.
**virtual_midi_keyboard** erfordert kein ASIO und verwendet direkt das WASAPI-Backend von Qt Multimedia.

**Installation eines ASIO-Treibers für musicians_canvas (eine Option wählen):**

| Treiber | Hinweise |
|---------|----------|
| [ASIO4ALL](https://asio4all.org/) | Kostenlos, funktioniert mit den meisten integrierten und USB-Audio-Geräten |
| Herstellertreiber | Beste Latenz für dedizierte Audio-Interfaces (z. B. Focusrite, PreSonus, RME) |

**Toolchain — MSYS2 (empfohlen für MinGW-Builds):**

Laden Sie MSYS2 von <https://www.msys2.org> herunter und installieren Sie es, dann fügen Sie
`C:\msys64\mingw64\bin` zu Ihrem System-`PATH` hinzu. Installieren Sie die erforderlichen Pakete:

```bash
pacman -S mingw-w64-x86_64-qt6-base
pacman -S mingw-w64-x86_64-qt6-multimedia
pacman -S mingw-w64-x86_64-fluidsynth
pacman -S mingw-w64-ucrt-x86_64-rtmidi
pacman -S mingw-w64-x86_64-flac
pacman -S mingw-w64-x86_64-libsndfile
pacman -S mingw-w64-x86_64-portaudio
pacman -S mingw-w64-x86_64-soundfont-fluid
```

> `mingw-w64-x86_64-soundfont-fluid` installiert `FluidR3_GM.sf2` nach
> `C:\msys64\mingw64\share\soundfonts\` (anpassen, wenn MSYS2 an einem anderen Ort installiert ist).
> Da Windows keinen Standard-SoundFont-Suchpfad hat, müssen Sie diesen Pfad
> nach dem ersten Start manuell in **Settings → Configuration → MIDI** konfigurieren.
>
> PipeWire ist ein reines Linux-Subsystem; unter Windows wird kein PipeWire-Paket benötigt.

Paketreferenzseiten:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Toolchain — Visual Studio 2022:**

Installieren Sie Qt 6 über den [Qt Online Installer](https://www.qt.io/download) und beschaffen Sie sich
FluidSynth-, RtMidi- und libFLAC-Binärdateien (oder erstellen Sie sie aus dem Quellcode).
Der CMake-Build wird sie über `find_library` / `find_path` finden, solange die
entsprechenden Verzeichnisse in `CMAKE_PREFIX_PATH` enthalten sind.

Ein SoundFont muss separat heruntergeladen werden (z. B.
[GeneralUser GS](https://schristiancollins.com/generaluser.php) oder
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)) und sein
Pfad muss nach dem ersten Start in **Settings → Configuration → MIDI** konfiguriert werden.

> **Hinweis:** `advapi32` (Windows-Registrierung) und `winmm` (Windows Multimedia) werden
> von CMake automatisch gelinkt; dafür ist keine zusätzliche Installation erforderlich.

---

## Erstellen

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

Wenn erforderliche DLLs zur Laufzeit nicht gefunden werden, führen Sie das Hilfsskript aus dem Build-Ausgabeverzeichnis aus:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Dokumentation generieren

Das Benutzerhandbuch kann als HTML und PDF aus der Markdown-Quelle generiert werden:

```bash
cd docs
./generate_docs.sh          # HTML und PDF generieren
./generate_docs.sh html     # Nur HTML generieren
./generate_docs.sh pdf      # Nur PDF generieren
```

**Voraussetzungen:**

- **Python (bevorzugt):** `pip3 install markdown weasyprint`
- **Fallback:** `pandoc` und `wkhtmltopdf` (über den System-Paketmanager)

Das generierte HTML wird nach `docs/html/` geschrieben und das PDF nach `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Ausführen

```bash
./build/musicians_canvas
# oder
./build/virtual_midi_keyboard   # Begleitanwendung: Virtual MIDI Keyboard
```

---

## Verwendung

### musicians_canvas

1. **Projektverzeichnis festlegen**: Geben Sie einen Ordner ein oder navigieren Sie zu einem Ordner, in dem Aufnahmen und die Projektdatei gespeichert werden
2. **Spuren hinzufügen**: Klicken Sie auf "+ Add Track"; benennen Sie jede Spur im Textfeld neben "Options"
3. **Spurtyp konfigurieren**: Klicken Sie auf "Options" einer Spur (oder klicken Sie auf das Spurtyp-Symbol zwischen "Options" und dem Namensfeld), um Audio oder MIDI auszuwählen und die Eingangsquelle festzulegen
4. **Spur entfernen**: Klicken Sie auf die "×"-Schaltfläche rechts in der Spurzeile
5. **Globale Einstellungen**: Verwenden Sie **Settings → Configuration**, um globale Standards festzulegen:
   - Registerkarte *General*: Design (dunkel/hell)
   - Registerkarte *MIDI*: Standard-MIDI-Ausgabegerät (integrierter FluidSynth-Synthesizer oder ein externes MIDI-Gerät) und Standard-SoundFont (`.sf2`) für die Synthese
   - Registerkarte *Audio*: Standard-Audio-Eingabe-/Ausgabegerät für Aufnahme und Wiedergabe
6. **Projekteinstellungen**: Verwenden Sie **Project → Project Settings** (Ctrl+P), um MIDI- und Audio-Einstellungen nur für das aktuelle Projekt zu überschreiben (z. B. eine andere Abtastrate oder ein anderer SoundFont pro Song)
7. **Aufnehmen**: Aktivieren Sie "Arm" auf der Zielspur und klicken Sie dann auf die Aufnahmetaste (roter Kreis). Nur eine Spur kann gleichzeitig aufnahmebereit sein
8. **Wiedergabe**: Klicken Sie auf die Wiedergabetaste, um alle aktivierten Spuren zu mischen und wiederzugeben
9. **Zu Datei mischen**: Verwenden Sie **Tools → Mix tracks to file** (Ctrl+M), um nach WAV, FLAC, MP3, Ogg Vorbis oder AIFF zu exportieren. Bei **MP3** und **Ogg** legen Sie Bitrate oder Qualität im Bereich **Encoding** des Dialogs fest, bevor gemischt wird.
10. **Speichern / Öffnen**: Verwenden Sie **File → Save Project** (Ctrl+S) und **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Konfiguration öffnen**: Verwenden Sie die **Configuration**-Schaltfläche oder das Menü (Ctrl+,), um den Einstellungsdialog zu öffnen
2. **MIDI-Registerkarte**:
   - Wählen Sie ein MIDI-Ausgabegerät (einen externen Hardware-/Software-Synthesizer) oder lassen Sie das Feld leer, um den integrierten FluidSynth-Synthesizer zu verwenden
   - Wählen Sie ein MIDI-Eingabegerät, um eingehende MIDI-Noten an die Tastaturanzeige weiterzuleiten
   - Passen Sie die **Synthesizer Volume (Master Gain)** an, um den Ausgangspegel des integrierten Synthesizers zu steuern (10 %–200 %)
3. **Audio-Registerkarte**: Wählen Sie das Audioausgabegerät, das vom integrierten Synthesizer verwendet wird
4. **SoundFont**: Wählen Sie eine `.sf2`-SoundFont-Datei für den integrierten Synthesizer (wird unter Linux automatisch erkannt, wenn ein System-SoundFont installiert ist)
5. **Noten spielen**: Klicken Sie auf Tasten auf der Bildschirm-Klaviertastatur, verwenden Sie einen angeschlossenen MIDI-Controller oder nutzen Sie die Computertastatur:
   - Untere Oktave: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = Halbtöne
   - Obere Oktave: Q/W/E/R/T/Y/U/I/O/P = C bis E, 2/3/5/6/7/9/0 = Halbtöne
6. **Werkzeugleistensteuerung**: MIDI-Lautstärke (0–127), Oktave (-3 bis +5), Chorus-/Effektpegel anpassen und Instrumente auswählen

---

## Tastenkombinationen

**musicians_canvas:**

| Tastenkombination | Aktion |
|-------------------|--------|
| Ctrl+S | Projekt speichern |
| Ctrl+O | Projekt öffnen |
| Ctrl+M | Spuren zu Datei mischen |
| Ctrl+P | Projekteinstellungen |
| Ctrl+, | Einstellungen / Konfiguration |
| Ctrl+Q / Alt+F4 | Beenden |

**virtual_midi_keyboard:**

| Tastenkombination | Aktion |
|-------------------|--------|
| Ctrl+, | Konfigurationsdialog |
| Ctrl+U | Hilfe / Nutzungshinweise |
| Ctrl+Q | Schließen |

---

## Lizenz

Dieses Projekt wird ohne Gewähr für Bildungs- und persönliche Nutzung bereitgestellt.
