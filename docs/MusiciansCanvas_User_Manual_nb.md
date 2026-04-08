---
title: "Musician's Canvas Brukerhåndbok"
subtitle: "Flerspors musikkopptaksapplikasjon"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "nb"
---

# Musician's Canvas Brukerhåndbok

## Introduksjon

Musician's Canvas er en flerspors musikkopptaksapplikasjon for stasjonære datamaskiner. Den støtter lydopptak fra mikrofoner og line-in-enheter, MIDI-opptak fra keyboards og kontrollere, og miksing av alle spor til en enkelt WAV- eller FLAC-fil. En følgeapplikasjon, Virtual MIDI Keyboard, gir et programvareklaviatur for sending av MIDI-noter.

Musician's Canvas er designet for å være enkel å bruke, samtidig som den tilbyr funksjoner som ofte finnes i digitale lydarbeidsstasjoner (DAW-er):

- Flerspors lyd- og MIDI-opptak
- Overdub-opptak med synkronisert avspilling av eksisterende spor
- Innebygd FluidSynth MIDI-synthesizer med SoundFont-støtte
- Høykvalitets samplingsfrekvenskonvertering ved opptak med enhver prosjektsamplingsfrekvens
- Automatisk mono/stereo-enhetsdeteksjon
- Prosjektbaserte innstillinger med prosjektspesifikke overstyringer
- Miks til WAV eller FLAC
- Mørke og lyse temaer
- Lokalisert på flere språk (engelsk, tysk, spansk, fransk, japansk, portugisisk, kinesisk og pirat)
- Følgeapplikasjon Virtual MIDI Keyboard

## Kom i gang

### Starte applikasjonen

Kjør den kjørbare filen `musicians_canvas` fra byggemappen eller installasjonsplasseringen din:

```
./musicians_canvas
```

Ved første oppstart åpner applikasjonen med et tomt prosjekt. Du må angi en prosjektmappe før du kan ta opp.

Ved oppstart bruker applikasjonen det lagrede temaet (mørkt eller lyst), og hvis en prosjektmappe tidligere har blitt brukt og inneholder en `project.json`-fil, lastes prosjektet automatisk inn.

### Sette opp et prosjekt

1. **Angi prosjektmappen**: Skriv inn eller bla til en mappe i feltet "Project Location" øverst i vinduet. Det er her opptak og prosjektfilen lagres.

2. **Legg til et spor**: Klikk på knappen **+ Add Track**. Et nytt spor vises med standardinnstillinger. Hvis det er det eneste sporet i prosjektet og ennå ikke er tatt opp, armeres det automatisk for opptak.

3. **Navngi sporet**: Skriv et navn i tekstfeltet ved siden av knappen "Options". Dette navnet brukes som filnavn for den innspilte lydfilen.

![Hovedvindu](../screenshots/MusiciansCanvas_1_MainWin.png)

### Knapperad

Like under menylinjen er det en verktøylinje med hurtigtilgangsknapper:

![Knapperad](../screenshots/ButtonBar.png)

- **Open Project**: Samme som **File > Open Project** — åpner et tidligere lagret prosjekt.
- **Save Project**: Samme som **File > Save Project** — lagrer gjeldende prosjekt. Denne
  knappen er bare aktivert når en prosjektmappe er angitt.
- **Project Settings**: Samme som **Project > Project Settings** — åpner dialogen for
  prosjektinnstillinger. Denne knappen er bare aktivert når en prosjektmappe er angitt.
- **Configuration**: Samme som **Settings > Configuration** — åpner dialogen for globale
  applikasjonsinnstillinger.
- **Metronome Settings**: Åpner dialogen for metronominnstillinger (se avsnittet Metronom nedenfor).

### Lagre og åpne prosjekter

- **Lagre**: Bruk **File > Save Project** (Ctrl+S) for å lagre det gjeldende prosjektet som en JSON-fil i prosjektmappen.
- **Åpne**: Bruk **File > Open Project** (Ctrl+O) for å laste inn et tidligere lagret prosjekt.

Prosjektfilen (`project.json`) lagrer spornavn, typer, MIDI-noter, lydfilreferanser og alle prosjektspesifikke innstillinger. Lydfiler lagres i samme mappe som `project.json` og navngis etter sporene sine (f.eks. `My_Track.flac`).

Hvis du lukker applikasjonen med ulagrede endringer, spør en bekreftelsesdialog om du vil lagre før du avslutter.

## Sporhåndtering

### Legge til og fjerne spor

- Klikk på **+ Add Track** for å legge til et nytt spor i arrangementet.
- Klikk på **x**-knappen på høyre side av en sporrad for å fjerne det.
- Klikk på **Clear Tracks** (den røde knappen i verktøylinjen) for å fjerne alle spor. En bekreftelsesdialog vises før handlingen utføres.

### Legge til spor ved dra og slipp

Mens et prosjekt er åpent, kan du dra én eller flere støttede lydfiler fra
filutforskeren din (Windows Utforsker, macOS Finder, Linux-filbehandler osv.)
direkte til Musician's Canvas-vinduet for å legge dem til som nye lydspor.

- **Støttede formater:** `.wav` og `.flac`. Filer i andre formater hoppes
  stille over, og en dialog på slutten viser hvilke filer som ble hoppet
  over.
- **Filkopiering:** Hvis den slupne filen ikke allerede er i prosjektmappen,
  kopieres den dit automatisk. Hvis en fil med samme navn allerede finnes i
  prosjektmappen, blir du spurt om du vil erstatte den.
- **Spornavn:** Filens basisnavn (uten filendelsen) brukes som navn på det
  nye sporet. For eksempel oppretter det å slippe `Bass Line.wav` et lydspor
  med navnet "Bass Line".
- **Flere filer samtidig:** Flere filer kan dras samtidig; hver støttede fil
  blir sitt eget spor i ett enkelt slipp.
- **Når slipp avvises:** Slipp godtas bare mens et prosjekt er åpent og
  Musician's Canvas **ikke** spiller av eller spiller inn. Stopp avspilling
  eller innspilling først hvis du vil dra inn flere spor.

### Konfigurere sportype

Hvert spor kan konfigureres som enten **Audio** (for mikrofon/line-in-opptak) eller **MIDI** (for keyboard/kontroller-opptak).

For å endre sportypen:

- Klikk på knappen **Options** på sporet, eller
- Klikk på **sportypeikonet** (mellom "Options" og navnefeltet)

Dette åpner dialogen Sporkonfigurasjon, der du kan velge inndatakilden.

![Sporkonfigurasjon](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Sporkontroller

Hver sporrad har følgende kontroller:

- **Aktiver avkrysningsboks**: Slår av/på om sporet er inkludert i avspilling og miksing. Deaktivering av et spor avarmerer det også automatisk hvis det var armert.
- **Armer radioknapp**: Velger dette sporet som opptaksmål. Bare ett spor kan armeres om gangen; armering av et nytt spor avarmerer automatisk ethvert tidligere armert spor.
- **Navnefelt**: Redigerbart tekstfelt for spornavnet. Dette navnet brukes som lydfilnavn (med ugyldige filsystemtegn erstattet av understreker).
- **Options-knapp**: Åpner dialogen Sporkonfigurasjon.
- **Typeikon**: Viser et høyttalerikon for lydspor eller et pianoikon for MIDI-spor. Klikk på det åpner dialogen Sporkonfigurasjon.
- **Fjern-knapp (x)**: Fjerner sporet fra prosjektet.

### Automatisk armering

Når et prosjekt har nøyaktig ett spor og det sporet ennå ikke er tatt opp, armeres det automatisk for opptak. Dette gjelder både når det første sporet legges til i et nytt prosjekt, og når et eksisterende prosjekt med et enkelt tomt spor åpnes.

### Sporvisualisering

- **Lydspor** viser en bølgeformvisualisering av det innspilte lydsignalet. Når ingen lyd er tatt opp, viser området "No audio recorded".
- **MIDI-spor** viser en pianorull-visualisering som viser innspilte noter på et rutenett som spenner fra A0 til C8. Noter er farget etter velocity. Når ingen MIDI-data er tatt opp, viser området "No MIDI data recorded".

## Opptak

### Lydopptak

1. Sørg for at prosjektmappen er angitt.
2. Armér målsporet (merk radioknappen "Arm").
3. Klikk på knappen **Record** (rød sirkel).
4. En 3-sekunders nedtelling vises på sporet ("Get ready... 3", "2", "1"), deretter begynner opptaket.
5. Under opptak vises en sanntids nivåmåler i sporets bølgeformområde, som viser gjeldende amplitude som en gradientlinje (grønn til gul til rød) med etiketten "Recording".
6. Klikk på knappen **Stop** for å avslutte opptaket.

Det innspilte lydsignalet lagres som en FLAC-fil i prosjektmappen, navngitt etter sporet.

Under opptak og avspilling deaktiveres alle interaktive kontroller (sporknapper, innstillinger osv.) for å forhindre utilsiktede endringer.

#### Overdub-opptak

Når du tar opp et nytt spor mens andre aktiverte spor allerede inneholder lyd- eller MIDI-data, utfører Musician's Canvas overdub-opptak: de eksisterende sporene mikses sammen og spilles av i sanntid mens det nye sporet tas opp. Dette lar deg høre tidligere innspilte deler mens du legger inn en ny del.

Miksen av eksisterende spor forberedes før opptaket begynner, slik at opptak og avspilling starter på omtrent samme tidspunkt, noe som holder alle spor synkroniserte.

#### Opptaksbackender

Musician's Canvas støtter to lydopptaksbackender:

- **PortAudio** (standard når tilgjengelig): Gir pålitelig opptak med lav latenstid og er den anbefalte backenden.
- **Qt Multimedia**: En reservebackend som bruker Qts innebygde lydopptak. Brukes når PortAudio ikke er tilgjengelig, eller når den eksplisitt velges i Prosjektinnstillinger.

Opptaksbackenden kan konfigureres per prosjekt i **Project > Project Settings > Audio**.

#### Samplingsfrekvens og enhetshåndtering

Musician's Canvas tar opp med lydinndataenhetens opprinnelige samplingsfrekvens og konverterer deretter automatisk til prosjektets konfigurerte samplingsfrekvens ved hjelp av høykvalitets resampling. Dette betyr at du kan angi hvilken som helst prosjektsamplingsfrekvens (f.eks. 44100 Hz eller 48000 Hz) uavhengig av enhetens opprinnelige frekvens. Konverteringen bevarer tonehøyde og varighet nøyaktig.

#### Mono-enhetsdeteksjon

Noen lydenheter (f.eks. USB-webkameramikrofoner) er fysisk mono, men annonseres som stereo av operativsystemet. Musician's Canvas oppdager dette automatisk og justerer kanalantallet tilsvarende. Hvis prosjektet er konfigurert for stereo, dupliseres monosignalet til begge kanaler.

### MIDI-opptak

1. Sett sportypen til **MIDI** via Options-knappen.
2. Sørg for at en MIDI-inndataenhet er konfigurert i **Settings > Configuration > MIDI**.
3. Armér sporet og klikk på Record.
4. Spill noter på MIDI-kontrolleren din.
5. Klikk på Stop for å avslutte opptaket.

MIDI-noter vises i en pianorull-visualisering på sporet.

## Metronom

Musician's Canvas inkluderer en innebygd metronom som kan brukes under opptak for å
hjelpe deg å holde takten. Klikk på metronomknappen på knapperaden (under menylinjen) for å
åpne dialogen for metronominnstillinger:

![Metronominnstillinger](../screenshots/MetronomeSettings.png)

Dialogen tilbyr:

- **Enable metronome during recording**: Når dette er avkrysset, spiller metronomen en
  tikkelyd mens opptaket er aktivt. Tikket spilles gjennom systemlyden og **tas ikke opp**
  på det innspilte sporet.
- **Beats per minute**: En numerisk inndata for tempoet, i slag per minutt (BPM). Området
  er 20–300 BPM.

Når metronomen er aktivert, begynner den å tikke når opptaket faktisk starter (etter at
3-sekunders nedtellingen er fullført), og stopper når opptaket avsluttes.

## Avspilling

Klikk på knappen **Play** for å mikse og spille av alle aktiverte spor. Knappens verktøytips endres for å indikere om den vil spille eller ta opp, basert på om et spor er armert. Deaktiverte spor (ikke avkrysset) ekskluderes fra avspillingen.

Under avspilling dekodes lydspor fra FLAC-filene sine, og MIDI-spor gjengis til lyd ved hjelp av den innebygde FluidSynth-synthesizeren. Alle spor mikses sammen og spilles gjennom systemets lydutdataenhet.

Klikk på knappen **Stop** for å avslutte avspillingen når som helst.

## Miksing til en fil

Bruk **Tools > Mix tracks to file** (Ctrl+M) for å eksportere alle aktiverte spor til en enkelt lydfil. En dialog lar deg velge utdatabanen og formatet:

- **Utdatafil**: Bla for å velge destinasjonsfilbanen.
- **Format**: Velg mellom FLAC (tapsfri komprimering, mindre filer) eller WAV (ukomprimert).

Miksen bruker prosjektets konfigurerte samplingsfrekvens. MIDI-spor gjengis ved hjelp av den konfigurerte SoundFont.

## Innstillinger

### Globale innstillinger

Bruk **Settings > Configuration** (Ctrl+,) for å angi globale standarder som gjelder for alle prosjekter:

![Generelle innstillinger](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### Fanen Generelt

- **Tema**: Velg mellom mørkt og lyst tema.

#### Fanen Visning

- **Farge på numerisk LED-display**: Velg fargen som brukes for det numeriske LED-tidsdisplayet som vises på verktøylinjen i hovedvinduet. De aktive sifferssegmentene tegnes i den valgte fargen, og inaktive segmenter tegnes som en nedtonet versjon av samme farge. Tilgjengelige farger er Light Red, Dark Red, Light Green, Dark Green, Light Blue, Dark Blue, Yellow, Orange, Light Cyan og Dark Cyan. Standard er Light Green.

![Visningsinnstillinger](../screenshots/MusiciansCanvas_DisplaySettings.png)

#### Fanen Språk

- **Språk**: Velg visningsspråket for applikasjonen. Standard er "System Default", som bruker operativsystemets språkinnstilling. Tilgjengelige språk er engelsk, Deutsch (tysk), Español (spansk), Français (fransk), japansk, Português (brasiliansk portugisisk), kinesisk (tradisjonelt), kinesisk (forenklet) og pirat. Grensesnittet oppdateres umiddelbart når du endrer språket.

![Språkinnstillinger](../screenshots/MusiciansCanvas_LanguageSettings.png)

#### Fanen MIDI

- **MIDI-utdataenhet**: Velg den innebygde FluidSynth-synthesizeren eller en ekstern MIDI-enhet. Bruk knappen **Refresh** for å skanne etter tilgjengelige MIDI-enheter på nytt.
- **SoundFont**: Bla til en `.sf2` SoundFont-fil for MIDI-syntese. På Linux kan en system-SoundFont bli automatisk oppdaget hvis pakken `fluid-soundfont-gm` er installert. På Windows og macOS må du konfigurere SoundFont-banen manuelt.

![MIDI-innstillinger](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Fanen Lyd

- **Lydinndataenhet**: Velg mikrofonen eller line-in-enheten for opptak.
- **Lydutdataenhet**: Velg høyttaleren eller hodetelefonenheten for avspilling.

![Lydinnstillinger](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Prosjektinnstillinger

Bruk **Project > Project Settings** (Ctrl+P) for å overstyre globale standarder kun for det gjeldende prosjektet. Dette er nyttig for prosjekter som trenger en bestemt samplingsfrekvens, SoundFont eller lydenhet. Prosjektspesifikke innstillinger lagres i `project.json`-filen.

![Prosjekt-MIDI-innstillinger](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### Fanen MIDI

- **MIDI-enhet**: Velg en MIDI-enhet for dette prosjektet, eller la den stå på standard for å bruke den globale innstillingen.
- **SoundFont**: Velg en SoundFont-fil for dette prosjektet.
- **Refresh**: Skann etter tilgjengelige MIDI-enheter på nytt.

#### Fanen Lyd

- **Lydinndataenhet**: Velg opptaksenheten for dette prosjektet.
- **Opptaks-capture-backend** (når PortAudio er tilgjengelig):
  - **PortAudio (native input)**: Anbefalt. Bruker det samme lydbiblioteket som Audacity.
  - **Qt Multimedia**: Reservevalg som bruker Qts innebygde lydopptak.
- **PortAudio-inndataenhet**: Når PortAudio-backenden brukes, velg den spesifikke PortAudio-inndataenheten.
- **Lydutdataenhet**: Velg avspillingsenheten for dette prosjektet.

##### Lydformatinnstillinger

- **Samplingsfrekvens**: Velg blant standardfrekvenser (8000 Hz til 192000 Hz). Enhetens opprinnelige frekvens er merket "(native)". Frekvenser som krever resampling er merket "(resampled)". Du kan velge hvilken som helst frekvens uavhengig av enhetens kapabilitet; Musician's Canvas vil automatisk resample etter behov.
- **Kanaler**: Mono eller Stereo. Hvis enheten bare støtter mono, er Stereo-alternativet deaktivert.

![Prosjekt-lydinnstillinger](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

## Menyer

### Fil-menyen

| Menypunkt     | Snarvei  | Beskrivelse                          |
|---------------|----------|--------------------------------------|
| Save Project  | Ctrl+S   | Lagre det gjeldende prosjektet til disk |
| Open Project  | Ctrl+O   | Åpne en eksisterende prosjektfil     |
| Close         | Ctrl+Q   | Avslutte applikasjonen               |

### Prosjekt-menyen

| Menypunkt                           | Snarvei  | Beskrivelse                                     |
|-------------------------------------|----------|-------------------------------------------------|
| Project Settings                    | Ctrl+P   | Konfigurere prosjektspesifikke innstillinger    |
| Add Demo Data to Selected Track     |          | Legge til eksempel-MIDI-noter for demonstrasjon |

### Innstillingsmenyen

| Menypunkt     | Snarvei  | Beskrivelse                                |
|---------------|----------|--------------------------------------------|
| Configuration | Ctrl+,   | Åpne globale applikasjonsinnstillinger     |

### Verktøymenyen

| Menypunkt             | Snarvei  | Beskrivelse                                     |
|-----------------------|----------|-------------------------------------------------|
| Mix tracks to file    | Ctrl+M   | Eksportere alle aktiverte spor til en fil       |
| Virtual MIDI Keyboard |          | Starte følgeapplikasjonen for keyboard          |

## Tastatursnarveier

| Snarvei         | Handling                          |
|-----------------|-----------------------------------|
| Ctrl+S          | Lagre prosjekt                    |
| Ctrl+O          | Åpne prosjekt                     |
| Ctrl+M          | Mikse spor til fil                |
| Ctrl+P          | Prosjektinnstillinger             |
| Ctrl+,          | Innstillinger / Konfigurasjon     |
| Ctrl+Q / Alt+F4 | Avslutte                         |

## Virtual MIDI Keyboard

Virtual MIDI Keyboard er en følgeapplikasjon (`virtual_midi_keyboard`) som gir et skjermklaviatur for sending av MIDI-noter. Den kan startes fra menyen **Tools > Virtual MIDI Keyboard** i hovedapplikasjonen, eller kjøres uavhengig.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Funksjoner

- Klikk på tangenter på skjermklaviaturet for å spille noter
- Bruk datamaskinens tastatur som et klaviatur (se tastaturkartlegging nedenfor)
- Koble til eksterne MIDI-utdataenheter eller bruk den innebygde FluidSynth-synthesizeren
- Koble til en MIDI-inndataenhet for å vise innkommende noter på klaviaturet
- Justerbart synthesizervolum (master gain, 10%--200%)
- SoundFont-valg for den innebygde synthesizeren
- MIDI-instrument/programvalg (General MIDI eller SoundFont-presets)
- Chorus/effekt-kontrollknapp
- Oktavskift (-3 til +5)
- MIDI-volumkontroll (CC#7, 0--127)

### Verktøylinjekontroller

- **Volume**: MIDI-volum (CC#7), justerbart fra 0 til 127 via spinnboksen.
- **Octave**: Skift tastaturets oktav ved hjelp av knappene **<** og **>** eller spinnboksen. Området er -3 til +5.
- **Chorus/Effect**: En dreieknapp og et tekstfelt (1--127) for å stille inn chorus/effektnivået (MIDI CC#93). Klikk på **Apply** for å sende verdien.
- **MIDI-kanal**: Et numerisk inntastingsfelt (1--16) velger MIDI-kanalen som noter, programendringer og kontrollendringer sendes på. Standard er kanal 1. Den valgte kanalen trer i kraft umiddelbart etter endring. Hvis du velger et SoundFont-preset mens en bestemt kanal er valgt, brukes den bank/program på den kanalen, slik at du kan spille forskjellige lyder på forskjellige kanaler.
- **Trommer**: En avmerkingsboks rett over MIDI-kanalvelgeren. Når den ikke er merket av (standard), er MIDI-kanalen den brukeren har valgt. Når den er merket av, byttes MIDI-kanalen automatisk til kanal 10 — General MIDIs standard trommekanal — slik at alt som spilles, høres ut som perkusjon. Når boksen fjernes, gjenopprettes kanalen som var valgt før Trommer ble aktivert.
- **Instrumentvelger**: Velg et MIDI-instrument. Når den innebygde FluidSynth-synthesizeren brukes, viser den SoundFont-presets (Bank:Program Navn). Når den er koblet til en ekstern MIDI-enhet, viser den de 128 General MIDI-instrumentene.

### Spille med datamaskinens tastatur

Datamaskinens tastatur er kartlagt til pianotangenter over to oktaver:

**Nedre oktav (starter ved gjeldende oktav):**

| Tast | Note  |
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

**Øvre oktav (en oktav høyere):**

| Tast | Note  |
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
| I    | C (neste oktav) |
| 9    | C#/Db |
| O    | D     |
| 0    | D#/Eb |
| P    | E     |

Tangenter lyser opp visuelt når de trykkes (hvite tangenter blir lyseblå, svarte tangenter mørkner).

### Konfigurasjon

Åpne konfigurasjonsdialogen (**Settings > Configuration**, Ctrl+,) for å sette opp MIDI- og lydenheter:

![Virtual MIDI Keyboard konfigurasjon](../screenshots/VMIDIKeyboard2.png)

#### Fanen MIDI

- **MIDI-utdataenhet**: Velg en ekstern enhet eller la feltet stå tomt for den innebygde FluidSynth-synthesizeren.
- **MIDI-inndataenhet**: Velg en kontroller for å videresende noter til klaviaturvisningen.
- **Synthesizervolum (Master Gain)**: Juster utdatanivået for den innebygde synthesizeren (10%--200%).
- **SoundFont**: Bla til en `.sf2` SoundFont-fil for den innebygde synthesizeren.
- **Refresh**: Skann etter tilgjengelige MIDI-enheter på nytt.

#### Fanen Lyd

- **Lydutdataenhet**: Velg utdataenheten for den innebygde synthesizeren.

#### Fanen Språk

- **Språk**: Velg visningsspråket. De samme språkene som i Musician's Canvas er tilgjengelige. Grensesnittet oppdateres umiddelbart når du endrer språket.

### Virtual MIDI Keyboard snarveier

| Snarvei  | Handling                     |
|----------|------------------------------|
| Ctrl+,   | Konfigurasjonsdialog         |
| Ctrl+U   | Hjelp / Bruksinformasjon     |
| Ctrl+Q   | Lukke                        |

## Feilsøking

### Ingen lydutdata

- Sjekk at riktig lydutdataenhet er valgt i Settings > Configuration > Audio.
- På Linux, verifiser at PipeWire eller PulseAudio kjører og at utdataen ikke er dempet. Bruk `amixer` eller skrivebordsmiljøets lydinnstillinger for å sjekke volumnivåer.
- På Windows, sørg for at en ASIO-lyddriver er installert (f.eks. [ASIO4ALL](https://asio4all.org/) eller en produsentlevert ASIO-driver for lydgrensesnittet ditt). Musician's Canvas krever ASIO for lav-latens lyd på Windows.

### MIDI-spor er stille

- Sørg for at en SoundFont (`.sf2`-fil) er konfigurert i Settings > Configuration > MIDI.
- På Linux kan en system-SoundFont bli automatisk oppdaget hvis pakken `fluid-soundfont-gm` er installert.
- På Windows og macOS må du konfigurere SoundFont-banen manuelt.

### Opptak høres forvrengt ut eller har feil tonehøyde

- Dette kan skje når lydinndataenhetens opprinnelige samplingsfrekvens avviker fra prosjektets konfigurerte frekvens. Musician's Canvas håndterer dette automatisk via resampling, men hvis problemene vedvarer, prøv å sette prosjektets samplingsfrekvens til å matche enhetens opprinnelige frekvens.
- USB-webkameramikrofoner har ofte uvanlige opprinnelige frekvenser (f.eks. 32000 Hz). Applikasjonen oppdager disse automatisk.
- Hvis du bruker Qt Multimedia-backenden og opplever problemer, prøv å bytte til PortAudio-backenden i Project Settings > Audio.

### Virtual MIDI Keyboard har ingen lyd

- På Linux med PipeWire, sørg for at pakken `libpipewire-0.3-dev` er installert (nødvendig for PipeWire-integrasjon med FluidSynth-synthesizeren).
- Sjekk at en SoundFont er lastet (se MIDI-fanen i konfigurasjonsdialogen).
- Verifiser at lydutdataenheten er valgt og at systemvolumet ikke er dempet.

## Bygge fra kildekode

Se [README](../README.md) for fullstendige byggeinstruksjoner for Linux, macOS og Windows, inkludert alle nødvendige avhengigheter.
