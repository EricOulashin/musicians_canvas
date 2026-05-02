# Musician's Canvas

Is feidhmchlár taifeadta ceoil ilrianach é seo do ríomhairí deisce. Tá sé scríofa i C++ agus úsáideann sé Qt mar
leabharlann GUI. Tá an feidhmchlár seo ceaptha le tógáil do Linux, Windows, agus Mac OS. Is príomhchúis dearaidh é
éascaíocht úsáide. Thosaigh an tionscadal seo mar thurgnamh le Cursor & Claude (uirlisí códaithe AI) le feiceáil cad
a bheadh sé in ann a chruthú mar fheidhmchlár den chineál seo, le húsáid leanúnach d'fhorbairt AI chomh maith
le forbairt bogearraí traidisiúnta.

Blianta ó shin, rinne mé clár taifeadta ceoil ilrianach sa choláiste (leis an ainm céanna); is iarracht é seo
rud éigin níos fearr a dhéanamh.

## Gnéithe

- **Socrú ilrianach**: Cuir rianta iolracha leis agus bain iad chun amhráin a chumadh
- **Rianta ainmnithe**: Tá ainm in-eagraithe ag gach rian a úsáidtear mar ainm comhaid don fhuaim taifeadta
- **Cineálacha rianta**: Cumraigh gach rian le haghaidh taifeadadh fuaime (ó mhicreafón/ionchur líne) nó taifeadadh MIDI (ó mhéarchlár/rialaitheoir); cliceáil an deilbhín cineál riain chun an fhoinse ionchuir a athrú go tapa
- **Armáil riain**: Seiceáil "Arm" ar rian chun é a roghnú mar sprioc taifeadta; ní féidir ach rian amháin a bheith armáilte ag an am. Armáiltear rian singil neamhthaifeadta go huathoibríoch ar mhaithe le háisiúlacht
- **Comhaireamh siar taifeadta**: Comhaireamh siar amhairc 3 soicind roimh thús an taifeadta, ag tabhairt ama don cheoltóir ullmhú
- **Taifeadadh overdub**: Nuair a thaifeadtar rian nua agus rianta reatha cumasaithe, measctar agus seinntear na rianta reatha i bhfíor-am ionas gur féidir leat iad a chloisteáil agus tú ag taifeadadh. Tá an athsheinm agus an ghabháil sioncronaithe chun na rianta go léir a choinneáil ailínithe
- **Aiseolas amhairc**: Taispeáint tonnfhoirm fuaime do rianta fuaime (le méadar leibhéil beo le linn taifeadta), radharcra MIDI piano roll do rianta MIDI
- **Sintéiseoir MIDI ionsuite**: Rindreálann rianta MIDI go fuaim ag úsáid FluidSynth le SoundFont inchumraithe
- **Measc go comhad fuaime singil**: Easpórtáil na rianta cumasaithe go léir go comhad measctha amháin (WAV, FLAC, MP3, Ogg Vorbis nó AIFF) ag baint úsáide as leabharlann [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) (**Tools → Mix tracks to file**). Úsáideann an ionchódú na ranganna céanna leis an tionscadal gaolta (`MP3File`, `OggFile`, `AiffFile`) le **libsndfile**. Nuair is **MP3** nó **Ogg Vorbis** an spriocean, taispeántar rannóg **Encoding** sa dialóg le hidrbande agus caighdeán (CBR/ABR/VBR MP3 agus Vorbis de réir caighdeáin nó bitrate ainmnithe gar-mheasta); ní thairgtear roghanna bitrate ann do **WAV**, **FLAC** ná **AIFF**.
- **Sábháil / Oscail tionscadal**: Srathú agus athchóiriú an tionscadail iomlán (rianta, ainmneacha, cineálacha, nótaí MIDI, tagairtí comhad fuaime) go/ó chomhad JSON, le brath athruithe neamhshábháilte ar dhúnadh
- **Socruithe sainiúla tionscadail**: Sáraigh réamhshocruithe domhanda MIDI agus fuaime in aghaidh an tionscadail (ráta samplála, SoundFont, gléas MIDI)
- **Gabháil PortAudio (tógáil roghnach)**: Nuair a thógtar an tionscadal le PortAudio (`HAVE_PORTAUDIO`), is féidir leis an taifeadadh cosán ionchuir dhúchais PortAudio a úsáid (cosúil i spiorad le Audacity) in ionad Qt Multimedia. Ligeann **Project → Project Settings → Audio** duit **PortAudio** nó **Qt Multimedia** a roghnú agus gléas ionchuir PortAudio a phiocadh. Mura bhfuil PortAudio suiteáilte, éiríonn leis an tógáil fós agus úsáideann an taifeadadh Qt Multimedia amháin.
- **Tiontú ráta samplála ardcháilíochta**: Taifeadann ag ráta dúchais an ghléis fuaime agus tiontaíonn go ráta an tionscadail ag úsáid idirshuíomh sinc fuinneogaithe (fuinneog Kaiser, ~96 dB maolú banna coiscthe), an teaghlach algartam céanna a úsáidtear ag Audacity / libsoxr. Ligeann sé seo taifeadadh ag aon ráta samplála tionscadail (8000 Hz go 192000 Hz) beag beann ar ráta dúchais an ghléis, gan athrú ar thuinairde ná fad.
- **Láimhseáil uathoibríoch monafónach/steiréafónach**: Braithíonn gléasanna atá go fisiciúil monafónach ach a fhógraítear mar steiréafónacha (coitianta le micreafóin ceamara gréasáin USB ar PipeWire) agus tiontaíonn idir monafónach agus steiréafónach de réir mar is gá (dúbailte nó meánú), ag leanúint cur chuige ródú cainéil Audacity
- **Éifeachtaí insert le linn taifeadta (rianta fuaime amháin)**: Úsáid an cnaipe **Effects** (faoi **Options** ar gach rian fuaime) chun an dialóg **Track effects** a oscailt. Cuir **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion** agus **Amp & cabinet** (samhaltú sainmhéadra agus cáibín) leis agus cumraigh iad, tarraing **≡** chun an slabhra a athordú (an ceann is airde ar dtús) agus sábháil leis an tionscadal. Cuirtear éifeachtaí i bhfeidhm ar an gcuid taifeadta nuair a stopann tú an taifeadadh; fanann paraiméadair in aonaid fhíor (ms, Hz) i gcomhréir le mónó nó stirió agus rátaí samplála tíosúla an tionscadail (8 kHz–192 kHz) tar éis an ghabháil a normalú. Féach an [lámhleabhar úsáideora](docs/MusiciansCanvas_User_Manual_ga.md). Is féidir le haistritheoirí teaghráin a nuashonrú go bulc trí `scripts/effect_i18n.tsv` (gintear ag `scripts/build_effect_tsv.py`) agus `scripts/fill_effect_i18n.py` tar éis `lupdate`.
- **Éifeachtaí meascáin (bus mór)**: **Project → Project Settings → Mix Effects** cumraíonn slabhra ar an **meascán iomlán** (cineálacha agus ord céanna le rian). Feidhmíonn sé agus **gach rian á sheinm** agus **comhad meascáin á onnmhairiú** (WAV/FLAC). Sábháiltear i `project.json` faoin `mixEffectChain`.
- **Frithghearradh digiteach**: DSP ionsuite a bhíonn ag teorannú bog roimh PCM 16-giotán. **EffectWidget**: `guardFloatSampleForInt16Pcm()` agus `softLimitFloatSampleForInt16Pcm()`.
- **Monatóireacht taifeadta**: **Monatóir fuaim agus í á taifeadadh** (taobh thiar den chlog) seolann **ionchur beo** go **aschur fuaime an tionscadail** le linn gabhála. **Fuaim** — an rud céanna atá á thaifeadadh; **MIDI** — sintéiseoir bog le rindreáil MIDI agus SoundFont. Sábháiltear san **tionscadal** (`monitorWhileRecording`). Múch chun aischuir a laghdú.
- **Fuaim íseal-mhoille**: Ar Windows, cinntíonn brath tiománaí ASIO fuaim íseal-mhoille; ar Linux, ardaítear tosaíocht sceidealaithe próisis le haghaidh nervousness níos ísle le PipeWire / PulseAudio / ALSA
- **Cuir rian drumaí leis**: **Tools → Cuir rian drumaí leis** (aicearra **D**) cuireann sé rian MIDI ar chainéal 10 General MIDI (drumaí) agus scríobhann sé comhad gearr `.mid` dhá bharra sa chomhadlann tionscadail. Má tá **Metronome** ag **Cumasaigh an mhéadrán le linn taifeadta**, úsáidtear an BPM sin; nó meastar ó fhuaim chumasaithe nó 120 BPM. Féach an [lámhleabhar úsáideora](docs/MusiciansCanvas_User_Manual_ga.md).
- **Virtual MIDI Keyboard**: Feidhmchlár tionlacain chun nótaí MIDI a sheoladh trí mhéarchlár pianó bogearraí, le sintéiseoir FluidSynth ionsuite, gnóthachan máistir inchóirithe, mapáil méarchlár ríomhaire go pianó, roghnú ionstraime/cláir, rialú cóir/éifeacht agus aistriú ochtach
- **Cumraíocht**: Roghnú gléas ionchuir fuaime, gléas MIDI agus comhad SoundFont (réamhshocruithe domhanda agus sáruithe in aghaidh an tionscadail)
- **Téama dorcha / éadrom**: Inchumraithe trí Settings → Configuration
- **Logánú**: Tá an dá fheidhmchlár logánaithe i 18 dteanga: Béarla, Gearmáinis, Spáinnis, Fraincis, Seapáinis, Portaingéilis (an Bhrasaíl), Sínis Thraidisiúnta, Sínis Shimplithe, Rúisis, Sualainnis, Fionlainnis, Danmhairgis, Ioruais, Polainnis, Gréigis, Gaeilge, Breatnais agus Foghlaeireacht. Socraítear an teanga de réir réamhshocraithe ó shocrú an chórais oibriúcháin agus is féidir é a athrú i **Settings → Configuration → Language**. Úsáidtear córas caighdeánach i18n Qt (`QTranslator` / `.ts` / `.qm`), agus nuashonraítear an comhéadan úsáideora láithreach nuair a athraítear an teanga.
- **Lámhleabhar úsáideora**: Doiciméadacht HTML agus PDF ginte ó fhoinse Markdown, ar fáil i ngach teanga tacaithe (féach [docs/](docs/))

## Gabhálacha scáileáin

<p align="center">
	<a href="screenshots/i18n/ga/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/i18n/ga/MusiciansCanvas_1_MainWin.png" alt="Príomhfhuinneog" width="800"></a>
	<a href="screenshots/i18n/ga/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/i18n/ga/MusiciansCanvas_2_TrackConfig.png" alt="Cumraíocht riain" width="800"></a>
	<a href="screenshots/i18n/ga/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/i18n/ga/MusiciansCanvas_3_GeneralSettings.png" alt="Socruithe ginearálta" width="800"></a>
	<a href="screenshots/i18n/ga/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/i18n/ga/MusiciansCanvas_4_MIDISettings.png" alt="Réamhshocruithe/socruithe ginearálta MIDI" width="800"></a>
	<a href="screenshots/i18n/ga/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/i18n/ga/MusiciansCanvas_5_AudioSettings.png" alt="Réamhshocruithe/socruithe ginearálta fuaime" width="800"></a>
	<a href="screenshots/i18n/ga/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/i18n/ga/MusiciansCanvas_DisplaySettings.png" alt="Socruithe taispeána" width="800"></a>
	<a href="screenshots/i18n/ga/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/i18n/ga/MusiciansCanvas_LanguageSettings.png" alt="Socruithe teanga" width="800"></a>
	<a href="screenshots/i18n/ga/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/i18n/ga/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Socruithe MIDI sainiúla tionscadail" width="800"></a>
	<a href="screenshots/i18n/ga/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/i18n/ga/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Socruithe fuaime sainiúla tionscadail" width="800"></a>
	<a href="screenshots/i18n/ga/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/i18n/ga/Track_Effects_Dialog.png" alt="Éifeachtaí rian" width="800"></a>
	<a href="screenshots/i18n/ga/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/i18n/ga/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/i18n/ga/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/i18n/ga/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/i18n/ga/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/i18n/ga/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/i18n/ga/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/i18n/ga/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Spleáchais

### Tiomsaitheoir

Tá tiomsaitheoir atá in ann **C++17** ag teastáil (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (riachtanach ar gach ardán)

Braitheann feidhmiúlacht an mheasctha agus easpórtáil FLAC ar leabharlann
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp).
Clónaigh é mar chomhadlann deirfiúr den stóras seo roimh thógáil:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

Tá an tógáil CMake ag súil go bhfaighidh sé é ag `../audio_mixer_cpp` i gcoibhneas le fréamh an tionscadail seo.

---

### Qt6 Multimedia (roghnach ach molta go láidir)

Is roghnach é `qt6-multimedia`. Gan é, tógtar agus ritheann an feidhmchlár fós, ach
**tá taifeadadh agus athsheinm fuaime díchumasaithe** — ní oibreoidh ach eagarthóireacht MIDI agus bainistiú tionscadail. Suiteáil é in éineacht le leabharlanna croí Qt6 ag úsáid na n-orduithe ardáin thíos.

---

### PortAudio (roghnach)

Má tá leabharlann agus ceanntásca PortAudio suiteáilte, cumasaíonn CMake **`HAVE_PORTAUDIO`** agus
nascann é le **musicians_canvas**. Úsáideann an taifeadadh an cosán PortAudio de réir réamhshocraithe mura
bhfuil an tionscadal socraithe go **Qt Multimedia** i **Project → Project Settings → Audio**. Mura
bhfaightear PortAudio, leanann an tógáil gan é (tiomsaítear an wrapper mar stuib).

---

### SoundFont le haghaidh sintéise MIDI

Tá comhad SoundFont (`.sf2`) ag teastáil chun go ndéanfaidh rianta MIDI fuaim. Gan ceann,
rindreálfar rianta MIDI mar chiúnas. Ar **Linux**, braithfidh an sintéiseoir FluidSynth ionsuite
SoundFont go huathoibríoch má tá ceann suiteáilte ar chosán caighdeánach córais
(féach na hainmneacha pacáiste thíos). Ar **macOS agus Windows** níl aon chosán caighdeánach córais ann,
mar sin ní mór duit an SoundFont a chumrú de láimh i
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

> Suiteálann `fluid-soundfont-gm` `FluidR3_GM.sf2` go `/usr/share/sounds/sf2/` agus
> braitear go huathoibríoch é ag tosú. Is rogha níos lú é `timgm6mb-soundfont`.
>
> Tá `libpipewire-0.3-dev` ag teastáil ar chórais bunaithe ar PipeWire ionas gur féidir le Virtual MIDI
> Keyboard `pw_init()` a ghlaoch sula gcruthaíonn FluidSynth a acmhainní fuaime.
> Leanann an tógáil gan é; ní shainítear an bhratach `HAVE_PIPEWIRE` go simplí.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel libsndfile-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> Suiteálann `fluid-soundfont-gm` `FluidR3_GM.sf2` go `/usr/share/soundfonts/` agus
> braitear go huathoibríoch é ag tosú.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac libsndfile portaudio \
  pipewire \
  soundfont-fluid
```

> Suiteálann `soundfont-fluid` `FluidR3_GM.sf2` go `/usr/share/soundfonts/` agus
> braitear go huathoibríoch é ag tosú. De ghnáth bíonn `pipewire` suiteáilte cheana féin ar chórais
> nua-aimseartha Arch; tá na ceanntásca forbartha san áireamh sa phríomhphacáiste.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac libsndfile portaudio
```

> Is fochóras Linux amháin é PipeWire agus **ní** theastaíonn sé ar macOS. Úsáidfidh FluidSynth
> CoreAudio go huathoibríoch tríd an gcúlchóras Qt Multimedia.
>
> Íoslódáil SoundFont General MIDI (m.sh.
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) nó
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> agus cumraigh a chosán i **Settings → Configuration → MIDI**.

---

### Windows

Teastaíonn **tiománaí fuaime ASIO** ó **musicians_canvas** le haghaidh taifeadta agus athsheinm íseal-mhoille.
Ní thosóidh an feidhmchlár mura mbraitear tiománaí ASIO.
Ní theastaíonn ASIO ó **virtual_midi_keyboard** agus úsáideann sé cúlchóras WASAPI Qt Multimedia go díreach.

**Suiteáil tiománaí ASIO do musicians_canvas (roghnaigh ceann):**

| Tiománaí | Nótaí |
|----------|-------|
| [ASIO4ALL](https://asio4all.org/) | Saor in aisce, oibríonn le formhór na crua-earraí fuaime ionsuite agus USB |
| Tiománaí déantóra | Moill is fearr d'idirchéimeanna fuaime tiomnaithe (m.sh. Focusrite, PreSonus, RME) |

**Slabhra uirlisí — MSYS2 (molta do thógálacha MinGW):**

Íoslódáil agus suiteáil MSYS2 ó <https://www.msys2.org>, ansin cuir
`C:\msys64\mingw64\bin` le do `PATH` córais. Suiteáil na pacáistí riachtanacha:

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

> Suiteálann `mingw-w64-x86_64-soundfont-fluid` `FluidR3_GM.sf2` go
> `C:\msys64\mingw64\share\soundfonts\` (coigeartaigh má tá MSYS2 suiteáilte in áit eile).
> Toisc nach bhfuil cosán cuardaigh SoundFont caighdeánach ag Windows, ní mór duit an cosán seo a chumrú
> de láimh i **Settings → Configuration → MIDI** tar éis an chéad lainseáil.
>
> Is fochóras Linux amháin é PipeWire; ní theastaíonn pacáiste PipeWire ar Windows.

Leathanaigh tagartha pacáistí:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Slabhra uirlisí — Visual Studio 2022:**

Suiteáil Qt 6 tríd an [Qt Online Installer](https://www.qt.io/download) agus faigh
dénártha FluidSynth, RtMidi agus libFLAC (nó tóg iad ón gcód foinse).
Aimseoidh an tógáil CMake iad trí `find_library` / `find_path` fad is atá na
comhadlanna cuí i `CMAKE_PREFIX_PATH`.

Ní mór SoundFont a íoslódáil go leithleach (m.sh.
[GeneralUser GS](https://schristiancollins.com/generaluser.php) nó
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)) agus a
chosán a chumrú i **Settings → Configuration → MIDI** tar éis an chéad lainseáil.

> **Nóta:** Nasctar `advapi32` (clárlann Windows) agus `winmm` (Windows Multimedia)
> go huathoibríoch ag CMake; ní theastaíonn aon suiteáil bhreise dóibh siúd.

---

## Tógáil

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

Mura bhfaightear na DLL-anna riachtanacha ag am rite, rith an script cúntóra ón gcomhadlann aschuir tógála:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Giniúint doiciméadachta

Is féidir an lámhleabhar úsáideora a ghiniúint mar HTML agus PDF ón bhfoinse Markdown:

```bash
cd docs
./generate_docs.sh          # Gin HTML agus PDF araon
./generate_docs.sh html     # Gin HTML amháin
./generate_docs.sh pdf      # Gin PDF amháin
```

**Réamhriachtanais:**

- **Python (rogha tosaíochta):** `pip3 install markdown weasyprint`
- **Rogha eile:** `pandoc` agus `wkhtmltopdf` (trí bhainisteoir pacáiste córais)

Scríobhtar an HTML ginte go `docs/html/` agus an PDF go `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Rith

```bash
./build/musicians_canvas
# nó
./build/virtual_midi_keyboard   # feidhmchlár tionlacain Virtual MIDI Keyboard
```

---

## Úsáid

### musicians_canvas

1. **Socraigh comhadlann tionscadail**: Cuir isteach nó brabhsáil go fillteán ina stórálfar taifeadtaí agus an comhad tionscadail
2. **Cuir rianta leis**: Cliceáil "+ Add Track"; ainmnigh gach rian sa réimse téacs in aice le "Options"
3. **Cumraigh cineál riain**: Cliceáil "Options" ar rian (nó cliceáil an deilbhín cineál riain idir "Options" agus an réimse ainm) chun Audio nó MIDI a roghnú agus an fhoinse ionchuir a shocrú
4. **Bain rian**: Cliceáil an cnaipe "×" ar thaobh na láimhe deise den ró riain
5. **Socruithe domhanda**: Úsáid **Settings → Configuration** chun réamhshocruithe domhanda a shocrú:
   - Cluaisín *General*: Téama (dorcha/éadrom)
   - Cluaisín *MIDI*: Réamhghléas aschuir MIDI (sintéiseoir FluidSynth ionsuite nó gléas MIDI seachtrach) agus réamh-SoundFont (`.sf2`) le haghaidh sintéise
   - Cluaisín *Audio*: Réamhghléas ionchuir/aschuir fuaime le haghaidh taifeadta agus athsheinm
6. **Socruithe tionscadail**: Úsáid **Project → Project Settings** (Ctrl+P) chun socruithe MIDI agus fuaime a shárú don tionscadal reatha amháin (m.sh. ráta samplála nó SoundFont difriúil in aghaidh an amhráin)
7. **Taifead**: Seiceáil "Arm" ar an rian sprice, ansin cliceáil an cnaipe taifeadta (ciorcal dearg). Ní féidir ach rian amháin a bheith armáilte ag an am
8. **Seinn**: Cliceáil an cnaipe seinnte chun na rianta cumasaithe go léir a mheascadh agus a sheinm
9. **Measc go comhad**: Úsáid **Tools → Mix tracks to file** (Ctrl+M) chun easpórtáil go WAV, FLAC, MP3, Ogg Vorbis nó AIFF. Do **MP3** agus **Ogg**, socraigh bitrate nó caighdeán sa rannóg **Encoding** roimh an meascadh.
10. **Sábháil / Oscail**: Úsáid **File → Save Project** (Ctrl+S) agus **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Oscail cumraíocht**: Úsáid an cnaipe nó roghchlár **Configuration** (Ctrl+,) chun an dialóg socruithe a oscailt
2. **Cluaisín MIDI**:
   - Roghnaigh gléas aschuir MIDI (sintéiseoir seachtrach crua-earraí/bogearraí) nó fág bán chun an sintéiseoir FluidSynth ionsuite a úsáid
   - Roghnaigh gléas ionchuir MIDI chun nótaí MIDI isteach a chur ar aghaidh chuig taispeáint an mhéarchláir
   - Coigeartaigh **Synthesizer Volume (Master Gain)** chun leibhéal aschuir an tsintéiseora ionsuite a rialú (10%–200%)
3. **Cluaisín Audio**: Roghnaigh an gléas aschuir fuaime a úsáideann an sintéiseoir ionsuite
4. **SoundFont**: Roghnaigh comhad SoundFont `.sf2` don sintéiseoir ionsuite (braitear go huathoibríoch ar Linux má tá SoundFont córais suiteáilte)
5. **Seinn nótaí**: Cliceáil eochracha ar mhéarchlár pianó an scáileáin, úsáid rialaitheoir MIDI ceangailte nó úsáid méarchlár an ríomhaire:
   - Ochtach íochtair: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = géaracha/maolacha
   - Ochtach uachtair: Q/W/E/R/T/Y/U/I/O/P = C go E, 2/3/5/6/7/9/0 = géaracha/maolacha
6. **Rialuithe barra uirlisí**: Coigeartaigh airde MIDI (0–127), ochtach (-3 go +5), leibhéal cóir/éifeachta agus roghnaigh ionstraimí

---

## Aicearraí méarchláir

**musicians_canvas:**

| Aicearra | Gníomh |
|----------|--------|
| Ctrl+S | Sábháil tionscadal |
| Ctrl+O | Oscail tionscadal |
| Ctrl+M | Measc rianta go comhad |
| Ctrl+P | Socruithe tionscadail |
| Ctrl+, | Socruithe / Cumraíocht |
| Ctrl+Q / Alt+F4 | Scoir |

**virtual_midi_keyboard:**

| Aicearra | Gníomh |
|----------|--------|
| Ctrl+, | Dialóg cumraíochta |
| Ctrl+U | Cabhair / Eolas úsáide |
| Ctrl+Q | Dún |

---

## Ceadúnas

Cuirtear an tionscadal seo ar fáil mar atá sé le haghaidh úsáid oideachais agus pearsanta.
