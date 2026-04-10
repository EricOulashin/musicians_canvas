---
title: "Musician's Canvas Käyttöopas"
subtitle: "Moniraitainen musiikin tallennussovellus"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "fi"
---

# Musician's Canvas Käyttöopas

## Johdanto

Musician's Canvas on moniraitainen musiikin tallennussovellus pöytätietokoneille. Se tukee
äänen tallennusta mikrofoneista ja linjasisääntuloista, MIDI-tallennusta koskettimistoista ja
ohjaimista sekä kaikkien raitojen miksausta yhdeksi WAV- tai FLAC-tiedostoksi. Lisäsovellus
Virtual MIDI Keyboard tarjoaa ohjelmistopianokoskettimiston MIDI-nuottien lähettämiseen.

Musician's Canvas on suunniteltu helppokäyttöiseksi, ja se tarjoaa samalla digitaalisille
äänityöasemille (DAW) tyypillisiä ominaisuuksia:

- Moniraitainen ääni- ja MIDI-tallennus
- Päälleäänitys olemassa olevien raitojen synkronoidulla toistolla
- Sisäänrakennettu FluidSynth MIDI-syntetisaattori SoundFont-tuella
- Korkealaatuinen näytteenottotaajuuden muunnos tallennukseen millä tahansa projektin näytteenottotaajuudella
- Automaattinen mono/stereo-laitetunnistus
- Projektikohtaiset asetukset projektikohtaisilla ohituksilla
- Miksaus WAV- tai FLAC-muotoon
- Tumma ja vaalea teema
- Lokalisoitu useille kielille (englanti, saksa, espanja, ranska, japani, portugali, kiina ja Pirate)
- Lisäsovellus Virtual MIDI Keyboard

## Aloittaminen

### Sovelluksen käynnistäminen

Suorita `musicians_canvas`-suoritustiedosto käännöshakemistosta tai asennuspaikasta:

```
./musicians_canvas
```

Ensimmäisellä käynnistyskerralla sovellus avautuu tyhjällä projektilla. Sinun on asetettava
projektihakemisto ennen tallennusta.

Käynnistettäessä sovellus käyttää tallennettua teemaa (tumma tai vaalea) ja, jos aiemmin käytetty
projektihakemisto sisältää `project.json`-tiedoston, projekti ladataan automaattisesti.

### Projektin asettaminen

1. **Aseta projektihakemisto**: Kirjoita tai selaa kansioon "Project Location"-kentässä ikkunan
   yläosassa. Tänne tallenteet ja projektitiedosto tallennetaan.

2. **Lisää raita**: Napsauta **+ Add Track** -painiketta. Uusi raita ilmestyy oletusasetuksilla.
   Jos se on projektin ainoa raita eikä sitä ole vielä tallennettu, se valmistellaan automaattisesti
   tallennusta varten.

3. **Nimeä raita**: Kirjoita nimi tekstikenttään "Options"-painikkeen viereen. Tätä nimeä käytetään
   tallennetun äänitiedoston tiedostonimenä.

![Pääikkuna](../screenshots/MusiciansCanvas_1_MainWin.png)

### Painikepalkki

Aivan valikkopalkin alapuolella on työkalupalkki pikakäyttöpainikkeineen:

![Painikepalkki](../screenshots/ButtonBar.png)

- **Open Project**: Sama kuin **File > Open Project** — avaa aiemmin tallennetun projektin.
- **Save Project**: Sama kuin **File > Save Project** — tallentaa nykyisen projektin. Tämä
  painike on käytössä vain, kun projektihakemisto on asetettu.
- **Project Settings**: Sama kuin **Project > Project Settings** — avaa projektin asetukset
  -valintaikkunan. Tämä painike on käytössä vain, kun projektihakemisto on asetettu.
- **Configuration**: Sama kuin **Settings > Configuration** — avaa sovelluksen yleisten
  asetusten valintaikkunan.
- **Metronome Settings**: Avaa metronomin asetusten valintaikkunan (katso Metronomi-osio alla).

### Projektien tallentaminen ja avaaminen

- **Tallenna**: Käytä **File > Save Project** (Ctrl+S) tallentaaksesi nykyisen projektin JSON-tiedostona
  projektihakemistoon.
- **Avaa**: Käytä **File > Open Project** (Ctrl+O) ladataksesi aiemmin tallennetun projektin.

Projektitiedosto (`project.json`) tallentaa raitojen nimet, tyypit, MIDI-nuotit, äänitiedostoviittaukset
ja kaikki projektikohtaiset asetukset. Äänitiedostot tallennetaan samaan hakemistoon kuin `project.json`
ja nimetään raitojensa mukaan (esim. `My_Track.flac`).

Jos suljet sovelluksen tallentamattomien muutosten kanssa, vahvistusvalintaikkuna kysyy, haluatko
tallentaa ennen poistumista.

## Raitojen hallinta

### Raitojen lisääminen ja poistaminen

- Napsauta **+ Add Track** lisätäksesi uuden raidan sovitukseen.
- Napsauta **x**-painiketta raitarivin oikealla puolella poistaaksesi sen.
- Napsauta **Clear Tracks** (punainen painike työkalupalkissa) poistaaksesi kaikki raidat. Vahvistus-
  valintaikkuna näytetään ennen toiminnon suorittamista.

### Raitojen lisääminen vetämällä ja pudottamalla

Kun projekti on auki, voit vetää yhden tai useamman tuetun äänitiedoston
tiedostonhallinnasta (Windowsin Resurssienhallinta, macOS Finder, Linuxin
tiedostonhallinta jne.) suoraan Musician's Canvas -ikkunaan lisätäksesi ne
uusiksi ääniraidoiksi.

- **Tuetut muodot:** `.wav` ja `.flac`. Muissa muodoissa olevat tiedostot
  ohitetaan hiljaisesti, ja lopuksi valintaikkuna luettelee, mitkä tiedostot
  ohitettiin.
- **Tiedostojen kopiointi:** Jos pudotettu tiedosto ei ole jo projektin
  hakemistossa, se kopioidaan sinne automaattisesti. Jos samanniminen tiedosto
  on jo projektin hakemistossa, sinulta kysytään, haluatko korvata sen.
- **Raidan nimi:** Tiedoston perusnimeä (ilman tunnistetta) käytetään uuden
  raidan nimenä. Esimerkiksi `Bass Line.wav`:n pudottaminen luo ääniraidan
  nimeltä "Bass Line".
- **Useita tiedostoja kerralla:** Useita tiedostoja voi vetää yhdessä; kukin
  tuettu tiedosto muodostaa oman raitansa yhdellä pudotuksella.
- **Kun pudotus hylätään:** Pudotukset hyväksytään vain, kun projekti on auki
  eikä Musician's Canvas **ole** parhaillaan toistamassa tai tallentamassa.
  Pysäytä toisto tai tallennus ensin, jos haluat vetää lisäraitoja.

### Raitatyypin määrittäminen

Jokainen raita voidaan määrittää joko **Audio**-raidaksi (mikrofoni-/linjasisääntulotallennus) tai
**MIDI**-raidaksi (koskettimisto-/ohjaintallennus).

Raitatyypin muuttaminen:

- Napsauta raidan **Options**-painiketta, tai
- Napsauta **raitatyypin kuvaketta** ("Options"-painikkeen ja nimikentän välissä)

Tämä avaa raidan asetusvalintaikkunan, jossa voit valita sisääntulon lähteen.

![Raidan asetukset](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Raidan ohjaimet

Jokainen raitarivi sisältää seuraavat ohjaimet:

- **Käyttöönottovalintaruutu**: Vaihtaa, sisällytetäänkö raita toistoon ja miksaukseen.
  Raidan poistaminen käytöstä poistaa automaattisesti myös tallennusvalmiuden, jos se oli asetettu.
- **Tallennusvalmiuspainike**: Valitsee tämän raidan tallennuskohteeksi. Vain yksi raita voi olla
  tallennusvalmiudessa kerrallaan; uuden raidan valmistelu poistaa automaattisesti aiemmin
  valmistellun raidan.
- **Nimikenttä**: Muokattava tekstikenttä raidan nimelle. Tätä nimeä käytetään äänitiedoston nimenä
  (virheelliset tiedostojärjestelmän merkit korvataan alaviivoilla).
- **Options-painike**: Avaa raidan asetusvalintaikkunan.
- **Tyyppikuvake**: Näyttää kaiutinkuvakkeen ääniraidoille tai pianokuvakkeen MIDI-raidoille.
  Napsautus avaa raidan asetusvalintaikkunan.
- **Poistopainike (x)**: Poistaa raidan projektista.

### Automaattinen tallennusvalmius

Kun projektissa on tasan yksi raita eikä sitä ole vielä tallennettu, se valmistellaan automaattisesti
tallennusta varten. Tämä pätee sekä lisättäessä ensimmäistä raitaa uuteen projektiin että avattaessa
olemassa olevaa projektia, jossa on yksi tyhjä raita.

### Raidan visualisointi

- **Ääniraidat** näyttävät tallennetun äänen aaltomuodon visualisoinnin. Kun ääntä ei ole tallennettu,
  alueella näkyy "No audio recorded".
- **MIDI-raidat** näyttävät pianorullavan visualisoinnin, joka näyttää tallennetut nuotit ruudukossa
  A0:sta C8:aan. Nuotit on väritetty iskuvoimakkuuden mukaan. Kun MIDI-dataa ei ole tallennettu,
  alueella näkyy "No MIDI data recorded".

## Tallennus

### Äänen tallennus

1. Varmista, että projektihakemisto on asetettu.
2. Valmistele kohderaita (valitse "Arm"-valintapainike).
3. Napsauta **Record**-painiketta (punainen ympyrä).
4. Raidalla näytetään 3 sekunnin lähtölaskenta ("Get ready... 3", "2", "1"), jonka jälkeen
   tallennus alkaa.
5. Tallennuksen aikana raidan aaltomuotoalueella näytetään reaaliaikainen tasomittari, joka näyttää
   nykyisen amplitudin gradienttipalkkina (vihreästä keltaiseen ja punaiseen) "Recording"-tekstillä.
6. Napsauta **Stop**-painiketta lopettaaksesi tallennuksen.

Tallennettu ääni tallennetaan FLAC-tiedostona projektihakemistoon raidan nimellä.

Tallennuksen ja toiston aikana kaikki interaktiiviset ohjaimet (raitapainikkeet, asetukset jne.)
poistetaan käytöstä tahattomien muutosten estämiseksi.

### Lisäys tehosteet (vain ääniraidat)

Ääniraidalla on **Tehosteet**-painike suoraan **Options**-painikkeen alla. Se avaa **Raidan tehosteet**
-valintaikkunan, jossa rakennat **järjestyksessä olevan ketjun** lisäystehosteita tämän raidan tallennukseen:

![Raidan tehosteet -valintaikkuna](../screenshots/Track_Effects_Dialog.png)

Saatavilla ovat mm. **Kaiku**, **Kuoro**, **Flanger**, **Overdrive / distortion** ja **Amp & cabinet** (vahvistimen ja kaapin mallinnus: vahvistintyyppi, kaappi, vahvistus, basso/keski/discantti, ilmavuus ja sekoitus). Kuvakaappaukset **Amp & cabinet** -tehosteesta:

![Amp & cabinet -mallinnus (1)](../screenshots/Amp_And_Cabinet_Model_1.png)

![Amp & cabinet -mallinnus (2)](../screenshots/Amp_And_Cabinet_Model_2.png)

- **Lisää tehoste…** ja valitse tehostetyyppi. Useita voi lisätä; punainen **✕** poistaa tehosteen.
- Vedä **≡** uudelleenjärjestämiseen. **Ylin** tehoste ajetaan **ensin**.
- ms ja Hz säilyttävät merkityksensä **projektin näytteenottotaajuuteen** muunnon jälkeen. **Mono** ja **stereo**
  ovat tuettuja (mono käsitellään dual-monona ja summataan takaisin yhdelle kanavalle).
- **OK** tallentaa projektiin; **Peruuta** palauttaa ketjun avaushetkeä vastaavaksi.

Tehosteet käytetään **tallennuksen päätyttyä**, tavallisen kaappauksen ja resamplauksen jälkeen. Asetukset
tallennetaan `project.json`-tiedostoon avaimella `audioEffectChain`.

### Monitorointi tallennuksen aikana

**Aikanäytön** vieressä **Kuuntele ääntä tallennuksen aikana** määrittää, lähetetäänkö **live-sisääntulo**
**projektin äänilähtöön** tallennuksen aikana:

- **Ääniraidat**: sama signaali kuuluu reaaliajassa (tallennusketju ennallaan). Lisäksi mahdollinen **overdub**-toisto.
- **MIDI-raidat**: jos **renderöi MIDI ääneksi toistoa varten** ja **SoundFont** on asetettu, nuotet kuuluvat ohjelmistosyntikan kautta. **Ulkoinen MIDI-lähtö** → käytä laitteen omaa monitorointia.

Asetus **tallennetaan projektiin** (`monitorWhileRecording` tiedostossa `project.json`). Poista käytöstä, jos haluat välttää mikrofonikieroutta.

#### Päälleäänitys

Kun tallennat uutta raitaa samalla kun muut käytössä olevat raidat sisältävät jo ääni- tai MIDI-dataa,
Musician's Canvas suorittaa päälleäänityksen: olemassa olevat raidat miksataan yhteen ja toistetaan
reaaliajassa uuden raidan tallennuksen aikana. Tämä mahdollistaa aiemmin tallennettujen osien kuuntelun
uutta osaa tallennettaessa.

Olemassa olevien raitojen miksaus valmistellaan ennen tallennuksen aloittamista, joten tallennus ja
toisto alkavat suunnilleen samanaikaisesti pitäen kaikki raidat synkronoituina.

#### Tallennustaustajärjestelmät

Musician's Canvas tukee kahta äänen tallennuksen taustajärjestelmää:

- **PortAudio** (oletus kun saatavilla): Tarjoaa luotettavan, matalan latenssin tallennuksen ja on
  suositeltu taustajärjestelmä.
- **Qt Multimedia**: Varatausjärjestelmä, joka käyttää Qt:n sisäänrakennettua äänen tallennusta.
  Käytetään, kun PortAudio ei ole saatavilla tai kun se on nimenomaisesti valittu projektiasetuksissa.

Tallennustaustajärjestelmä voidaan määrittää projektikohtaisesti kohdassa **Project > Project Settings >
Audio**.

#### Näytteenottotaajuus ja laitteiden käsittely

Musician's Canvas tallentaa äänsisääntulolaitteen omalla näytteenottotaajuudella ja muuntaa sitten
automaattisesti projektin määritettyyn näytteenottotaajuuteen korkealaatuisella uudelleennäytteistämisellä.
Tämä tarkoittaa, että voit asettaa minkä tahansa projektin näytteenottotaajuuden (esim. 44100 Hz tai
48000 Hz) riippumatta laitteen omasta taajuudesta. Muunnos säilyttää sävelkorkeuden ja keston tarkasti.

#### Monolaitteiden tunnistus

Jotkin äänilaitteet (esim. USB-webkameramikrofonit) ovat fyysisesti mono, mutta käyttöjärjestelmä
ilmoittaa ne stereona. Musician's Canvas tunnistaa tämän automaattisesti ja säätää kanavamäärän
vastaavasti. Jos projekti on määritetty stereoksi, monosignaali kopioidaan molempiin kanaviin.

### MIDI-tallennus

1. Aseta raitatyypiksi **MIDI** Options-painikkeella.
2. Varmista, että MIDI-sisääntulolaite on määritetty kohdassa **Settings > Configuration > MIDI**.
3. Valmistele raita ja napsauta Record.
4. Soita nuotteja MIDI-ohjaimellasi.
5. Napsauta Stop lopettaaksesi tallennuksen.

MIDI-nuotit näytetään pianorullavisualisoinnissa raidalla.

## Metronomi

Musician's Canvas sisältää sisäänrakennetun metronomin, jota voidaan käyttää tallennuksen
aikana pitämään tahtia yllä. Napsauta metronomipainiketta painikepalkissa (valikkopalkin
alapuolella) avataksesi metronomin asetusten valintaikkunan:

![Metronomin asetukset](../screenshots/MetronomeSettings.png)

Valintaikkuna tarjoaa:

- **Enable metronome during recording**: Kun valittu, metronomi toistaa tikityksen, kun
  tallennus on käynnissä. Tikitys toistetaan järjestelmän äänen kautta, eikä sitä **tallenneta**
  tallennettavalle raidalle.
- **Beats per minute**: Numeerinen syöte tempolle, lyönteinä minuutissa (BPM). Alue on
  20–300 BPM.

Kun metronomi on käytössä, se alkaa tikittää, kun tallennus varsinaisesti alkaa (3 sekunnin
lähtölaskennan päätyttyä) ja pysähtyy, kun tallennus loppuu.

## Toisto

Napsauta **Play**-painiketta miksataksesi ja toistaaksesi kaikki käytössä olevat raidat. Painikkeen
työkaluvihje muuttuu osoittamaan, toistetaanko vai tallennetaanko sen mukaan, onko raita
tallennusvalmiudessa. Käytöstä poistetut raidat (valitsemattomat) jätetään toiston ulkopuolelle.

Toiston aikana ääniraidat puretaan FLAC-tiedostoistaan ja MIDI-raidat renderöidään ääneksi
sisäänrakennetulla FluidSynth-syntetisaattorilla. Kaikki raidat miksataan yhteen ja toistetaan
järjestelmän äänilähtölaitteen kautta.

Napsauta **Stop**-painiketta lopettaaksesi toiston milloin tahansa.

## Miksaus tiedostoon

Käytä **Tools > Mix tracks to file** (Ctrl+M) viedäksesi kaikki käytössä olevat raidat yhdeksi
äänitiedostoksi. Valintaikkuna antaa sinun valita tulostuspolun ja muodon:

- **Tulostiedosto**: Selaa valitaksesi kohdetiedoston polun.
- **Muoto**: Valitse FLAC (häviötön pakkaus, pienemmät tiedostot) tai WAV (pakkaamaton).

Miksaus käyttää projektin määritettyä näytteenottotaajuutta. MIDI-raidat renderöidään käyttäen
määritettyä SoundFont-tiedostoa.

## Asetukset

### Yleiset asetukset

Käytä **Settings > Configuration** (Ctrl+,) asettaaksesi yleiset oletusarvot, jotka koskevat kaikkia
projekteja:

![Yleiset asetukset](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### Yleiset-välilehti

- **Teema**: Valitse tumman ja vaalean teeman väliltä.

#### Näyttö-välilehti

- **Numeronäytön LED-väri**: Valitse väri, jota käytetään pääikkunan työkalurivillä näkyvässä
  numeerisessa LED-aikanäytössä. Aktiiviset numerosegmentit piirretään valitulla värillä, ja
  passiiviset segmentit piirretään saman värin himmennettynä versiona. Saatavilla olevat värit
  ovat Light Red, Dark Red, Light Green, Dark Green, Light Blue, Dark Blue, Yellow, Orange,
  Light Cyan ja Dark Cyan. Oletusarvo on Light Green.

![Näyttöasetukset](../screenshots/MusiciansCanvas_DisplaySettings.png)

#### Kieli-välilehti

- **Kieli**: Valitse sovelluksen näyttökieli. Oletus on "System Default", joka käyttää käyttöjärjestelmän
  kieliasetusta. Saatavilla olevat kielet ovat englanti, Deutsch (saksa), Español (espanja),
  Français (ranska), japani, Português (brasilianportugali), Chinese (perinteinen), Chinese (yksinkertaistettu)
  ja Pirate.
  Käyttöliittymä päivittyy välittömästi kieltä vaihdettaessa.

![Kieliasetukset](../screenshots/MusiciansCanvas_LanguageSettings.png)

#### MIDI-välilehti

- **MIDI-lähtölaite**: Valitse sisäänrakennettu FluidSynth-syntetisaattori tai ulkoinen MIDI-laite.
  Käytä **Refresh**-painiketta etsiäksesi saatavilla olevia MIDI-laitteita uudelleen.
- **SoundFont**: Selaa `.sf2` SoundFont-tiedostoon MIDI-synteesiä varten. Linuxissa järjestelmän
  SoundFont voidaan tunnistaa automaattisesti, jos `fluid-soundfont-gm`-paketti on asennettu.
  Windowsissa ja macOS:ssä SoundFont-polku on määritettävä manuaalisesti.

![MIDI-asetukset](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Ääni-välilehti

- **Äänen sisääntulolaite**: Valitse mikrofoni tai linjasisääntulo tallennusta varten.
- **Äänen lähtölaite**: Valitse kaiutin tai kuulokkeet toistoa varten.

![Ääniasetukset](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Projektiasetukset

Käytä **Project > Project Settings** (Ctrl+P) ohittaaksesi yleiset oletusarvot vain nykyiselle
projektille. Tämä on hyödyllistä projekteille, jotka tarvitsevat tietyn näytteenottotaajuuden,
SoundFont-tiedoston tai äänilaitteen. Projektikohtaiset asetukset tallennetaan `project.json`-tiedostoon.

![Projektin MIDI-asetukset](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### MIDI-välilehti

- **MIDI-laite**: Valitse MIDI-laite tälle projektille tai jätä oletusarvoon käyttääksesi yleistä asetusta.
- **SoundFont**: Valitse SoundFont-tiedosto tälle projektille.
- **Refresh**: Etsi saatavilla olevia MIDI-laitteita uudelleen.

#### Ääni-välilehti

- **Äänen sisääntulolaite**: Valitse tallennuslaite tälle projektille.
- **Tallennuksen taustajärjestelmä** (kun PortAudio on saatavilla):
  - **PortAudio (native input)**: Suositeltu. Käyttää samaa äänikirjastoa kuin Audacity.
  - **Qt Multimedia**: Varavaihtoehto, joka käyttää Qt:n sisäänrakennettua äänen tallennusta.
- **PortAudio-sisääntulolaite**: Kun käytetään PortAudio-taustajärjestelmää, valitse tietty
  PortAudio-sisääntulolaite.
- **Äänen lähtölaite**: Valitse toistolaite tälle projektille.

##### Äänimuotoasetukset

- **Näytteenottotaajuus**: Valitse vakiotaajuuksista (8000 Hz - 192000 Hz). Laitteen oma taajuus on
  merkitty "(native)". Taajuudet, jotka vaativat uudelleennäytteistämistä, on merkitty "(resampled)".
  Voit valita minkä tahansa taajuuden laitteen kyvyistä riippumatta; Musician's Canvas suorittaa
  uudelleennäytteistämisen automaattisesti tarvittaessa.
- **Kanavat**: Mono tai stereo. Jos laite tukee vain monoa, stereovaihtoehto on poistettu käytöstä.

![Projektin ääniasetukset](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

## Valikot

### File-valikko

| Valikkokohta  | Pikanäppäin | Kuvaus                                  |
|---------------|-------------|-----------------------------------------|
| Save Project  | Ctrl+S      | Tallenna nykyinen projekti levylle      |
| Open Project  | Ctrl+O      | Avaa olemassa oleva projektitiedosto    |
| Close         | Ctrl+Q      | Sulje sovellus                          |

### Project-valikko

| Valikkokohta                        | Pikanäppäin | Kuvaus                                        |
|-------------------------------------|-------------|-----------------------------------------------|
| Project Settings                    | Ctrl+P      | Määritä projektikohtaiset asetukset            |
| Add Demo Data to Selected Track     |             | Lisää esimerkki-MIDI-nuotteja esittelyä varten|

### Settings-valikko

| Valikkokohta  | Pikanäppäin | Kuvaus                                        |
|---------------|-------------|-----------------------------------------------|
| Configuration | Ctrl+,      | Avaa yleiset sovellusasetukset                |

### Tools-valikko

| Valikkokohta          | Pikanäppäin | Kuvaus                                        |
|------------------------|-------------|-----------------------------------------------|
| Mix tracks to file     | Ctrl+M      | Vie kaikki käytössä olevat raidat tiedostoon  |
| Virtual MIDI Keyboard  |             | Käynnistä lisäkoskettimistosovellus           |

## Pikanäppäimet

| Pikanäppäin     | Toiminto                        |
|-----------------|--------------------------------|
| Ctrl+S          | Tallenna projekti              |
| Ctrl+O          | Avaa projekti                  |
| Ctrl+M          | Miksaa raidat tiedostoon       |
| Ctrl+P          | Projektiasetukset              |
| Ctrl+,          | Asetukset / Määritykset        |
| Ctrl+Q / Alt+F4 | Sulje                          |

## Virtual MIDI Keyboard

Virtual MIDI Keyboard on lisäsovellus (`virtual_midi_keyboard`), joka tarjoaa näytöllä näkyvän
pianokoskettimiston MIDI-nuottien lähettämiseen. Se voidaan käynnistää pääsovelluksen valikosta
**Tools > Virtual MIDI Keyboard** tai suorittaa itsenäisesti.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Ominaisuudet

- Napsauta näyttöpianon koskettimia soittaaksesi nuotteja
- Käytä tietokoneen näppäimistöä pianokoskettimistona (katso näppäinkartta alla)
- Yhdistä ulkoisiin MIDI-lähtölaitteisiin tai käytä sisäänrakennettua FluidSynth-syntetisaattoria
- Yhdistä MIDI-sisääntulolaitteeseen näyttääksesi saapuvat nuotit koskettimistolla
- Säädettävä syntetisaattorin äänenvoimakkuus (pääsäätö, 10%--200%)
- SoundFont-valinta sisäänrakennetulle syntetisaattorille
- MIDI-instrumentin/ohjelman valinta (General MIDI tai SoundFont-esiasetukset)
- Kuoro/efekti-säädin
- Oktaavin siirto (-3 - +5)
- MIDI-äänenvoimakkuuden säätö (CC#7, 0--127)

### Työkalupalkin ohjaimet

- **Volume**: MIDI-äänenvoimakkuus (CC#7), säädettävä 0-127 syöttökentällä.
- **Octave**: Siirrä koskettimiston oktaavia **<**- ja **>**-painikkeilla tai syöttökentällä.
  Alue on -3 - +5.
- **Chorus/Effect**: Pyöritettävä säädin ja tekstikenttä (1--127) kuoro/efekti-tason asettamiseen
  (MIDI CC#93). Napsauta **Apply** lähettääksesi arvon.
- **MIDI-kanava**: Numeerinen syöttökenttä (1--16) valitsee MIDI-kanavan, jolla
  nuotit, ohjelmanvaihdot ja control changet lähetetään. Oletus on kanava 1.
  Valittu kanava tulee voimaan heti muutoksen jälkeen. Kun valitaan SoundFont-
  esiasetus tietyn kanavan ollessa valittuna, kyseinen bank/program otetaan
  käyttöön sillä kanavalla, joten voit soittaa eri ääniä eri kanavilla.
- **Rummut**: Valintaruutu aivan MIDI-kanavan valitsimen yläpuolella. Kun se ei
  ole valittuna (oletus), MIDI-kanava on se, jonka käyttäjä on valinnut. Kun se
  on valittuna, MIDI-kanava vaihdetaan automaattisesti kanavalle 10 — General
  MIDI -standardin rumpukanavalle — jolloin kaikki soitettu kuulostaa
  lyömäsoittimilta. Valinnan poistaminen palauttaa kanavan, joka oli valittuna
  ennen Rummut-valinnan käyttöönottoa.
- **Instrumenttivalitsin**: Valitse MIDI-instrumentti. Käytettäessä sisäänrakennettua
  FluidSynth-syntetisaattoria näytetään SoundFont-esiasetukset (Pankki:Ohjelma Nimi). Ulkoiseen
  MIDI-laitteeseen yhdistettäessä näytetään luettelo 128 General MIDI -instrumentista.

### Soittaminen tietokoneen näppäimistöllä

Tietokoneen näppäimistö on kartoitettu pianokoskettimiin kahden oktaavin yli:

**Alempi oktaavi (alkaen nykyisestä oktaavista):**

| Näppäin | Nuotti |
|---------|--------|
| Z       | C      |
| S       | C#/Db  |
| X       | D      |
| D       | D#/Eb  |
| C       | E      |
| V       | F      |
| G       | F#/Gb  |
| B       | G      |
| H       | G#/Ab  |
| N       | A      |
| J       | A#/Bb  |
| M       | B      |

**Ylempi oktaavi (yksi oktaavi korkeammalla):**

| Näppäin | Nuotti |
|---------|--------|
| Q       | C      |
| 2       | C#/Db  |
| W       | D      |
| 3       | D#/Eb  |
| E       | E      |
| R       | F      |
| 5       | F#/Gb  |
| T       | G      |
| 6       | G#/Ab  |
| Y       | A      |
| 7       | A#/Bb  |
| U       | B      |
| I       | C (seuraava oktaavi) |
| 9       | C#/Db  |
| O       | D      |
| 0       | D#/Eb  |
| P       | E      |

Koskettimet valaistuvat visuaalisesti painettaessa (valkoiset koskettimet muuttuvat vaaleansinisiksi,
mustat tummenevat).

### Määritykset

Avaa määritysvalintaikkuna (**Settings > Configuration**, Ctrl+,) MIDI- ja äänilaitteiden
määrittämiseksi:

![Virtual MIDI Keyboard -määritykset](../screenshots/VMIDIKeyboard2.png)

#### MIDI-välilehti

- **MIDI-lähtölaite**: Valitse ulkoinen laite tai jätä tyhjäksi sisäänrakennetun
  FluidSynth-syntetisaattorin käyttämiseksi.
- **MIDI-sisääntulolaite**: Valitse ohjain nuottien välittämiseksi koskettimistonäyttöön.
- **Syntetisaattorin äänenvoimakkuus (pääsäätö)**: Säädä sisäänrakennetun syntetisaattorin
  lähtötasoa (10%--200%).
- **SoundFont**: Selaa `.sf2` SoundFont-tiedostoon sisäänrakennettua syntetisaattoria varten.
- **Refresh**: Etsi saatavilla olevia MIDI-laitteita uudelleen.

#### Ääni-välilehti

- **Äänen lähtölaite**: Valitse sisäänrakennetun syntetisaattorin lähtölaite.

#### Kieli-välilehti

- **Kieli**: Valitse näyttökieli. Samat kielet kuin Musician's Canvas -sovelluksessa ovat saatavilla.
  Käyttöliittymä päivittyy välittömästi kieltä vaihdettaessa.

### Virtual MIDI Keyboard -pikanäppäimet

| Pikanäppäin | Toiminto                 |
|-------------|--------------------------|
| Ctrl+,      | Määritysvalintaikkuna     |
| Ctrl+U      | Ohje / Käyttötiedot      |
| Ctrl+Q      | Sulje                    |

## Usein kysytyt kysymykset (FAQ)

### Miten tallennan MIDI-raidan?

1. Aseta **projektikansio** (tallennus vaatii sen).
2. Lisää tai valitse raita ja avaa **Options** (tai napsauta raidantyyppikuvaketta).
3. Aseta tyypiksi **MIDI** ja sulje valintaikkuna.
4. Kohdassa **Settings > Configuration > MIDI** valitse **MIDI-sisääntulo** (laiteportti tai virtuaalikaapeli) ja **SoundFont** (`.sf2`) myöhempää toistoa varten.
5. **Aseuta (Arm)** kyseinen raita (vain yksi voi olla aseutettuna kerrallaan).
6. Napsauta **Record**, odota lähtölaskenta, soita ohjaimella ja sitten **Stop**.

Nuotit näkyvät pianorullassa. **File > Save Project** kirjoittaa `.mid`-tiedoston ja `project.json`-tiedoston projektikansioon.

### Miksi MIDI-raitani on äänetön toistossa?

Toisto käyttää **FluidSynthiä** asetusten **SoundFontilla**. Tarkista **Settings > Configuration > MIDI** (tai **Project > Project Settings**, jos projektikohtaisia ohituksia): kelvollinen `.sf2`-polku tarvitaan. Linuxissa järjestelmän SoundFont voi löytyä automaattisesti; Windowsissa ja macOSissa tiedosto valitaan usein käsin.

### Miten Virtual MIDI Keyboard liittyy Musician's Canvasiin?

Ne ovat **erillisiä ohjelmia**. Käynnistä Virtual MIDI Keyboard valikosta **Tools > Virtual MIDI Keyboard** (tai erikseen). Jotta näppäimistö **ohjautuisi** Musician's Canvasiin MIDI-tallennuksen aikana, käyttöjärjestelmän on reititettävä näppäimistön **MIDI-ulostulo** Musician's Canvasin käyttämään **sisääntuloon** — usein virtuaalisella MIDI-kaapelilla tai yhteensopivilla porteilla molemmissa sovelluksissa. Automaattista kytkentää ei ole.

### Mitä eroa on Configurationilla ja Project Settingsillä?

**Settings > Configuration** määrittää **globaalit oletukset** (teema, kieli, MIDI/äänilaitteet, SoundFont jne.). **Project > Project Settings** korvaa osan niistä **vain nykyiselle projektille** ja tallentuu `project.json`-tiedostoon. Jos kenttä jää projektin oletukseen, käytetään Configurationin globaalia arvoa.

### Miksi raahaa ja pudota ei lisää äänitiedostoja?

Tiedostot hyväksytään vain, kun **projektikansio on asetettu** eikä Musician's Canvas **toista tai tallenna**. Tuetut muodot: **`.wav`** ja **`.flac`**; muut päätteet ohitetaan ja listataan ikkunassa. Jokaisesta tiedostosta tulee uusi **ääni**raita, jonka nimi on tiedoston perusnimi.

### Minne tallenteet tallentuvat?

Ääni tallennetaan **projektikansioon** nimellä **`<raidan_nimi>.flac`** (merkit kuten `/ \ : * ? " < > |` korvataan alaviivalla). Projektitiedosto on **`project.json`** samassa kansiossa. MIDI-raidat tallentuvat **`<raidan_nimi>.mid`** projektin tallennuksessa (nuotit myös `project.json`-tiedostossa).

### Mikä tiedostonimi MIDI-renderöinnissä miksausta varten?

**Miksauksessa** tai **toistossa** MIDI renderöidään ensin väliaikaiseen WAV-tiedostoon. Jos projektin polku tunnetaan, Musician's Canvas kirjoittaa myös **FLAC-välimuistin** projektikansioon: **`<puhdistettu_raidan_nimi>.flac`** (samat säännöt kuin muilla raidatiedostoilla). Nimi perustuu **raidan nimeen**, ei sisäiseen tunnisteeseen.

### Voinko tallentaa kahta raitaa yhtä aikaa?

Et. Vain **yksi** raita voi olla **aseutettuna** kerrallaan; se saa seuraavan tallenteen. Rakenna kappale **yksi raita kerrallaan** (**overdub** toistaa olemassa olevat raidat uutta tallennettaessa).

### Tallentaako metronomi raidalle?

Ei. Kun metronomi on päällä, se kuuluu **järjestelmän äänen** kautta vain sinulle. Sitä **ei sekoiteta** tallennustiedostoon.

### Miksi Musician's Canvas vaatii ASIO:n Windowsissa?

Windowsissa pääohjelma olettaa **ASIO**-ajurin luotettavaan matalan viiveen ääneen. Asenna **ASIO4ALL** tai valmistajan ajuri, jos tulee virheitä.

### Missä Virtual MIDI Keyboard on macOS:llä?

**`.app`-paketissa** Virtual MIDI Keyboard -ohjelmatiedosto **kopioidaan** `Musician's Canvas.app`-sovelluksen sisään (**Contents/MacOS/**), jotta yksi sovelluskansio riittää jakelussa. Käynnistä silti valikosta **Tools > Virtual MIDI Keyboard**.

## Vianmääritys

### Ei äänilähtöä

- Tarkista, että oikea äänen lähtölaite on valittu kohdassa Settings > Configuration > Audio.
- Linuxissa varmista, että PipeWire tai PulseAudio on käynnissä ja ettei lähtö ole mykistetty.
  Käytä `amixer`-komentoa tai työpöytäsi ääniasetuksia äänenvoimakkuustasojen tarkistamiseen.
- Windowsissa varmista, että ASIO-ääniajuri on asennettu (esim.
  [ASIO4ALL](https://asio4all.org/) tai valmistajan toimittama ASIO-ajuri äänilaitteeseesi).
  Musician's Canvas vaatii ASIO:n matalan latenssin äänentoistoon Windowsissa.

### MIDI-raidat ovat äänettömiä

- Varmista, että SoundFont (`.sf2`-tiedosto) on määritetty kohdassa Settings > Configuration > MIDI.
- Linuxissa järjestelmän SoundFont voidaan tunnistaa automaattisesti, jos `fluid-soundfont-gm`-paketti
  on asennettu.
- Windowsissa ja macOS:ssä SoundFont-polku on määritettävä manuaalisesti.

### Tallennus kuulostaa vääristyneeltä tai on väärässä sävelkorkeudessa

- Tämä voi tapahtua, kun äänen sisääntulolaitteen oma näytteenottotaajuus eroaa projektin määritetystä
  taajuudesta. Musician's Canvas käsittelee tämän automaattisesti uudelleennäytteistämällä, mutta jos
  ongelmat jatkuvat, kokeile asettaa projektin näytteenottotaajuus vastaamaan laitteen omaa taajuutta.
- USB-webkameramikrofoneilla on usein epätavallisia omia taajuuksia (esim. 32000 Hz). Sovellus
  tunnistaa nämä automaattisesti.
- Jos käytät Qt Multimedia -taustajärjestelmää ja koet ongelmia, kokeile vaihtaa
  PortAudio-taustajärjestelmään kohdassa Project Settings > Audio.

### Virtual MIDI Keyboard ei tuota ääntä

- Linuxissa PipeWire-ympäristössä varmista, että `libpipewire-0.3-dev`-paketti on asennettu (tarvitaan
  PipeWire-integraatioon FluidSynth-syntetisaattorin kanssa).
- Tarkista, että SoundFont on ladattu (katso MIDI-välilehti määritysvalintaikkunassa).
- Varmista, että äänen lähtölaite on valittu ja järjestelmän äänenvoimakkuus ei ole mykistetty.

## Kääntäminen lähdekoodista

Täydelliset käännösohjeet Linuxille, macOS:lle ja Windowsille, mukaan lukien kaikki tarvittavat
riippuvuudet, löytyvät [README](../README.md)-tiedostosta.
