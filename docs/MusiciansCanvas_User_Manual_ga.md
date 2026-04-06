---
title: "Lámhleabhar Úsáideora Musician's Canvas"
subtitle: "Feidhmchlár Taifeadta Ceoil Il-Rian"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "ga"
---

# Lámhleabhar Úsáideora Musician's Canvas

## Réamhrá

Is feidhmchlár taifeadta ceoil il-rian do ríomhairí deisce é Musician's Canvas. Tacaíonn sé le taifeadadh fuaime ó mhicreafóin agus gléasanna line-in, taifeadadh MIDI ó mhéarchláir agus rialtóirí, agus meascadh na rianta go léir i gcomhad WAV nó FLAC amháin. Soláthraíonn feidhmchlár comhpháirteach, Virtual MIDI Keyboard, méarchlár pianó bogearraí chun nótaí MIDI a sheoladh.

Tá Musician's Canvas deartha le bheith éasca le húsáid agus gnéithe a thairiscint a fhaightear go coitianta i stáisiúin oibre fuaime digiteacha (DAWanna):

- Taifeadadh fuaime agus MIDI il-rian
- Taifeadadh overdub le hathsheinm sioncronaithe na rianta atá ann cheana
- Sintéiseoir FluidSynth MIDI ionsuite le tacaíocht SoundFont
- Tiontú ardchaighdeáin ráta samplála le haghaidh taifeadta ag aon ráta samplála tionscadail
- Braith uathoibríoch monafónach/steiréafónach ar ghléasanna
- Socruithe bunaithe ar thionscadal le forchealú in aghaidh an tionscadail
- Measc go WAV nó FLAC
- Téamaí dorcha agus éadroma
- Logánaithe i dteangacha iomaí (Béarla, Gearmáinis, Spáinnis, Fraincis, Seapáinis, Portaingéilis, Sínis, agus Foghlaeireacht)
- Feidhmchlár comhpháirteach Virtual MIDI Keyboard

## Ag Tosú

### Ag Seoladh an Fheidhmchláir

Rith an comhad inrite `musicians_canvas` ón gcomhadlann tógála nó ó shuíomh do shuiteála:

```
./musicians_canvas
```

Ar an gcéad seoladh, osclaíonn an feidhmchlár le tionscadal folamh. Beidh ort comhadlann tionscadail a shocrú sula ndéanfaidh tú taifeadadh.

Ar thosach, cuireann an feidhmchlár an téama sábháilte i bhfeidhm (dorcha nó éadrom) agus, má úsáideadh comhadlann tionscadail roimhe seo agus má tá comhad `project.json` inti, luchtaítear an tionscadal go huathoibríoch.

### Tionscadal a Shocrú

1. **Socraigh an chomhadlann tionscadail**: Cuir isteach nó brabhsáil chuig fillteán sa réimse "Project Location" ag barr na fuinneoige. Seo an áit a stórálfar na taifeadtaí agus an comhad tionscadail.

2. **Cuir rian leis**: Cliceáil an cnaipe **+ Add Track**. Feictear rian nua le socruithe réamhshocraithe. Más é an t-aon rian sa tionscadal é agus nach bhfuil taifeadta fós, armáiltear go huathoibríoch é le haghaidh taifeadta.

3. **Ainmnigh an rian**: Clóscríobh ainm sa réimse téacs in aice leis an gcnaipe "Options". Úsáidtear an t-ainm seo mar ainm comhaid don chomhad fuaime taifeadta.

![Príomhfhuinneog](../screenshots/MusiciansCanvas_1_MainWin.png)

### Barra Cnaipí

Díreach faoin mbarra roghchláir tá barra uirlisí le cnaipí mear-rochtana:

![Barra Cnaipí](../screenshots/ButtonBar.png)

- **Open Project**: Mar an gcéanna le **File > Open Project** — osclaíonn tionscadal a sábháladh roimhe seo.
- **Save Project**: Mar an gcéanna le **File > Save Project** — sábhálann an tionscadal reatha. Níl
  an cnaipe seo cumasaithe ach amháin nuair a shocraítear eolaire tionscadail.
- **Project Settings**: Mar an gcéanna le **Project > Project Settings** — osclaíonn an dialóg
  socruithe tionscadail. Níl an cnaipe seo cumasaithe ach amháin nuair a shocraítear eolaire tionscadail.
- **Configuration**: Mar an gcéanna le **Settings > Configuration** — osclaíonn dialóg
  socruithe feidhmchláir domhanda.
- **Metronome Settings**: Osclaíonn an dialóg socruithe méadranóim (féach an t-alt Méadranóm thíos).

### Tionscadail a Shábháil agus a Oscailt

- **Sábháil**: Úsáid **File > Save Project** (Ctrl+S) chun an tionscadal reatha a shábháil mar chomhad JSON sa chomhadlann tionscadail.
- **Oscail**: Úsáid **File > Open Project** (Ctrl+O) chun tionscadal a sábháladh roimhe seo a luchtú.

Stórálann an comhad tionscadail (`project.json`) ainmneacha rianta, cineálacha, nótaí MIDI, tagairtí comhad fuaime, agus gach socrú a bhaineann leis an tionscadal. Stóráiltear comhaid fuaime sa chomhadlann chéanna le `project.json` agus ainmnítear iad i ndiaidh a gcuid rianta (m.sh., `My_Track.flac`).

Má dhúnann tú an feidhmchlár le hathruithe neamhshábháilte, iarrann dialóg deimhniúcháin an bhfuil tú ag iarraidh sábháil sula scoirfidh tú.

## Bainistíocht Rianta

### Rianta a Chur Leis agus a Bhaint

- Cliceáil **+ Add Track** chun rian nua a chur leis an socrú.
- Cliceáil an cnaipe **x** ar thaobh na láimhe deise de ró rian chun é a bhaint.
- Cliceáil **Clear Tracks** (an cnaipe dearg sa bharra uirlisí) chun gach rian a bhaint. Taispeántar dialóg deimhniúcháin sula dtéitear ar aghaidh.

### Cineál Rian a Chumrú

Is féidir gach rian a chumrú mar **Audio** (le haghaidh taifeadta micreafóin/line-in) nó **MIDI** (le haghaidh taifeadta méarchláir/rialtóra).

Chun an cineál rian a athrú:

- Cliceáil an cnaipe **Options** ar an rian, nó
- Cliceáil an **deilbhín cineáil rian** (idir "Options" agus an réimse ainm)

Osclaíonn sé seo an dialóg Cumraíochta Rian inar féidir leat an fhoinse ionchuir a roghnú.

![Cumraíocht rian](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Rialuithe Rian

Soláthraíonn gach ró rian na rialuithe seo a leanas:

- **Ticbhosca cumasaithe**: Scoránann an bhfuil an rian san áireamh in athsheinm agus meascadh. Má dhíchumasaítear rian, díarmáiltear go huathoibríoch é má bhí sé armáilte.
- **Cnaipe raidió armála**: Roghnaíonn sé an rian seo mar sprioc taifeadta. Ní féidir ach rian amháin a bheith armáilte ag an am céanna; nuair a armáiltear rian nua, díarmáiltear go huathoibríoch aon rian a bhí armáilte roimhe seo.
- **Réimse ainm**: Réimse téacs ineagair d'ainm an rian. Úsáidtear an t-ainm seo mar ainm comhaid fuaime (le carachtair neamhbhailí córais comhad á n-athchur le fostríoc).
- **Cnaipe Options**: Osclaíonn sé an dialóg Cumraíochta Rian.
- **Deilbhín cineáil**: Taispeánann sé deilbhín callaire do rianta fuaime nó deilbhín pianó do rianta MIDI. Má chliceáiltear é osclaíonn sé an dialóg Cumraíochta Rian.
- **Cnaipe bainte (x)**: Baineann sé an rian ón tionscadal.

### Uath-Armáil

Nuair nach bhfuil ach rian amháin ag tionscadal agus nár taifeadadh an rian sin fós, armáiltear go huathoibríoch é le haghaidh taifeadta. Baineann sé seo le cur an chéad rian le tionscadal nua agus le tionscadal atá ann cheana a oscailt a bhfuil rian folamh amháin aige.

### Amharcléiriú Rian

- **Rianta fuaime**: Taispeánann siad amharcléiriú tonnchrothaí den fhuaim taifeadta. Nuair nach bhfuil fuaim taifeadta, taispeánann an limistéar "No audio recorded".
- **Rianta MIDI**: Taispeánann siad amharcléiriú piano roll a léiríonn nótaí taifeadta ar ghreille ó A0 go C8. Dathanna na nótaí de réir treise. Nuair nach bhfuil sonraí MIDI taifeadta, taispeánann an limistéar "No MIDI data recorded".

## Taifeadadh

### Taifeadadh Fuaime

1. Cinntigh go bhfuil an chomhadlann tionscadail socraithe.
2. Armáil an rian sprioc (tic an cnaipe raidió "Arm").
3. Cliceáil an cnaipe **Record** (ciorcal dearg).
4. Feictear comhaireamh síos 3 soicind ar an rian ("Get ready... 3", "2", "1"), ansin tosaíonn an taifeadadh.
5. Le linn taifeadta, taispeántar méadar leibhéil beo i limistéar tonnchrothaí an rian, ag taispeáint an aimplitiúid reatha mar bharra grádáin (glas go buí go dearg) le lipéad "Recording".
6. Cliceáil an cnaipe **Stop** chun an taifeadadh a chríochnú.

Sábháiltear an fhuaim taifeadta mar chomhad FLAC sa chomhadlann tionscadail, ainmnithe i ndiaidh an rian.

Le linn taifeadta agus athsheinm, díchumasaítear gach rialú idirghníomhach (cnaipí rianta, socruithe, srl.) chun athruithe de thaisme a chosc.

#### Taifeadadh Overdub

Nuair a thaifeadtar rian nua agus rianta cumasaithe eile ann cheana le sonraí fuaime nó MIDI, déanann Musician's Canvas taifeadadh overdub: measctar na rianta atá ann cheana le chéile agus seinntar iad i bhfíor-am agus an rian nua á thaifeadadh. Ligeann sé seo duit na codanna a taifeadadh roimhe seo a chloisteáil agus tú ag leagan síos ceann nua.

Ullmhaítear meascán na rianta atá ann cheana sula dtosaíonn an ghabháil, ionas go dtosaíonn an taifeadadh agus an athsheinm ag an nóiméad céanna beagnach, ag coinneáil gach rian sioncronaithe.

#### Cúlaí Taifeadta

Tacaíonn Musician's Canvas le dhá chúla gabhála fuaime:

- **PortAudio** (réamhshocraithe nuair atá sé ar fáil): Soláthraíonn sé taifeadadh iontaofa íseal-aga freagartha agus is é an cúla molta.
- **Qt Multimedia**: Cúla cúltaca ag baint úsáide as gabháil fuaime ionsuite Qt. Úsáidtear é nuair nach bhfuil PortAudio ar fáil nó nuair a roghnaítear go sainráite é i Socruithe Tionscadail.

Is féidir an cúla taifeadta a chumrú in aghaidh an tionscadail ag **Project > Project Settings > Audio**.

#### Ráta Samplála agus Láimhseáil Gléasanna

Taifeadann Musician's Canvas ag ráta samplála dúchais an ghléis ionchuir fuaime agus ansin tiontaíonn sé go huathoibríoch go ráta samplála cumraithe an tionscadail ag úsáid athshamplála ardchaighdeáin. Ciallaíonn sé seo gur féidir leat aon ráta samplála tionscadail a shocrú (m.sh., 44100 Hz nó 48000 Hz) beag beann ar ráta dúchais an ghléis. Caomhnaíonn an tiontú tuinairde agus fad go beacht.

#### Braith Gléas Monafónach

Tá roinnt gléasanna fuaime (m.sh., micreafóin ceamara gréasáin USB) monafónach go fisiciúil ach fógraítear iad mar steiréafónach ag an gcóras oibriúcháin. Braitheann Musician's Canvas é seo go huathoibríoch agus coigeartaíonn sé an líon cainéal dá réir. Má tá an tionscadal cumraithe le haghaidh steiréafónach, dúblaítear an comhartha monafónach go dtí an dá chainéal.

### Taifeadadh MIDI

1. Socraigh cineál an rian go **MIDI** tríd an gcnaipe Options.
2. Cinntigh go bhfuil gléas ionchuir MIDI cumraithe ag **Settings > Configuration > MIDI**.
3. Armáil an rian agus cliceáil Record.
4. Seinn nótaí ar do rialtóir MIDI.
5. Cliceáil Stop chun an taifeadadh a chríochnú.

Taispeántar nótaí MIDI in amharcléiriú piano roll ar an rian.

## Méadranóm

Tá méadranóm ionsuite ag Musician's Canvas is féidir a úsáid le linn taifeadta chun cabhrú
leis an am a choinneáil. Cliceáil an cnaipe méadranóim ar an mbarra cnaipí (faoin mbarra
roghchláir) chun an dialóg socruithe méadranóim a oscailt:

![Socruithe méadranóim](../screenshots/MetronomeSettings.png)

Soláthraíonn an dialóg:

- **Enable metronome during recording**: Nuair atá sé ticeáilte, seinneann an méadranóm fuaim
  tic agus taifeadadh ar siúl. Seinntear an tic trí fhuaim an chórais agus **ní** ghabhtar é
  isteach sa rian taifeadta.
- **Beats per minute**: Ionchur uimhriúil don luas, i mbuillí sa nóiméad (BPM). Is é an raon
  20–300 BPM.

Nuair atá an méadranóm cumasaithe, tosaíonn sé ag ticeáil nuair a thosaíonn an taifeadadh i
ndáiríre (tar éis don chomhaireamh síos 3 soicind críochnú), agus stopann sé nuair a chríochnaíonn
an taifeadadh.

## Athsheinm

Cliceáil an cnaipe **Play** chun gach rian cumasaithe a mheascadh agus a athsheinm. Athraíonn leid uirlise an chnaipe chun a léiriú an seinnfidh sé nó an dtaifeadfaidh sé bunaithe ar an gcé acu an bhfuil rian armáilte. Ní chuirtear rianta díchumasaithe (gan tic) san áireamh san athsheinm.

Le linn athsheinm, díchódaítear rianta fuaime óna gcomhaid FLAC agus rindreáiltear rianta MIDI go fuaim ag úsáid an tsintéiseora FluidSynth ionsuite. Measctar gach rian le chéile agus seinntar iad trí ghléas aschuir fuaime an chórais.

Cliceáil an cnaipe **Stop** chun an athsheinm a chríochnú ag am ar bith.

## Meascadh go Comhad

Úsáid **Tools > Mix tracks to file** (Ctrl+M) chun gach rian cumasaithe a easpórtáil go comhad fuaime amháin. Ligeann dialóg duit an cosán aschuir agus an formáid a roghnú:

- **Comhad aschuir**: Brabhsáil chun cosán comhad cinn scríbe a roghnú.
- **Formáid**: Roghnaigh idir FLAC (comhbhrú gan chaillteanas, comhaid níos lú) nó WAV (neamh-chomhbhrúite).

Úsáideann an meascadh ráta samplála cumraithe an tionscadail. Rindreáiltear rianta MIDI ag úsáid an SoundFont cumraithe.

## Socruithe

### Socruithe Domhanda

Úsáid **Settings > Configuration** (Ctrl+,) chun réamhshocruithe domhanda a shocrú a bhaineann le gach tionscadal:

![Socruithe ginearálta](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### Cluaisín General

- **Téama**: Roghnaigh idir téamaí dorcha agus éadroma.

#### Cluaisín Display

- **Dath an taispeána LED uimhriúil**: Roghnaigh an dath a úsáidtear le haghaidh an taispeána ama LED uimhriúil atá le feiceáil ar bharra uirlisí na príomhfhuinneoige. Tarraingítear deighleáin ghníomhacha na ndigití sa dath roghnaithe, agus tarraingítear deighleáin neamhghníomhacha mar leagan lagaithe den dath céanna. Is iad na dathanna atá ar fáil ná Light Red, Dark Red, Light Green, Dark Green, Light Blue, Dark Blue, Yellow, Orange, Light Cyan agus Dark Cyan. Is é Light Green an réamhshocrú.

![Socruithe taispeána](../screenshots/MusiciansCanvas_DisplaySettings.png)

#### Cluaisín Language

- **Teanga**: Roghnaigh teanga taispeána an fheidhmchláir. Is é "System Default" an réamhshocrú, a úsáideann socrú teanga an chórais oibriúcháin. Is iad na teangacha atá ar fáil ná Béarla, Deutsch (Gearmáinis), Español (Spáinnis), Français (Fraincis), Seapáinis, Português (Portaingéilis na Brasaíle), Sínis (Traidisiúnta), Sínis (Simplithe), agus Foghlaeireacht. Déantar an comhéadan a nuashonrú láithreach nuair a athraíonn tú an teanga.

![Socruithe teanga](../screenshots/MusiciansCanvas_LanguageSettings.png)

#### Cluaisín MIDI

- **Gléas Aschuir MIDI**: Roghnaigh an sintéiseoir FluidSynth ionsuite nó gléas MIDI seachtrach. Úsáid an cnaipe **Refresh** chun athscanadh a dhéanamh le haghaidh gléasanna MIDI atá ar fáil.
- **SoundFont**: Brabhsáil chuig comhad SoundFont `.sf2` le haghaidh sintéise MIDI. Ar Linux, d'fhéadfaí SoundFont córais a bhrath go huathoibríoch má tá an pacáiste `fluid-soundfont-gm` suiteáilte. Ar Windows agus macOS, ní mór duit cosán SoundFont a chumrú de láimh.

![Socruithe MIDI](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Cluaisín Audio

- **Gléas Ionchuir Fuaime**: Roghnaigh an micreafón nó an gléas line-in le haghaidh taifeadta.
- **Gléas Aschuir Fuaime**: Roghnaigh an callaire nó an gléas cluasán le haghaidh athsheinm.

![Socruithe fuaime](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Socruithe Tionscadail

Úsáid **Project > Project Settings** (Ctrl+P) chun réamhshocruithe domhanda a fhorchealú don tionscadal reatha amháin. Tá sé seo úsáideach do thionscadail a dteastaíonn ráta samplála sonrach, SoundFont, nó gléas fuaime uathu. Sábháiltear socruithe a bhaineann leis an tionscadal taobh istigh den chomhad `project.json`.

![Socruithe MIDI tionscadail](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### Cluaisín MIDI

- **Gléas MIDI**: Roghnaigh gléas MIDI don tionscadal seo, nó fág ag an réamhshocrú chun an socrú domhanda a úsáid.
- **SoundFont**: Roghnaigh comhad SoundFont don tionscadal seo.
- **Refresh**: Athscanadh le haghaidh gléasanna MIDI atá ar fáil.

#### Cluaisín Audio

- **Gléas Ionchuir Fuaime**: Roghnaigh an gléas taifeadta don tionscadal seo.
- **Cúla gabhála taifeadta** (nuair atá PortAudio ar fáil):
  - **PortAudio (native input)**: Molta. Úsáideann sé an leabharlann fuaime chéanna le Audacity.
  - **Qt Multimedia**: Rogha chúltaca ag úsáid gabháil fuaime ionsuite Qt.
- **Gléas Ionchuir PortAudio**: Nuair a úsáidtear an cúla PortAudio, roghnaigh an gléas ionchuir PortAudio sonrach.
- **Gléas Aschuir Fuaime**: Roghnaigh an gléas athsheinm don tionscadal seo.

##### Socruithe Formáid Fuaime

- **Ráta samplála**: Roghnaigh ó rátaí caighdeánacha (8000 Hz go 192000 Hz). Lipéadaítear ráta dúchais an ghléis mar "(native)". Lipéadaítear rátaí a dteastaíonn athshamplú uathu mar "(resampled)". Is féidir leat aon ráta a roghnú beag beann ar chumas an ghléis; déanfaidh Musician's Canvas athshamplú go huathoibríoch de réir mar is gá.
- **Cainéil**: Monafónach nó Steiréafónach. Mura dtacaíonn an gléas ach le monafónach, tá an rogha Steiréafónach díchumasaithe.

![Socruithe fuaime tionscadail](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

## Roghchláir

### Roghchlár File

| Mír Roghchláir  | Aicearra   | Cur Síos                                |
|-----------------|------------|-----------------------------------------|
| Save Project    | Ctrl+S     | Sábháil an tionscadal reatha ar dhiosca |
| Open Project    | Ctrl+O     | Oscail comhad tionscadail atá ann cheana|
| Close           | Ctrl+Q     | Scoir den fheidhmchlár                  |

### Roghchlár Project

| Mír Roghchláir                       | Aicearra   | Cur Síos                                        |
|--------------------------------------|------------|-------------------------------------------------|
| Project Settings                     | Ctrl+P     | Cumraigh socruithe a bhaineann leis an tionscadal|
| Add Demo Data to Selected Track      |            | Cuir nótaí samplacha MIDI leis le haghaidh taispeána|

### Roghchlár Settings

| Mír Roghchláir  | Aicearra   | Cur Síos                                    |
|-----------------|------------|---------------------------------------------|
| Configuration   | Ctrl+,     | Oscail socruithe domhanda an fheidhmchláir  |

### Roghchlár Tools

| Mír Roghchláir         | Aicearra   | Cur Síos                                        |
|------------------------|------------|-------------------------------------------------|
| Mix tracks to file     | Ctrl+M     | Easpórtáil gach rian cumasaithe go comhad       |
| Virtual MIDI Keyboard  |            | Seoladh an fheidhmchláir mhéarchláir chomhpháirtigh|

## Aicearraí Méarchláir

| Aicearra        | Gníomh                          |
|-----------------|--------------------------------|
| Ctrl+S          | Sábháil tionscadal              |
| Ctrl+O          | Oscail tionscadal               |
| Ctrl+M          | Measc rianta go comhad          |
| Ctrl+P          | Socruithe Tionscadail           |
| Ctrl+,          | Socruithe / Cumraíocht          |
| Ctrl+Q / Alt+F4 | Scoir                          |

## Virtual MIDI Keyboard

Is feidhmchlár comhpháirteach é Virtual MIDI Keyboard (`virtual_midi_keyboard`) a sholáthraíonn méarchlár pianó ar an scáileán chun nótaí MIDI a sheoladh. Is féidir é a sheoladh ón roghchlár **Tools > Virtual MIDI Keyboard** sa phríomhfheidhmchlár, nó é a rith go neamhspleách.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Gnéithe

- Cliceáil eochracha ar an méarchlár pianó ar an scáileán chun nótaí a sheinm
- Úsáid méarchlár an ríomhaire mar mhéarchlár pianó (féach ar mhapáil eochracha thíos)
- Ceangail le gléasanna aschuir MIDI seachtracha nó úsáid an sintéiseoir FluidSynth ionsuite
- Ceangail le gléas ionchuir MIDI chun nótaí isteach a thaispeáint ar an méarchlár
- Toirt sintéiseora inchóirithe (master gain, 10%--200%)
- Roghnú SoundFont don tsintéiseoir ionsuite
- Roghnú ionstraime/cláir MIDI (General MIDI nó réamhshocruithe SoundFont)
- Cnaipe rialaithe Chorus/éifeacht
- Aistriú ochtáif (-3 go +5)
- Rialú toirte MIDI (CC#7, 0--127)

### Rialuithe Barra Uirlisí

- **Volume**: Toirt MIDI (CC#7), inchóirithe ó 0 go 127 tríd an mbosca snáthaide.
- **Octave**: Aistrigh ochtáif an mhéarchláir ag úsáid na gcnaipí **<** agus **>** nó an bhosca snáthaide. Raon -3 go +5.
- **Chorus/Effect**: Cnaipe rothlach agus réimse téacs (1--127) chun leibhéal chorus/éifeachta a shocrú (MIDI CC#93). Cliceáil **Apply** chun an luach a sheoladh.
- **Roghnóir ionstraime**: Roghnaigh ionstraim MIDI. Nuair a úsáidtear an sintéiseoir FluidSynth ionsuite, taispeánann sé seo réamhshocruithe SoundFont (Bank:Program Name). Nuair atá sé ceangailte le gléas MIDI seachtrach, liostáítear na 128 ionstraim General MIDI.

### Ag Seinm le Méarchlár an Ríomhaire

Tá méarchlár an ríomhaire mapáilte go heochracha pianó trasna dhá ochtáif:

**Ochtáif íochtair (ag tosú ag an ochtáif reatha):**

| Eochair | Nóta  |
|---------|-------|
| Z       | C     |
| S       | C#/Db |
| X       | D     |
| D       | D#/Eb |
| C       | E     |
| V       | F     |
| G       | F#/Gb |
| B       | G     |
| H       | G#/Ab |
| N       | A     |
| J       | A#/Bb |
| M       | B     |

**Ochtáif uachtair (ochtáif amháin níos airde):**

| Eochair | Nóta  |
|---------|-------|
| Q       | C     |
| 2       | C#/Db |
| W       | D     |
| 3       | D#/Eb |
| E       | E     |
| R       | F     |
| 5       | F#/Gb |
| T       | G     |
| 6       | G#/Ab |
| Y       | A     |
| 7       | A#/Bb |
| U       | B     |
| I       | C (an chéad ochtáif eile) |
| 9       | C#/Db |
| O       | D     |
| 0       | D#/Eb |
| P       | E     |

Lasann eochracha go hamhairc nuair a bhrúitear iad (athraíonn eochracha bána go gorm éadrom, éiríonn eochracha dubha níos dorcha).

### Cumraíocht

Oscail an dialóg Cumraíochta (**Settings > Configuration**, Ctrl+,) chun gléasanna MIDI agus fuaime a shocrú:

![Cumraíocht Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard2.png)

#### Cluaisín MIDI

- **Gléas Aschuir MIDI**: Roghnaigh gléas seachtrach nó fág folamh don tsintéiseoir FluidSynth ionsuite.
- **Gléas Ionchuir MIDI**: Roghnaigh rialtóir chun nótaí a chur ar aghaidh chuig taispeáint an mhéarchláir.
- **Toirt an tSintéiseora (Master Gain)**: Coigeartaigh leibhéal aschuir na sintéiseora ionsuite (10%--200%).
- **SoundFont**: Brabhsáil chuig comhad SoundFont `.sf2` don tsintéiseoir ionsuite.
- **Refresh**: Athscanadh le haghaidh gléasanna MIDI atá ar fáil.

#### Cluaisín Audio

- **Gléas Aschuir Fuaime**: Roghnaigh an gléas aschuir don tsintéiseoir ionsuite.

#### Cluaisín Language

- **Teanga**: Roghnaigh an teanga taispeána. Tá na teangacha céanna ar fáil agus atá i Musician's Canvas. Déantar an comhéadan a nuashonrú láithreach nuair a athraíonn tú an teanga.

### Aicearraí Virtual MIDI Keyboard

| Aicearra   | Gníomh                       |
|------------|------------------------------|
| Ctrl+,     | Dialóg cumraíochta           |
| Ctrl+U     | Cabhair / Eolas úsáide       |
| Ctrl+Q     | Dún                          |

## Fabhtcheartú

### Gan Aschur Fuaime

- Seiceáil go bhfuil an gléas aschuir fuaime ceart roghnaithe ag Settings > Configuration > Audio.
- Ar Linux, fíoraigh go bhfuil PipeWire nó PulseAudio ag rith agus nach bhfuil an t-aschur balbhaithe. Úsáid `amixer` nó socruithe fuaime do dheasc chun leibhéil toirte a sheiceáil.
- Ar Windows, cinntigh go bhfuil tiománaí fuaime ASIO suiteáilte (m.sh., [ASIO4ALL](https://asio4all.org/) nó tiománaí ASIO ón monaróir do do chomhéadan fuaime). Éilíonn Musician's Canvas ASIO le haghaidh fuaime íseal-aga freagartha ar Windows.

### Tá Rianta MIDI Ciúin

- Cinntigh go bhfuil SoundFont (comhad `.sf2`) cumraithe ag Settings > Configuration > MIDI.
- Ar Linux, d'fhéadfaí SoundFont córais a bhrath go huathoibríoch má tá an pacáiste `fluid-soundfont-gm` suiteáilte.
- Ar Windows agus macOS, ní mór duit cosán SoundFont a chumrú de láimh.

### Fuaimeann an Taifeadadh Saobhtha nó Tá an Tuinairde Mícheart

- Is féidir é seo a tharlú nuair a bhíonn ráta samplála dúchais an ghléis ionchuir fuaime difriúil ó ráta cumraithe an tionscadail. Láimhseálann Musician's Canvas é seo go huathoibríoch trí athshamplú, ach má leanann na fadhbanna ar aghaidh, bain triail as ráta samplála an tionscadail a shocrú chun ráta dúchais an ghléis a mheaitseáil.
- Is minic a bhíonn rátaí dúchais neamhghnácha ag micreafóin ceamara gréasáin USB (m.sh., 32000 Hz). Braitheann an feidhmchlár iad seo go huathoibríoch.
- Má tá tú ag úsáid an chúla Qt Multimedia agus má tá fadhbanna agat, bain triail as athrú go dtí an cúla PortAudio ag Project Settings > Audio.

### Níl Fuaim ag Virtual MIDI Keyboard

- Ar Linux le PipeWire, cinntigh go bhfuil an pacáiste `libpipewire-0.3-dev` suiteáilte (teastaíonn sé le haghaidh comhtháthú PipeWire leis an tsintéiseoir FluidSynth).
- Seiceáil go bhfuil SoundFont luchtaithe (féach an cluaisín MIDI sa dialóg cumraíochta).
- Fíoraigh go bhfuil an gléas aschuir fuaime roghnaithe agus nach bhfuil toirt an chórais balbhaithe.

## Tógáil ón mBunchód

Féach ar an [README](../README.md) le haghaidh treoracha iomlána tógála do Linux, macOS, agus Windows, lena n-áirítear gach spleáchas riachtanach.
