---
title: "Manuel d'utilisation de Musician's Canvas"
subtitle: "Application d'enregistrement musical multipiste"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "fr"
---

# Manuel d'utilisation de Musician's Canvas

## Introduction

Musician's Canvas est une application d'enregistrement musical multipiste pour ordinateurs
de bureau. Elle prend en charge l'enregistrement audio depuis des microphones et des
entrées ligne, l'enregistrement MIDI depuis des claviers et des contrôleurs, ainsi que le
mixage de toutes les pistes dans un seul fichier WAV ou FLAC. Une application compagnon,
Virtual MIDI Keyboard, fournit un clavier de piano logiciel pour envoyer des notes MIDI.

Musician's Canvas est conçue pour être facile à utiliser tout en offrant des
fonctionnalités couramment présentes dans les stations de travail audio numériques (DAW) :

- Enregistrement audio et MIDI multipiste
- Enregistrement en overdub avec lecture synchronisée des pistes existantes
- Synthétiseur MIDI FluidSynth intégré avec prise en charge des SoundFont
- Conversion de fréquence d'échantillonnage de haute qualité pour enregistrer à n'importe quel taux d'échantillonnage du projet
- Détection automatique mono/stéréo des périphériques
- Paramètres basés sur le projet avec possibilité de remplacement par projet
- Mixage vers WAV ou FLAC
- Thèmes sombre et clair
- Localisé en 18 langues dont English, Deutsch, Español, Français, Japanese, Português, Chinese, Русский, Svenska, Suomi, Dansk, Norsk, Polski, Ελληνικά, Gaeilge, Cymraeg et Pirate
- Application compagnon Virtual MIDI Keyboard

## Premiers pas

### Lancement de l'application

Exécutez le programme `musicians_canvas` depuis le répertoire de compilation ou votre
emplacement d'installation :

```
./musicians_canvas
```

Au premier lancement, l'application s'ouvre avec un projet vide. Vous devrez définir un
répertoire de projet avant d'enregistrer.

Au démarrage, l'application applique le thème enregistré (sombre ou clair) et, si un
répertoire de projet a été utilisé précédemment et contient un fichier `project.json`,
le projet est automatiquement chargé.

### Configuration d'un projet

1. **Définir le répertoire du projet** : Saisissez ou naviguez vers un dossier dans le
   champ "Project Location" en haut de la fenêtre. C'est là que les enregistrements et
   le fichier de projet seront stockés.

2. **Ajouter une piste** : Cliquez sur le bouton **+ Add Track**. Une nouvelle piste
   apparaît avec les paramètres par défaut. S'il s'agit de la seule piste du projet et
   qu'elle n'a pas encore été enregistrée, elle est automatiquement armée pour
   l'enregistrement.

3. **Nommer la piste** : Saisissez un nom dans le champ texte à côté du bouton "Options".
   Ce nom est utilisé comme nom de fichier pour le fichier audio enregistré.

![Fenêtre principale](../screenshots/i18n/fr/MusiciansCanvas_1_MainWin.png)

### Barre de boutons

Juste en dessous de la barre de menus se trouve une barre d'outils avec des boutons d'accès rapide :

![Barre de boutons](../screenshots/i18n/fr/ButtonBar.png)

- **Open Project** : Identique à **File > Open Project** — ouvre un projet précédemment sauvegardé.
- **Save Project** : Identique à **File > Save Project** — sauvegarde le projet en cours. Ce
  bouton n'est activé que lorsqu'un répertoire de projet est défini.
- **Project Settings** : Identique à **Project > Project Settings** — ouvre la boîte de dialogue
  des paramètres du projet. Ce bouton n'est activé que lorsqu'un répertoire de projet est défini.
- **Configuration** : Identique à **Settings > Configuration** — ouvre la boîte de dialogue des
  paramètres globaux de l'application.
- **Metronome Settings** : Ouvre la boîte de dialogue des paramètres du métronome (voir la section Métronome ci-dessous).

### Sauvegarde et ouverture de projets

- **Sauvegarder** : Utilisez **File > Save Project** (Ctrl+S) pour sauvegarder le projet
  en cours sous forme de fichier JSON dans le répertoire du projet.
- **Ouvrir** : Utilisez **File > Open Project** (Ctrl+O) pour charger un projet
  précédemment sauvegardé.

Le fichier de projet (`project.json`) stocke les noms des pistes, les types, les notes
MIDI, les références aux fichiers audio et tous les paramètres spécifiques au projet. Les
fichiers audio sont stockés dans le même répertoire que `project.json` et sont nommés
d'après leurs pistes (par ex., `My_Track.flac`).

Si vous fermez l'application avec des modifications non sauvegardées, une boîte de
dialogue de confirmation vous demande si vous souhaitez sauvegarder avant de quitter.

## Gestion des pistes

### Ajout et suppression de pistes

- Cliquez sur **+ Add Track** pour ajouter une nouvelle piste à l'arrangement.
- Cliquez sur le bouton **x** à droite d'une ligne de piste pour la supprimer.
- Cliquez sur **Clear Tracks** (le bouton rouge dans la barre d'outils) pour supprimer
  toutes les pistes. Une boîte de dialogue de confirmation s'affiche avant de procéder.

### Ajout de pistes par glisser-déposer

Lorsqu'un projet est ouvert, vous pouvez faire glisser un ou plusieurs fichiers
audio pris en charge depuis votre gestionnaire de fichiers (Explorateur Windows,
Finder macOS, gestionnaire de fichiers Linux, etc.) directement sur la fenêtre
de Musician's Canvas pour les ajouter en tant que nouvelles pistes audio.

- **Formats pris en charge :** `.wav` et `.flac`. Les fichiers dans tout autre
  format sont ignorés silencieusement, et une boîte de dialogue à la fin
  répertorie les fichiers qui ont été ignorés.
- **Copie de fichiers :** Si le fichier déposé ne se trouve pas déjà dans le
  répertoire du projet, il y est copié automatiquement. Si un fichier portant
  le même nom existe déjà dans le répertoire du projet, il vous est demandé si
  vous souhaitez le remplacer.
- **Nom de la piste :** Le nom de base du fichier (sans l'extension) est utilisé
  comme nom de la nouvelle piste. Par exemple, déposer `Bass Line.wav` crée une
  piste audio nommée « Bass Line ».
- **Plusieurs fichiers à la fois :** Plusieurs fichiers peuvent être glissés
  ensemble ; chaque fichier pris en charge devient sa propre piste en un seul
  dépôt.
- **Lorsque le dépôt est refusé :** Les dépôts ne sont acceptés que lorsqu'un
  projet est ouvert et que Musician's Canvas n'est **pas** en cours de lecture
  ou d'enregistrement. Arrêtez d'abord la lecture ou l'enregistrement si vous
  souhaitez glisser d'autres pistes.

### Configuration du type de piste

Chaque piste peut être configurée en tant que **Audio** (pour l'enregistrement depuis un
microphone ou une entrée ligne) ou **MIDI** (pour l'enregistrement depuis un clavier ou
un contrôleur).

Pour changer le type de piste :

- Cliquez sur le bouton **Options** de la piste, ou
- Cliquez sur l'**icône du type de piste** (entre "Options" et le champ de nom)

Cela ouvre la boîte de dialogue de configuration de la piste où vous pouvez sélectionner
la source d'entrée.

![Configuration de la piste](../screenshots/i18n/fr/MusiciansCanvas_2_TrackConfig.png)

### Contrôles des pistes

Chaque ligne de piste fournit les contrôles suivants :

- **Case à cocher Activer** : Active ou désactive l'inclusion de la piste dans la lecture
  et le mixage. Désactiver une piste la désarme également automatiquement si elle était
  armée.
- **Bouton radio Armer** : Sélectionne cette piste comme cible d'enregistrement. Une
  seule piste peut être armée à la fois ; armer une nouvelle piste désarme
  automatiquement toute piste précédemment armée.
- **Champ de nom** : Champ texte modifiable pour le nom de la piste. Ce nom est utilisé
  comme nom de fichier audio (les caractères invalides pour le système de fichiers sont
  remplacés par des tirets bas).
- **Bouton Options** : Ouvre la boîte de dialogue de configuration de la piste.
- **Icône de type** : Affiche une icône de haut-parleur pour les pistes audio ou une
  icône de piano pour les pistes MIDI. Cliquer dessus ouvre la boîte de dialogue de
  configuration de la piste.
- **Bouton de suppression (x)** : Supprime la piste du projet.

### Armement automatique

Lorsqu'un projet contient exactement une piste et que cette piste n'a pas encore été
enregistrée, elle est automatiquement armée pour l'enregistrement. Cela s'applique aussi
bien lors de l'ajout de la première piste à un nouveau projet que lors de l'ouverture
d'un projet existant comportant une seule piste vide.

### Visualisation des pistes

- Les **pistes audio** affichent une visualisation de la forme d'onde de l'audio
  enregistré. Lorsqu'aucun audio n'a été enregistré, la zone affiche "No audio recorded".
- Les **pistes MIDI** affichent une visualisation en piano roll montrant les notes
  enregistrées sur une grille allant de A0 à C8. Les notes sont colorées en fonction de
  la vélocité. Lorsqu'aucune donnée MIDI n'a été enregistrée, la zone affiche "No MIDI
  data recorded".

## Enregistrement

### Enregistrement audio

1. Assurez-vous que le répertoire du projet est défini.
2. Armez la piste cible (cochez le bouton radio "Arm").
3. Cliquez sur le bouton **Record** (cercle rouge).
4. Un compte à rebours de 3 secondes apparaît sur la piste ("Get ready... 3", "2", "1"),
   puis l'enregistrement commence.
5. Pendant l'enregistrement, un indicateur de niveau en temps réel est affiché dans la
   zone de forme d'onde de la piste, montrant l'amplitude actuelle sous forme d'une barre
   à gradient (du vert au jaune puis au rouge) avec le libellé "Recording".
6. Cliquez sur le bouton **Stop** pour arrêter l'enregistrement.

L'audio enregistré est sauvegardé sous forme de fichier FLAC dans le répertoire du
projet, nommé d'après la piste.

Pendant l'enregistrement et la lecture, tous les contrôles interactifs (boutons de piste,
paramètres, etc.) sont désactivés pour éviter les modifications accidentelles.

### Effets d'insertion (pistes audio uniquement)

Les pistes audio ont un bouton **Effets** juste sous **Options**. Il ouvre la boîte de dialogue
**Effets de piste**, où vous configurez une **chaîne ordonnée** d'effets d'insertion pour les
enregistrements sur cette piste :

![Boîte de dialogue Effets de piste](../screenshots/i18n/fr/Track_Effects_Dialog.png)

Parmi les types proposés : **Réverb**, **Chorus**, **Flanger**, **Overdrive / distorsion**, **Amp & cab** (modélisation ampli et baffle : type d’ampli, baffle, gain, grave/médium/aigu, présence/air et mixage) et **Vibrato (trémolo)**.

**Vibrato (trémolo)** correspond au “Vibrato” typique des amplis Fender : une **modulation périodique du volume**. Le réglage **Speed** définit la vitesse de pulsation, et **Intensity** (Depth) la profondeur (d’un léger frémissement à un hachage complet).

Captures du module **Amp & cab** :

![Modélisation ampli et baffle (1)](../screenshots/i18n/fr/Amp_And_Cabinet_Model_1.png)

![Modélisation ampli et baffle (2)](../screenshots/i18n/fr/Amp_And_Cabinet_Model_2.png)

- Cliquez sur **Ajouter un effet…** et choisissez un type d’effet. Ajoutez plusieurs
  blocs si besoin ; le **✕** rouge dans l’en-tête supprime un effet.
- Faites glisser la poignée **≡** pour **réordonner** la chaîne. L’effet **en haut** s’exécute
  **en premier**.
- Les réglages utilisent des millisecondes et des hertz, cohérents après conversion vers le
  **taux d’échantillonnage du projet**. **Mono** et **stéréo** sont pris en charge (le mono est
  traité en dual-mono puis remixé sur un canal).
- **OK** enregistre les changements dans le projet ; **Annuler** restaure la chaîne telle qu’à
  l’ouverture du dialogue.

Les effets sont appliqués à l’**arrêt de l’enregistrement**, après la capture et le
rééchantillonnage habituels. La configuration est stockée dans `project.json` sous
`audioEffectChain`.

### Mix effects (full project)

**Project → Project Settings → Mix Effects** lets you build the same kind of ordered effect chain as **Track effects** (**Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion**, **Amp & cabinet**, **Vibrato (Tremolo)**), but applied to the **entire mixed program**: when you press **Play** to hear all enabled tracks together, and when you export with **Mix tracks to file** (toolbar or **Tools** menu). The chain is saved in `project.json` under `projectSettings` → `mixEffectChain`.

**Project → Project Settings → Aux / Send Bus** configures a **shared effect chain** fed by each track’s **Aux** send slider (on the track row). The dry mix of all tracks is summed, each track’s post-gain/post-pan signal is scaled by its **Aux** level and sent through this bus, then the **wet aux** output is added back to the dry sum **before** **Mix Effects** run. Use it for a single shared reverb/delay while keeping per-track insert effects independent.

To reduce harsh [digital clipping](https://en.wikipedia.org/wiki/Clipping_%28audio%29) when processing pushes peaks toward full scale, the effect engine applies a **soft limiter** to normalized float samples immediately before conversion to 16-bit PCM. The **EffectWidget** base class documents `guardFloatSampleForInt16Pcm()` and `softLimitFloatSampleForInt16Pcm()` for any new real-time code that writes to 16-bit audio.

### Per-track mixer, mute/solo, trim, and MIDI details

Each track row includes a compact **mixer strip**:

- **Gain**: Per-track level in decibels (the slider uses tenths of a dB; 0 dB = unity gain).
- **Pan**: Stereo placement (-100 = full left, +100 = full right).
- **Aux**: Send amount (0–100%) into the project **Aux / Send Bus** (see above).
- **Mute**: Silences the track in the mix without disabling it in the arrange view.
- **Solo**: If **any** track has **Solo** enabled, **only** soloed tracks are heard (unless they are also muted).

**Options → Track Configuration** also offers **Clip trim (non-destructive)**: **Trim start** and **Trim end** skip that many seconds from the beginning and end of the clip for **playback, mix, and export** without deleting the underlying recording.

MIDI tracks can carry **control change (CC)** automation stored in the project and in exported `.mid` files; offline playback and mix use these events when rendering MIDI to audio.

**Edit → Undo** / **Redo** (standard shortcuts) apply to mixer and trim changes made on tracks.

### Écoute pendant l’enregistrement

À côté de l’**affichage du temps**, la case **Écouter l’audio pendant l’enregistrement** active ou non l’envoi de l’**entrée en direct** vers la **sortie audio du projet** pendant l’enregistrement :

- **Pistes audio** : le signal d’entrée est joué en temps réel (la chaîne d’enregistrement habituelle ne change pas). Cela s’ajoute à la lecture **overdub** des autres pistes.
- **Pistes MIDI** : si le projet **restitue le MIDI en audio pour la lecture** et qu’un **SoundFont** est défini, vous entendez le jeu via le synthétiseur logiciel. Avec une **sortie MIDI externe**, utilisez le monitoring de l’instrument.

Le réglage est **enregistré dans le projet** (`monitorWhileRecording` dans `project.json`). Décochez pour limiter l’acoustique vers le micro (retour).

#### Enregistrement en overdub

Lors de l'enregistrement d'une nouvelle piste alors que d'autres pistes activées
contiennent déjà des données audio ou MIDI, Musician's Canvas effectue un enregistrement
en overdub : les pistes existantes sont mixées ensemble et lues en temps réel pendant
que la nouvelle piste est enregistrée. Cela vous permet d'écouter les parties
précédemment enregistrées tout en en enregistrant une nouvelle.

Le mixage des pistes existantes est préparé avant le début de la capture, de sorte que
l'enregistrement et la lecture démarrent approximativement au même instant, maintenant
toutes les pistes synchronisées.

#### Moteurs d'enregistrement

Musician's Canvas prend en charge deux moteurs de capture audio :

- **PortAudio** (par défaut lorsque disponible) : Fournit un enregistrement fiable à
  faible latence et constitue le moteur recommandé.
- **Qt Multimedia** : Un moteur de secours utilisant la capture audio intégrée de Qt.
  Utilisé lorsque PortAudio n'est pas disponible ou lorsqu'il est explicitement
  sélectionné dans les paramètres du projet.

Le moteur d'enregistrement peut être configuré par projet dans **Project > Project
Settings > Audio**.

#### Fréquence d'échantillonnage et gestion des périphériques

Musician's Canvas enregistre à la fréquence d'échantillonnage native du périphérique
d'entrée audio, puis convertit automatiquement à la fréquence d'échantillonnage
configurée du projet en utilisant un rééchantillonnage de haute qualité. Cela signifie
que vous pouvez définir n'importe quelle fréquence d'échantillonnage pour le projet
(par ex., 44100 Hz ou 48000 Hz) indépendamment de la fréquence native du périphérique.
La conversion préserve exactement la hauteur et la durée.

#### Détection des périphériques mono

Certains périphériques audio (par ex., les microphones de webcams USB) sont physiquement
mono mais sont annoncés comme stéréo par le système d'exploitation. Musician's Canvas
détecte automatiquement cette situation et ajuste le nombre de canaux en conséquence. Si
le projet est configuré en stéréo, le signal mono est dupliqué sur les deux canaux.

### Enregistrement MIDI

1. Définissez le type de piste sur **MIDI** via le bouton Options.
2. Assurez-vous qu'un périphérique d'entrée MIDI est configuré dans **Settings >
   Configuration > MIDI**.
3. Armez la piste et cliquez sur Record.
4. Jouez des notes sur votre contrôleur MIDI.
5. Cliquez sur Stop pour arrêter l'enregistrement.

Les notes MIDI sont affichées dans une visualisation en piano roll sur la piste.

## Métronome

Musician's Canvas inclut un métronome intégré qui peut être utilisé pendant l'enregistrement
pour aider à garder le tempo. Cliquez sur le bouton du métronome dans la barre de boutons
(sous la barre de menus) pour ouvrir la boîte de dialogue des paramètres du métronome :

![Paramètres du métronome](../screenshots/i18n/fr/MetronomeSettings.png)

La boîte de dialogue fournit :

- **Enable metronome during recording** : Lorsque cette option est cochée, le métronome joue
  un son de tic pendant que l'enregistrement est actif. Le tic est joué via l'audio système et
  **n'est pas** capturé dans la piste enregistrée.
- **Beats per minute** : Une entrée numérique pour le tempo, en battements par minute (BPM).
  La plage est de 20 à 300 BPM.

  When **Project → Tempo map** defines tempo changes, the metronome follows those markers during recording (the BPM field still sets the initial tempo until a marker applies).
Lorsque le métronome est activé, il commence à tictaquer une fois que l'enregistrement démarre
réellement (après la fin du compte à rebours de 3 secondes) et s'arrête lorsque l'enregistrement
se termine.

## Lecture

Cliquez sur le bouton **Play** pour mixer et lire toutes les pistes activées. L'info-bulle
du bouton change pour indiquer s'il va lire ou enregistrer selon qu'une piste est armée
ou non. Les pistes désactivées (décochées) sont exclues de la lecture.

Pendant la lecture, les pistes audio sont décodées depuis leurs fichiers FLAC et les
pistes MIDI sont rendues en audio à l'aide du synthétiseur FluidSynth intégré. Toutes
les pistes sont mixées ensemble et jouées via le périphérique de sortie audio du système.

Cliquez sur le bouton **Stop** pour arrêter la lecture à tout moment.

## Mixage vers un fichier

Utilisez **Tools > Mix tracks to file** (Ctrl+M) pour exporter toutes les pistes activées
dans un seul fichier audio. Une boîte de dialogue vous permet de choisir le chemin de
sortie et le format :

- **Fichier de sortie** : Naviguez pour sélectionner le chemin du fichier de destination.
- **Format** : Choisissez entre FLAC (compression sans perte, fichiers plus petits) ou
  WAV (non compressé).

Le mixage utilise la fréquence d'échantillonnage configurée du projet. Les pistes MIDI
sont rendues en utilisant le SoundFont configuré.

## Paramètres

### Paramètres globaux

Utilisez **Settings > Configuration** (Ctrl+,) pour définir les valeurs par défaut
globales qui s'appliquent à tous les projets :

![Paramètres généraux](../screenshots/i18n/fr/MusiciansCanvas_3_GeneralSettings.png)

#### Onglet General

- **Thème** : Choisissez entre les thèmes sombre (Dark) et clair (Light).
- **Écrire le journal de débogage d'enregistrement** : Lorsque cette option est activée,
  Musician's Canvas écrit des informations de diagnostic détaillées sur l'enregistrement
  dans un fichier appelé `recording_debug.txt` dans le répertoire du projet. Cela inclut
  le moteur de capture, le nom du périphérique, la fréquence d'échantillonnage, le nombre
  de canaux et les données de chronométrage. Cette option est désactivée par défaut et est
  principalement utile pour le dépannage des problèmes d'enregistrement.

#### Onglet Display

- **Couleur de l'afficheur LED numérique** : Choisissez la couleur utilisée pour
  l'afficheur LED numérique du temps dans la barre d'outils de la fenêtre principale.
  Les segments actifs des chiffres sont dessinés dans la couleur choisie, et les
  segments inactifs sont dessinés dans une version atténuée de la même couleur. Les
  couleurs disponibles sont Light Red, Dark Red, Light Green, Dark Green, Light Blue,
  Dark Blue, Yellow, Orange, Light Cyan et Dark Cyan. La valeur par défaut est Light
  Green.

![Paramètres d'affichage](../screenshots/i18n/fr/MusiciansCanvas_DisplaySettings.png)

#### Onglet Language

- **Langue** : Choisissez la langue d'affichage de l'application. La valeur par défaut
  est « System Default », qui utilise le paramètre de langue du système d'exploitation.
  Les langues disponibles sont English, Deutsch (German), Español (Spanish), Français
  (French), Japanese, Português (Brazilian Portuguese), Chinese (Traditional), Chinese
  (Simplified), Dansk (Danish), Ελληνικά (Greek), Gaeilge (Irish), Norsk (Norwegian),
  Polski (Polish), Русский (Russian), Suomi (Finnish), Svenska (Swedish), Cymraeg (Welsh)
  et Pirate. L'interface se met à jour immédiatement lorsque vous changez la langue.

![Paramètres de langue](../screenshots/i18n/fr/MusiciansCanvas_LanguageSettings.png)

#### Onglet MIDI

- **Périphérique de sortie MIDI** : Sélectionnez le synthétiseur FluidSynth intégré ou
  un périphérique MIDI externe. Utilisez le bouton **Refresh** pour relancer la détection
  des périphériques MIDI disponibles.
- **SoundFont** : Naviguez vers un fichier SoundFont `.sf2` pour la synthèse MIDI. Sous
  Linux, un SoundFont système peut être détecté automatiquement si le paquet
  `fluid-soundfont-gm` est installé. Sous Windows et macOS, vous devez configurer le
  chemin du SoundFont manuellement.

![Paramètres MIDI](../screenshots/i18n/fr/MusiciansCanvas_4_MIDISettings.png)

#### Onglet Audio

- **Périphérique d'entrée audio** : Sélectionnez le microphone ou le périphérique d'entrée
  ligne pour l'enregistrement.
- **Périphérique de sortie audio** : Sélectionnez le haut-parleur ou le casque pour la
  lecture.

![Paramètres audio](../screenshots/i18n/fr/MusiciansCanvas_5_AudioSettings.png)

### Paramètres du projet

Utilisez **Project > Project Settings** (Ctrl+P) pour remplacer les valeurs par défaut
globales pour le projet en cours uniquement. Ceci est utile pour les projets nécessitant
une fréquence d'échantillonnage, un SoundFont ou un périphérique audio spécifique. Les
paramètres spécifiques au projet sont sauvegardés dans le fichier `project.json`.

![Paramètres MIDI du projet](../screenshots/i18n/fr/MusiciansCanvas_6_ProjectMIDISettings.png)

#### Onglet MIDI

- **Périphérique MIDI** : Sélectionnez un périphérique MIDI pour ce projet, ou laissez la
  valeur par défaut pour utiliser le paramètre global.
- **SoundFont** : Sélectionnez un fichier SoundFont pour ce projet.
- **Refresh** : Relancer la détection des périphériques MIDI disponibles.

#### Onglet Audio

- **Périphérique d'entrée audio** : Sélectionnez le périphérique d'enregistrement pour
  ce projet.
- **Moteur de capture d'enregistrement** (lorsque PortAudio est disponible) :
  - **PortAudio (native input)** : Recommandé. Utilise la même bibliothèque audio que
    Audacity.
  - **Qt Multimedia** : Option de secours utilisant la capture audio intégrée de Qt.
- **Périphérique d'entrée PortAudio** : Lors de l'utilisation du moteur PortAudio,
  sélectionnez le périphérique d'entrée PortAudio spécifique.
- **Périphérique de sortie audio** : Sélectionnez le périphérique de lecture pour ce
  projet.

##### Paramètres du format audio

- **Fréquence d'échantillonnage** : Choisissez parmi les taux standard (8000 Hz à
  192000 Hz). La fréquence native du périphérique est libellée "(native)". Les fréquences
  nécessitant un rééchantillonnage sont libellées "(resampled)". Vous pouvez sélectionner
  n'importe quelle fréquence indépendamment des capacités du périphérique ;
  Musician's Canvas rééchantillonnera automatiquement si nécessaire.
- **Canaux** : Mono ou Stéréo. Si le périphérique ne prend en charge que le mono, l'option
  Stéréo est désactivée.

![Paramètres audio du projet](../screenshots/i18n/fr/MusiciansCanvas_7_ProjectAudioSettings.png)

#### Mix Effects tab

The **Mix Effects** tab is a scrollable list with the same controls as **Track effects** (**Add effect…**, drag **≡** to reorder, **✕** to remove). Processing order is **top to bottom** on the **combined** mix of all enabled tracks. These effects run during **whole-project playback** and when **mixing to a single WAV or FLAC file**; they are **not** baked into individual track files on disk. An empty list leaves the mixed signal unchanged aside from the mixer's own level handling.

#### Aux / Send Bus tab

Configure the **shared aux effect chain** (same effect types as track inserts). Each track’s **Aux** slider on the track row controls how much of that track’s signal is sent through this bus; the wet aux return is summed with the dry mix **before** **Mix Effects** are applied.

## Menus

### Menu File

| Élément du menu | Raccourci | Description                            |
|-----------------|-----------|----------------------------------------|
| Save Project    | Ctrl+S    | Sauvegarder le projet en cours         |
| Open Project    | Ctrl+O    | Ouvrir un fichier de projet existant   |
| Close           | Ctrl+Q    | Quitter l'application                  |

### Menu Project

| Élément du menu                     | Raccourci | Description                                          |
|-------------------------------------|-----------|------------------------------------------------------|
| Project Settings                    | Ctrl+P   | Configure project-specific settings (includes **Aux / Send Bus**) |
| Tempo map                           |          | Edit tempo changes (seconds vs BPM) for metronome and MIDI quantization |
| Add Demo Data to Selected Track     |           | Ajouter des notes MIDI de démonstration              |

### Edit Menu

| Menu Item | Shortcut | Description |
|-----------|----------|-------------|
| Undo      | Ctrl+Z   | Undo recent mixer/trim edits on tracks |
| Redo      | Ctrl+Shift+Z | Redo |

### Menu Settings

| Élément du menu | Raccourci | Description                                    |
|-----------------|-----------|------------------------------------------------|
| Configuration   | Ctrl+,    | Ouvrir les paramètres globaux de l'application |

### Menu Tools

| Élément du menu       | Raccourci | Description                                      |
|-----------------------|-----------|--------------------------------------------------|
| Mix tracks to file    | Ctrl+M    | Exporter toutes les pistes activées vers un fichier |
| Export stems to folder |          | One WAV stem per track (gain/pan/trim; no master Mix Effects) |
| Recording options     |          | **Punch-in** region for audio; **loop playback** for the whole project |
| Quantize MIDI         |          | Snap MIDI note starts to a grid (all MIDI tracks or armed track only) |
| Add drum track        | D        | Piste MIDI batterie et groove `.mid` (voir ci-dessous) |
| Virtual MIDI Keyboard |           | Lancer l'application clavier compagnon           |

### Menu Help

| Élément du menu | Raccourci | Description |
|-----------------|-----------|-------------|
| Manual          | Alt+M     | Ouvre le manuel PDF dans la langue actuellement sélectionnée |
| About           |           | Affiche les informations de version et de l’application |

## Raccourcis clavier

| Raccourci       | Action                              |
|-----------------|-------------------------------------|
| Ctrl+S          | Sauvegarder le projet               |
| Ctrl+Z          | Undo (mixer/trim)              |
| Ctrl+Shift+Z    | Redo                           |
| Ctrl+O          | Ouvrir un projet                    |
| Ctrl+M          | Mixer les pistes vers un fichier    |
| D               | Ajouter une piste de batterie (menu Tools) |
| Ctrl+P          | Paramètres du projet                |
| Ctrl+,          | Paramètres / Configuration          |
| Alt+M           | Open PDF manual (Help)         |
| Ctrl+Q / Alt+F4 | Quitter                            |


### Ajouter une piste de batterie

**Tools → Add drum track** (raccourci **D**) ajoute une piste **MIDI** percussions sur le **canal 10** General MIDI (index 9). Nom par défaut **Drums** (suffixe numérique si besoin).

Un fichier **`.mid`** est écrit tout de suite dans le **dossier du projet** : deux mesures 4/4 (grosse caisse, caisse claire, charleston fermé). Tempo :

- Si **Activer le métronome pendant l’enregistrement** est coché dans le **métronome**, le **BPM** configuré est utilisé.
- Sinon le **BPM est estimé** à partir des pistes **audio** **activées** ; sinon **120 BPM**.

**Note :** estimation simple ; risque de demi/double temps.

**Liens :** [Audient](https://audient.com/tutorial/how-to-program-realistic-midi-drum-tracks/), [MDrummer](https://www.meldaproduction.com/MDrummer), [Reddit](https://www.reddit.com/r/ableton/comments/1e51a7g/generating_midi_drum_patterns_based_on_audio_input/), [CS229 PDF](https://cs229.stanford.edu/proj2014/Louis%20Eugene,%20Guillaume%20Rostaing,%20Automated%20Music%20Track%20Generation.pdf).

## Virtual MIDI Keyboard

Virtual MIDI Keyboard est une application compagnon (`virtual_midi_keyboard`) qui fournit
un clavier de piano à l'écran pour envoyer des notes MIDI. Elle peut être lancée depuis
le menu **Tools > Virtual MIDI Keyboard** dans l'application principale, ou exécutée
indépendamment.

![Virtual MIDI Keyboard](../screenshots/i18n/fr/VMIDIKeyboard1.png)

### Fonctionnalités

- Cliquez sur les touches du piano à l'écran pour jouer des notes
- Utilisez le clavier de l'ordinateur comme clavier de piano (voir la correspondance des touches ci-dessous)
- Connexion à des périphériques de sortie MIDI externes ou utilisation du synthétiseur FluidSynth intégré
- Connexion à un périphérique d'entrée MIDI pour afficher les notes entrantes sur le clavier
- Volume du synthétiseur ajustable (gain principal, 10%--200%)
- Sélection du SoundFont pour le synthétiseur intégré
- Sélection de l'instrument/programme MIDI (General MIDI ou presets SoundFont)
- Bouton de contrôle chorus/effet
- Décalage d'octave (-3 à +5)
- Contrôle du volume MIDI (CC#7, 0--127)

### Contrôles de la barre d'outils

- **Volume** : Volume MIDI (CC#7), ajustable de 0 à 127 via la boîte de sélection.
- **Octave** : Décalez l'octave du clavier en utilisant les boutons **<** et **>** ou la
  boîte de sélection. La plage va de -3 à +5.
- **Chorus/Effect** : Un bouton rotatif et un champ texte (1--127) pour définir le niveau
  de chorus/effet (MIDI CC#93). Cliquez sur **Apply** pour envoyer la valeur.
- **Canal MIDI** : Une zone de saisie numérique (1--16) sélectionne le canal MIDI sur
  lequel les notes, les changements de programme et les changements de contrôle sont
  envoyés. La valeur par défaut est le canal 1. Le canal sélectionné prend effet
  immédiatement lors du changement. Choisir un preset SoundFont alors qu'un canal
  particulier est sélectionné applique ce bank/program à ce canal, ce qui vous permet
  de jouer des sons différents sur des canaux différents.
- **Batterie** : Une case à cocher située juste au-dessus du sélecteur de canal MIDI.
  Lorsqu'elle n'est pas cochée (valeur par défaut), le canal MIDI est celui que
  l'utilisateur a choisi. Lorsqu'elle est cochée, le canal MIDI est automatiquement
  basculé sur le canal 10 — le canal de batterie standard de General MIDI — pour que
  tout ce qui est joué sonne comme des percussions. Décocher la case restaure le canal
  qui était sélectionné avant l'activation de Batterie.
- **Sélecteur d'instrument** : Choisissez un instrument MIDI. Lors de l'utilisation du
  synthétiseur FluidSynth intégré, cela affiche les presets du SoundFont
  (Banque:Programme Nom). Lorsqu'il est connecté à un périphérique MIDI externe, cela
  liste les 128 instruments General MIDI.

### Jouer avec le clavier de l'ordinateur

Le clavier de l'ordinateur est mappé sur les touches de piano sur deux octaves :

**Octave inférieure (commençant à l'octave actuelle) :**

| Touche | Note  |
|--------|-------|
| Z      | C     |
| S      | C#/Db |
| X      | D     |
| D      | D#/Eb |
| C      | E     |
| V      | F     |
| G      | F#/Gb |
| B      | G     |
| H      | G#/Ab |
| N      | A     |
| J      | A#/Bb |
| M      | B     |

**Octave supérieure (une octave plus haut) :**

| Touche | Note  |
|--------|-------|
| Q      | C     |
| 2      | C#/Db |
| W      | D     |
| 3      | D#/Eb |
| E      | E     |
| R      | F     |
| 5      | F#/Gb |
| T      | G     |
| 6      | G#/Ab |
| Y      | A     |
| 7      | A#/Bb |
| U      | B     |
| I      | C (octave suivante) |
| 9      | C#/Db |
| O      | D     |
| 0      | D#/Eb |
| P      | E     |

Les touches s'illuminent visuellement lorsqu'elles sont pressées (les touches blanches
deviennent bleu clair, les touches noires s'assombrissent).

### Configuration

Ouvrez la boîte de dialogue de configuration (**Settings > Configuration**, Ctrl+,) pour
configurer les périphériques MIDI et audio :

![Configuration de Virtual MIDI Keyboard](../screenshots/i18n/fr/VMIDIKeyboard2.png)

#### Onglet MIDI

- **Périphérique de sortie MIDI** : Sélectionnez un périphérique externe ou laissez vide
  pour le synthétiseur FluidSynth intégré.
- **Périphérique d'entrée MIDI** : Sélectionnez un contrôleur pour transmettre les notes
  à l'affichage du clavier.
- **Volume du synthétiseur (gain principal)** : Ajustez le niveau de sortie du
  synthétiseur intégré (10%--200%).
- **SoundFont** : Naviguez vers un fichier SoundFont `.sf2` pour le synthétiseur intégré.
- **Refresh** : Relancer la détection des périphériques MIDI disponibles.

#### Onglet Audio

- **Périphérique de sortie audio** : Sélectionnez le périphérique de sortie pour le
  synthétiseur intégré.

#### Onglet Language

- **Langue** : Choisissez la langue d'affichage. Les mêmes langues que Musician's Canvas
  sont disponibles. L'interface se met à jour immédiatement lorsque vous changez la langue.

### Raccourcis de Virtual MIDI Keyboard

| Raccourci | Action                          |
|-----------|---------------------------------|
| Ctrl+,    | Boîte de dialogue Configuration |
| Ctrl+U    | Aide / Informations d'utilisation |
| Ctrl+Q    | Fermer                          |

## FAQ (Foire aux questions)

### Comment enregistrer une piste MIDI ?

1. Définissez un **répertoire de projet** (obligatoire pour enregistrer).
2. Ajoutez ou sélectionnez une piste et ouvrez **Options** (ou cliquez sur l’icône de type de piste).
3. Réglez le type de piste sur **MIDI**, puis fermez la boîte de dialogue.
4. Dans **Settings > Configuration > MIDI**, choisissez l’**entrée MIDI** (port matériel ou câble virtuel) et un **SoundFont** (`.sf2`) pour la lecture ultérieure.
5. **Armez** cette piste (une seule piste peut être armée à la fois).
6. Cliquez sur **Record**, attendez le décompte, jouez sur le contrôleur, puis **Stop**.

Les notes s’affichent dans le piano roll. **File > Save Project** écrit un fichier `.mid` (et `project.json`) dans le dossier du projet.

### Pourquoi ma piste MIDI est-elle silencieuse à la lecture ?

La lecture utilise **FluidSynth** avec le **SoundFont** défini dans les réglages. Vérifiez **Settings > Configuration > MIDI** (ou **Project > Project Settings** si vous surchargez par projet) : un chemin `.sf2` valide doit être défini. Sous Linux, un SoundFont système peut être détecté automatiquement ; sous Windows et macOS, il faut en général choisir manuellement un fichier SoundFont.

### Quel est le lien entre Virtual MIDI Keyboard et Musician's Canvas ?

Ce sont **deux applications distinctes**. Lancez Virtual MIDI Keyboard via **Tools > Virtual MIDI Keyboard** (ou seul). Pour envoyer les notes du clavier à l’écran **vers** Musician's Canvas pendant l’enregistrement d’une piste MIDI, le système doit router la **sortie MIDI** du clavier vers une **entrée** utilisée par Musician's Canvas — souvent via un câble MIDI virtuel ou des ports cohérents dans les deux apps. Elles ne se connectent pas automatiquement.

### Quelle est la différence entre Configuration et Project Settings ?

**Settings > Configuration** définit les **valeurs par défaut globales** (thème, langue, périphériques MIDI/audio, SoundFont, etc.) dans les paramètres de l’application. **Project > Project Settings** remplace certaines valeurs **pour le projet courant uniquement** et est enregistré dans `project.json`. Si un champ reste au défaut projet, la valeur globale de Configuration s’applique.

### Pourquoi le glisser-déposer n’ajoute-t-il pas de fichiers audio ?

Le dépôt n’est accepté que si un **répertoire de projet est défini** et que Musician's Canvas **ne lit ni n’enregistre** pas. Les types pris en charge sont **`.wav`** et **`.flac`** ; les autres extensions sont ignorées et listées dans une boîte de dialogue. Chaque fichier devient une nouvelle piste **audio** nommée d’après le nom de fichier sans extension.

### Où sont stockés mes enregistrements ?

Les enregistrements audio sont dans le **répertoire du projet** sous **`<nom_de_piste>.flac`** (caractères tels que `/ \ : * ? " < > |` sont remplacés par des tirets bas). Le fichier projet est **`project.json`** dans le même dossier. Les pistes MIDI sont enregistrées en **`<nom_de_piste>.mid`** lorsque vous sauvegardez le projet (en plus des notes dans `project.json`).

### Quel nom de fichier pour le rendu MIDI lors du mixage ?

Lors du **mixage** ou de la **lecture**, le MIDI est d’abord rendu en WAV temporaire en interne. Si le chemin du projet est connu, Musician's Canvas écrit aussi un **FLAC de cache** dans le dossier du projet : **`<nom_de_piste_assaini>.flac`** (même règle d’assainissement que les autres fichiers de piste). Le nom suit le **nom de piste**, pas un identifiant interne.

### Puis-je enregistrer deux pistes en même temps ?

Non. Une seule piste peut être **armée** à la fois ; elle reçoit l’enregistrement suivant. Vous construisez le morceau en enregistrant **l’une après l’autre** (l’**overdub** lit les pistes existantes pendant une nouvelle prise).

### Le métronome est-il enregistré sur la piste ?

Non. S’il est activé, le métronome passe par l’**audio système** pour vous guider. Il **n’est pas** inclus dans le fichier enregistré.

### Pourquoi Musician's Canvas exige-t-il ASIO sous Windows ?

Sous Windows, l’application principale attend un pilote **ASIO** pour une latence faible et fiable. Installez par exemple **ASIO4ALL** ou le pilote du fabricant de votre interface en cas d’erreur au démarrage ou audio.

### Sous macOS, où se trouve Virtual MIDI Keyboard ?

Dans le **bundle `.app`**, l’exécutable Virtual MIDI Keyboard est **copié dans** `Musician's Canvas.app` (**Contents/MacOS/**) pour ne distribuer qu’un seul dossier d’application. Vous pouvez toujours le lancer via **Tools > Virtual MIDI Keyboard**.

## Dépannage

### Pas de sortie audio

- Vérifiez que le bon périphérique de sortie audio est sélectionné dans Settings >
  Configuration > Audio.
- Sous Linux, vérifiez que PipeWire ou PulseAudio est en cours d'exécution et que la
  sortie n'est pas coupée. Utilisez `amixer` ou les paramètres sonores de votre bureau
  pour vérifier les niveaux de volume.
- Sous Windows, assurez-vous qu'un pilote audio ASIO est installé (par ex.,
  [ASIO4ALL](https://asio4all.org/) ou un pilote ASIO fourni par le fabricant pour votre
  interface audio). Musician's Canvas nécessite ASIO pour un audio à faible latence sous
  Windows.

### Les pistes MIDI sont silencieuses

- Assurez-vous qu'un SoundFont (fichier `.sf2`) est configuré dans Settings >
  Configuration > MIDI.
- Sous Linux, un SoundFont système peut être détecté automatiquement si le paquet
  `fluid-soundfont-gm` est installé.
- Sous Windows et macOS, vous devez configurer le chemin du SoundFont manuellement.

### L'enregistrement sonne déformé ou a une mauvaise hauteur

- Cela peut se produire lorsque la fréquence d'échantillonnage native du périphérique
  d'entrée audio diffère de la fréquence configurée du projet. Musician's Canvas gère
  automatiquement cela via le rééchantillonnage, mais si les problèmes persistent, essayez
  de régler la fréquence d'échantillonnage du projet pour correspondre à la fréquence
  native du périphérique.
- Les microphones de webcams USB ont souvent des fréquences natives inhabituelles (par
  ex., 32000 Hz). L'application les détecte automatiquement.
- Si vous utilisez le moteur Qt Multimedia et rencontrez des problèmes, essayez de passer
  au moteur PortAudio dans Project Settings > Audio.

### Virtual MIDI Keyboard n'a pas de son

- Sous Linux avec PipeWire, assurez-vous que le paquet `libpipewire-0.3-dev` est installé
  (nécessaire pour l'intégration de PipeWire avec le synthétiseur FluidSynth).
- Vérifiez qu'un SoundFont est chargé (voir l'onglet MIDI dans la boîte de dialogue de
  configuration).
- Vérifiez que le périphérique de sortie audio est sélectionné et que le volume du système
  n'est pas coupé.

## Compilation depuis les sources

Consultez le [README](../README.md) pour les instructions complètes de compilation pour
Linux, macOS et Windows, y compris toutes les dépendances requises.
