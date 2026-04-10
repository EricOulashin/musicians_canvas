# Musician's Canvas

Dette er en flerspors musikkinnspillingsapplikasjon for stasjonære PC-er. Den er skrevet i C++ og bruker Qt som
GUI-bibliotek. Denne applikasjonen er ment å bygges for Linux, Windows og Mac OS. Brukervennlighet er en
viktig designvurdering. Dette prosjektet startet som et eksperiment med Cursor (AI-kodingsverktøy) for å se hva
det ville være i stand til å lage som en applikasjon av denne typen, med fortsatt bruk av AI-utvikling samt
tradisjonell programvareutvikling.

For mange år siden laget jeg et flerspors musikkinnspillingsprogram på universitetet (med det samme navnet); dette er et forsøk
på å lage noe bedre.

## Funksjoner

- **Flersporarrangement**: Legg til og fjern flere spor for å komponere sanger
- **Navngitte spor**: Hvert spor har et redigerbart navn som brukes som filnavn for innspilt lyd
- **Sportyper**: Konfigurer hvert spor for lydopptak (fra mikrofon/linjeinngang) eller MIDI-opptak (fra keyboard/kontroller); klikk på sportypeikonet for raskt å endre inngangskilde
- **Sporarmering**: Kryss av "Arm" på et spor for å velge det som opptaksmål; bare ett spor kan være armert om gangen. Et enkelt uinnspilt spor armeres automatisk for bekvemmelighet
- **Opptaksnedtelling**: En visuell 3-sekunders nedtelling før opptaket begynner, som gir utøveren tid til å forberede seg
- **Overdub-opptak**: Når du tar opp et nytt spor mens eksisterende spor er aktivert, mikses og spilles de eksisterende sporene av i sanntid slik at du kan høre dem mens du tar opp. Avspilling og opptak er synkronisert for å holde alle spor justert
- **Visuell tilbakemelding**: Lydbølgeformvisning for lydspor (med live-nivåmåler under opptak), MIDI-pianorull for MIDI-spor
- **Innebygd MIDI-synthesizer**: Renderer MIDI-spor til lyd ved hjelp av FluidSynth med en konfigurerbar SoundFont
- **Miks til enkelt lydfil**: Eksporter alle aktiverte spor til en enkelt mikset WAV- eller FLAC-fil ved hjelp av [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-biblioteket
- **Lagre / Åpne prosjekt**: Serialiser og gjenopprett hele prosjektet (spor, navn, typer, MIDI-noter, lydfilreferanser) til/fra en JSON-fil, med deteksjon av ulagrede endringer ved avslutning
- **Prosjektspesifikke innstillinger**: Overstyr globale MIDI- og lydstandardinnstillinger per prosjekt (samplingsfrekvens, SoundFont, MIDI-enhet)
- **PortAudio-opptak (valgfritt bygg)**: Når prosjektet bygges med PortAudio (`HAVE_PORTAUDIO`), kan opptaket bruke en nativ PortAudio-inngangsbane (lignende i ånd som Audacity) i stedet for Qt Multimedia. **Project → Project Settings → Audio** lar deg velge **PortAudio** eller **Qt Multimedia** og velge en PortAudio-inngangsenhet. Hvis PortAudio ikke er installert, lykkes bygget likevel, og opptaket bruker bare Qt Multimedia.
- **Høykvalitets samplingsfrekvenskonvertering**: Tar opp med lydenhetens native frekvens og konverterer til prosjektfrekvensen ved hjelp av vindusert sinc-interpolasjon (Kaiser-vindu, ~96 dB stoppbåndsdemping), den samme algoritmefamilien som brukes av Audacity / libsoxr. Dette muliggjør opptak med hvilken som helst prosjektsamplingsfrekvens (8000 Hz til 192000 Hz) uavhengig av enhetens native frekvens, uten tonehøyde- eller varighetsendring.
- **Automatisk mono/stereo-håndtering**: Oppdager fysisk mono-enheter som annonseres som stereo (vanlig med USB-webkameramikrofoner på PipeWire) og konverterer mellom mono og stereo etter behov (duplisering eller gjennomsnittsberegning), i samsvar med Audacitys kanalrutingsmetode
- **Innsettings-effekter ved opptak (lydspor)**: Knappen **Effects** (under **Options** på hvert lydspor) åpner dialogen **Track effects**. Legg til og konfigurer **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion** og **Amp & cabinet** (forsterker- og høyttalermodellering), dra **≡**-grepet for å endre rekkefølgen på kjeden (øverst kjører først), og lagre med prosjektet. Effektene brukes på opptaket når du stopper opptaket; parametre i virkelige enheter (ms, Hz) gir konsistent oppførsel for mono eller stereo og typiske prosjektsamplingsfrekvenser (8 kHz–192 kHz) etter normalisering av opptaket. Se [brukerhåndboken](docs/MusiciansCanvas_User_Manual_nb.md). Oversettere kan også oppdatere strenger i bulk via `scripts/effect_i18n.tsv` (generert av `scripts/build_effect_tsv.py`) og `scripts/fill_effect_i18n.py` etter `lupdate`.
- **Overvåk under opptak**: **Overvåk lyd under opptak** (til høyre for klokken) sender **live-inngang** til **prosjektets lydutgang** under opptak. **Lyd** – det samme som tas opp; **MIDI** – programvaresynth ved MIDI-til-lyd og SoundFont. Lagres i **prosjektet** (`monitorWhileRecording`). Skru av mot tilbakekobling.
- **Lyd med lav latens**: På Windows sikrer ASIO-driverdeteksjon lyd med lav latens; på Linux heves prosessplanleggingsprioriteten for lavere jitter med PipeWire / PulseAudio / ALSA
- **Virtual MIDI Keyboard**: En ledsagende applikasjon for å sende MIDI-noter via et programvarepianokeyboard, med innebygd FluidSynth-synthesizer, justerbar masterforsterkning, datatastatur-til-piano-tilordning, instrument-/programvalg, kor-/effektstyring og oktavskift
- **Konfigurasjon**: Valg av lydinngansenhet, MIDI-enhet og SoundFont-fil (globale standardinnstillinger og prosjektspesifikke overstyringer)
- **Mørkt / lyst tema**: Konfigurerbart via Settings → Configuration
- **Lokalisering**: Begge applikasjonene er lokalisert på 18 språk: engelsk, tysk, spansk, fransk, japansk, portugisisk (Brasil), tradisjonell kinesisk, forenklet kinesisk, russisk, svensk, finsk, dansk, norsk, polsk, gresk, irsk, walisisk og pirat. Språket er som standard satt etter operativsystemets innstilling og kan endres i **Settings → Configuration → Language**. Qts standard i18n-system (`QTranslator` / `.ts` / `.qm`) brukes, og brukergrensesnittet oppdateres umiddelbart når språket endres.
- **Brukerhåndbok**: HTML- og PDF-dokumentasjon generert fra Markdown-kilde, tilgjengelig på alle støttede språk (se [docs/](docs/))

## Skjermbilder

<p align="center">
	<a href="screenshots/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/MusiciansCanvas_1_MainWin.png" alt="Hovedvindu" width="800"></a>
	<a href="screenshots/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/MusiciansCanvas_2_TrackConfig.png" alt="Sporkonfigurasjon" width="800"></a>
	<a href="screenshots/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_3_GeneralSettings.png" alt="Generelle innstillinger" width="800"></a>
	<a href="screenshots/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_4_MIDISettings.png" alt="Standard-/generelle MIDI-innstillinger" width="800"></a>
	<a href="screenshots/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_5_AudioSettings.png" alt="Standard-/generelle lydinnstillinger" width="800"></a>
	<a href="screenshots/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_DisplaySettings.png" alt="Visningsinnstillinger" width="800"></a>
	<a href="screenshots/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_LanguageSettings.png" alt="Språkinnstillinger" width="800"></a>
	<a href="screenshots/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Prosjektspesifikke MIDI-innstillinger" width="800"></a>
	<a href="screenshots/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Prosjektspesifikke lydinnstillinger" width="800"></a>
	<a href="screenshots/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/Track_Effects_Dialog.png" alt="Spor-effekter" width="800"></a>
	<a href="screenshots/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Avhengigheter

### Kompilator

En **C++17**-kompatibel kompilator er påkrevd (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (påkrevd på alle plattformer)

Miksings- og FLAC-eksportfunksjonaliteten avhenger av
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-biblioteket.
Klon det som en søskenkatalog til dette depotet før bygging:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

CMake-bygget forventer å finne det på `../audio_mixer_cpp` relativt til dette prosjektets rotkatalog.

---

### Qt6 Multimedia (valgfritt men sterkt anbefalt)

`qt6-multimedia` er valgfritt. Uten det bygges og kjøres applikasjonen fortsatt, men
**lydopptak og -avspilling er deaktivert** — bare MIDI-redigering og prosjektstyring
vil fungere. Installer det sammen med Qt6-kjernebibliotekene ved hjelp av plattformkommandoene nedenfor.

---

### PortAudio (valgfritt)

Hvis PortAudio-biblioteket og header-filer er installert, aktiverer CMake **`HAVE_PORTAUDIO`** og
linker det inn i **musicians_canvas**. Opptaket bruker deretter som standard PortAudio-banen med mindre
prosjektet er satt til **Qt Multimedia** i **Project → Project Settings → Audio**. Hvis PortAudio
ikke finnes, fortsetter bygget uten det (wrapperen kompileres som stubber).

---

### SoundFont for MIDI-syntese

En SoundFont-fil (`.sf2`) er påkrevd for at MIDI-spor skal produsere lyd. Uten en
vil MIDI-spor bli rendert som stillhet. På **Linux** vil den innebygde FluidSynth-synthesizeren
automatisk oppdage en SoundFont hvis en er installert på en standard systembane
(se pakkenavnene nedenfor). På **macOS og Windows** finnes det ingen standard systembane,
så du må konfigurere SoundFont manuelt i
**Settings → Configuration → MIDI**.

---

### Linux — Ubuntu / Debian

```bash
sudo apt install build-essential cmake \
  qt6-base-dev qt6-multimedia-dev \
  qt6-l10n-tools \
  libfluidsynth-dev librtmidi-dev libflac-dev \
  libportaudio2 portaudio19-dev \
  libpipewire-0.3-dev \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` installerer `FluidR3_GM.sf2` til `/usr/share/sounds/sf2/` og
> oppdages automatisk ved oppstart. `timgm6mb-soundfont` er et mindre alternativ.
>
> `libpipewire-0.3-dev` er påkrevd på PipeWire-baserte systemer slik at Virtual MIDI
> Keyboard kan kalle `pw_init()` før FluidSynth oppretter sine lydressurser.
> Bygget fortsetter uten det; `HAVE_PIPEWIRE`-flagget defineres rett og slett ikke.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` installerer `FluidR3_GM.sf2` til `/usr/share/soundfonts/` og
> oppdages automatisk ved oppstart.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` installerer `FluidR3_GM.sf2` til `/usr/share/soundfonts/` og
> oppdages automatisk ved oppstart. `pipewire` er vanligvis allerede installert på moderne
> Arch-systemer; utviklingsheadere er inkludert i hovedpakken.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac portaudio
```

> PipeWire er et Linux-eksklusivt undersystem og er **ikke** påkrevd på macOS. FluidSynth
> vil automatisk bruke CoreAudio via Qt Multimedia-backend.
>
> Last ned en General MIDI SoundFont (f.eks.
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) eller
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> og konfigurer banen i **Settings → Configuration → MIDI**.

---

### Windows

**musicians_canvas** krever en **ASIO-lyddriver** for opptak og avspilling med lav latens.
Applikasjonen vil ikke starte hvis ingen ASIO-driver oppdages.
**virtual_midi_keyboard** krever ikke ASIO og bruker Qt Multimedias WASAPI-backend direkte.

**Installere en ASIO-driver for musicians_canvas (velg én):**

| Driver | Merknader |
|--------|-----------|
| [ASIO4ALL](https://asio4all.org/) | Gratis, fungerer med de fleste innebygde og USB-lydenheter |
| Produsentdriver | Best latens for dedikerte lydgrensesnitt (f.eks. Focusrite, PreSonus, RME) |

**Verktøykjede — MSYS2 (anbefalt for MinGW-bygg):**

Last ned og installer MSYS2 fra <https://www.msys2.org>, legg deretter til
`C:\msys64\mingw64\bin` i system-`PATH`. Installer de nødvendige pakkene:

```bash
pacman -S mingw-w64-x86_64-qt6-base
pacman -S mingw-w64-x86_64-qt6-multimedia
pacman -S mingw-w64-x86_64-fluidsynth
pacman -S mingw-w64-ucrt-x86_64-rtmidi
pacman -S mingw-w64-x86_64-flac
pacman -S mingw-w64-x86_64-portaudio
pacman -S mingw-w64-x86_64-soundfont-fluid
```

> `mingw-w64-x86_64-soundfont-fluid` installerer `FluidR3_GM.sf2` til
> `C:\msys64\mingw64\share\soundfonts\` (juster hvis MSYS2 er installert et annet sted).
> Siden Windows ikke har en standard SoundFont-søkebane, må du konfigurere denne banen
> manuelt i **Settings → Configuration → MIDI** etter første start.
>
> PipeWire er et Linux-eksklusivt undersystem; ingen PipeWire-pakke er nødvendig på Windows.

Pakkereferansesider:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Verktøykjede — Visual Studio 2022:**

Installer Qt 6 via [Qt Online Installer](https://www.qt.io/download) og skaff
FluidSynth-, RtMidi- og libFLAC-binærfiler (eller bygg dem fra kildekode).
CMake-bygget vil finne dem via `find_library` / `find_path` så lenge de
aktuelle katalogene er i `CMAKE_PREFIX_PATH`.

En SoundFont må lastes ned separat (f.eks.
[GeneralUser GS](https://schristiancollins.com/generaluser.php) eller
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)) og dens
bane konfigureres i **Settings → Configuration → MIDI** etter første start.

> **Merk:** `advapi32` (Windows-registeret) og `winmm` (Windows Multimedia) linkes
> automatisk av CMake; ingen ekstra installasjon er nødvendig for disse.

---

## Bygging

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

Hvis nødvendige DLL-er ikke finnes ved kjøring, kjør hjelpeskriptet fra byggets utdatakatalog:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Generering av dokumentasjon

Brukerhåndboken kan genereres som HTML og PDF fra Markdown-kilden:

```bash
cd docs
./generate_docs.sh          # Generer både HTML og PDF
./generate_docs.sh html     # Generer bare HTML
./generate_docs.sh pdf      # Generer bare PDF
```

**Forutsetninger:**

- **Python (foretrukket):** `pip3 install markdown weasyprint`
- **Alternativ:** `pandoc` og `wkhtmltopdf` (via systempakkebehandler)

Den genererte HTML-en skrives til `docs/html/` og PDF-en til `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Kjøring

```bash
./build/musicians_canvas
# eller
./build/virtual_midi_keyboard   # ledsagende applikasjon Virtual MIDI Keyboard
```

---

## Bruk

### musicians_canvas

1. **Angi prosjektkatalog**: Skriv inn eller bla til en mappe der opptak og prosjektfilen vil bli lagret
2. **Legg til spor**: Klikk på "+ Add Track"; navngi hvert spor i tekstfeltet ved siden av "Options"
3. **Konfigurer sportype**: Klikk på "Options" på et spor (eller klikk på sportypeikonet mellom "Options" og navnefeltet) for å velge Audio eller MIDI og angi inngangskilden
4. **Fjern et spor**: Klikk på "×"-knappen på høyre side av sporraden
5. **Globale innstillinger**: Bruk **Settings → Configuration** for å angi globale standardverdier:
   - Fanen *General*: Tema (mørkt/lyst)
   - Fanen *MIDI*: Standard MIDI-utgangsenhet (innebygd FluidSynth-synthesizer eller en ekstern MIDI-enhet) og standard-SoundFont (`.sf2`) for syntese
   - Fanen *Audio*: Standard lydinngangs-/utgangsenhet for opptak og avspilling
6. **Prosjektinnstillinger**: Bruk **Project → Project Settings** (Ctrl+P) for å overstyre MIDI- og lydinnstillinger bare for det gjeldende prosjektet (f.eks. en annen samplingsfrekvens eller SoundFont per sang)
7. **Ta opp**: Kryss av "Arm" på målsporet, klikk deretter på opptaksknappen (rød sirkel). Bare ett spor kan være armert om gangen
8. **Spill av**: Klikk på avspillingsknappen for å mikse og spille av alle aktiverte spor
9. **Miks til fil**: Bruk **Tools → Mix tracks to file** (Ctrl+M) for å eksportere til WAV eller FLAC
10. **Lagre / Åpne**: Bruk **File → Save Project** (Ctrl+S) og **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Åpne konfigurasjon**: Bruk **Configuration**-knappen eller menyen (Ctrl+,) for å åpne innstillingsdialogen
2. **MIDI-fanen**:
   - Velg en MIDI-utgangsenhet (en ekstern maskinvare-/programvaresynthesizer) eller la den stå tom for å bruke den innebygde FluidSynth-synthesizeren
   - Velg en MIDI-inngangsenhet for å videresende innkommende MIDI-noter til tastaturvisningen
   - Juster **Synthesizer Volume (Master Gain)** for å styre utgangsnivået til den innebygde synthesizeren (10 %–200 %)
3. **Lydfanen**: Velg lydutgangsenheten som brukes av den innebygde synthesizeren
4. **SoundFont**: Velg en `.sf2` SoundFont-fil for den innebygde synthesizeren (autodetekteres på Linux hvis en system-SoundFont er installert)
5. **Spill noter**: Klikk på tangenter på skjermens pianokeyboard, bruk en tilkoblet MIDI-kontroller eller bruk datatastaturet:
   - Nedre oktav: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = kryss/b-fortegn
   - Øvre oktav: Q/W/E/R/T/Y/U/I/O/P = C til E, 2/3/5/6/7/9/0 = kryss/b-fortegn
6. **Verktøylinjekontroller**: Juster MIDI-volum (0–127), oktav (-3 til +5), kor-/effektnivå og velg instrumenter

---

## Tastatursnarveier

**musicians_canvas:**

| Snarvei | Handling |
|---------|----------|
| Ctrl+S | Lagre prosjekt |
| Ctrl+O | Åpne prosjekt |
| Ctrl+M | Miks spor til fil |
| Ctrl+P | Prosjektinnstillinger |
| Ctrl+, | Innstillinger / Konfigurasjon |
| Ctrl+Q / Alt+F4 | Avslutt |

**virtual_midi_keyboard:**

| Snarvei | Handling |
|---------|----------|
| Ctrl+, | Konfigurasjonsdialog |
| Ctrl+U | Hjelp / Bruksinformasjon |
| Ctrl+Q | Lukk |

---

## Lisens

Dette prosjektet tilbys som det er for utdannings- og personlig bruk.
