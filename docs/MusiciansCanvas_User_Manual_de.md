---
title: "Musician's Canvas Benutzerhandbuch"
subtitle: "Mehrspurige Musikaufnahme-Anwendung"
version: "0.1.1"
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
- Lokalisiert in 18 Sprachen, darunter English, Deutsch, Español, Français, Japanese, Português, Chinese, Русский, Svenska, Suomi, Dansk, Norsk, Polski, Ελληνικά, Gaeilge, Cymraeg und Pirate
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

![Hauptfenster](../screenshots/i18n/de/MusiciansCanvas_1_MainWin.png)

### Schaltflächenleiste

Direkt unter der Menüleiste befindet sich eine Werkzeugleiste mit Schnellzugriffs-Schaltflächen:

![Schaltflächenleiste](../screenshots/i18n/de/ButtonBar.png)

- **Open Project**: Entspricht **File > Open Project** — öffnet ein zuvor gespeichertes Projekt.
- **Save Project**: Entspricht **File > Save Project** — speichert das aktuelle Projekt. Diese
  Schaltfläche ist nur aktiviert, wenn ein Projektverzeichnis festgelegt ist.
- **Project Settings**: Entspricht **Project > Project Settings** — öffnet den Dialog der
  Projekteinstellungen. Diese Schaltfläche ist nur aktiviert, wenn ein Projektverzeichnis festgelegt ist.
- **Configuration**: Entspricht **Settings > Configuration** — öffnet den Dialog der globalen
  Anwendungseinstellungen.
- **Metronome Settings**: Öffnet den Dialog der Metronom-Einstellungen (siehe den Abschnitt Metronom weiter unten).

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

### Spuren per Drag-and-Drop hinzufügen

Während ein Projekt geöffnet ist, können Sie eine oder mehrere unterstützte
Audiodateien aus Ihrem Dateimanager (Windows Explorer, macOS Finder, Linux-
Dateimanager usw.) direkt auf das Musician's Canvas-Fenster ziehen, um sie
als neue Audiospuren hinzuzufügen.

- **Unterstützte Formate:** `.wav` und `.flac`. Dateien in anderen Formaten
  werden stillschweigend übersprungen, und am Ende zeigt ein Dialog an,
  welche Dateien übersprungen wurden.
- **Dateikopieren:** Wenn sich die abgelegte Datei noch nicht im Projektordner
  befindet, wird sie automatisch dorthin kopiert. Wenn bereits eine Datei mit
  demselben Namen im Projektordner vorhanden ist, werden Sie gefragt, ob sie
  ersetzt werden soll.
- **Spurname:** Der Basisname der Datei (ohne Erweiterung) wird als Name der
  neuen Spur verwendet. Das Ablegen von `Bass Line.wav` erstellt beispielsweise
  eine Audiospur mit dem Namen "Bass Line".
- **Mehrere Dateien gleichzeitig:** Mehrere Dateien können zusammen gezogen
  werden; jede unterstützte Datei wird in einem einzigen Ablegevorgang zu
  ihrer eigenen Spur.
- **Wenn das Ablegen abgelehnt wird:** Drops werden nur akzeptiert, während ein
  Projekt geöffnet ist und Musician's Canvas **nicht** gerade wiedergibt oder
  aufnimmt. Stoppen Sie zuerst die Wiedergabe oder Aufnahme, wenn Sie weitere
  Spuren hineinziehen möchten.

### Spurtyp konfigurieren

Jede Spur kann entweder als **Audio** (für Mikrofon-/Line-In-Aufnahme) oder **MIDI** (für
Keyboard-/Controller-Aufnahme) konfiguriert werden.

Um den Spurtyp zu ändern:

- Klicken Sie auf die Schaltfläche **Options** der Spur, oder
- Klicken Sie auf das **Spurtyp-Symbol** (zwischen "Options" und dem Namensfeld)

Dies öffnet den Dialog zur Spurkonfiguration, in dem Sie die Eingangsqülle auswählen
können.

![Spurkonfiguration](../screenshots/i18n/de/MusiciansCanvas_2_TrackConfig.png)

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

### Insert-Effekte (nur Audiospuren)

Audiospuren haben eine Schaltfläche **Effekte** direkt unter **Optionen**. Sie öffnet den
Dialog **Spureffekte**, in dem Sie eine **geordnete Kette** von Insert-Effekten für Aufnahmen
auf dieser Spur einrichten:

![Dialog Spureffekte](../screenshots/i18n/de/Track_Effects_Dialog.png)

Zur Verfügung stehen u. a. **Hall**, **Chorus**, **Flanger**, **Overdrive / distortion**, **Amp & cabinet** (Modellierung von Verstärker und Lautsprecherbox: Amp-Typ, Box, Gain, Bass/Mitten/Höhen, Air und Mischung) sowie **Vibrato (Tremolo)**.

**Vibrato (Tremolo)** ist das klassische Fender‑„Vibrato“ aus vielen Verstärkern: eine **periodische Lautstärkemodulation**. Mit **Speed** stellen Sie die Puls‑Geschwindigkeit ein, mit **Intensity** (Depth) die Stärke (von dezentem Schimmern bis zum „Chop“).

Screenshots des Effekts **Amp & cabinet**:

![Amp & cabinet-Modellierung (1)](../screenshots/i18n/de/Amp_And_Cabinet_Model_1.png)

![Amp & cabinet-Modellierung (2)](../screenshots/i18n/de/Amp_And_Cabinet_Model_2.png)

- Klicken Sie auf **Effekt hinzufügen…** und wählen Sie einen Effekttyp. Mehrere
  Instanzen sind möglich; mit dem roten **✕** im Kopfzeilenbereich eines Effekts entfernen
  Sie diesen.
- Ziehen Sie den **≡**-Griff, um die Reihenfolge zu ändern. Der **oberste** Effekt läuft
  **zuerst** auf dem aufgenommenen Signal.
- Regler und Anzeigen (Vorhalt, Abklingzeit, Mix, Modulation, Chorus-Rate/-Tiefe, Flanger-Manual/Rate/Tiefe/Feedback, EQ usw.)
  sind in Millisekunden und Hertz angegeben und bleiben nach der Konvertierung auf die
  **Projekt-Abtastrate** sinnvoll. **Mono** und **Stereo** werden unterstützt: Mono wird
  intern dual-mono verarbeitet und wieder auf einen Kanal gemischt; Stereo behält getrennte
  Dry-Pfade, wo die Algorithmen es zulassen.
- **OK** übernimmt die Einstellungen (sie landen im Projekt). **Abbrechen** stellt die Kette
  wie beim Öffnen des Dialogs wieder her.

Die Effekte werden auf das Audio **angewendet, wenn Sie die Aufnahme beenden** – nach der
üblichen Capture- und SRC-Pipeline. Die Konfiguration wird in `project.json` unter
`audioEffectChain` der Spur gespeichert.

### Mix-Effekte (gesamtes Projekt)

Unter **Project → Project Settings → Mix Effects** legen Sie dieselbe Art von Effektkette wie bei **Track effects** fest, angewendet auf die **komplette Mischung** aller aktivierten Spuren: beim **Abspielen des ganzen Projekts** und beim **Export als gemischte Datei** (WAV/FLAC). Die Kette wird in `project.json` unter `projectSettings` → `mixEffectChain` gespeichert.

**Project → Project Settings → Aux / Send Bus** legt eine **gemeinsame Effektkette** fest, die von jedem **Aux**-Senderegler auf der Spurzeile gespeist wird. Zuerst wird das Trockensignal aller Spuren summiert; das Signal jeder Spur nach Gain und Pan wird mit dem **Aux**-Pegel skaliert, durch diese Bus-Kette verarbeitet und der **nasse Aux**-Anteil wird zum Trockensum zurückaddiert, **bevor** **Mix Effects** laufen. So nutzen Sie z. B. einen gemeinsamen Hall oder Delay bei unabhängigen Insert-Effekten pro Spur.

Um hartes [digitales Clipping](https://en.wikipedia.org/wiki/Clipping_%28audio%29) zu vermeiden, wenn Pegel nahe Vollaussteuerung steigen, wendet die Engine eine **weiche Begrenzung** auf normalisierte Float-Samples an, unmittelbar bevor sie in 16-Bit-PCM gewandelt werden. Die Basisklasse **EffectWidget** dokumentiert `guardFloatSampleForInt16Pcm()` und `softLimitFloatSampleForInt16Pcm()` für neuen DSP-Code.

### Mixer pro Spur, Stummschaltung/Solo, Trim und MIDI

Jede Spurzeile enthält einen kompakten **Mixer-Bereich**:

- **Gain**: Pegel in Dezibel (Regler in Zehntel-dB; 0 dB = Unity).
- **Pan**: Stereo-Position (-100 = ganz links, +100 = ganz rechts).
- **Aux**: Send-Pegel (0–100 %) in den **Aux- / Send-Bus** des Projekts (siehe oben).
- **Stumm**: blendet die Spur in der Mischung aus, ohne sie in der Arranger-Ansicht zu deaktivieren.
- **Solo**: Wenn **irgendeine** Spur **Solo** aktiviert hat, sind **nur** solche Spuren hörbar (außer sie sind zusätzlich stummgeschaltet).

**Options → Track Configuration** bietet **Clip trim (nicht-destruktiv)**: **Trim start** und **Trim end** lassen so viele Sekunden am Anfang und Ende des Clips für **Wiedergabe, Mix und Export** weg, ohne die zugrundeliegende Aufnahme zu löschen.

MIDI-Spuren können **Control-Change- (CC-)Automatisierung** im Projekt und in exportierten `.mid`-Dateien tragen; Offline-Wiedergabe und Mix verwenden diese Ereignisse beim Rendern von MIDI zu Audio.

**Edit → Undo** / **Redo** (übliche Tastenkürzel) gelten für Mixer- und Trim-Änderungen auf den Spuren.

### Mithören während der Aufnahme

Neben der **Zeitanzeige** steuert das Kontrollkästchen **Ton während der Aufnahme mithören**,
ob **Live-Eingang** während der Aufnahme an den **Projekt-Audioausgang** geleitet wird:

- **Audiospuren**: Der Eingang wird in Echtzeit wiedergegeben (die eigentliche Aufnahme-Pipeline bleibt unverändert). Das läuft zusätzlich zur **Overdub**-Wiedergabe anderer Spuren.
- **MIDI-Spuren**: Wenn im Projekt **MIDI für Wiedergabe in Audio rendern** aktiv ist und ein **SoundFont** gesetzt ist, erscheinen gespielte Noten über den Software-Synthesizer. Bei **externem MIDI-Ausgang** nutzen Sie die Monitoring-Funktion Ihres Geräts.

Die Einstellung wird **im Projekt** gespeichert (`monitorWhileRecording` in `project.json`).
Deaktivieren Sie das Mithören, um z. B. Rückkopplung vom Mikrofon zu vermeiden.

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

## Metronom

Musician's Canvas enthält ein integriertes Metronom, das während der Aufnahme zum Halten
des Takts verwendet werden kann. Klicken Sie auf die Metronom-Schaltfläche in der
Schaltflächenleiste (unter der Menüleiste), um den Dialog der Metronom-Einstellungen zu öffnen:

![Metronom-Einstellungen](../screenshots/i18n/de/MetronomeSettings.png)

Der Dialog bietet:

- **Enable metronome during recording**: Wenn aktiviert, spielt das Metronom während der
  Aufnahme einen Tick-Ton ab. Der Tick wird über das Systemaudio wiedergegeben und **nicht**
  in die aufgenommene Spur aufgenommen.
- **Beats per minute**: Eine numerische Eingabe für das Tempo in Schlägen pro Minute (BPM).
  Der Bereich ist 20–300 BPM. Wenn **Project → Tempo map** Tempoänderungen definiert, folgt das Metronom diesen Markern während der Aufnahme (das BPM-Feld legt weiterhin das Ausgangstempo fest, solange noch kein Marker greift).

Wenn das Metronom aktiviert ist, beginnt es zu ticken, sobald die Aufnahme tatsächlich
startet (nach Ablauf des 3-Sekunden-Countdowns), und stoppt, wenn die Aufnahme endet.

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
konfigurierten SoundFont gerendert. **Gain**, **Pan**, **Stumm/Solo**, **Aux-Send** und **Trim**
pro Spur werden wie bei der Wiedergabe angewendet.

**Tools → Export stems to folder** schreibt eine **WAV**-Datei pro Spur (aktivierte Spuren, die am Mix
teilnehmen). Jeder Stem übernimmt die Mixer-Einstellungen und den Trim dieser Spur; die **Mix Effects**
des Gesamtprogramms werden **nicht** auf einzelne Stems angewendet.

## Einstellungen

### Globale Einstellungen

Verwenden Sie **Settings > Configuration** (Ctrl+,), um globale Standardwerte festzulegen,
die für alle Projekte gelten:

![Allgemeine Einstellungen](../screenshots/i18n/de/MusiciansCanvas_3_GeneralSettings.png)

#### Registerkarte "General"

- **Theme**: Wählen Sie zwischen dunklem und hellem Design.
- **Debug-Protokoll für Aufnahme schreiben**: Wenn aktiviert, schreibt Musician's Canvas
  detaillierte Diagnoseinformationen zur Aufnahme in eine Datei namens `recording_debug.txt`
  im Projektverzeichnis. Dies umfasst das Aufnahme-Backend, den Gerätenamen, die
  Abtastrate, die Kanalanzahl und Zeitdaten. Diese Option ist standardmäßig deaktiviert
  und dient hauptsächlich zur Fehlerbehebung bei Aufnahmeproblemen.

#### Registerkarte "Display"

- **Farbe der numerischen LED-Anzeige**: Wählen Sie die Farbe für die numerische
  LED-Zeitanzeige in der Symbolleiste des Hauptfensters. Die aktiven Ziffernsegmente
  werden in der gewählten Farbe gezeichnet, inaktive Segmente als abgedunkelte
  Version derselben Farbe. Verfügbare Farben sind Light Red, Dark Red, Light Green,
  Dark Green, Light Blue, Dark Blue, Yellow, Orange, Light Cyan und Dark Cyan. Die
  Standardeinstellung ist Light Green.

![Anzeigeeinstellungen](../screenshots/i18n/de/MusiciansCanvas_DisplaySettings.png)

#### Registerkarte "Language"

- **Sprache**: Wählen Sie die Anzeigesprache der Anwendung. Die Standardeinstellung ist
  „Systemstandard", die die Spracheinstellung des Betriebssystems verwendet. Verfügbare
  Sprachen sind English, Deutsch (German), Español (Spanish), Français (French), Japanese,
  Português (Brazilian Portuguese), Chinese (Traditional), Chinese (Simplified), Dansk
  (Danish), Ελληνικά (Greek), Gaeilge (Irish), Norsk (Norwegian), Polski (Polish),
  Русский (Russian), Suomi (Finnish), Svenska (Swedish), Cymraeg (Welsh) und Pirate.
  Die Benutzeroberfläche wird sofort aktualisiert, wenn Sie die Sprache ändern.

![Spracheinstellungen](../screenshots/i18n/de/MusiciansCanvas_LanguageSettings.png)

#### Registerkarte "MIDI"

- **MIDI Output Device**: Wählen Sie den integrierten FluidSynth-Synthesizer oder ein
  externes MIDI-Gerät. Verwenden Sie die Schaltfläche **Refresh**, um nach verfügbaren
  MIDI-Geräten erneut zu suchen.
- **SoundFont**: Navigieren Sie zu einer `.sf2` SoundFont-Datei für die MIDI-Synthese.
  Unter Linux kann ein System-SoundFont automatisch erkannt werden, wenn das Paket
  `fluid-soundfont-gm` installiert ist. Unter Windows und macOS müssen Sie den
  SoundFont-Pfad manuell konfigurieren.

![MIDI-Einstellungen](../screenshots/i18n/de/MusiciansCanvas_4_MIDISettings.png)

#### Registerkarte "Audio"

- **Audio Input Device**: Wählen Sie das Mikrofon oder Line-In-Gerät für die Aufnahme.
- **Audio Output Device**: Wählen Sie den Lautsprecher oder Kopfhörer für die Wiedergabe.

![Audio-Einstellungen](../screenshots/i18n/de/MusiciansCanvas_5_AudioSettings.png)

### Projekteinstellungen

Verwenden Sie **Project > Project Settings** (Ctrl+P), um die globalen Standardwerte nur
für das aktuelle Projekt zu überschreiben. Dies ist nützlich für Projekte, die eine
bestimmte Abtastrate, ein bestimmtes SoundFont oder ein bestimmtes Audiogerät benötigen.
Projektspezifische Einstellungen werden in der Datei `project.json` gespeichert.

![Projekt-MIDI-Einstellungen](../screenshots/i18n/de/MusiciansCanvas_6_ProjectMIDISettings.png)

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

![Projekt-Audio-Einstellungen](../screenshots/i18n/de/MusiciansCanvas_7_ProjectAudioSettings.png)

#### Registerkarte Mix Effects

Wie bei **Track effects**: scrollbare Liste mit **Effekt hinzufügen…**, **≡** zum Neuordnen, **✕** zum Entfernen. Die Reihenfolge ist **von oben nach unten** auf dem **Summensignal**, nachdem alle aktivierten Spuren gemischt wurden. Diese Effekte laufen bei **Wiedergabe aller Spuren** und beim **Mix zu einer Datei**; sie werden **nicht** in die einzelnen Spurdateien auf der Festplatte „eingebacken“. Eine leere Liste lässt den Mix unverändert (abgesehen von der internen Pegelführung des Mixers).

#### Registerkarte Aux / Send Bus

Hier konfigurieren Sie die **gemeinsame Aux-Effektkette** (dieselben Effekttypen wie bei Spur-Inserts). Der **Aux**-Regler jeder Spur steuert, wie viel des Signals dieser Spur durch diesen Bus läuft; der nasse Aux-Rückweg wird zum Trockensum addiert, **bevor** **Mix Effects** angewendet werden.

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
| Project Settings                    | Ctrl+P        | Projektspezifische Einstellungen konfigurieren (inkl. **Aux / Send Bus**)  |
| Tempo map                           |               | Tempoänderungen (Sekunden vs. BPM) bearbeiten; Metronom-Hinweise und MIDI-Quantisierung |
| Add Demo Data to Selected Track     |               | Beispiel-MIDI-Noten zur Demonstration hinzufügen |

### Menue "Edit"

| Menüeintrag | Tastenkürzel | Beschreibung |
|---------------|---------------|--------------|
| Undo          | Ctrl+Z        | Mixer-/Trim-Änderungen rückgängig machen |
| Redo          | Ctrl+Shift+Z  | Wiederholen |

### Menue "Settings"

| Menüeintrag  | Tastenkürzel | Beschreibung                                  |
|---------------|---------------|-----------------------------------------------|
| Configuration | Ctrl+,        | Globale Anwendungseinstellungen öffnen       |

### Menue "Tools"

| Menüeintrag          | Tastenkürzel | Beschreibung                                  |
|-----------------------|---------------|-----------------------------------------------|
| Mix tracks to file    | Ctrl+M        | Alle aktivierten Spuren in eine Datei exportieren |
| Export stems to folder |               | Ein WAV-Stem pro Spur (Gain/Pan/Trim; ohne Master-Mix-Effects) |
| Recording options     |               | **Punch-in**-Bereich für Audio-Retakes; **Loop**-Wiedergabe für das ganze Projekt |
| Quantize MIDI         |               | MIDI-Notenanfänge auf ein Raster rastern (alle MIDI-Spuren oder nur scharfgeschaltete) |
| Add drum track        | D        | MIDI-Schlagzeugspur anlegen und `.mid`-Groove schreiben |
| Virtual MIDI Keyboard |               | Die Begleit-Tastaturanwendung starten         |

### Menue "Help"

| Menüeintrag | Tastenkürzel | Beschreibung |
|------------|--------------|--------------|
| Manual     | Alt+M        | Öffnet das PDF‑Benutzerhandbuch in der aktuell ausgewählten Sprache |
| About      |              | Zeigt Versions- und Programminformationen |

## Tastenkürzel

| Tastenkürzel   | Aktion                                |
|-----------------|---------------------------------------|
| Ctrl+S          | Projekt speichern                     |
| Ctrl+O          | Projekt öffnen                       |
| Ctrl+Z          | Rückgängig (Mixer/Trim)              |
| Ctrl+Shift+Z    | Wiederholen                          |
| Ctrl+M          | Spuren in eine Datei abmischen        |
| D               | Schlagzeugspur hinzufügen (Tools-Menü) |
| Ctrl+P          | Projekteinstellungen                  |
| Ctrl+,          | Einstellungen / Konfiguration         |
| Alt+M           | PDF-Handbuch öffnen (Help)            |
| Ctrl+Q / Alt+F4 | Beenden                              |


### Schlagzeugspur hinzufügen

**Tools → Add drum track** (Tastenkürzel **D**) erzeugt eine **MIDI**-Spur für Schlagzeug auf **MIDI-Kanal 10** (General MIDI; intern Kanalindex 9). Standardname z. B. **Drums** (mit Nummer bei Bedarf).

Sofort wird eine **Standard-MIDI-Datei** (`.mid`) im **Projektordner** geschrieben: **zwei Takte 4/4** mit Kick, Snare und geschlossenem Hi-Hat. Tempo:

- Ist im **Metronom**-Dialog **Metronom während Aufnahme aktiv** eingeschaltet, gilt der eingestellte **BPM**-Wert.
- Sonst wird **BPM** aus einem Mix der **aktivierten** **Audio**spuren mit Inhalt geschätzt; ohne brauchbares Audio wird **120 BPM** angenommen.

**Hinweis:** Die Schätzung ist bewusst einfach (Onset-Autokorrelation); bei manchem Material kann Halb-/Doppelzeit auftreten.

**Weiterführend:** [Audient — realistische MIDI-Drums](https://audient.com/tutorial/how-to-program-realistic-midi-drum-tracks/), [Melda MDrummer](https://www.meldaproduction.com/MDrummer), [Reddit — Drum-Patterns aus Audio](https://www.reddit.com/r/ableton/comments/1e51a7g/generating_midi_drum_patterns_based_on_audio_input/), [Stanford CS229 — Automated Music Track Generation](https://cs229.stanford.edu/proj2014/Louis%20Eugene,%20Guillaume%20Rostaing,%20Automated%20Music%20Track%20Generation.pdf).

## Virtual MIDI Keyboard

Das Virtual MIDI Keyboard ist eine Begleitanwendung (`virtual_midi_keyboard`), die eine
Bildschirm-Klaviertastatur zum Senden von MIDI-Noten bereitstellt. Es kann über das Menue
**Tools > Virtual MIDI Keyboard** in der Hauptanwendung gestartet oder unabhängig
ausgeführt werden.

![Virtual MIDI Keyboard](../screenshots/i18n/de/VMIDIKeyboard1.png)

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
- **MIDI-Kanal**: Ein numerisches Eingabefeld (1--16) wählt den MIDI-Kanal aus, auf dem
  Noten, Programmwechsel und Control Changes gesendet werden. Der Standard ist Kanal 1.
  Der ausgewählte Kanal wird sofort nach dem Ändern wirksam. Wenn ein SoundFont-Preset
  ausgewählt wird, während ein bestimmter Kanal aktiv ist, wird dieses bank/program
  diesem Kanal zugewiesen, sodass Sie verschiedene Klänge auf verschiedenen Kanälen
  spielen können.
- **Schlagzeug**: Ein Kontrollkästchen direkt über der MIDI-Kanal-Auswahl. Wenn nicht
  aktiviert (Standard), ist der MIDI-Kanal derjenige, den der Benutzer gewählt hat.
  Wenn aktiviert, wird der MIDI-Kanal automatisch auf Kanal 10 umgeschaltet — den
  Standard-Schlagzeugkanal von General MIDI —, sodass alles Gespielte als Perkussion
  klingt. Beim Deaktivieren des Kontrollkästchens wird der zuvor gewählte Kanal
  wiederhergestellt.
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

![Virtual MIDI Keyboard Konfiguration](../screenshots/i18n/de/VMIDIKeyboard2.png)

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

#### Registerkarte "Language"

- **Sprache**: Wählen Sie die Anzeigesprache. Die gleichen Sprachen wie bei Musician's
  Canvas stehen zur Verfügung. Die Benutzeroberfläche wird sofort aktualisiert, wenn Sie
  die Sprache ändern.

### Virtual MIDI Keyboard Tastenkürzel

| Tastenkürzel | Aktion                         |
|---------------|--------------------------------|
| Ctrl+,        | Konfigurationsdialog           |
| Ctrl+U        | Hilfe / Nutzungsinformationen  |
| Ctrl+Q        | Schließen                     |

## FAQ (Häufig gestellte Fragen)

### Wie nehme ich eine MIDI-Spur auf?

1. Legen Sie ein **Projektverzeichnis** fest (erforderlich zum Speichern).
2. Fügen Sie eine Spur hinzu oder wählen Sie sie und öffnen Sie **Options** (oder klicken Sie auf das Spurtypsymbol).
3. Stellen Sie den Spurtyp auf **MIDI** und schließen Sie den Dialog.
4. Unter **Settings > Configuration > MIDI** wählen Sie den **MIDI-Eingang** (Hardware-Port oder virtuelles Kabel) und einen **SoundFont** (`.sf2`) für die spätere Wiedergabe.
5. **Armen** Sie diese Spur (nur eine Spur kann gleichzeitig armed sein).
6. Klicken Sie auf **Record**, warten Sie den Countdown ab, spielen Sie auf dem Controller, dann **Stop**.

Die Noten erscheinen in der Piano-Roll-Ansicht. **File > Save Project** schreibt eine `.mid`-Datei (und `project.json`) in den Projektordner.

### Warum ist meine MIDI-Spur bei der Wiedergabe stumm?

Die Wiedergabe nutzt **FluidSynth** mit dem **SoundFont** aus Ihren Einstellungen. Prüfen Sie **Settings > Configuration > MIDI** (oder **Project > Project Settings** bei Projekt-Overrides): ein gültiger `.sf2`-Pfad muss gesetzt sein. Unter Linux wird ein System-SoundFont oft automatisch gefunden; unter Windows und macOS müssen Sie meist manuell eine SoundFont-Datei wählen.

### Wie hängt Virtual MIDI Keyboard mit Musician's Canvas zusammen?

Es sind **zwei getrennte Programme**. Starten Sie Virtual MIDI Keyboard über **Tools > Virtual MIDI Keyboard** (oder eigenständig). Damit die Bildschirmtastatur **in** Musician's Canvas auf einer MIDI-Spur aufgenommen wird, muss das Betriebssystem den **MIDI-Ausgang** der Tastatur zu einem **Eingangsport** routen, den Musician's Canvas nutzt—z. B. über ein virtuelles MIDI-Kabel oder passende Portwahl in beiden Apps. Die Programme verbinden sich nicht automatisch.

### Was ist der Unterschied zwischen Configuration und Project Settings?

**Settings > Configuration** legt **globale Standardwerte** (Theme, Sprache, MIDI/Audio, SoundFont usw.) in den Programmeinstellungen fest. **Project > Project Settings** überschreibt einige Werte **nur für das aktuelle Projekt** und wird in `project.json` gespeichert. Steht ein Feld auf dem Projekt-Default, gilt der globale Wert aus der Configuration.

### Warum funktioniert Drag-and-Drop für Audiodateien nicht?

Dateien werden nur akzeptiert, wenn ein **Projektverzeichnis gesetzt** ist und **weder Wiedergabe noch Aufnahme** läuft. Unterstützt werden **`.wav`** und **`.flac`**; andere Endungen werden übersprungen und in einem Dialog aufgelistet. Jede Datei wird eine neue **Audio**-Spur mit Namen aus dem Dateinamen (ohne Erweiterung).

### Wo werden meine Aufnahmen gespeichert?

Audioaufnahmen liegen im **Projektverzeichnis** als **`<Spurname>.flac`** (Zeichen wie `/ \ : * ? " < > |` werden zu Unterstrichen). Die Projektdatei ist **`project.json`** im gleichen Ordner. MIDI-Spuren werden beim Speichern als **`<Spurname>.mid`** abgelegt (zusätzlich zu den Noten in `project.json`).

### Welcher Dateiname wird verwendet, wenn eine MIDI-Spur fürs Mischen gerendert wird?

Beim **Mischen** oder **Abspielen** wird MIDI intern zunächst in ein temporäres WAV gerendert. Ist der Projektpfad bekannt, schreibt Musician's Canvas zusätzlich einen **FLAC-Cache** im Projektordner: **`<bereinigter_Spurname>.flac`** (gleiche Bereinigung wie bei anderen Spurdateien). Der Name folgt dem **Spurnamen**, nicht einer internen ID.

### Kann ich zwei Spuren gleichzeitig aufnehmen?

Nein. Nur **eine** Spur kann **armed** sein; sie erhält die nächste Aufnahme. Sie bauen ein Stück, indem Sie nacheinander aufnehmen (**Overdub** spielt bestehende Spuren während einer neuen Aufnahme ab).

### Wird das Metronom in die Aufnahme gemischt?

Nein. Das Metronom läuft über die **Systemwiedergabe** nur für Sie. Es landet **nicht** in der Aufnahmedatei.

### Warum verlangt Musician's Canvas unter Windows ASIO?

Unter Windows erwartet die Hauptanwendung einen **ASIO**-Treiber für zuverlässige niedrige Latenz. Installieren Sie z. B. **ASIO4ALL** oder einen Herstellertreiber, wenn Start- oder Audiofehler auftreten.

### Wo liegt Virtual MIDI Keyboard unter macOS?

Im **`.app`-Bundle** wird die Virtual-MIDI-Keyboard-Programmdatei **in** `Musician's Canvas.app` kopiert (**Contents/MacOS/**), damit ein einzelner App-Ordner verteilt werden kann. Start weiterhin über **Tools > Virtual MIDI Keyboard**.

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
