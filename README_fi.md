# Musician's Canvas

Tämä on moniraitainen musiikin tallennussovellus pöytätietokoneille. Se on kirjoitettu C++:lla ja käyttää Qt:tä
graafisen käyttöliittymän kirjastona. Tämä sovellus on tarkoitettu rakennettavaksi Linuxille, Windowsille ja Mac OS:lle. Helppokäyttöisyys on
tärkein suunnitteluperiaate. Tämä projekti alkoi kokeiluna Cursorin (tekoälykoodaustyökalu) kanssa nähdäkseen, mitä
se pystyisi luomaan tämänkaltaisena sovelluksena, jatkuvalla tekoälykehityksellä sekä
perinteisellä ohjelmistokehityksellä.

Vuosia sitten tein moniraitaisen musiikin tallennusohjelman yliopistossa (samalla nimellä); tämä on yritys
tehdä jotain parempaa.

## Ominaisuudet

- **Moniraitainen sovitus**: Lisää ja poista useita raitoja laulujen säveltämiseen
- **Nimetyt raidat**: Jokaisella raidalla on muokattava nimi, jota käytetään tallennetun äänen tiedostonimenä
- **Raitatyypit**: Määritä jokainen raita äänitallennukseen (mikrofonista/linjasisääntulosta) tai MIDI-tallennukseen (koskettimistolta/ohjaimelta); napsauta raitatyyppi-kuvaketta vaihtaaksesi syöttölähteen nopeasti
- **Raidan virittäminen**: Valitse raidan "Arm" valitaksesi sen tallennuskohteeksi; vain yksi raita voi olla viritettynä kerrallaan. Yksittäinen tallentamaton raita viritetään automaattisesti käyttömukavuuden vuoksi
- **Tallennuksen lähtölaskenta**: Visuaalinen 3 sekunnin lähtölaskenta ennen tallennuksen alkua, joka antaa esittäjälle aikaa valmistautua
- **Overdub-tallennus**: Kun tallennat uutta raitaa olemassa olevien raitojen ollessa käytössä, olemassa olevat raidat miksataan ja toistetaan reaaliaikaisesti, jotta voit kuulla ne tallennuksen aikana. Toisto ja tallennus on synkronoitu pitämään kaikki raidat kohdistettuina
- **Visuaalinen palaute**: Ääniaaltokäyrän näyttö ääniraidoille (live-tasomittarilla tallennuksen aikana), MIDI-pianorulla MIDI-raidoille
- **Sisäänrakennettu MIDI-syntetisaattori**: Renderöi MIDI-raidat ääneksi FluidSynth-ohjelmalla konfiguroitavalla SoundFont-tiedostolla
- **Miksaus yhdeksi äänitiedostoksi**: Vie kaikki käytössä olevat raidat yhdeksi miksatuksi WAV- tai FLAC-tiedostoksi [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-kirjastolla
- **Tallenna / Avaa projekti**: Sarjallista ja palauta koko projekti (raidat, nimet, tyypit, MIDI-nuotit, äänitiedostoviittaukset) JSON-tiedostoon/tiedostosta, tallentamattomien muutosten tunnistuksella sulkemisen yhteydessä
- **Projektikohtaiset asetukset**: Ohita yleiset MIDI- ja ääniasetukset projektikohtaisesti (näytteenottotaajuus, SoundFont, MIDI-laite)
- **PortAudio-tallennus (valinnainen koonti)**: Kun projekti rakennetaan PortAudio-tuella (`HAVE_PORTAUDIO`), tallennus voi käyttää natiivia PortAudio-syöttöpolkua (hengeltään samanlainen kuin Audacity) Qt Multimedian sijaan. **Project → Project Settings → Audio** antaa valita **PortAudio** tai **Qt Multimedia** ja valita PortAudio-syöttölaitteen. Jos PortAudio ei ole asennettu, koonti onnistuu silti ja tallennus käyttää vain Qt Multimediaa.
- **Korkealaatuinen näytteenottotaajuuden muunnos**: Tallentaa äänilaitteen natiivitaajuudella ja muuntaa projektitaajuuteen ikkunoidulla sinc-interpolaatiolla (Kaiser-ikkuna, ~96 dB päästökaistan vaimennus), sama algoritmiperhe kuin Audacityssä / libsoxr:ssä. Tämä mahdollistaa tallennuksen millä tahansa projektin näytteenottotaajuudella (8000 Hz - 192000 Hz) riippumatta laitteen natiivitaajuudesta, ilman sävelkorkeuden tai keston muutosta.
- **Automaattinen mono/stereo-käsittely**: Tunnistaa fyysisesti mono-laitteet, jotka ilmoittavat olevansa stereo (yleistä USB-webkameramikrofoneilla PipeWirellä), ja muuntaa monon ja stereon välillä tarpeen mukaan (kahdentaminen tai keskiarvoistus), Audacityn kanavaroututusmenetelmän mukaisesti
- **Tallennukseen liitetyt insert-efektit (ääniraidat)**: **Effects**-painike (**Options**-painikkeen alla jokaisella ääniraidalla) avaa **Track effects** -valintaikkunan. Lisää ja säädä **Reverb**-, **Chorus**-, **Flanger**-, **Overdrive / distortion**- ja **Amp & cabinet** -efektejä (vahvistimen ja kaapin mallinnus), vedä **≡**-kahvaa järjestääksesi ketjun uudelleen (ylimmäinen ajetaan ensin) ja tallenna projektin mukana. Efektit kohdistuvat otokseen tallennuksen lopetettaessa; parametrit todellisissa yksiköissä (ms, Hz) pitävät käyttäytymisen yhdenmukaisena mono- tai stereotilassa ja tyypillisillä projektin näytteenottotaajuuksilla (8 kHz–192 kHz) tallennuksen normalisoinnin jälkeen. Lisätietoja: [käyttöohje](docs/MusiciansCanvas_User_Manual_fi.md). Kääntäjät voivat päivittää merkkijonoja eränä tiedostolla `scripts/effect_i18n.tsv` (tuottaa `scripts/build_effect_tsv.py`) ja `scripts/fill_effect_i18n.py` komennon `lupdate` jälkeen.
- **Miksausefektit (pääväylä)**: **Project → Project Settings → Mix Effects** määrittää efektiketjun **koko miksille** (samat tyypit ja järjestys kuin raidalla). Käytössä **toista kaikki raidat** -toiminnossa ja **miksaa tiedostoon** (WAV/FLAC). Tallennetaan projektiin `project.json`-kenttään `mixEffectChain`.
- **Pehmeä rajoitus ennen 16-bittistä PCM:ää**: Sisäänrakennettu DSP vähentää kovaa digitaalista leikkausta. **EffectWidget** tarjoaa `guardFloatSampleForInt16Pcm()` ja `softLimitFloatSampleForInt16Pcm()` uutta reaaliaikaista käsittelyä varten.
- **Monitorointi tallennuksessa**: **Kuuntele ääntä tallennuksen aikana** (ajanäytön oikealla) lähettää **suoran sisääntulon** **projektin äänilähtöön** tallennuksen aikana. **Ääni**: sama signaali kuin tallennus; **MIDI**: renderöinti ääneksi ja SoundFont — kuulet softasyntikan kautta. Asetus **projektissa** (`monitorWhileRecording`). Poista käytöstä palautteen välttämiseksi.
- **Matalan viiveen ääni**: Windowsissa ASIO-ajurin tunnistus varmistaa matalan viiveen äänen; Linuxissa prosessin ajoitusprioriteettia nostetaan vähemmän jitterin saavuttamiseksi PipeWiren / PulseAudion / ALSA:n kanssa
- **Lisää rumpuraita**: **Tools → Lisää rumpuraita** (pikanäppäin **D**) lisää MIDI-raidan General MIDI -kanavalla 10 (rummut) ja kirjoittaa lyhyen kahden tahdin `.mid`-tiedoston projektikansioon. Jos **Metronomissa** on **Käytä metronomia tallennuksen aikana**, käytetään sen BPM:ää; muuten tempo arvioidaan käytössä olevasta äänestä tai oletus 120 BPM. Katso [käyttöohje](docs/MusiciansCanvas_User_Manual_fi.md).
- **Virtual MIDI Keyboard**: Seuralaissovellus MIDI-nuottien lähettämiseen ohjelmistopianokoskettimiston kautta, sisäänrakennetulla FluidSynth-syntetisaattorilla, säädettävällä pääkanavan vahvistuksella, tietokoneen näppäimistöstä pianoon -kartoituksella, instrumentin/ohjelman valinnalla, kuoro-/efektisäädöllä ja oktaavisiirrolla
- **Asetukset**: Äänisisääntulolaitteen, MIDI-laitteen ja SoundFont-tiedoston valinta (yleiset oletusarvot ja projektikohtaiset ohitukset)
- **Tumma / vaalea teema**: Määritettävissä kohdassa Settings → Configuration
- **Lokalisointi**: Molemmat sovellukset on lokalisoitu 18 kielelle: englanti, saksa, espanja, ranska, japani, portugali (Brasilia), perinteinen kiina, yksinkertaistettu kiina, venäjä, ruotsi, suomi, tanska, norja, puola, kreikka, irlanti, kymri ja merirosvo. Kieli on oletuksena käyttöjärjestelmän asetuksen mukainen ja sitä voi muuttaa kohdassa **Settings → Configuration → Language**. Qt:n vakio i18n-järjestelmää (`QTranslator` / `.ts` / `.qm`) käytetään, ja käyttöliittymä päivittyy välittömästi kielen vaihtuessa.
- **Käyttöohje**: Markdown-lähteestä generoitu HTML- ja PDF-dokumentaatio, saatavilla kaikilla tuetuilla kielillä (katso [docs/](docs/))

## Kuvakaappaukset

<p align="center">
	<a href="screenshots/i18n/fi/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/i18n/fi/MusiciansCanvas_1_MainWin.png" alt="Pääikkuna" width="800"></a>
	<a href="screenshots/i18n/fi/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/i18n/fi/MusiciansCanvas_2_TrackConfig.png" alt="Raidan asetukset" width="800"></a>
	<a href="screenshots/i18n/fi/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/i18n/fi/MusiciansCanvas_3_GeneralSettings.png" alt="Yleiset asetukset" width="800"></a>
	<a href="screenshots/i18n/fi/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/i18n/fi/MusiciansCanvas_4_MIDISettings.png" alt="Oletus-/yleiset MIDI-asetukset" width="800"></a>
	<a href="screenshots/i18n/fi/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/i18n/fi/MusiciansCanvas_5_AudioSettings.png" alt="Oletus-/yleiset ääniasetukset" width="800"></a>
	<a href="screenshots/i18n/fi/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/i18n/fi/MusiciansCanvas_DisplaySettings.png" alt="Näyttöasetukset" width="800"></a>
	<a href="screenshots/i18n/fi/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/i18n/fi/MusiciansCanvas_LanguageSettings.png" alt="Kieliasetukset" width="800"></a>
	<a href="screenshots/i18n/fi/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/i18n/fi/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Projektikohtaiset MIDI-asetukset" width="800"></a>
	<a href="screenshots/i18n/fi/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/i18n/fi/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Projektikohtaiset ääniasetukset" width="800"></a>
	<a href="screenshots/i18n/fi/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/i18n/fi/Track_Effects_Dialog.png" alt="Raidan tehosteet" width="800"></a>
	<a href="screenshots/i18n/fi/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/i18n/fi/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/i18n/fi/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/i18n/fi/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/i18n/fi/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/i18n/fi/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/i18n/fi/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/i18n/fi/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Riippuvuudet

### Kääntäjä

Tarvitaan **C++17**-yhteensopiva kääntäjä (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (vaaditaan kaikilla alustoilla)

Miksaus- ja FLAC-vientitoiminnallisuus riippuu
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)-kirjastosta.
Kloonaa se tämän arkiston sisarhakemistoksi ennen koontiversion tekemistä:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

CMake-koonti odottaa löytävänsä sen kohdasta `../audio_mixer_cpp` suhteessa tämän projektin juurihakemistoon.

---

### Qt6 Multimedia (valinnainen mutta vahvasti suositeltu)

`qt6-multimedia` on valinnainen. Ilman sitä sovellus rakennetaan ja suoritetaan silti, mutta
**äänen tallennus ja toisto ovat pois käytöstä** — vain MIDI-muokkaus ja projektinhallinta
toimivat. Asenna se yhdessä Qt6-ydinkirjastojen kanssa alla olevilla alustakomennoilla.

---

### PortAudio (valinnainen)

Jos PortAudio-kirjasto ja otsikkotiedostot on asennettu, CMake ottaa käyttöön **`HAVE_PORTAUDIO`** ja
linkittää sen **musicians_canvas**-sovellukseen. Tallennus käyttää oletuksena PortAudio-polkua, ellei
projekti ole asetettu käyttämään **Qt Multimedia** -vaihtoehtoa kohdassa **Project → Project Settings → Audio**. Jos PortAudiota
ei löydy, koonti jatkuu ilman sitä (kääreluokka käännetään tynkinä).

---

### SoundFont MIDI-synteesiin

SoundFont-tiedosto (`.sf2`) vaaditaan, jotta MIDI-raidat tuottavat ääntä. Ilman sitä
MIDI-raidat renderöidään hiljaisuutena. **Linuxissa** sisäänrakennettu FluidSynth-syntetisaattori
tunnistaa automaattisesti SoundFontin, jos sellainen on asennettu vakiojärjestelmäpolkuun
(katso pakettien nimet alla). **macOS:ssä ja Windowsissa** ei ole vakiojärjestelmäpolkua,
joten SoundFont on määritettävä manuaalisesti kohdassa
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

> `fluid-soundfont-gm` asentaa `FluidR3_GM.sf2` kohteeseen `/usr/share/sounds/sf2/` ja
> tunnistetaan automaattisesti käynnistyksen yhteydessä. `timgm6mb-soundfont` on pienempi vaihtoehto.
>
> `libpipewire-0.3-dev` vaaditaan PipeWire-pohjaisissa järjestelmissä, jotta Virtual MIDI
> Keyboard voi kutsua `pw_init()` ennen kuin FluidSynth luo ääniresurssinsa.
> Koonti jatkuu ilman sitä; `HAVE_PIPEWIRE`-lippu jää yksinkertaisesti määrittelemättä.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel libsndfile-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` asentaa `FluidR3_GM.sf2` kohteeseen `/usr/share/soundfonts/` ja
> tunnistetaan automaattisesti käynnistyksen yhteydessä.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac libsndfile portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` asentaa `FluidR3_GM.sf2` kohteeseen `/usr/share/soundfonts/` ja
> tunnistetaan automaattisesti käynnistyksen yhteydessä. `pipewire` on tyypillisesti jo asennettu moderneissa
> Arch-järjestelmissä; kehitysotsikkotiedostot sisältyvät pääpakettiin.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac libsndfile portaudio
```

> PipeWire on vain Linuxille tarkoitettu alijärjestelmä eikä sitä **vaadita** macOS:ssä. FluidSynth
> käyttää automaattisesti CoreAudiota Qt Multimedia -taustajärjestelmän kautta.
>
> Lataa General MIDI SoundFont (esim.
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) tai
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> ja määritä sen polku kohdassa **Settings → Configuration → MIDI**.

---

### Windows

**musicians_canvas** vaatii **ASIO-ääniajurin** matalan viiveen tallennukseen ja toistoon.
Sovellus ei käynnisty, jos ASIO-ajuria ei tunnisteta.
**virtual_midi_keyboard** ei vaadi ASIO:ta ja käyttää suoraan Qt Multimedian WASAPI-taustajärjestelmää.

**ASIO-ajurin asentaminen musicians_canvas-sovellukselle (valitse yksi):**

| Ajuri | Huomautukset |
|-------|--------------|
| [ASIO4ALL](https://asio4all.org/) | Ilmainen, toimii useimpien sisäänrakennettujen ja USB-äänilaitteiden kanssa |
| Valmistajan ajuri | Paras viive dedikoituille äänirajapinnoille (esim. Focusrite, PreSonus, RME) |

**Työkaluketju — MSYS2 (suositeltu MinGW-koonneille):**

Lataa ja asenna MSYS2 osoitteesta <https://www.msys2.org>, lisää sitten
`C:\msys64\mingw64\bin` järjestelmän `PATH`-muuttujaan. Asenna tarvittavat paketit:

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

> `mingw-w64-x86_64-soundfont-fluid` asentaa `FluidR3_GM.sf2` kohteeseen
> `C:\msys64\mingw64\share\soundfonts\` (muuta, jos MSYS2 on asennettu muualle).
> Koska Windowsissa ei ole vakio SoundFont-hakupolkua, tämä polku on määritettävä
> manuaalisesti kohdassa **Settings → Configuration → MIDI** ensimmäisen käynnistyksen jälkeen.
>
> PipeWire on vain Linuxille tarkoitettu alijärjestelmä; PipeWire-pakettia ei tarvita Windowsissa.

Pakettien viitesivut:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Työkaluketju — Visual Studio 2022:**

Asenna Qt 6 [Qt Online Installer](https://www.qt.io/download) -asennusohjelmalla ja hanki
FluidSynth-, RtMidi- ja libFLAC-binäärit (tai käännä ne lähdekoodista).
CMake-koonti löytää ne `find_library` / `find_path` -komennoilla, kunhan
asianmukaiset hakemistot ovat `CMAKE_PREFIX_PATH`-muuttujassa.

SoundFont on ladattava erikseen (esim.
[GeneralUser GS](https://schristiancollins.com/generaluser.php) tai
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)) ja sen
polku on määritettävä kohdassa **Settings → Configuration → MIDI** ensimmäisen käynnistyksen jälkeen.

> **Huom:** `advapi32` (Windowsin rekisteri) ja `winmm` (Windows Multimedia)
> linkitetään automaattisesti CMaken toimesta; niille ei tarvita lisäasennusta.

---

## Koontiversio

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

Jos vaadittuja DLL-tiedostoja ei löydy suorituksen aikana, suorita apuskripti koontiversion tulostehakemistosta:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Dokumentaation generointi

Käyttöohje voidaan generoida HTML- ja PDF-muodossa Markdown-lähteestä:

```bash
cd docs
./generate_docs.sh          # Generoi sekä HTML että PDF
./generate_docs.sh html     # Generoi vain HTML
./generate_docs.sh pdf      # Generoi vain PDF
```

**Edellytykset:**

- **Python (suositeltu):** `pip3 install markdown weasyprint`
- **Vaihtoehto:** `pandoc` ja `wkhtmltopdf` (järjestelmän paketinhallinnan kautta)

Generoitu HTML kirjoitetaan kohteeseen `docs/html/` ja PDF kohteeseen `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Suorittaminen

```bash
./build/musicians_canvas
# tai
./build/virtual_midi_keyboard   # seuralaissovellus Virtual MIDI Keyboard
```

---

## Käyttö

### musicians_canvas

1. **Aseta projektihakemisto**: Syötä tai selaa kansioon, johon tallenteet ja projektitiedosto tallennetaan
2. **Lisää raitoja**: Napsauta "+ Add Track"; nimeä jokainen raita tekstikentässä "Options"-kohdan vieressä
3. **Määritä raitatyyppi**: Napsauta raidan "Options" (tai napsauta raitatyyppi-kuvaketta "Options"- ja nimikentän välissä) valitaksesi Audio tai MIDI ja asettaaksesi syöttölähteen
4. **Poista raita**: Napsauta "×"-painiketta raitarivin oikealla puolella
5. **Yleiset asetukset**: Käytä **Settings → Configuration** asettaaksesi yleiset oletusarvot:
   - *General*-välilehti: Teema (tumma/vaalea)
   - *MIDI*-välilehti: Oletus-MIDI-lähtölaite (sisäänrakennettu FluidSynth-syntetisaattori tai ulkoinen MIDI-laite) ja oletus-SoundFont (`.sf2`) synteesiin
   - *Audio*-välilehti: Oletus-äänen sisääntulo-/lähtölaite tallennukseen ja toistoon
6. **Projektiasetukset**: Käytä **Project → Project Settings** (Ctrl+P) ohittaaksesi MIDI- ja ääniasetukset vain nykyiselle projektille (esim. eri näytteenottotaajuus tai SoundFont per kappale)
7. **Tallenna**: Valitse kohderaidan "Arm", napsauta sitten tallennuspainiketta (punainen ympyrä). Vain yksi raita voi olla viritettynä kerrallaan
8. **Toista**: Napsauta toistopainiketta miksataksesi ja toistaaksesi kaikki käytössä olevat raidat
9. **Miksaa tiedostoon**: Käytä **Tools → Mix tracks to file** (Ctrl+M) viedäksesi WAV- tai FLAC-muotoon
10. **Tallenna / Avaa**: Käytä **File → Save Project** (Ctrl+S) ja **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Avaa asetukset**: Käytä **Configuration**-painiketta tai -valikkoa (Ctrl+,) avataksesi asetusten valintaikkunan
2. **MIDI-välilehti**:
   - Valitse MIDI-lähtölaite (ulkoinen laitteisto-/ohjelmistosyntetisaattori) tai jätä tyhjäksi käyttääksesi sisäänrakennettua FluidSynth-syntetisaattoria
   - Valitse MIDI-sisääntulolaite välittääksesi saapuvat MIDI-nuotit näppäimistönäyttöön
   - Säädä **Synthesizer Volume (Master Gain)** hallitaksesi sisäänrakennetun syntetisaattorin lähtötasoa (10 %–200 %)
3. **Audio-välilehti**: Valitse sisäänrakennetun syntetisaattorin käyttämä äänilähtölaite
4. **SoundFont**: Valitse `.sf2` SoundFont-tiedosto sisäänrakennetulle syntetisaattorille (tunnistetaan automaattisesti Linuxissa, jos järjestelmän SoundFont on asennettu)
5. **Soita nuotteja**: Napsauta näytön pianokoskettimiston näppäimiä, käytä liitettyä MIDI-ohjainta tai käytä tietokoneen näppäimistöä:
   - Alempi oktaavi: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = ylennysmerkit/alennusmerkit
   - Ylempi oktaavi: Q/W/E/R/T/Y/U/I/O/P = C:stä E:hen, 2/3/5/6/7/9/0 = ylennysmerkit/alennusmerkit
6. **Työkalurivin säätimet**: Säädä MIDI-äänenvoimakkuutta (0–127), oktaavia (-3 – +5), kuoro-/efektitasoa ja valitse instrumentteja

---

## Pikanäppäimet

**musicians_canvas:**

| Pikanäppäin | Toiminto |
|-------------|----------|
| Ctrl+S | Tallenna projekti |
| Ctrl+O | Avaa projekti |
| Ctrl+M | Miksaa raidat tiedostoon |
| Ctrl+P | Projektiasetukset |
| Ctrl+, | Asetukset / Määritykset |
| Ctrl+Q / Alt+F4 | Lopeta |

**virtual_midi_keyboard:**

| Pikanäppäin | Toiminto |
|-------------|----------|
| Ctrl+, | Asetusten valintaikkuna |
| Ctrl+U | Ohje / Käyttötiedot |
| Ctrl+Q | Sulje |

---

## Lisenssi

Tämä projekti tarjotaan sellaisenaan opetus- ja henkilökohtaiseen käyttöön.
