---
title: "Musician's Canvas Brugervejledning"
subtitle: "Multispors musikoptagelsesapplikation"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "da"
---

# Musician's Canvas Brugervejledning

## Introduktion

Musician's Canvas er en multispors musikoptagelsesapplikation til stationære computere. Den understøtter lydoptagelse fra mikrofoner og line-in-enheder, MIDI-optagelse fra keyboards og controllere samt miksning af alle spor til en enkelt WAV- eller FLAC-fil. Et ledsagerprogram, Virtual MIDI Keyboard, giver et software-klaviatur til afsendelse af MIDI-noter.

Musician's Canvas er designet til at være let at bruge, samtidig med at det tilbyder funktioner, der ofte findes i digitale lydarbejdsstationer (DAW'er):

- Multispors lyd- og MIDI-optagelse
- Overdub-optagelse med synkroniseret afspilning af eksisterende spor
- Indbygget FluidSynth MIDI-synthesizer med SoundFont-understøttelse
- Højkvalitets sampleratekonvertering ved optagelse med enhver projektsamplerate
- Automatisk mono/stereo-enhedsdetektering
- Projektbaserede indstillinger med projektspecifikke tilsidesættelser
- Miks til WAV eller FLAC
- Mørke og lyse temaer
- Lokaliseret på flere sprog (engelsk, tysk, spansk, fransk, japansk, portugisisk, kinesisk og pirat)
- Ledsagerapplikation Virtual MIDI Keyboard

## Kom godt i gang

### Start af applikationen

Kør den eksekverbare fil `musicians_canvas` fra byggmappen eller din installationsplacering:

```
./musicians_canvas
```

Ved første start åbner applikationen med et tomt projekt. Du skal angive en projektmappe, før du kan optage.

Ved opstart anvender applikationen det gemte tema (mørkt eller lyst), og hvis en projektmappe tidligere er blevet brugt og indeholder en `project.json`-fil, indlæses projektet automatisk.

### Opsætning af et projekt

1. **Angiv projektmappen**: Indtast eller gennemse til en mappe i feltet "Project Location" øverst i vinduet. Det er her, optagelser og projektfilen gemmes.

2. **Tilføj et spor**: Klik på knappen **+ Add Track**. Et nyt spor vises med standardindstillinger. Hvis det er det eneste spor i projektet og endnu ikke er optaget, armeres det automatisk til optagelse.

3. **Navngiv sporet**: Skriv et navn i tekstfeltet ved siden af knappen "Options". Dette navn bruges som filnavn for den optagede lydfil.

![Hovedvindue](../screenshots/MusiciansCanvas_1_MainWin.png)

### Knaplinje

Lige under menulinjen er der en værktøjslinje med hurtigadgangsknapper:

![Knaplinje](../screenshots/ButtonBar.png)

- **Open Project**: Samme som **File > Open Project** — åbner et tidligere gemt projekt.
- **Save Project**: Samme som **File > Save Project** — gemmer det aktuelle projekt. Denne
  knap er kun aktiveret, når en projektmappe er angivet.
- **Project Settings**: Samme som **Project > Project Settings** — åbner dialogen
  projektindstillinger. Denne knap er kun aktiveret, når en projektmappe er angivet.
- **Configuration**: Samme som **Settings > Configuration** — åbner dialogen med globale
  applikationsindstillinger.
- **Metronome Settings**: Åbner dialogen med metronomindstillinger (se afsnittet Metronom nedenfor).

### Gem og åbn projekter

- **Gem**: Brug **File > Save Project** (Ctrl+S) til at gemme det aktuelle projekt som en JSON-fil i projektmappen.
- **Åbn**: Brug **File > Open Project** (Ctrl+O) til at indlæse et tidligere gemt projekt.

Projektfilen (`project.json`) gemmer spornavne, typer, MIDI-noter, lydfilreferencer og alle projektspecifikke indstillinger. Lydfiler gemmes i samme mappe som `project.json` og navngives efter deres spor (f.eks. `My_Track.flac`).

Hvis du lukker applikationen med ikke-gemte ændringer, spørger en bekræftelsesdialog, om du vil gemme, før du afslutter.

## Sporhåndtering

### Tilføjelse og fjernelse af spor

- Klik på **+ Add Track** for at tilføje et nyt spor til arrangementet.
- Klik på **x**-knappen i højre side af en sporrække for at fjerne det.
- Klik på **Clear Tracks** (den røde knap i værktøjslinjen) for at fjerne alle spor. En bekræftelsesdialog vises, før handlingen udføres.

### Konfiguration af sportype

Hvert spor kan konfigureres som enten **Audio** (til mikrofon/line-in-optagelse) eller **MIDI** (til keyboard/controller-optagelse).

For at ændre sportypen:

- Klik på knappen **Options** på sporet, eller
- Klik på **sportypeikonet** (mellem "Options" og navnefeltet)

Dette åbner dialogen Sporkonfiguration, hvor du kan vælge inputkilden.

![Sporkonfiguration](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Sporkontroller

Hver sporrække har følgende kontroller:

- **Aktiver afkrydsningsfelt**: Skifter, om sporet er inkluderet i afspilning og miksning. Deaktivering af et spor afarmerer det også automatisk, hvis det var armeret.
- **Armer radioknap**: Vælger dette spor som optagelsesmål. Kun ét spor kan armeres ad gangen; armering af et nyt spor afarmerer automatisk ethvert tidligere armeret spor.
- **Navnefelt**: Redigerbart tekstfelt til spornavnet. Dette navn bruges som lydfilnavn (med ugyldige filsystemtegn erstattet af understregninger).
- **Options-knap**: Åbner dialogen Sporkonfiguration.
- **Typeikon**: Viser et højttalerikon for lydspor eller et klaverikon for MIDI-spor. Klik på det åbner dialogen Sporkonfiguration.
- **Fjern-knap (x)**: Fjerner sporet fra projektet.

### Automatisk armering

Når et projekt har præcis ét spor, og det spor endnu ikke er optaget, armeres det automatisk til optagelse. Dette gælder både, når det første spor tilføjes til et nyt projekt, og når et eksisterende projekt med et enkelt tomt spor åbnes.

### Sporvisualisering

- **Lydspor** viser en bølgeformsvisualisering af den optagede lyd. Når ingen lyd er optaget, viser området "No audio recorded".
- **MIDI-spor** viser en pianorulle-visualisering, der viser optagede noter på et gitter, der spænder fra A0 til C8. Noter er farvet efter velocity. Når ingen MIDI-data er optaget, viser området "No MIDI data recorded".

## Optagelse

### Lydoptagelse

1. Sørg for, at projektmappen er angivet.
2. Armér målsporet (marker radioknappen "Arm").
3. Klik på knappen **Record** (rød cirkel).
4. En 3-sekunders nedtælling vises på sporet ("Get ready... 3", "2", "1"), derefter begynder optagelsen.
5. Under optagelse vises en live-niveaumåler i sporets bølgeformsområde, der viser den aktuelle amplitude som en gradientbjælke (grøn til gul til rød) med etiketten "Recording".
6. Klik på knappen **Stop** for at afslutte optagelsen.

Den optagede lyd gemmes som en FLAC-fil i projektmappen, navngivet efter sporet.

Under optagelse og afspilning deaktiveres alle interaktive kontroller (sporknapper, indstillinger osv.) for at forhindre utilsigtede ændringer.

#### Overdub-optagelse

Når du optager et nyt spor, mens andre aktiverede spor allerede indeholder lyd- eller MIDI-data, udfører Musician's Canvas overdub-optagelse: de eksisterende spor mikses sammen og afspilles i realtid, mens det nye spor optages. Dette giver dig mulighed for at høre tidligere optagede dele, mens du lægger en ny del.

Mikset af eksisterende spor forberedes, før optagelsen begynder, så optagelse og afspilning starter på omtrent samme tidspunkt, hvilket holder alle spor synkroniserede.

#### Optagelsesbackends

Musician's Canvas understøtter to lyd-capture-backends:

- **PortAudio** (standard når tilgængelig): Giver pålidelig optagelse med lav latenstid og er den anbefalede backend.
- **Qt Multimedia**: En fallback-backend, der bruger Qt's indbyggede lydoptagelse. Bruges når PortAudio ikke er tilgængelig, eller når den eksplicit vælges i Projektindstillinger.

Optagelsesbackenden kan konfigureres pr. projekt i **Project > Project Settings > Audio**.

#### Samplerate og enhedshåndtering

Musician's Canvas optager med lydinputenhedens native samplerate og konverterer derefter automatisk til projektets konfigurerede samplerate ved hjælp af resampling i høj kvalitet. Det betyder, at du kan indstille enhver projektsamplerate (f.eks. 44100 Hz eller 48000 Hz) uanset enhedens native rate. Konverteringen bevarer tonehøjde og varighed nøjagtigt.

#### Mono-enhedsdetektering

Nogle lydenheder (f.eks. USB-webcam-mikrofoner) er fysisk mono, men annonceres som stereo af operativsystemet. Musician's Canvas detekterer dette automatisk og justerer kanalantallet i overensstemmelse hermed. Hvis projektet er konfigureret til stereo, duplikeres monosignalet til begge kanaler.

### MIDI-optagelse

1. Indstil sportypen til **MIDI** via Options-knappen.
2. Sørg for, at en MIDI-inputenhed er konfigureret i **Settings > Configuration > MIDI**.
3. Armér sporet og klik på Record.
4. Spil noter på din MIDI-controller.
5. Klik på Stop for at afslutte optagelsen.

MIDI-noter vises i en pianorulle-visualisering på sporet.

## Metronom

Musician's Canvas indeholder en indbygget metronom, der kan bruges under optagelse til at
hjælpe med at holde takten. Klik på metronomknappen på knaplinjen (under menulinjen) for at
åbne dialogen med metronomindstillinger:

![Metronomindstillinger](../screenshots/MetronomeSettings.png)

Dialogen indeholder:

- **Enable metronome during recording**: Når dette er afkrydset, afspiller metronomen en
  tikkelyd, mens optagelsen er aktiv. Tikket afspilles gennem systemlyden og **optages ikke**
  på det optagede spor.
- **Beats per minute**: Et numerisk input til tempoet, i slag pr. minut (BPM). Området er
  20–300 BPM.

Når metronomen er aktiveret, begynder den at tikke, når optagelsen faktisk starter (efter
at 3-sekunders nedtællingen er gennemført), og stopper, når optagelsen slutter.

## Afspilning

Klik på knappen **Play** for at mikse og afspille alle aktiverede spor. Knappens værktøjstip ændres for at angive, om den vil afspille eller optage, baseret på om et spor er armeret. Deaktiverede spor (ikke afkrydset) udelukkes fra afspilningen.

Under afspilning dekodes lydspor fra deres FLAC-filer, og MIDI-spor renderes til lyd ved hjælp af den indbyggede FluidSynth-synthesizer. Alle spor mikses sammen og afspilles gennem systemets lydoutputenhed.

Klik på knappen **Stop** for at afslutte afspilningen når som helst.

## Miksning til en fil

Brug **Tools > Mix tracks to file** (Ctrl+M) til at eksportere alle aktiverede spor til en enkelt lydfil. En dialog giver dig mulighed for at vælge outputstien og formatet:

- **Outputfil**: Gennemse for at vælge destinationsfilstien.
- **Format**: Vælg mellem FLAC (tabsfri komprimering, mindre filer) eller WAV (ukomprimeret).

Mikset bruger projektets konfigurerede samplerate. MIDI-spor renderes ved hjælp af den konfigurerede SoundFont.

## Indstillinger

### Globale indstillinger

Brug **Settings > Configuration** (Ctrl+,) til at indstille globale standarder, der gælder for alle projekter:

![Generelle indstillinger](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### Fanen Generelt

- **Tema**: Vælg mellem mørkt og lyst tema.

#### Fanen Visning

- **Farve på numerisk LED-display**: Vælg den farve, der bruges til det numeriske LED-tidsdisplay, som vises på værktøjslinjen i hovedvinduet. De aktive cifres segmenter tegnes i den valgte farve, og inaktive segmenter tegnes som en dæmpet version af samme farve. Tilgængelige farver er Light Red, Dark Red, Light Green, Dark Green, Light Blue, Dark Blue, Yellow, Orange, Light Cyan og Dark Cyan. Standard er Light Green.

![Visningsindstillinger](../screenshots/MusiciansCanvas_DisplaySettings.png)

#### Fanen Sprog

- **Sprog**: Vælg visningssproget for applikationen. Standard er "System Default", som bruger operativsystemets sprogindstilling. Tilgængelige sprog er engelsk, Deutsch (tysk), Español (spansk), Français (fransk), japansk, Português (brasiliansk portugisisk), kinesisk (traditionelt), kinesisk (forenklet) og pirat. Grænsefladen opdateres med det samme, når du ændrer sproget.

![Sprogindstillinger](../screenshots/MusiciansCanvas_LanguageSettings.png)

#### Fanen MIDI

- **MIDI-outputenhed**: Vælg den indbyggede FluidSynth-synthesizer eller en ekstern MIDI-enhed. Brug knappen **Refresh** til at scanne efter tilgængelige MIDI-enheder igen.
- **SoundFont**: Gennemse til en `.sf2` SoundFont-fil til MIDI-syntese. På Linux kan en system-SoundFont blive automatisk detekteret, hvis pakken `fluid-soundfont-gm` er installeret. På Windows og macOS skal du konfigurere SoundFont-stien manuelt.

![MIDI-indstillinger](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Fanen Lyd

- **Lydinputenhed**: Vælg mikrofonen eller line-in-enheden til optagelse.
- **Lydoutputenhed**: Vælg højttaleren eller hovedtelefonenheden til afspilning.

![Lydindstillinger](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Projektindstillinger

Brug **Project > Project Settings** (Ctrl+P) til at tilsidesætte globale standarder kun for det aktuelle projekt. Dette er nyttigt for projekter, der kræver en bestemt samplerate, SoundFont eller lydenhed. Projektspecifikke indstillinger gemmes i `project.json`-filen.

![Projekt-MIDI-indstillinger](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### Fanen MIDI

- **MIDI-enhed**: Vælg en MIDI-enhed til dette projekt, eller lad den stå på standard for at bruge den globale indstilling.
- **SoundFont**: Vælg en SoundFont-fil til dette projekt.
- **Refresh**: Scan efter tilgængelige MIDI-enheder igen.

#### Fanen Lyd

- **Lydinputenhed**: Vælg optagelsesenheden til dette projekt.
- **Optagelses-capture-backend** (når PortAudio er tilgængelig):
  - **PortAudio (native input)**: Anbefalet. Bruger det samme lydbibliotek som Audacity.
  - **Qt Multimedia**: Fallback-mulighed, der bruger Qt's indbyggede lydoptagelse.
- **PortAudio-inputenhed**: Når PortAudio-backenden bruges, vælg den specifikke PortAudio-inputenhed.
- **Lydoutputenhed**: Vælg afspilningsenheden til dette projekt.

##### Lydformatindstillinger

- **Samplerate**: Vælg mellem standardrater (8000 Hz til 192000 Hz). Enhedens native rate er mærket "(native)". Rater, der kræver resampling, er mærket "(resampled)". Du kan vælge enhver rate uanset enhedens kapabilitet; Musician's Canvas vil automatisk resample efter behov.
- **Kanaler**: Mono eller Stereo. Hvis enheden kun understøtter mono, er Stereo-indstillingen deaktiveret.

![Projekt-lydindstillinger](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

## Menuer

### Filen-menuen

| Menupunkt     | Genvej   | Beskrivelse                          |
|---------------|----------|--------------------------------------|
| Save Project  | Ctrl+S   | Gem det aktuelle projekt til disk    |
| Open Project  | Ctrl+O   | Åbn en eksisterende projektfil      |
| Close         | Ctrl+Q   | Afslut applikationen                 |

### Projekt-menuen

| Menupunkt                           | Genvej   | Beskrivelse                                    |
|-------------------------------------|----------|-------------------------------------------------|
| Project Settings                    | Ctrl+P   | Konfigurer projektspecifikke indstillinger      |
| Add Demo Data to Selected Track     |          | Tilføj eksempel-MIDI-noter til demonstration   |

### Indstillingsmenuen

| Menupunkt     | Genvej   | Beskrivelse                               |
|---------------|----------|-------------------------------------------|
| Configuration | Ctrl+,   | Åbn globale applikationsindstillinger     |

### Værktøjsmenuen

| Menupunkt             | Genvej   | Beskrivelse                                    |
|-----------------------|----------|-------------------------------------------------|
| Mix tracks to file    | Ctrl+M   | Eksportér alle aktiverede spor til en fil      |
| Virtual MIDI Keyboard |          | Start ledsagerapplikationen til keyboard       |

## Tastaturgenveje

| Genvej          | Handling                        |
|-----------------|---------------------------------|
| Ctrl+S          | Gem projekt                     |
| Ctrl+O          | Åbn projekt                     |
| Ctrl+M          | Miks spor til fil               |
| Ctrl+P          | Projektindstillinger            |
| Ctrl+,          | Indstillinger / Konfiguration   |
| Ctrl+Q / Alt+F4 | Afslut                         |

## Virtual MIDI Keyboard

Virtual MIDI Keyboard er en ledsagerapplikation (`virtual_midi_keyboard`), der giver et skærmklaviatur til afsendelse af MIDI-noter. Den kan startes fra menuen **Tools > Virtual MIDI Keyboard** i hovedapplikationen eller køres selvstændigt.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Funktioner

- Klik på tangenter på skærmklaviaturet for at spille noter
- Brug computerens tastatur som et klaviatur (se tastaturkortlægning nedenfor)
- Opret forbindelse til eksterne MIDI-outputenheder eller brug den indbyggede FluidSynth-synthesizer
- Opret forbindelse til en MIDI-inputenhed for at vise indgående noter på klaviaturet
- Justerbar synthesizervolumen (master gain, 10%--200%)
- SoundFont-valg til den indbyggede synthesizer
- MIDI-instrument/programvalg (General MIDI eller SoundFont-presets)
- Chorus/effekt-kontrolknap
- Oktavskift (-3 til +5)
- MIDI-volumenkontrol (CC#7, 0--127)

### Værktøjslinjekontroller

- **Volume**: MIDI-volumen (CC#7), justerbar fra 0 til 127 via spinboksen.
- **Octave**: Skift tastaturets oktav ved hjælp af knapperne **<** og **>** eller spinboksen. Området er -3 til +5.
- **Chorus/Effect**: En drejeknap og et tekstfelt (1--127) til at indstille chorus/effektniveauet (MIDI CC#93). Klik på **Apply** for at sende værdien.
- **Instrumentvælger**: Vælg et MIDI-instrument. Når den indbyggede FluidSynth-synthesizer bruges, viser den SoundFont-presets (Bank:Program Navn). Når den er forbundet til en ekstern MIDI-enhed, viser den de 128 General MIDI-instrumenter.

### Spil med computerens tastatur

Computerens tastatur er kortlagt til klavertangenter over to oktaver:

**Nedre oktav (starter ved den aktuelle oktav):**

| Tast | Node  |
|------|-------|
| Z    | C     |
| S    | C#/Db |
| X    | D     |
| D    | D#/Eb |
| C    | E     |
| V    | F     |
| G    | F#/Gb |
| B    | G     |
| H    | G#/Ab |
| N    | A     |
| J    | A#/Bb |
| M    | B     |

**Øvre oktav (en oktav højere):**

| Tast | Node  |
|------|-------|
| Q    | C     |
| 2    | C#/Db |
| W    | D     |
| 3    | D#/Eb |
| E    | E     |
| R    | F     |
| 5    | F#/Gb |
| T    | G     |
| 6    | G#/Ab |
| Y    | A     |
| 7    | A#/Bb |
| U    | B     |
| I    | C (næste oktav) |
| 9    | C#/Db |
| O    | D     |
| 0    | D#/Eb |
| P    | E     |

Taster lyser visuelt op, når de trykkes ned (hvide tangenter bliver lyseblå, sorte tangenter mørkner).

### Konfiguration

Åbn konfigurationsdialogen (**Settings > Configuration**, Ctrl+,) for at opsætte MIDI- og lydenheder:

![Virtual MIDI Keyboard konfiguration](../screenshots/VMIDIKeyboard2.png)

#### Fanen MIDI

- **MIDI-outputenhed**: Vælg en ekstern enhed eller lad feltet være tomt for den indbyggede FluidSynth-synthesizer.
- **MIDI-inputenhed**: Vælg en controller til at videresende noter til klaviaturvisningen.
- **Synthesizervolumen (Master Gain)**: Juster outputniveauet for den indbyggede synthesizer (10%--200%).
- **SoundFont**: Gennemse til en `.sf2` SoundFont-fil til den indbyggede synthesizer.
- **Refresh**: Scan efter tilgængelige MIDI-enheder igen.

#### Fanen Lyd

- **Lydoutputenhed**: Vælg outputenheden til den indbyggede synthesizer.

#### Fanen Sprog

- **Sprog**: Vælg visningssproget. De samme sprog som i Musician's Canvas er tilgængelige. Grænsefladen opdateres med det samme, når du ændrer sproget.

### Virtual MIDI Keyboard genveje

| Genvej   | Handling                    |
|----------|-----------------------------|
| Ctrl+,   | Konfigurationsdialog        |
| Ctrl+U   | Hjælp / Brugsinformation    |
| Ctrl+Q   | Luk                         |

## Fejlfinding

### Ingen lydoutput

- Kontrollér, at den korrekte lydoutputenhed er valgt i Settings > Configuration > Audio.
- På Linux, verificér at PipeWire eller PulseAudio kører, og at outputtet ikke er sat på lydløs. Brug `amixer` eller dit skrivebordsmiljøs lydindstillinger til at kontrollere lydstyrkeniveauer.
- På Windows, sørg for at en ASIO-lyddriver er installeret (f.eks. [ASIO4ALL](https://asio4all.org/) eller en producentleveret ASIO-driver til dit lydinterface). Musician's Canvas kræver ASIO til lyd med lav latenstid på Windows.

### MIDI-spor er tavse

- Sørg for, at en SoundFont (`.sf2`-fil) er konfigureret i Settings > Configuration > MIDI.
- På Linux kan en system-SoundFont blive automatisk detekteret, hvis pakken `fluid-soundfont-gm` er installeret.
- På Windows og macOS skal du konfigurere SoundFont-stien manuelt.

### Optagelse lyder forvrænget eller har forkert tonehøjde

- Dette kan ske, når lydinputenhedens native samplerate adskiller sig fra projektets konfigurerede rate. Musician's Canvas håndterer dette automatisk via resampling, men hvis problemerne fortsætter, prøv at indstille projektets samplerate til at matche enhedens native rate.
- USB-webcam-mikrofoner har ofte usædvanlige native rater (f.eks. 32000 Hz). Applikationen detekterer disse automatisk.
- Hvis du bruger Qt Multimedia-backenden og oplever problemer, prøv at skifte til PortAudio-backenden i Project Settings > Audio.

### Virtual MIDI Keyboard har ingen lyd

- På Linux med PipeWire, sørg for at pakken `libpipewire-0.3-dev` er installeret (nødvendig for PipeWire-integration med FluidSynth-synthesizeren).
- Kontrollér, at en SoundFont er indlæst (se MIDI-fanen i konfigurationsdialogen).
- Verificér, at lydoutputenheden er valgt, og at systemets lydstyrke ikke er sat på lydløs.

## Bygning fra kildekode

Se [README](../README.md) for komplet byggevejledning til Linux, macOS og Windows, inklusive alle nødvendige afhængigheder.
