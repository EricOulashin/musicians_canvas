# Musician's Canvas

Dette er en flersporsprogramm til musikoptagelse til stationære computere. Det er skrevet i C++ og bruger Qt som
GUI-bibliotek. Denne applikation er beregnet til at blive bygget til Linux, Windows og Mac OS. Brugervenlighed er en
vigtig designovervejelse. Dette projekt startede som et eksperiment med Cursor & Claude (AI-kodningsværktøjer) for at se, hvad
det ville være i stand til at skabe som en applikation af denne type, med fortsat brug af AI-udvikling samt
traditionel softwareudvikling.

For mange år siden lavede jeg et flerspors musikoptagelsesprogram på universitetet (med det samme navn); dette er et forsøg
på at lave noget bedre.

## Funktioner

- **Flersporsarrangement**: Tilføj og fjern flere spor for at komponere sange
- **Navngivne spor**: Hvert spor har et redigerbart navn, der bruges som filnavn for optaget lyd
- **Sportyper**: Konfigurer hvert spor til lydoptagelse (fra mikrofon/linjeinput) eller MIDI-optagelse (fra keyboard/controller); klik på sportypeikonet for hurtigt at ændre inputkilde
- **Sporarmering**: Sæt hak ved "Arm" på et spor for at vælge det som optagelsesmål; kun ét spor kan være armeret ad gangen. Et enkelt uoptaget spor armeres automatisk for nemheds skyld
- **Optagelsesnedtælling**: En visuel 3-sekunders nedtælling før optagelsen begynder, der giver udøveren tid til at forberede sig
- **Overdub-optagelse**: Når du optager et nyt spor, mens eksisterende spor er aktiveret, mikses og afspilles de eksisterende spor i realtid, så du kan høre dem, mens du optager. Afspilning og optagelse er synkroniseret for at holde alle spor justeret
- **Visuel feedback**: Lydbølgeformsvisning for lydspor (med live-niveaumåler under optagelse), MIDI-pianorulle for MIDI-spor
- **Indbygget MIDI-synthesizer**: Renderer MIDI-spor til lyd ved hjælp af FluidSynth med en konfigurerbar SoundFont
- **Miks til enkelt lydfil**: Eksporter alle aktiverede spor til én mikset fil (WAV, FLAC, MP3, Ogg Vorbis eller AIFF) med [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). Kodning bruger de samme klasser som søsterprojektet (`MP3File`, `OggFile`, `AiffFile`) og **libsndfile**. Ved destination **MP3** eller **Ogg Vorbis** vises **Encoding** til bitrate og kvalitet (MP3 CBR/ABR/VBR og Ogg kvalitet eller ca. nominel bitrate); **WAV**, **FLAC** og **AIFF** har ikke disse valg.
- **Gem / Åbn projekt**: Serialiser og gendan det fulde projekt (spor, navne, typer, MIDI-noder, lydfil-referencer) til/fra en JSON-fil med detektion af ugemte ændringer ved afslutning
- **Projektspecifikke indstillinger**: Tilsidesæt globale MIDI- og lydstandardindstillinger per projekt (samplerate, SoundFont, MIDI-enhed)
- **PortAudio-optagelse (valgfrit build)**: Når projektet bygges med PortAudio (`HAVE_PORTAUDIO`), kan optagelsen bruge en nativ PortAudio-inputsti (lignende i ånd som Audacity) i stedet for Qt Multimedia. **Project → Project Settings → Audio** lader dig vælge **PortAudio** eller **Qt Multimedia** og vælge en PortAudio-inputenhed. Hvis PortAudio ikke er installeret, lykkes bygningen stadig, og optagelsen bruger kun Qt Multimedia.
- **Højkvalitets sampleratekonvertering**: Optager med lydenhedes native rate og konverterer til projektraten ved hjælp af fønstret sinc-interpolation (Kaiser-vindue, ~96 dB stopbåndsdæmpning), den samme algoritmefamilie som bruges af Audacity / libsoxr. Dette muliggør optagelse med enhver projektsamplerate (8000 Hz til 192000 Hz) uanset enhedens native rate, uden tonehøjde- eller varighedsændring.
- **Automatisk mono/stereo-håndtering**: Detekterer fysisk mono-enheder, der annonceres som stereo (almindeligt med USB-webcam-mikrofoner på PipeWire), og konverterer mellem mono og stereo efter behov (duplikering eller gennemsnitsdannelse), i overensstemmelse med Audacitys kanalrutningstilgang
- **Indsætnings-effekter ved optagelse (lydspor)**: Knappen **Effects** (under **Options** på hvert lydspor) åbner dialogen **Track effects**. Tilføj og konfigurer **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion** og **Amp & cabinet** (forstærker- og kabinetmodellering), træk **≡**-grebet for at omordne kæden (øverst kører først), og gem med projektet. Effekterne anvendes på tagningen, når du stopper optagelsen; parametre i rigtige enheder (ms, Hz) giver ensartet adfærd for mono eller stereo og typiske projektsamplingsfrekvenser (8 kHz–192 kHz) efter normalisering af optagelsen. Se [brugermanualen](docs/MusiciansCanvas_User_Manual_da.md). Oversættere kan masseopdatere strenge via `scripts/effect_i18n.tsv` (genereres af `scripts/build_effect_tsv.py`) og `scripts/fill_effect_i18n.py` efter `lupdate`.
- **Mix-effekter (hovedbus)**: **Project → Project Settings → Mix Effects** konfigurerer en kæde på **hele mixet** (samme typer og rækkefølge som på spor). Anvendes ved **afspilning af alle spor** og **eksport af mix** (WAV/FLAC). Gemmes i `project.json` som `mixEffectChain`.
- **Blød begrænsning før 16-bit PCM**: Integreret DSP mindsker hård digital clipping. **EffectWidget** har `guardFloatSampleForInt16Pcm()` og `softLimitFloatSampleForInt16Pcm()`.
- **Lyt med under optagelse**: **Lyt til lyd under optagelse** (til højre for uret) sender **live input** til **projektets lydudgang** under optagelse. **Lyd** – samme som optages; **MIDI** – softsynth ved MIDI-til-lyd og SoundFont. Gemmes i **projektet** (`monitorWhileRecording`). Slå fra mod feedback.
- **Lyd med lav latens**: På Windows sikrer ASIO-driverdetektion lyd med lav latens; på Linux hæves procesplanlægningsprioriteten for lavere jitter med PipeWire / PulseAudio / ALSA
- **Tilføj trommespor**: **Tools → Tilføj trommespor** (genvej **D**) tilføjer et MIDI-spor på General MIDI kanal 10 (trommer) og skriver en kort to-takts `.mid` (kick, snare, hi-hat) i projektmappen. Hvis **Metronom** har **Aktivér metronom under optagelse** slået til, bruges den BPM; ellers estimeres tempo fra aktiveret lyd eller 120 BPM. Se [brugermanualen](docs/MusiciansCanvas_User_Manual_da.md).
- **Virtual MIDI Keyboard**: En ledsagende applikation til at sende MIDI-noder via et softwarepianokeyboard med indbygget FluidSynth-synthesizer, justerbar masterforstærkning, computertastatur-til-piano-mapping, instrument-/programvalg, kor-/effektstyring og oktavskift
- **Konfiguration**: Valg af lydinputenhed, MIDI-enhed og SoundFont-fil (globale standardindstillinger og projektspecifikke tilsidesættelser)
- **Mørkt / lyst tema**: Konfigurerbart via Settings → Configuration
- **Lokalisering**: Begge applikationer er lokaliseret på 18 sprog: engelsk, tysk, spansk, fransk, japansk, portugisisk (Brasilien), traditionelt kinesisk, forenklet kinesisk, russisk, svensk, finsk, dansk, norsk, polsk, græsk, irsk, walisisk og pirat. Sproget er som standard indstillet efter operativsystemets indstilling og kan ændres i **Settings → Configuration → Language**. Qts standard i18n-system (`QTranslator` / `.ts` / `.qm`) bruges, og brugerfladen opdateres øjeblikkeligt, når sproget ændres.
- **Brugervejledning**: HTML- og PDF-dokumentation genereret fra Markdown-kilde, tilgængelig på alle understøttede sprog (se [docs/](docs/))

## Skærmbilleder

<p align="center">
	<a href="screenshots/i18n/da/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/i18n/da/MusiciansCanvas_1_MainWin.png" alt="Hovedvindue" width="800"></a>
	<a href="screenshots/i18n/da/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/i18n/da/MusiciansCanvas_2_TrackConfig.png" alt="Sporkonfiguration" width="800"></a>
	<a href="screenshots/i18n/da/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/i18n/da/MusiciansCanvas_3_GeneralSettings.png" alt="Generelle indstillinger" width="800"></a>
	<a href="screenshots/i18n/da/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/i18n/da/MusiciansCanvas_4_MIDISettings.png" alt="Standard-/generelle MIDI-indstillinger" width="800"></a>
	<a href="screenshots/i18n/da/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/i18n/da/MusiciansCanvas_5_AudioSettings.png" alt="Standard-/generelle lydindstillinger" width="800"></a>
	<a href="screenshots/i18n/da/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/i18n/da/MusiciansCanvas_DisplaySettings.png" alt="Visningsindstillinger" width="800"></a>
	<a href="screenshots/i18n/da/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/i18n/da/MusiciansCanvas_LanguageSettings.png" alt="Sprogindstillinger" width="800"></a>
	<a href="screenshots/i18n/da/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/i18n/da/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Projektspecifikke MIDI-indstillinger" width="800"></a>
	<a href="screenshots/i18n/da/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/i18n/da/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Projektspecifikke lydindstillinger" width="800"></a>
	<a href="screenshots/i18n/da/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/i18n/da/Track_Effects_Dialog.png" alt="Spor-effekter" width="800"></a>
	<a href="screenshots/i18n/da/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/i18n/da/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/i18n/da/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/i18n/da/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/i18n/da/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/i18n/da/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/i18n/da/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/i18n/da/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Afhængigheder

### Compiler

En **C++17**-kompatibel compiler er påkrævet (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (påkrævet på alle platforme)

Miksnings- og FLAC-eksportfunktionaliteten afhænger af
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-biblioteket.
Klon det som en søskendemappe til dette repository før bygning:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

CMake-buildet forventer at finde det på `../audio_mixer_cpp` relativt til dette projekts rodmappe.

---

### Qt6 Multimedia (valgfrit men stærkt anbefalet)

`qt6-multimedia` er valgfrit. Uden det bygges og kører applikationen stadig, men
**lydoptagelse og -afspilning er deaktiveret** — kun MIDI-redigering og projektstyring
vil fungere. Installer det sammen med Qt6-kernebibliotekerne ved hjælp af platformkommandoerne nedenfor.

---

### PortAudio (valgfrit)

Hvis PortAudio-biblioteket og headerfiler er installeret, aktiverer CMake **`HAVE_PORTAUDIO`** og
linker det ind i **musicians_canvas**. Optagelsen bruger derefter som standard PortAudio-stien, medmindre
projektet er indstillet til **Qt Multimedia** i **Project → Project Settings → Audio**. Hvis PortAudio
ikke findes, fortsætter bygningen uden det (wrapperen kompileres som stubs).

---

### SoundFont til MIDI-syntese

En SoundFont-fil (`.sf2`) er påkrævet for at MIDI-spor kan producere lyd. Uden en
vil MIDI-spor blive renderet som stilhed. På **Linux** vil den indbyggede FluidSynth-synthesizer
automatisk detektere en SoundFont, hvis en er installeret på en standard systemsti
(se pakkenavnene nedenfor). På **macOS og Windows** er der ingen standard systemsti,
så du skal konfigurere SoundFont manuelt i
**Settings → Configuration → MIDI**.

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

> `fluid-soundfont-gm` installerer `FluidR3_GM.sf2` til `/usr/share/sounds/sf2/` og
> detekteres automatisk ved opstart. `timgm6mb-soundfont` er et mindre alternativ.
>
> `libpipewire-0.3-dev` er påkrævet på PipeWire-baserede systemer, så Virtual MIDI
> Keyboard kan kalde `pw_init()`, før FluidSynth opretter sine lydressourcer.
> Bygningen fortsætter uden det; `HAVE_PIPEWIRE`-flaget defineres simpelthen ikke.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel libsndfile-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` installerer `FluidR3_GM.sf2` til `/usr/share/soundfonts/` og
> detekteres automatisk ved opstart.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac libsndfile portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` installerer `FluidR3_GM.sf2` til `/usr/share/soundfonts/` og
> detekteres automatisk ved opstart. `pipewire` er typisk allerede installeret på moderne
> Arch-systemer; udviklingsheadere er inkluderet i hovedpakken.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac libsndfile portaudio
```

> PipeWire er et Linux-eksklusivt undersystem og er **ikke** påkrævet på macOS. FluidSynth
> vil automatisk bruge CoreAudio via Qt Multimedia-backend.
>
> Download en General MIDI SoundFont (f.eks.
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) eller
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> og konfigurer dens sti i **Settings → Configuration → MIDI**.

---

### Windows

**musicians_canvas** kræver en **ASIO-lyddriver** til optagelse og afspilning med lav latens.
Applikationen starter ikke, hvis ingen ASIO-driver detekteres.
**virtual_midi_keyboard** kræver ikke ASIO og bruger Qt Multmedias WASAPI-backend direkte.

**Installation af en ASIO-driver til musicians_canvas (vælg én):**

| Driver | Noter |
|--------|-------|
| [ASIO4ALL](https://asio4all.org/) | Gratis, fungerer med de fleste indbyggede og USB-lydenheder |
| Producentdriver | Bedste latens til dedikerede lydgrænseflader (f.eks. Focusrite, PreSonus, RME) |

**Værktøjskæde — MSYS2 (anbefalet til MinGW-builds):**

Download og installer MSYS2 fra <https://www.msys2.org>, tilføj derefter
`C:\msys64\mingw64\bin` til din system-`PATH`. Installer de påkrævede pakker:

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

> `mingw-w64-x86_64-soundfont-fluid` installerer `FluidR3_GM.sf2` til
> `C:\msys64\mingw64\share\soundfonts\` (juster, hvis MSYS2 er installeret andetsteds).
> Da Windows ikke har en standard SoundFont-søgesti, skal du konfigurere denne sti
> manuelt i **Settings → Configuration → MIDI** efter første start.
>
> PipeWire er et Linux-eksklusivt undersystem; ingen PipeWire-pakke er nødvendig på Windows.

Pakkereferencesider:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Værktøjskæde — Visual Studio 2022:**

Installer Qt 6 via [Qt Online Installer](https://www.qt.io/download) og skaf
FluidSynth-, RtMidi- og libFLAC-binærfiler (eller byg dem fra kildekode).
CMake-buildet finder dem via `find_library` / `find_path`, så længe de
relevante mapper er i `CMAKE_PREFIX_PATH`.

En SoundFont skal downloades separat (f.eks.
[GeneralUser GS](https://schristiancollins.com/generaluser.php) eller
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)), og dens
sti skal konfigureres i **Settings → Configuration → MIDI** efter første start.

> **Bemærk:** `advapi32` (Windows-registreringsdatabase) og `winmm` (Windows Multimedia)
> linkes automatisk af CMake; ingen yderligere installation er påkrævet for disse.

---

## Bygning

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

Hvis påkrævede DLL'er ikke findes ved kørsel, kør hjælpescriptet fra build-outputmappen:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Generering af dokumentation

Brugervejledningen kan genereres som HTML og PDF fra Markdown-kilden:

```bash
cd docs
./generate_docs.sh          # Generer både HTML og PDF
./generate_docs.sh html     # Generer kun HTML
./generate_docs.sh pdf      # Generer kun PDF
```

**Forudsætninger:**

- **Python (foretrukket):** `pip3 install markdown weasyprint`
- **Alternativ:** `pandoc` og `wkhtmltopdf` (via systempakkehåndtering)

Den genererede HTML skrives til `docs/html/` og PDF'en til `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Kørsel

```bash
./build/musicians_canvas
# eller
./build/virtual_midi_keyboard   # ledsagende applikation Virtual MIDI Keyboard
```

---

## Brug

### musicians_canvas

1. **Angiv projektmappe**: Indtast eller gennemse til en mappe, hvor optagelser og projektfilen vil blive gemt
2. **Tilføj spor**: Klik på "+ Add Track"; navngiv hvert spor i tekstfeltet ved siden af "Options"
3. **Konfigurer sportype**: Klik på "Options" på et spor (eller klik på sportypeikonet mellem "Options" og navnefeltet) for at vælge Audio eller MIDI og indstille inputkilden
4. **Fjern et spor**: Klik på "×"-knappen i højre side af sporrækken
5. **Globale indstillinger**: Brug **Settings → Configuration** til at indstille globale standardværdier:
   - Fanen *General*: Tema (mørkt/lyst)
   - Fanen *MIDI*: Standard MIDI-outputenhed (indbygget FluidSynth-synthesizer eller en ekstern MIDI-enhed) og standard-SoundFont (`.sf2`) til syntese
   - Fanen *Audio*: Standard lydinput-/outputenhed til optagelse og afspilning
6. **Projektindstillinger**: Brug **Project → Project Settings** (Ctrl+P) til at tilsidesætte MIDI- og lydindstillinger kun for det aktuelle projekt (f.eks. en anden samplerate eller SoundFont per sang)
7. **Optag**: Sæt hak ved "Arm" på målsporet, klik derefter på optagelsesknappen (rød cirkel). Kun ét spor kan være armeret ad gangen
8. **Afspil**: Klik på afspilningsknappen for at mikse og afspille alle aktiverede spor
9. **Miks til fil**: Brug **Tools → Mix tracks to file** (Ctrl+M) til at eksportere til WAV, FLAC, MP3, Ogg Vorbis eller AIFF. For **MP3** og **Ogg** skal du indstille bitrate eller kvalitet under **Encoding** før miks.
10. **Gem / Åbn**: Brug **File → Save Project** (Ctrl+S) og **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Åbn konfiguration**: Brug **Configuration**-knappen eller menuen (Ctrl+,) for at åbne indstillingsdialogen
2. **MIDI-fanen**:
   - Vælg en MIDI-outputenhed (en ekstern hardware-/softwaresynthesizer) eller lad den være tom for at bruge den indbyggede FluidSynth-synthesizer
   - Vælg en MIDI-inputenhed for at videresende indgående MIDI-noder til keyboardvisningen
   - Juster **Synthesizer Volume (Master Gain)** for at styre outputniveauet for den indbyggede synthesizer (10 %–200 %)
3. **Lydfanen**: Vælg lydoutputenheden, der bruges af den indbyggede synthesizer
4. **SoundFont**: Vælg en `.sf2` SoundFont-fil til den indbyggede synthesizer (autodetekteres på Linux, hvis en system-SoundFont er installeret)
5. **Spil noder**: Klik på tangenter på skærmens pianokeyboard, brug en tilsluttet MIDI-controller eller brug computertastaturet:
   - Nedre oktav: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = kryds/b-fortegn
   - Øvre oktav: Q/W/E/R/T/Y/U/I/O/P = C til E, 2/3/5/6/7/9/0 = kryds/b-fortegn
6. **Værktøjslinjekontroller**: Juster MIDI-lydstyrke (0–127), oktav (-3 til +5), kor-/effektniveau og vælg instrumenter

---

## Tastaturgenveje

**musicians_canvas:**

| Genvej | Handling |
|--------|----------|
| Ctrl+S | Gem projekt |
| Ctrl+O | Åbn projekt |
| Ctrl+M | Miks spor til fil |
| Ctrl+P | Projektindstillinger |
| Ctrl+, | Indstillinger / Konfiguration |
| Ctrl+Q / Alt+F4 | Afslut |

**virtual_midi_keyboard:**

| Genvej | Handling |
|--------|----------|
| Ctrl+, | Konfigurationsdialog |
| Ctrl+U | Hjælp / Brugsinformation |
| Ctrl+Q | Luk |

---

## Licens

Dette projekt leveres som det er til uddannelses- og personlig brug.
