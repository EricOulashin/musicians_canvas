# Musician's Canvas

Detta är en flerspårs musikinspelningsapplikation för stationära datorer. Den är skriven i C++ och använder Qt som
GUI-bibliotek. Denna applikation är avsedd att byggas för Linux, Windows och Mac OS. Användarvänlighet är en
huvudsaklig designövervägande. Detta projekt startade som ett experiment med Cursor (AI-kodningsverktyg) för att se vad
det skulle kunna skapa som en applikation av detta slag, med fortsatt användning av AI-utveckling samt
traditionell mjukvaruutveckling.

För flera år sedan skapade jag ett flerspårs musikinspelningsprogram på universitetet (med samma namn); detta är ett försök
att skapa något bättre.

## Funktioner

- **Flerspårsarrangemang**: Lägg till och ta bort flera spår för att komponera låtar
- **Namngivna spår**: Varje spår har ett redigerbart namn som används som filnamn för inspelat ljud
- **Spårtyper**: Konfigurera varje spår för ljudinspelning (från mikrofon/linjeinmatning) eller MIDI-inspelning (från keyboard/controller); klicka på spårtypsikonen för att snabbt ändra ingångskälla
- **Spårarmering**: Markera "Arm" på ett spår för att välja det som inspelningsmål; bara ett spår kan vara armerat åt gången. Ett enstaka oinspelade spår armeras automatiskt för bekvämlighet
- **Inspelningsnedräkning**: En visuell 3-sekunders nedräkning innan inspelningen börjar, som ger utövaren tid att förbereda sig
- **Overdub-inspelning**: Vid inspelning av ett nytt spår medan befintliga spår är aktiverade, mixas och spelas de befintliga spåren upp i realtid så att du kan höra dem medan du spelar in. Uppspelning och fångst är synkroniserade för att hålla alla spår justerade
- **Visuell återkoppling**: Ljudvågsformsvisning för ljudspår (med livnivåmätare under inspelning), MIDI-pianorulle för MIDI-spår
- **Inbyggd MIDI-synthesizer**: Renderar MIDI-spår till ljud med FluidSynth och en konfigurerbar SoundFont
- **Mixa till en enda ljudfil**: Exportera alla aktiverade spår till en enda mixad WAV- eller FLAC-fil med hjälp av biblioteket [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)
- **Spara / Öppna projekt**: Serialisera och återställa hela projektet (spår, namn, typer, MIDI-noter, ljudfilsreferenser) till/från en JSON-fil, med detektering av osparade ändringar vid avslut
- **Projektspecifika inställningar**: Åsidosätta globala MIDI- och ljudstandardvärden per projekt (samplingsfrekvens, SoundFont, MIDI-enhet)
- **PortAudio-inspelning (valfritt bygge)**: När projektet byggs med PortAudio (`HAVE_PORTAUDIO`) kan inspelningen använda en nativ PortAudio-inmatningsväg (liknande i anda som Audacity) istället för Qt Multimedia. **Project → Project Settings → Audio** låter dig välja **PortAudio** eller **Qt Multimedia** och välja en PortAudio-inmatningsenhet. Om PortAudio inte är installerat lyckas bygget ändå och inspelningen använder bara Qt Multimedia.
- **Högkvalitativ samplingsfrekvenskonvertering**: Spelar in med ljudenhetens nativa frekvens och konverterar till projektfrekvensen med fönstrad sinc-interpolation (Kaiser-fönster, ~96 dB stoppbanddämpning), samma algoritmfamilj som används av Audacity / libsoxr. Detta möjliggör inspelning med vilken projektsamplingsfrekvens som helst (8000 Hz till 192000 Hz) oavsett enhetens nativa frekvens, utan tonhöjds- eller varaktighetsförändring.
- **Automatisk mono/stereo-hantering**: Detekterar fysiskt mono-enheter som annonseras som stereo (vanligt med USB-webbkameramikrofoner på PipeWire) och konverterar mellan mono och stereo vid behov (duplicering eller medelvärde), enligt Audacitys kanalroutningmetod
- **Inspelnings-inserteffekter (ljudspår)**: Knappen **Effects** (under **Options** på varje ljudspår) öppnar dialogen **Track effects**. Lägg till och konfigurera **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion** och **Amp & cabinet** (förstärkar- och högtalarmodellering), dra **≡**-handtaget för att sortera om kedjan (överst körs först) och spara med projektet. Effekterna tillämpas på tagningen när du stoppar inspelningen; parametrarna är i verkliga enheter (ms, Hz) så att beteendet förblir konsekvent för mono eller stereo och typiska projektsamplingsfrekvenser (8 kHz–192 kHz) efter att inspelningen normaliserats. Se [användarhandboken](docs/MusiciansCanvas_User_Manual_sv.md). Översättare kan massuppdatera strängar via `scripts/effect_i18n.tsv` (genereras av `scripts/build_effect_tsv.py`) och `scripts/fill_effect_i18n.py` efter `lupdate`.
- **Mix-effekter (huvudbuss)**: **Project → Project Settings → Mix Effects** ställer in en kedja på **hela mixen** (samma typer och ordning som på spår). Används vid **uppspelning av alla spår** och **export av mix** (WAV/FLAC). Sparas i `project.json` som `mixEffectChain`.
- **Mjuk begränsning före 16-bit PCM**: Inbyggd DSP minskar hård digital klippning. **EffectWidget** erbjuder `guardFloatSampleForInt16Pcm()` och `softLimitFloatSampleForInt16Pcm()`.
- **Inspelningsövervakning**: **Övervaka ljud under inspelning** (till höger om klockan) skickar **live-ingång** till **projektets ljudutgång** under inspelning. **Ljud** – samma som spelas in; **MIDI** – via mjukvarusynt om MIDI renderas till ljud med SoundFont. Sparas i **projektet** (`monitorWhileRecording`). Stäng av mot rundgång.
- **Ljud med låg latens**: På Windows säkerställer ASIO-drivrutinsdetektering ljud med låg latens; på Linux höjs processschemaläggningsprioriteten för lägre jitter med PipeWire / PulseAudio / ALSA
- **Lägg till trumspår**: **Tools → Lägg till trumspår** (kortkommando **D**) lägger till ett MIDI-spår på General MIDI kanal 10 (trummor) och skriver en kort tvåtakts `.mid` i projektmappen. Om **Metronom** har **Aktivera metronom under inspelning** används det BPM; annars uppskattas tempo från aktiverat ljud eller 120 BPM. Se [användarhandboken](docs/MusiciansCanvas_User_Manual_sv.md).
- **Virtual MIDI Keyboard**: En kompletterande applikation för att skicka MIDI-noter via ett mjukvarupianokeyboard, med inbyggd FluidSynth-synthesizer, justerbar masterförstärkning, datortangentbord-till-piano-mappning, instrument-/programval, chorus-/effektstyrning och oktavskiftning
- **Konfiguration**: Val av ljudinmatningsenhet, MIDI-enhet och SoundFont-fil (globala standardvärden och projektspecifika åsidosättningar)
- **Mörkt / ljust tema**: Konfigurerbart via Settings → Configuration
- **Lokalisering**: Båda applikationerna är lokaliserade på 18 språk: engelska, tyska, spanska, franska, japanska, portugisiska (Brasilien), traditionell kinesiska, förenklad kinesiska, ryska, svenska, finska, danska, norska, polska, grekiska, iriska, walesiska och piratspråk. Språket är som standard inställt efter operativsystemet och kan ändras i **Settings → Configuration → Language**. Qts standard-i18n-system (`QTranslator` / `.ts` / `.qm`) används, och gränssnittet uppdateras omedelbart när språket ändras.
- **Användarhandbok**: HTML- och PDF-dokumentation genererad från Markdown-källkod, tillgänglig på alla stödda språk (se [docs/](docs/))

## Skärmbilder

<p align="center">
	<a href="screenshots/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/MusiciansCanvas_1_MainWin.png" alt="Huvudfönster" width="800"></a>
	<a href="screenshots/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/MusiciansCanvas_2_TrackConfig.png" alt="Spårkonfiguration" width="800"></a>
	<a href="screenshots/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_3_GeneralSettings.png" alt="Allmänna inställningar" width="800"></a>
	<a href="screenshots/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_4_MIDISettings.png" alt="Standard-/allmänna MIDI-inställningar" width="800"></a>
	<a href="screenshots/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_5_AudioSettings.png" alt="Standard-/allmänna ljudinställningar" width="800"></a>
	<a href="screenshots/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_DisplaySettings.png" alt="Visningsinställningar" width="800"></a>
	<a href="screenshots/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_LanguageSettings.png" alt="Språkinställningar" width="800"></a>
	<a href="screenshots/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Projektspecifika MIDI-inställningar" width="800"></a>
	<a href="screenshots/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Projektspecifika ljudinställningar" width="800"></a>
	<a href="screenshots/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/Track_Effects_Dialog.png" alt="Spår-effekter" width="800"></a>
	<a href="screenshots/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Beroenden

### Kompilator

En **C++17**-kompatibel kompilator krävs (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (krävs på alla plattformar)

Mixnings- och FLAC-exportfunktionaliteten beror på biblioteket
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp).
Klona det som en syskondkatalog till detta repository innan du bygger:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

CMake-bygget förväntar sig att hitta det på `../audio_mixer_cpp` relativt till detta projekts rotkatalog.

---

### Qt6 Multimedia (valfritt men starkt rekommenderat)

`qt6-multimedia` är valfritt. Utan det byggs och körs applikationen fortfarande, men
**ljudinspelning och uppspelning är inaktiverade** — bara MIDI-redigering och projekthantering
fungerar. Installera det tillsammans med Qt6-kärnbiblioteken med plattformskommandona nedan.

---

### PortAudio (valfritt)

Om PortAudio-biblioteket och rubrikfilerna är installerade aktiverar CMake **`HAVE_PORTAUDIO`** och
länkar det till **musicians_canvas**. Inspelningen använder då som standard PortAudio-vägen om inte
projektet är inställt på **Qt Multimedia** i **Project → Project Settings → Audio**. Om PortAudio
inte hittas fortsätter bygget utan det (wrappern kompileras som stubbar).

---

### SoundFont för MIDI-syntes

En SoundFont-fil (`.sf2`) krävs för att MIDI-spår ska producera ljud. Utan en
renderas MIDI-spår som tystnad. På **Linux** kommer den inbyggda FluidSynth-synthesizern
automatiskt att detektera en SoundFont om en är installerad på en standardsystemsökväg
(se paketnamnen nedan). På **macOS och Windows** finns ingen standardsystemsökväg,
så du måste konfigurera SoundFont manuellt i
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

> `fluid-soundfont-gm` installerar `FluidR3_GM.sf2` till `/usr/share/sounds/sf2/` och
> detekteras automatiskt vid uppstart. `timgm6mb-soundfont` är ett mindre alternativ.
>
> `libpipewire-0.3-dev` krävs på PipeWire-baserade system för att Virtual MIDI
> Keyboard ska kunna anropa `pw_init()` innan FluidSynth skapar sina ljudresurser.
> Bygget fortsätter utan det; flaggan `HAVE_PIPEWIRE` definieras helt enkelt inte.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` installerar `FluidR3_GM.sf2` till `/usr/share/soundfonts/` och
> detekteras automatiskt vid uppstart.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` installerar `FluidR3_GM.sf2` till `/usr/share/soundfonts/` och
> detekteras automatiskt vid uppstart. `pipewire` är vanligtvis redan installerat på moderna
> Arch-system; utvecklingsrubrikerna ingår i huvudpaketet.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac portaudio
```

> PipeWire är ett Linux-exklusivt subsystem och krävs **inte** på macOS. FluidSynth
> kommer automatiskt att använda CoreAudio via Qt Multimedia-backend.
>
> Ladda ner en General MIDI SoundFont (t.ex.
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) eller
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> och konfigurera dess sökväg i **Settings → Configuration → MIDI**.

---

### Windows

**musicians_canvas** kräver en **ASIO-ljuddrivrutin** för inspelning och uppspelning med låg latens.
Applikationen startar inte om ingen ASIO-drivrutin detekteras.
**virtual_midi_keyboard** kräver inte ASIO och använder Qt Multmedias WASAPI-backend direkt.

**Installera en ASIO-drivrutin för musicians_canvas (välj en):**

| Drivrutin | Anteckningar |
|-----------|--------------|
| [ASIO4ALL](https://asio4all.org/) | Gratis, fungerar med de flesta inbyggda och USB-ljudhårdvaror |
| Tillverkarens drivrutin | Bäst latens för dedikerade ljudgränssnitt (t.ex. Focusrite, PreSonus, RME) |

**Verktygskedja — MSYS2 (rekommenderas för MinGW-byggen):**

Ladda ner och installera MSYS2 från <https://www.msys2.org>, lägg sedan till
`C:\msys64\mingw64\bin` i din system-`PATH`. Installera de nödvändiga paketen:

```bash
pacman -S mingw-w64-x86_64-qt6-base
pacman -S mingw-w64-x86_64-qt6-multimedia
pacman -S mingw-w64-x86_64-fluidsynth
pacman -S mingw-w64-ucrt-x86_64-rtmidi
pacman -S mingw-w64-x86_64-flac
pacman -S mingw-w64-x86_64-portaudio
pacman -S mingw-w64-x86_64-soundfont-fluid
```

> `mingw-w64-x86_64-soundfont-fluid` installerar `FluidR3_GM.sf2` till
> `C:\msys64\mingw64\share\soundfonts\` (justera om MSYS2 är installerat på en annan plats).
> Eftersom Windows inte har en standardsöksökväg för SoundFont måste du konfigurera denna sökväg
> manuellt i **Settings → Configuration → MIDI** efter första start.
>
> PipeWire är ett Linux-exklusivt subsystem; inget PipeWire-paket behövs på Windows.

Paketreferenssidor:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Verktygskedja — Visual Studio 2022:**

Installera Qt 6 via [Qt Online Installer](https://www.qt.io/download) och skaffa
FluidSynth-, RtMidi- och libFLAC-binärer (eller bygg dem från källkod).
CMake-bygget hittar dem via `find_library` / `find_path` så länge de
lämpliga katalogerna finns i `CMAKE_PREFIX_PATH`.

En SoundFont måste laddas ner separat (t.ex.
[GeneralUser GS](https://schristiancollins.com/generaluser.php) eller
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)) och dess
sökväg konfigureras i **Settings → Configuration → MIDI** efter första start.

> **Obs:** `advapi32` (Windows-register) och `winmm` (Windows Multimedia) länkas
> automatiskt av CMake; ingen ytterligare installation krävs för dessa.

---

## Bygga

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

Om nödvändiga DLL-filer inte hittas vid körning, kör hjälpskriptet från byggets utdatakatalog:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Generera dokumentation

Användarhandboken kan genereras som HTML och PDF från Markdown-källan:

```bash
cd docs
./generate_docs.sh          # Generera både HTML och PDF
./generate_docs.sh html     # Generera bara HTML
./generate_docs.sh pdf      # Generera bara PDF
```

**Förutsättningar:**

- **Python (föredraget):** `pip3 install markdown weasyprint`
- **Reserv:** `pandoc` och `wkhtmltopdf` (via systemets pakethanterare)

Den genererade HTML:en skrivs till `docs/html/` och PDF:en till `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Köra

```bash
./build/musicians_canvas
# eller
./build/virtual_midi_keyboard   # kompletterande applikation Virtual MIDI Keyboard
```

---

## Användning

### musicians_canvas

1. **Ange projektkatalog**: Skriv in eller bläddra till en mapp där inspelningar och projektfilen kommer att lagras
2. **Lägg till spår**: Klicka på "+ Add Track"; namnge varje spår i textfältet bredvid "Options"
3. **Konfigurera spårtyp**: Klicka på "Options" på ett spår (eller klicka på spårtypsikonen mellan "Options" och namnfältet) för att välja Audio eller MIDI och ställa in ingångskällan
4. **Ta bort ett spår**: Klicka på "×"-knappen på höger sida av spårraden
5. **Globala inställningar**: Använd **Settings → Configuration** för att ställa in globala standardvärden:
   - Fliken *General*: Tema (mörkt/ljust)
   - Fliken *MIDI*: Standard MIDI-utmatningsenhet (inbyggd FluidSynth-synthesizer eller en extern MIDI-enhet) och standard-SoundFont (`.sf2`) för syntes
   - Fliken *Audio*: Standard ljudinmatnings-/utmatningsenhet för inspelning och uppspelning
6. **Projektinställningar**: Använd **Project → Project Settings** (Ctrl+P) för att åsidosätta MIDI- och ljudinställningar bara för det aktuella projektet (t.ex. en annan samplingsfrekvens eller SoundFont per låt)
7. **Spela in**: Markera "Arm" på målspåret, klicka sedan på inspelningsknappen (röd cirkel). Bara ett spår kan vara armerat åt gången
8. **Spela upp**: Klicka på uppspelningsknappen för att mixa och spela upp alla aktiverade spår
9. **Mixa till fil**: Använd **Tools → Mix tracks to file** (Ctrl+M) för att exportera till WAV eller FLAC
10. **Spara / Öppna**: Använd **File → Save Project** (Ctrl+S) och **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Öppna konfiguration**: Använd **Configuration**-knappen eller menyn (Ctrl+,) för att öppna inställningsdialogen
2. **MIDI-fliken**:
   - Välj en MIDI-utmatningsenhet (en extern hårdvaru-/mjukvarusynthesizer) eller lämna tomt för att använda den inbyggda FluidSynth-synthesizern
   - Välj en MIDI-inmatningsenhet för att vidarebefordra inkommande MIDI-noter till tangentbordsdisplayen
   - Justera **Synthesizer Volume (Master Gain)** för att styra utgångsnivån på den inbyggda synthesizern (10 %–200 %)
3. **Ljudfliken**: Välj ljudutmatningsenheten som används av den inbyggda synthesizern
4. **SoundFont**: Välj en `.sf2` SoundFont-fil för den inbyggda synthesizern (autodetekteras på Linux om en system-SoundFont är installerad)
5. **Spela noter**: Klicka på tangenter på skärmpianokeyboardet, använd en ansluten MIDI-controller eller använd datortangentbordet:
   - Nedre oktav: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = korsförtecken/b-förtecken
   - Övre oktav: Q/W/E/R/T/Y/U/I/O/P = C till E, 2/3/5/6/7/9/0 = korsförtecken/b-förtecken
6. **Verktygsfältskontroller**: Justera MIDI-volym (0–127), oktav (-3 till +5), chorus-/effektnivå och välj instrument

---

## Tangentbordsgenvägar

**musicians_canvas:**

| Genväg | Åtgärd |
|--------|--------|
| Ctrl+S | Spara projekt |
| Ctrl+O | Öppna projekt |
| Ctrl+M | Mixa spår till fil |
| Ctrl+P | Projektinställningar |
| Ctrl+, | Inställningar / Konfiguration |
| Ctrl+Q / Alt+F4 | Avsluta |

**virtual_midi_keyboard:**

| Genväg | Åtgärd |
|--------|--------|
| Ctrl+, | Konfigurationsdialog |
| Ctrl+U | Hjälp / Användningsinformation |
| Ctrl+Q | Stäng |

---

## Licens

Detta projekt tillhandahålls i befintligt skick för utbildnings- och personligt bruk.
