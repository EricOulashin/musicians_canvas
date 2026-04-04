---
title: "Manuel d'utilisation de Musician's Canvas"
subtitle: "Application d'enregistrement musical multipiste"
version: "0.1.0"
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
- Localisé en plusieurs langues (English, Deutsch, Español, Français, Japanese, Português, Chinese et Pirate)
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

![Fenêtre principale](../screenshots/MusiciansCanvas_1_MainWin.png)

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

### Configuration du type de piste

Chaque piste peut être configurée en tant que **Audio** (pour l'enregistrement depuis un
microphone ou une entrée ligne) ou **MIDI** (pour l'enregistrement depuis un clavier ou
un contrôleur).

Pour changer le type de piste :

- Cliquez sur le bouton **Options** de la piste, ou
- Cliquez sur l'**icône du type de piste** (entre "Options" et le champ de nom)

Cela ouvre la boîte de dialogue de configuration de la piste où vous pouvez sélectionner
la source d'entrée.

![Configuration de la piste](../screenshots/MusiciansCanvas_2_TrackConfig.png)

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

![Paramètres généraux](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### Onglet General

- **Thème** : Choisissez entre les thèmes sombre (Dark) et clair (Light).

#### Onglet Language

- **Langue** : Choisissez la langue d'affichage de l'application. La valeur par défaut
  est « System Default », qui utilise le paramètre de langue du système d'exploitation.
  Les langues disponibles sont English, Deutsch (German), Español (Spanish), Français
  (French), Japanese, Português (Brazilian Portuguese), Chinese (Traditional), Chinese
  (Simplified) et Pirate. L'interface se met à jour immédiatement lorsque vous changez
  la langue.

#### Onglet MIDI

- **Périphérique de sortie MIDI** : Sélectionnez le synthétiseur FluidSynth intégré ou
  un périphérique MIDI externe. Utilisez le bouton **Refresh** pour relancer la détection
  des périphériques MIDI disponibles.
- **SoundFont** : Naviguez vers un fichier SoundFont `.sf2` pour la synthèse MIDI. Sous
  Linux, un SoundFont système peut être détecté automatiquement si le paquet
  `fluid-soundfont-gm` est installé. Sous Windows et macOS, vous devez configurer le
  chemin du SoundFont manuellement.

![Paramètres MIDI](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Onglet Audio

- **Périphérique d'entrée audio** : Sélectionnez le microphone ou le périphérique d'entrée
  ligne pour l'enregistrement.
- **Périphérique de sortie audio** : Sélectionnez le haut-parleur ou le casque pour la
  lecture.

![Paramètres audio](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Paramètres du projet

Utilisez **Project > Project Settings** (Ctrl+P) pour remplacer les valeurs par défaut
globales pour le projet en cours uniquement. Ceci est utile pour les projets nécessitant
une fréquence d'échantillonnage, un SoundFont ou un périphérique audio spécifique. Les
paramètres spécifiques au projet sont sauvegardés dans le fichier `project.json`.

![Paramètres MIDI du projet](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

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

![Paramètres audio du projet](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

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
| Project Settings                    | Ctrl+P    | Configurer les paramètres spécifiques au projet      |
| Add Demo Data to Selected Track     |           | Ajouter des notes MIDI de démonstration              |

### Menu Settings

| Élément du menu | Raccourci | Description                                    |
|-----------------|-----------|------------------------------------------------|
| Configuration   | Ctrl+,    | Ouvrir les paramètres globaux de l'application |

### Menu Tools

| Élément du menu       | Raccourci | Description                                      |
|-----------------------|-----------|--------------------------------------------------|
| Mix tracks to file    | Ctrl+M    | Exporter toutes les pistes activées vers un fichier |
| Virtual MIDI Keyboard |           | Lancer l'application clavier compagnon           |

## Raccourcis clavier

| Raccourci       | Action                              |
|-----------------|-------------------------------------|
| Ctrl+S          | Sauvegarder le projet               |
| Ctrl+O          | Ouvrir un projet                    |
| Ctrl+M          | Mixer les pistes vers un fichier    |
| Ctrl+P          | Paramètres du projet                |
| Ctrl+,          | Paramètres / Configuration          |
| Ctrl+Q / Alt+F4 | Quitter                            |

## Virtual MIDI Keyboard

Virtual MIDI Keyboard est une application compagnon (`virtual_midi_keyboard`) qui fournit
un clavier de piano à l'écran pour envoyer des notes MIDI. Elle peut être lancée depuis
le menu **Tools > Virtual MIDI Keyboard** dans l'application principale, ou exécutée
indépendamment.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

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

![Configuration de Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard2.png)

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
