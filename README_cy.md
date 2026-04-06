# Musician's Canvas

Mae hwn yn raglen recordio cerddoriaeth aml-drac ar gyfer cyfrifiaduron bwrdd gwaith. Mae wedi'i ysgrifennu yn C++ ac yn defnyddio Qt fel
llyfrgell GUI. Mae'r rhaglen hon wedi'i bwriadu i'w hadeiladu ar gyfer Linux, Windows, a Mac OS. Hawster defnydd yw
prif ystyriaeth y dyluniad. Dechreuodd y prosiect hwn fel arbrawf gyda Cursor (offeryn codio AI) i weld beth
fyddai'n gallu ei greu fel rhaglen o'r math hwn, gyda defnydd parhaus o ddatblygiad AI yn ogystal â
datblygiad meddalwedd traddodiadol.

Flynyddoedd yn ôl, gwnes i raglen recordio cerddoriaeth aml-drac yn y coleg (gyda'r un enw); mae hwn yn ymgais
i wneud rhywbeth gwell.

## Nodweddion

- **Trefniant aml-drac**: Ychwanegu a dileu traciau lluosog i gyfansoddi caneuon
- **Traciau â'u henwau**: Mae gan bob trac enw y gellir ei olygu a ddefnyddir fel enw ffeil ar gyfer y sain a recordiwyd
- **Mathau o draciau**: Ffurfweddu pob trac ar gyfer recordio sain (o feicroffon/mewnbwn llinell) neu recordio MIDI (o fysellfwrdd/rheolydd); cliciwch yr eicon math trac i newid y ffynhonnell fewnbwn yn gyflym
- **Arfogi trac**: Ticiwch "Arm" ar drac i'w ddewis fel targed recordio; dim ond un trac y gellir ei arfogi ar y tro. Mae trac sengl heb ei recordio yn cael ei arfogi'n awtomatig er hwylustod
- **Cyfrif i lawr recordio**: Cyfrif i lawr gweledol 3 eiliad cyn i'r recordiad ddechrau, gan roi amser i'r perfformiwr baratoi
- **Recordio overdub**: Wrth recordio trac newydd tra bod traciau presennol wedi'u galluogi, mae'r traciau presennol yn cael eu cymysgu a'u chwarae'n ôl mewn amser real fel y gallwch eu clywed wrth recordio. Mae chwarae'n ôl a chipio wedi'u cydamseru i gadw'r holl draciau wedi'u halinio
- **Adborth gweledol**: Arddangos ffurf ton sain ar gyfer traciau sain (gyda mesurydd lefel byw yn ystod recordio), golwg MIDI piano roll ar gyfer traciau MIDI
- **Syntheseisydd MIDI mewnol**: Rendro traciau MIDI i sain gan ddefnyddio FluidSynth gyda SoundFont y gellir ei ffurfweddu
- **Cymysgu i un ffeil sain**: Allforio'r holl draciau a alluogwyd i un ffeil WAV neu FLAC wedi'i chymysgu gan ddefnyddio llyfrgell [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)
- **Cadw / Agor prosiect**: Cyfresoli ac adfer y prosiect llawn (traciau, enwau, mathau, nodau MIDI, cyfeiriadau ffeiliau sain) i/o ffeil JSON, gyda chanfod newidiadau heb eu cadw wrth gau
- **Gosodiadau penodol i'r prosiect**: Diystyru rhagosodiadau MIDI a sain byd-eang fesul prosiect (cyfradd samplu, SoundFont, dyfais MIDI)
- **Cipio PortAudio (adeilad dewisol)**: Pan gaiff y prosiect ei adeiladu gyda PortAudio (`HAVE_PORTAUDIO`), gall y recordiad ddefnyddio llwybr mewnbwn brodorol PortAudio (tebyg o ran ysbryd i Audacity) yn lle Qt Multimedia. Mae **Project → Project Settings → Audio** yn gadael i chi ddewis **PortAudio** neu **Qt Multimedia** a dewis dyfais fewnbwn PortAudio. Os nad yw PortAudio wedi'i osod, mae'r adeilad yn llwyddo o hyd ac mae'r recordiad yn defnyddio Qt Multimedia yn unig.
- **Trosi cyfradd samplu o ansawdd uchel**: Recordio ar gyfradd frodorol y ddyfais sain a throsi i gyfradd y prosiect gan ddefnyddio rhyngosodiad sinc ffenestrog (ffenestr Kaiser, ~96 dB o leihad band atal), yr un teulu algorithm a ddefnyddir gan Audacity / libsoxr. Mae hyn yn caniatáu recordio ar unrhyw gyfradd samplu prosiect (8000 Hz i 192000 Hz) waeth beth yw cyfradd frodorol y ddyfais, heb newid traw na hyd.
- **Trin mono/stereo awtomatig**: Canfod dyfeisiau sy'n ffisegol mono ond sy'n cael eu hysbysebu fel stereo (cyffredin gyda meicroffonau camera gwe USB ar PipeWire) a throsi rhwng mono a stereo yn ôl yr angen (dyblygu neu gyfartaleddu), gan ddilyn dull llwybro sianeli Audacity
- **Sain latency isel**: Ar Windows, mae canfod gyrrwr ASIO yn sicrhau sain latency isel; ar Linux, codir blaenoriaeth trefniadaeth y broses ar gyfer llai o jitter gyda PipeWire / PulseAudio / ALSA
- **Virtual MIDI Keyboard**: Rhaglen gydymaith ar gyfer anfon nodau MIDI drwy fysellfwrdd piano meddalwedd, gyda syntheseisydd FluidSynth mewnol, enillion meistr addasadwy, mapio bysellfwrdd cyfrifiadur i biano, dewis offeryn/rhaglen, rheolaeth côr/effeithiau a shifftio octaf
- **Ffurfweddiad**: Dewis dyfais fewnbwn sain, dyfais MIDI a ffeil SoundFont (rhagosodiadau byd-eang a diystyriadau fesul prosiect)
- **Thema tywyll / golau**: Y gellir ei ffurfweddu drwy Settings → Configuration
- **Lleoleiddio**: Mae'r ddwy raglen wedi'u lleoleiddio mewn 18 o ieithoedd: Saesneg, Almaeneg, Sbaeneg, Ffrangeg, Siapaneeg, Portiwgaleg (Brasil), Tsieineeg Draddodiadol, Tsieineeg Symledig, Rwsieg, Swedeg, Ffinneg, Daneg, Norwyeg, Pwyleg, Groeg, Gwyddeleg, Cymraeg a Môr-ladroneg. Gosodiad y system weithredu yw'r iaith ragosodedig a gellir ei newid yn **Settings → Configuration → Language**. Defnyddir system i18n safonol Qt (`QTranslator` / `.ts` / `.qm`), ac mae'r rhyngwyneb yn diweddaru ar unwaith pan newidir yr iaith.
- **Llawlyfr defnyddiwr**: Dogfennaeth HTML a PDF wedi'i chynhyrchu o ffynhonnell Markdown, ar gael ym mhob iaith a gefnogir (gweler [docs/](docs/))

## Sgrinluniau

<p align="center">
	<a href="screenshots/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/MusiciansCanvas_1_MainWin.png" alt="Prif ffenestr" width="800"></a>
	<a href="screenshots/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/MusiciansCanvas_2_TrackConfig.png" alt="Ffurfweddiad trac" width="800"></a>
	<a href="screenshots/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_3_GeneralSettings.png" alt="Gosodiadau cyffredinol" width="800"></a>
	<a href="screenshots/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_4_MIDISettings.png" alt="Gosodiadau MIDI rhagosodedig/cyffredinol" width="800"></a>
	<a href="screenshots/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_5_AudioSettings.png" alt="Gosodiadau sain rhagosodedig/cyffredinol" width="800"></a>
	<a href="screenshots/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_DisplaySettings.png" alt="Gosodiadau arddangos" width="800"></a>
	<a href="screenshots/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_LanguageSettings.png" alt="Gosodiadau iaith" width="800"></a>
	<a href="screenshots/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Gosodiadau MIDI penodol i'r prosiect" width="800"></a>
	<a href="screenshots/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Gosodiadau sain penodol i'r prosiect" width="800"></a>
	<a href="screenshots/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Dibyniaethau

### Casglydd

Mae angen casglydd sy'n gallu **C++17** (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (gofynnol ar bob platfform)

Mae'r gallu cymysgu ac allforio FLAC yn dibynnu ar y llyfrgell
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp).
Cloniwch hi fel cyfeiriadur chwaer i'r storfa hon cyn adeiladu:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

Mae'r adeilad CMake yn disgwyl ei ganfod yn `../audio_mixer_cpp` mewn perthynas â gwraidd y prosiect hwn.

---

### Qt6 Multimedia (dewisol ond argymhellir yn gryf)

Mae `qt6-multimedia` yn ddewisol. Hebddo, mae'r rhaglen yn dal i adeiladu a rhedeg, ond
**mae recordio a chwarae sain wedi'u hanalluogi** — dim ond golygu MIDI a rheoli prosiectau
fydd yn gweithio. Gosodwch ef ochr yn ochr â llyfrgelloedd craidd Qt6 gan ddefnyddio'r gorchmynion platfform isod.

---

### PortAudio (dewisol)

Os yw llyfrgell a phenawdau PortAudio wedi'u gosod, mae CMake yn galluogi **`HAVE_PORTAUDIO`** ac
yn ei gysylltu â **musicians_canvas**. Yna mae'r recordiad yn defnyddio'r llwybr PortAudio yn ddiofyn oni bai fod
y prosiect wedi'i osod i **Qt Multimedia** yn **Project → Project Settings → Audio**. Os na chanfyddir
PortAudio, mae'r adeilad yn parhau hebddo (mae'r lapiwr yn casglu fel stybiau).

---

### SoundFont ar gyfer synthesis MIDI

Mae ffeil SoundFont (`.sf2`) yn ofynnol i draciau MIDI gynhyrchu sain. Hebddi,
bydd traciau MIDI yn cael eu rendro fel tawelwch. Ar **Linux**, bydd y syntheseisydd FluidSynth mewnol
yn canfod SoundFont yn awtomatig os yw un wedi'i osod ar lwybr system safonol
(gweler enwau'r pecynnau isod). Ar **macOS a Windows** nid oes llwybr system safonol,
felly rhaid i chi ffurfweddu'r SoundFont â llaw yn
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

> Mae `fluid-soundfont-gm` yn gosod `FluidR3_GM.sf2` i `/usr/share/sounds/sf2/` ac yn cael
> ei ganfod yn awtomatig wrth gychwyn. Mae `timgm6mb-soundfont` yn ddewis arall llai.
>
> Mae `libpipewire-0.3-dev` yn ofynnol ar systemau sy'n seiliedig ar PipeWire fel y gall Virtual MIDI
> Keyboard alw `pw_init()` cyn i FluidSynth greu ei adnoddau sain.
> Mae'r adeilad yn parhau hebddo; nid yw'r faner `HAVE_PIPEWIRE` yn cael ei diffinio yn syml.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> Mae `fluid-soundfont-gm` yn gosod `FluidR3_GM.sf2` i `/usr/share/soundfonts/` ac yn cael
> ei ganfod yn awtomatig wrth gychwyn.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac portaudio \
  pipewire \
  soundfont-fluid
```

> Mae `soundfont-fluid` yn gosod `FluidR3_GM.sf2` i `/usr/share/soundfonts/` ac yn cael
> ei ganfod yn awtomatig wrth gychwyn. Fel arfer mae `pipewire` eisoes wedi'i osod ar systemau
> Arch modern; mae'r penawdau datblygu wedi'u cynnwys yn y prif becyn.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac portaudio
```

> Is-system Linux yn unig yw PipeWire ac **nid** yw'n ofynnol ar macOS. Bydd FluidSynth
> yn defnyddio CoreAudio yn awtomatig drwy'r cefn-ddiwedd Qt Multimedia.
>
> Lawrlwythwch SoundFont General MIDI (e.e.
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) neu
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> a ffurfweddwch ei lwybr yn **Settings → Configuration → MIDI**.

---

### Windows

Mae **musicians_canvas** yn gofyn am **yrrwr sain ASIO** ar gyfer recordio a chwarae latency isel.
Ni fydd y rhaglen yn cychwyn os na chanfyddir gyrrwr ASIO.
Nid yw **virtual_midi_keyboard** yn gofyn am ASIO ac mae'n defnyddio cefn-ddiwedd WASAPI Qt Multimedia yn uniongyrchol.

**Gosod gyrrwr ASIO ar gyfer musicians_canvas (dewiswch un):**

| Gyrrwr | Nodiadau |
|--------|----------|
| [ASIO4ALL](https://asio4all.org/) | Am ddim, yn gweithio gyda'r rhan fwyaf o galedwedd sain mewnol ac USB |
| Gyrrwr y gwneuthurwr | Latency gorau ar gyfer rhyngwynebau sain penodedig (e.e. Focusrite, PreSonus, RME) |

**Cadwyn offer — MSYS2 (argymhellir ar gyfer adeiladau MinGW):**

Lawrlwythwch a gosodwch MSYS2 o <https://www.msys2.org>, yna ychwanegwch
`C:\msys64\mingw64\bin` at eich `PATH` system. Gosodwch y pecynnau gofynnol:

```bash
pacman -S mingw-w64-x86_64-qt6-base
pacman -S mingw-w64-x86_64-qt6-multimedia
pacman -S mingw-w64-x86_64-fluidsynth
pacman -S mingw-w64-ucrt-x86_64-rtmidi
pacman -S mingw-w64-x86_64-flac
pacman -S mingw-w64-x86_64-portaudio
pacman -S mingw-w64-x86_64-soundfont-fluid
```

> Mae `mingw-w64-x86_64-soundfont-fluid` yn gosod `FluidR3_GM.sf2` i
> `C:\msys64\mingw64\share\soundfonts\` (addaswch os yw MSYS2 wedi'i osod mewn man arall).
> Gan nad oes gan Windows lwybr chwilio SoundFont safonol, rhaid i chi ffurfweddu'r llwybr hwn
> â llaw yn **Settings → Configuration → MIDI** ar ôl y lansiad cyntaf.
>
> Is-system Linux yn unig yw PipeWire; nid oes angen pecyn PipeWire ar Windows.

Tudalennau cyfeirio pecynnau:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Cadwyn offer — Visual Studio 2022:**

Gosodwch Qt 6 drwy'r [Qt Online Installer](https://www.qt.io/download) a chael
deuolion FluidSynth, RtMidi a libFLAC (neu eu hadeiladu o'r cod ffynhonnell).
Bydd yr adeilad CMake yn dod o hyd iddynt drwy `find_library` / `find_path` cyn belled â bod y
cyfeiriaduron priodol yn `CMAKE_PREFIX_PATH`.

Rhaid lawrlwytho SoundFont ar wahân (e.e.
[GeneralUser GS](https://schristiancollins.com/generaluser.php) neu
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)) a'i
lwybr ei ffurfweddu yn **Settings → Configuration → MIDI** ar ôl y lansiad cyntaf.

> **Noder:** Mae `advapi32` (cofrestr Windows) a `winmm` (Windows Multimedia) yn cael eu
> cysylltu'n awtomatig gan CMake; nid oes angen gosod ychwanegol ar eu cyfer.

---

## Adeiladu

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

Os na chanfyddir y DLLau gofynnol wrth redeg, rhedwch y sgript cynorthwyol o gyfeiriadur allbwn yr adeilad:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Cynhyrchu dogfennaeth

Gellir cynhyrchu'r llawlyfr defnyddiwr fel HTML a PDF o'r ffynhonnell Markdown:

```bash
cd docs
./generate_docs.sh          # Cynhyrchu HTML a PDF
./generate_docs.sh html     # Cynhyrchu HTML yn unig
./generate_docs.sh pdf      # Cynhyrchu PDF yn unig
```

**Rhagofynion:**

- **Python (dewis cyntaf):** `pip3 install markdown weasyprint`
- **Dewis arall:** `pandoc` a `wkhtmltopdf` (drwy reolwr pecynnau'r system)

Ysgrifennir yr HTML a gynhyrchir i `docs/html/` a'r PDF i `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Rhedeg

```bash
./build/musicians_canvas
# neu
./build/virtual_midi_keyboard   # rhaglen gydymaith Virtual MIDI Keyboard
```

---

## Defnydd

### musicians_canvas

1. **Gosod cyfeiriadur prosiect**: Rhowch neu pori i ffolder lle bydd recordiadau a'r ffeil prosiect yn cael eu storio
2. **Ychwanegu traciau**: Cliciwch "+ Add Track"; enwch bob trac yn y maes testun wrth ymyl "Options"
3. **Ffurfweddu math trac**: Cliciwch "Options" ar drac (neu gliciwch yr eicon math trac rhwng "Options" a'r maes enw) i ddewis Audio neu MIDI a gosod y ffynhonnell fewnbwn
4. **Dileu trac**: Cliciwch y botwm "×" ar ochr dde'r rhes trac
5. **Gosodiadau byd-eang**: Defnyddiwch **Settings → Configuration** i osod rhagosodiadau byd-eang:
   - Tab *General*: Thema (tywyll/golau)
   - Tab *MIDI*: Dyfais allbwn MIDI rhagosodedig (syntheseisydd FluidSynth mewnol neu ddyfais MIDI allanol) a SoundFont rhagosodedig (`.sf2`) ar gyfer synthesis
   - Tab *Audio*: Dyfais fewnbwn/allbwn sain rhagosodedig ar gyfer recordio a chwarae
6. **Gosodiadau prosiect**: Defnyddiwch **Project → Project Settings** (Ctrl+P) i ddiystyru gosodiadau MIDI a sain ar gyfer y prosiect presennol yn unig (e.e. cyfradd samplu neu SoundFont gwahanol fesul cân)
7. **Recordio**: Ticiwch "Arm" ar y trac targed, yna cliciwch y botwm recordio (cylch coch). Dim ond un trac y gellir ei arfogi ar y tro
8. **Chwarae**: Cliciwch y botwm chwarae i gymysgu a chwarae'r holl draciau a alluogwyd
9. **Cymysgu i ffeil**: Defnyddiwch **Tools → Mix tracks to file** (Ctrl+M) i allforio i WAV neu FLAC
10. **Cadw / Agor**: Defnyddiwch **File → Save Project** (Ctrl+S) a **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Agor ffurfweddiad**: Defnyddiwch y botwm neu ddewislen **Configuration** (Ctrl+,) i agor y ddeialog gosodiadau
2. **Tab MIDI**:
   - Dewiswch ddyfais allbwn MIDI (syntheseisydd allanol caledwedd/meddalwedd) neu gadewch yn wag i ddefnyddio'r syntheseisydd FluidSynth mewnol
   - Dewiswch ddyfais fewnbwn MIDI i anfon ymlaen nodau MIDI sy'n dod i mewn i arddangosiad y bysellfwrdd
   - Addaswch **Synthesizer Volume (Master Gain)** i reoli lefel allbwn y syntheseisydd mewnol (10%–200%)
3. **Tab Audio**: Dewiswch y ddyfais allbwn sain a ddefnyddir gan y syntheseisydd mewnol
4. **SoundFont**: Dewiswch ffeil SoundFont `.sf2` ar gyfer y syntheseisydd mewnol (yn cael ei ganfod yn awtomatig ar Linux os yw SoundFont system wedi'i osod)
5. **Chwarae nodau**: Cliciwch allweddi ar fysellfwrdd piano'r sgrin, defnyddiwch reolydd MIDI wedi'i gysylltu neu ddefnyddiwch fysellfwrdd y cyfrifiadur:
   - Octaf isaf: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = llymion/fflation
   - Octaf uchaf: Q/W/E/R/T/Y/U/I/O/P = C i E, 2/3/5/6/7/9/0 = llymion/fflation
6. **Rheolyddion bar offer**: Addasu sain MIDI (0–127), octaf (-3 i +5), lefel côr/effaith a dewis offerynnau

---

## Llwybrau byr bysellfwrdd

**musicians_canvas:**

| Llwybr byr | Gweithred |
|------------|-----------|
| Ctrl+S | Cadw prosiect |
| Ctrl+O | Agor prosiect |
| Ctrl+M | Cymysgu traciau i ffeil |
| Ctrl+P | Gosodiadau prosiect |
| Ctrl+, | Gosodiadau / Ffurfweddiad |
| Ctrl+Q / Alt+F4 | Gadael |

**virtual_midi_keyboard:**

| Llwybr byr | Gweithred |
|------------|-----------|
| Ctrl+, | Deialog ffurfweddiad |
| Ctrl+U | Cymorth / Gwybodaeth defnydd |
| Ctrl+Q | Cau |

---

## Trwydded

Darperir y prosiect hwn fel y mae ar gyfer defnydd addysgol a phersonol.
