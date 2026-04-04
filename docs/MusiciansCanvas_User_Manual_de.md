---
title: "Musician's Canvas Benutzerhandbuch"
subtitle: "Mehrspurige Musikaufnahme-Anwendung"
version: "0.1.0"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "de"
---

# Musician's Canvas Benutzerhandbuch

## Einleitung

Musician's Canvas ist eine Mehrspuranwendung zur Musikaufnahme für Desktop-PCs. Sie unterstützt
Audioaufnahmen von Mikrofonen und Line-In-Geräten, MIDI-Aufnahmen von Keyboards und
Controllern sowie das Abmischen aller Spuren in eine einzelne WAV- oder FLAC-Datei. Eine
Begleitanwendung, Virtual MIDI Keyboard, stellt eine Software-Klaviertastatur zum Senden von
MIDI-Noten bereit.

Musician's Canvas ist auf einfache Bedienung ausgelegt und bietet gleichzeitig Funktionen, die
häufig in digitalen Audio-Workstations (DAWs) zu finden sind:

- Mehrspur-Audio- und MIDI-Aufnahme
- Overdub-Aufnahme mit synchronisierter Wiedergabe vorhandener Spuren
- Integrierter FluidSynth MIDI-Synthesizer mit SoundFont-Unterstützung
- Hochwertige Abtastratenkonvertierung für Aufnahmen mit beliebiger Projekt-Abtastrate
- Automatische Mono/Stereo-Geräteerkennung
- Projektbasierte Einstellungen mit projektspezifischen Überschreibungen
- Abmischen in WAV oder FLAC
- Dunkles und helles Design
- Begleitanwendung Virtual MIDI Keyboard

## Erste Schritte

### Starten der Anwendung

Führen Sie die ausführbare Datei `musicians_canvas` aus dem Build-Verzeichnis oder Ihrem
Installationsort aus:

```
./musicians_canvas
```

Beim ersten Start öffnet die Anwendung ein leeres Projekt. Sie müssen ein
Projektverzeichnis festlegen, bevor Sie aufnehmen können.

Beim Starten wendet die Anwendung das gespeicherte Design (dunkel oder hell) an, und wenn
zuvor ein Projektverzeichnis verwendet wurde, das eine `project.json`-Datei enthält, wird
das Projekt automatisch geladen.

### Ein Projekt einrichten

1. **Projektverzeichnis festlegen**: Geben Sie im Feld "Project Location" oben im Fenster
   einen Ordnerpfad ein oder navigieren Sie dorthin. Hier werden Aufnahmen und die
   Projektdatei gespeichert.

2. **Eine Spur hinzufügen**: Klicken Sie auf die Schaltfläche **+ Add Track**. Eine neue
   Spur erscheint mit Standardeinstellungen. Wenn es die einzige Spur im Projekt ist und
   noch nicht aufgenommen wurde, wird sie automatisch für die Aufnahme scharfgeschaltet.

3. **Die Spur benennen**: Geben Sie einen Namen in das Textfeld neben der Schaltfläche
   "Options" ein. Dieser Name wird als Dateiname für die aufgenommene Audiodatei verwendet.

![Hauptfenster](../screenshots/MusiciansCanvas_1_MainWin.png)

### Projekte speichern und öffnen

- **Speichern**: Verwenden Sie **File > Save Project** (Ctrl+S), um das aktuelle Projekt als
  JSON-Datei im Projektverzeichnis zu speichern.
- **Öffnen**: Verwenden Sie **File > Open Project** (Ctrl+O), um ein zuvor gespeichertes
  Projekt zu laden.

Die Projektdatei (`project.json`) speichert Spurnamen, -typen, MIDI-Noten,
Audiodatei-Referenzen und alle projektspezifischen Einstellungen. Audiodateien werden im
selben Verzeichnis wie `project.json` gespeichert und nach ihren Spuren benannt (z.B.
`My_Track.flac`).

Wenn Sie die Anwendung mit ungespeicherten Änderungen schließen, fragt ein
Bestätigungsdialog, ob Sie vor dem Beenden speichern möchten.

## Spurverwaltung

### Spuren hinzufügen und entfernen

- Klicken Sie auf **+ Add Track**, um eine neue Spur zur Anordnung hinzuzufügen.
- Klicken Sie auf die Schaltfläche **x** auf der rechten Seite einer Spurzeile, um sie zu
  entfernen.
- Klicken Sie auf **Clear Tracks** (die rote Schaltfläche in der Werkzeugleiste), um alle
  Spuren zu entfernen. Vor dem Fortfahren wird ein Bestätigungsdialog angezeigt.

### Spurtyp konfigurieren

Jede Spur kann entweder als **Audio** (für Mikrofon-/Line-In-Aufnahme) oder **MIDI** (für
Keyboard-/Controller-Aufnahme) konfiguriert werden.

Um den Spurtyp zu ändern:

- Klicken Sie auf die Schaltfläche **Options** der Spur, oder
- Klicken Sie auf das **Spurtyp-Symbol** (zwischen "Options" und dem Namensfeld)

Dies öffnet den Dialog zur Spurkonfiguration, in dem Sie die Eingangsqülle auswählen
können.

![Spurkonfiguration](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Spursteuerungen

Jede Spurzeile bietet folgende Steuerungen:

- **Aktivierungs-Kontrollkästchen**: Schaltet um, ob die Spur bei Wiedergabe und Abmischung
  berücksichtigt wird. Das Deaktivieren einer Spur entschärft sie automatisch, falls sie
  scharfgeschaltet war.
- **Scharfschaltungs-Radiobutton**: Wählt diese Spur als Aufnahmeziel aus. Es kann jeweils
  nur eine Spur scharfgeschaltet sein; das Scharfschalten einer neuen Spur entschärft
  automatisch jede zuvor scharfgeschaltete Spur.
- **Namensfeld**: Bearbeitbares Textfeld für den Spurnamen. Dieser Name wird als
  Audiodateiname verwendet (ungültige Dateisystemzeichen werden durch Unterstriche ersetzt).
- **Options-Schaltfläche**: Öffnet den Dialog zur Spurkonfiguration.
- **Typ-Symbol**: Zeigt ein Lautsprechersymbol für Audiospuren oder ein Klaviersymbol für
  MIDI-Spuren. Ein Klick darauf öffnet den Dialog zur Spurkonfiguration.
- **Entfernen-Schaltfläche (x)**: Entfernt die Spur aus dem Projekt.

### Automatisches Scharfschalten

Wenn ein Projekt genau eine Spur hat und diese Spur noch nicht aufgenommen wurde, wird sie
automatisch für die Aufnahme scharfgeschaltet. Dies gilt sowohl beim Hinzufügen der ersten
Spur zu einem neuen Projekt als auch beim Öffnen eines bestehenden Projekts mit einer
einzelnen leeren Spur.

### Spurvisualisierung

- **Audiospuren** zeigen eine Wellenformvisualisierung des aufgenommenen Audios. Wenn kein
  Audio aufgenommen wurde, zeigt der Bereich "No audio recorded" an.
- **MIDI-Spuren** zeigen eine Piano-Roll-Visualisierung mit aufgenommenen Noten auf einem
  Raster von A0 bis C8. Die Noten sind nach Anschlagstärke eingefärbt. Wenn keine
  MIDI-Daten aufgenommen wurden, zeigt der Bereich "No MIDI data recorded" an.

## Aufnahme

### Audioaufnahme

1. Stellen Sie sicher, dass das Projektverzeichnis festgelegt ist.
2. Schalten Sie die Zielspur scharf (aktivieren Sie den "Arm"-Radiobutton).
3. Klicken Sie auf die Schaltfläche **Record** (roter Kreis).
4. Ein 3-Sekunden-Countdown erscheint auf der Spur ("Get ready... 3", "2", "1"), dann
   beginnt die Aufnahme.
5. Während der Aufnahme wird ein Live-Pegelanzeiger im Wellenformbereich der Spur
   angezeigt, der die aktuelle Amplitude als Verlaufsbalken (grün nach gelb nach rot) mit
   einer "Recording"-Beschriftung darstellt.
6. Klicken Sie auf die Schaltfläche **Stop**, um die Aufnahme zu beenden.

Das aufgenommene Audio wird als FLAC-Datei im Projektverzeichnis gespeichert und nach der
Spur benannt.

Während der Aufnahme und Wiedergabe werden alle interaktiven Steuerelemente (Spur-
Schaltflächen, Einstellungen usw.) deaktiviert, um versehentliche Änderungen zu verhindern.

#### Overdub-Aufnahme

Wenn Sie eine neue Spur aufnehmen, während andere aktivierte Spuren bereits Audio- oder
MIDI-Daten enthalten, führt Musician's Canvas eine Overdub-Aufnahme durch: Die vorhandenen
Spuren werden zusammengemischt und in Echtzeit wiedergegeben, während die neue Spur
aufgenommen wird. So können Sie zuvor aufgenommene Teile hören, während Sie einen neuen
einspielen.

Die Mischung der vorhandenen Spuren wird vor Beginn der Aufnahme vorbereitet, sodass
Aufnahme und Wiedergabe annähernd gleichzeitig starten und alle Spuren synchron bleiben.

#### Aufnahme-Backends

Musician's Canvas unterstützt zwei Audio-Aufnahme-Backends:

- **PortAudio** (Standard, wenn verfügbar): Bietet zuverlässige Aufnahme mit niedriger
  Latenz und ist das empfohlene Backend.
- **Qt Multimedia**: Ein Ausweich-Backend, das die integrierte Audioaufnahme von Qt
  verwendet. Wird genutzt, wenn PortAudio nicht verfügbar ist oder wenn es in den
  Projekteinstellungen explizit ausgewählt wird.

Das Aufnahme-Backend kann projektspezifisch unter **Project > Project Settings > Audio**
konfiguriert werden.

#### Abtastrate und Gerätebehandlung

Musician's Canvas nimmt mit der nativen Abtastrate des Audio-Eingangsgeräts auf und
konvertiert anschließend automatisch in die konfigurierte Projekt-Abtastrate mittels
hochwertiger Neuabtastung. Das bedeutet, Sie können jede Projekt-Abtastrate festlegen
(z.B. 44100 Hz oder 48000 Hz), unabhängig von der nativen Rate des Geräts. Die Konvertierung
bewahrt Tonhöhe und Daür exakt.

#### Mono-Geräteerkennung

Einige Audiogeräte (z.B. USB-Webcam-Mikrofone) sind physisch mono, werden aber vom
Betriebssystem als Stereo angezeigt. Musician's Canvas erkennt dies automatisch und passt die
Kanalanzahl entsprechend an. Wenn das Projekt für Stereo konfiguriert ist, wird das
Mono-Signal auf beide Kanäle dupliziert.

### MIDI-Aufnahme

1. Setzen Sie den Spurtyp über die Options-Schaltfläche auf **MIDI**.
2. Stellen Sie sicher, dass ein MIDI-Eingangsgerät unter **Settings > Configuration > MIDI**
   konfiguriert ist.
3. Schalten Sie die Spur scharf und klicken Sie auf Record.
4. Spielen Sie Noten auf Ihrem MIDI-Controller.
5. Klicken Sie auf Stop, um die Aufnahme zu beenden.

MIDI-Noten werden in einer Piano-Roll-Visualisierung auf der Spur angezeigt.

## Wiedergabe

Klicken Sie auf die Schaltfläche **Play**, um alle aktivierten Spuren abzumischen und
wiederzugeben. Der Tooltip der Schaltfläche zeigt an, ob sie abspielen oder aufnehmen wird,
je nachdem ob eine Spur scharfgeschaltet ist. Deaktivierte Spuren (nicht angehakt) werden von
der Wiedergabe ausgeschlossen.

Während der Wiedergabe werden Audiospuren aus ihren FLAC-Dateien dekodiert und MIDI-Spuren
mit dem integrierten FluidSynth-Synthesizer in Audio umgewandelt. Alle Spuren werden
zusammengemischt und über das Audio-Ausgabegerät des Systems wiedergegeben.

Klicken Sie jederzeit auf die Schaltfläche **Stop**, um die Wiedergabe zu beenden.

## In eine Datei abmischen

Verwenden Sie **Tools > Mix tracks to file** (Ctrl+M), um alle aktivierten Spuren in eine
einzelne Audiodatei zu exportieren. Ein Dialog ermöglicht die Auswahl des Ausgabepfads und
Formats:

- **Ausgabedatei**: Navigieren Sie zum gewünschten Zieldateipfad.
- **Format**: Wählen Sie zwischen FLAC (verlustfreie Komprimierung, kleinere Dateien) oder
  WAV (unkomprimiert).

Die Abmischung verwendet die konfigurierte Projekt-Abtastrate. MIDI-Spuren werden mit dem
konfigurierten SoundFont gerendert.

## Einstellungen

### Globale Einstellungen

Verwenden Sie **Settings > Configuration** (Ctrl+,), um globale Standardwerte festzulegen,
die für alle Projekte gelten:

![Allgemeine Einstellungen](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### Registerkarte "General"

- **Theme**: Wählen Sie zwischen dunklem und hellem Design.

#### Registerkarte "MIDI"

- **MIDI Output Device**: Wählen Sie den integrierten FluidSynth-Synthesizer oder ein
  externes MIDI-Gerät. Verwenden Sie die Schaltfläche **Refresh**, um nach verfügbaren
  MIDI-Geräten erneut zu suchen.
- **SoundFont**: Navigieren Sie zu einer `.sf2` SoundFont-Datei für die MIDI-Synthese.
  Unter Linux kann ein System-SoundFont automatisch erkannt werden, wenn das Paket
  `fluid-soundfont-gm` installiert ist. Unter Windows und macOS müssen Sie den
  SoundFont-Pfad manuell konfigurieren.

![MIDI-Einstellungen](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Registerkarte "Audio"

- **Audio Input Device**: Wählen Sie das Mikrofon oder Line-In-Gerät für die Aufnahme.
- **Audio Output Device**: Wählen Sie den Lautsprecher oder Kopfhörer für die Wiedergabe.

![Audio-Einstellungen](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Projekteinstellungen

Verwenden Sie **Project > Project Settings** (Ctrl+P), um die globalen Standardwerte nur
für das aktuelle Projekt zu überschreiben. Dies ist nützlich für Projekte, die eine
bestimmte Abtastrate, ein bestimmtes SoundFont oder ein bestimmtes Audiogerät benötigen.
Projektspezifische Einstellungen werden in der Datei `project.json` gespeichert.

![Projekt-MIDI-Einstellungen](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### Registerkarte "MIDI"

- **MIDI Device**: Wählen Sie ein MIDI-Gerät für dieses Projekt, oder belassen Sie die
  Standardeinstellung, um die globale Einstellung zu verwenden.
- **SoundFont**: Wählen Sie eine SoundFont-Datei für dieses Projekt.
- **Refresh**: Erneute Suche nach verfügbaren MIDI-Geräten.

#### Registerkarte "Audio"

- **Audio Input Device**: Wählen Sie das Aufnahmegerät für dieses Projekt.
- **Recording capture backend** (wenn PortAudio verfügbar ist):
  - **PortAudio (native input)**: Empfohlen. Verwendet dieselbe Audiobibliothek wie Audacity.
  - **Qt Multimedia**: Ausweichoption mit der integrierten Audioaufnahme von Qt.
- **PortAudio Input Device**: Wählen Sie bei Verwendung des PortAudio-Backends das
  spezifische PortAudio-Eingangsgerät aus.
- **Audio Output Device**: Wählen Sie das Wiedergabegerät für dieses Projekt.

##### Audioformat-Einstellungen

- **Sample rate**: Wählen Sie aus Standardraten (8000 Hz bis 192000 Hz). Die native Rate des
  Geräts ist mit "(native)" gekennzeichnet. Raten, die eine Neuabtastung erfordern, sind mit
  "(resampled)" gekennzeichnet. Sie können jede Rate unabhängig von der Geräteeigenschaft
  wählen; Musician's Canvas führt bei Bedarf automatisch eine Neuabtastung durch.
- **Channels**: Mono oder Stereo. Wenn das Gerät nur Mono unterstützt, ist die
  Stereo-Option deaktiviert.

![Projekt-Audio-Einstellungen](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

## Menüs

### Menue "File"

| Menüeintrag  | Tastenkürzel | Beschreibung                              |
|---------------|---------------|-------------------------------------------|
| Save Project  | Ctrl+S        | Aktülles Projekt auf der Festplatte speichern |
| Open Project  | Ctrl+O        | Ein bestehendes Projekt öffnen           |
| Close         | Ctrl+Q        | Die Anwendung beenden                     |

### Menue "Project"

| Menüeintrag                        | Tastenkürzel | Beschreibung                                    |
|-------------------------------------|---------------|-------------------------------------------------|
| Project Settings                    | Ctrl+P        | Projektspezifische Einstellungen konfigurieren  |
| Add Demo Data to Selected Track     |               | Beispiel-MIDI-Noten zur Demonstration hinzufügen |

### Menue "Settings"

| Menüeintrag  | Tastenkürzel | Beschreibung                                  |
|---------------|---------------|-----------------------------------------------|
| Configuration | Ctrl+,        | Globale Anwendungseinstellungen öffnen       |

### Menue "Tools"

| Menüeintrag          | Tastenkürzel | Beschreibung                                  |
|-----------------------|---------------|-----------------------------------------------|
| Mix tracks to file    | Ctrl+M        | Alle aktivierten Spuren in eine Datei exportieren |
| Virtual MIDI Keyboard |               | Die Begleit-Tastaturanwendung starten         |

## Tastenkürzel

| Tastenkürzel   | Aktion                                |
|-----------------|---------------------------------------|
| Ctrl+S          | Projekt speichern                     |
| Ctrl+O          | Projekt öffnen                       |
| Ctrl+M          | Spuren in eine Datei abmischen        |
| Ctrl+P          | Projekteinstellungen                  |
| Ctrl+,          | Einstellungen / Konfiguration         |
| Ctrl+Q / Alt+F4 | Beenden                              |

## Virtual MIDI Keyboard

Das Virtual MIDI Keyboard ist eine Begleitanwendung (`virtual_midi_keyboard`), die eine
Bildschirm-Klaviertastatur zum Senden von MIDI-Noten bereitstellt. Es kann über das Menue
**Tools > Virtual MIDI Keyboard** in der Hauptanwendung gestartet oder unabhängig
ausgeführt werden.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Funktionen

- Klicken Sie auf die Tasten der Bildschirm-Klaviatur, um Noten zu spielen
- Verwenden Sie die Computertastatur als Klaviertastatur (siehe Tastenbelegung unten)
- Verbindung mit externen MIDI-Ausgabegeräten oder Verwendung des integrierten
  FluidSynth-Synthesizers
- Verbindung mit einem MIDI-Eingangsgerät zur Anzeige eingehender Noten auf der Tastatur
- Einstellbare Synthesizer-Lautstärke (Gesamtverstärkung, 10%--200%)
- SoundFont-Auswahl für den integrierten Synthesizer
- MIDI-Instrument-/Programmauswahl (General MIDI oder SoundFont-Presets)
- Chorus-/Effekt-Regler
- Oktavverschiebung (-3 bis +5)
- MIDI-Lautstärkeregelung (CC#7, 0--127)

### Werkzeugleisten-Steuerungen

- **Volume**: MIDI-Lautstärke (CC#7), einstellbar von 0 bis 127 über das Drehfeld.
- **Octave**: Verschieben Sie die Tastatur-Oktave mit den Schaltflächen **<** und **>** oder
  dem Drehfeld. Der Bereich ist -3 bis +5.
- **Chorus/Effect**: Ein Drehregler und Textfeld (1--127) zum Einstellen des Chorus-/Effekt-
  Pegels (MIDI CC#93). Klicken Sie auf **Apply**, um den Wert zu senden.
- **Instrumentenauswahl**: Wählen Sie ein MIDI-Instrument. Bei Verwendung des integrierten
  FluidSynth-Synthesizers werden SoundFont-Presets angezeigt (Bank:Programm Name). Bei
  Verbindung mit einem externen MIDI-Gerät werden die 128 General-MIDI-Instrumente
  aufgelistet.

### Spielen mit der Computertastatur

Die Computertastatur ist über zwei Oktaven auf Klaviertasten abgebildet:

**Untere Oktave (beginnend bei der aktuellen Oktave):**

| Taste | Note  |
|-------|-------|
| Z     | C     |
| S     | C#/Db |
| X     | D     |
| D     | D#/Eb |
| C     | E     |
| V     | F     |
| G     | F#/Gb |
| B     | G     |
| H     | G#/Ab |
| N     | A     |
| J     | A#/Bb |
| M     | B     |

**Obere Oktave (eine Oktave höher):**

| Taste | Note  |
|-------|-------|
| Q     | C     |
| 2     | C#/Db |
| W     | D     |
| 3     | D#/Eb |
| E     | E     |
| R     | F     |
| 5     | F#/Gb |
| T     | G     |
| 6     | G#/Ab |
| Y     | A     |
| 7     | A#/Bb |
| U     | B     |
| I     | C (nächste Oktave) |
| 9     | C#/Db |
| O     | D     |
| 0     | D#/Eb |
| P     | E     |

Tasten leuchten beim Drücken visuell auf (weiße Tasten werden hellblau, schwarze Tasten
werden dunkler).

### Konfiguration

Öffnen Sie den Konfigurationsdialog (**Settings > Configuration**, Ctrl+,), um MIDI- und
Audiogeräte einzurichten:

![Virtual MIDI Keyboard Konfiguration](../screenshots/VMIDIKeyboard2.png)

#### Registerkarte "MIDI"

- **MIDI Output Device**: Wählen Sie ein externes Gerät oder lassen Sie das Feld leer für
  den integrierten FluidSynth-Synthesizer.
- **MIDI Input Device**: Wählen Sie einen Controller, um Noten an die Tastaturanzeige
  weiterzuleiten.
- **Synthesizer Volume (Master Gain)**: Passen Sie den Ausgabepegel des integrierten
  Synthesizers an (10%--200%).
- **SoundFont**: Navigieren Sie zu einer `.sf2` SoundFont-Datei für den integrierten
  Synthesizer.
- **Refresh**: Erneute Suche nach verfügbaren MIDI-Geräten.

#### Registerkarte "Audio"

- **Audio Output Device**: Wählen Sie das Ausgabegerät für den integrierten Synthesizer.

### Virtual MIDI Keyboard Tastenkürzel

| Tastenkürzel | Aktion                         |
|---------------|--------------------------------|
| Ctrl+,        | Konfigurationsdialog           |
| Ctrl+U        | Hilfe / Nutzungsinformationen  |
| Ctrl+Q        | Schließen                     |

## Fehlerbehebung

### Keine Audioausgabe

- Überprüfen Sie, ob das korrekte Audio-Ausgabegerät unter Settings > Configuration >
  Audio ausgewählt ist.
- Unter Linux stellen Sie sicher, dass PipeWire oder PulseAudio läuft und die Ausgabe nicht
  stummgeschaltet ist. Verwenden Sie `amixer` oder die Klangeinstellungen Ihres Desktops, um
  die Lautstärkepegel zu überprüfen.
- Unter Windows stellen Sie sicher, dass ein ASIO-Audiotreiber installiert ist (z.B.
  [ASIO4ALL](https://asio4all.org/) oder ein vom Hersteller bereitgestellter ASIO-Treiber
  für Ihre Audio-Schnittstelle). Musician's Canvas benötigt ASIO für latenzarmes Audio
  unter Windows.

### MIDI-Spuren sind stumm

- Stellen Sie sicher, dass ein SoundFont (`.sf2`-Datei) unter Settings > Configuration >
  MIDI konfiguriert ist.
- Unter Linux kann ein System-SoundFont automatisch erkannt werden, wenn das Paket
  `fluid-soundfont-gm` installiert ist.
- Unter Windows und macOS müssen Sie den SoundFont-Pfad manuell konfigurieren.

### Aufnahme klingt verzerrt oder hat falsche Tonhöhe

- Dies kann auftreten, wenn die native Abtastrate des Audio-Eingangsgeräts von der
  konfigurierten Projektrate abweicht. Musician's Canvas handhabt dies automatisch durch
  Neuabtastung, aber falls Probleme bestehen bleiben, versuchen Sie, die Projekt-Abtastrate
  an die native Rate des Geräts anzupassen.
- USB-Webcam-Mikrofone haben oft ungewöhnliche native Raten (z.B. 32000 Hz). Die Anwendung
  erkennt diese automatisch.
- Wenn Sie das Qt Multimedia-Backend verwenden und Probleme auftreten, versuchen Sie, in den
  Projekteinstellungen unter Project Settings > Audio zum PortAudio-Backend zu wechseln.

### Virtual MIDI Keyboard hat keinen Ton

- Unter Linux mit PipeWire stellen Sie sicher, dass das Paket `libpipewire-0.3-dev`
  installiert ist (wird für die PipeWire-Integration mit dem FluidSynth-Synthesizer
  benötigt).
- Überprüfen Sie, ob ein SoundFont geladen ist (siehe die Registerkarte MIDI im
  Konfigurationsdialog).
- Stellen Sie sicher, dass das Audio-Ausgabegerät ausgewählt und die Systemlautstärke
  nicht stummgeschaltet ist.

## Aus dem Quellcode erstellen

Siehe die [README](../README.md) für vollständige Build-Anleitungen für Linux, macOS und
Windows, einschliesslich aller erforderlichen Abhängigkeiten.
