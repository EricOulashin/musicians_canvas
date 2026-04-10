# Musician's Canvas

Ceci est une application d'enregistrement musical multipiste pour PC de bureau. Elle est écrite en C++ et utilise Qt comme
bibliothèque d'interface graphique. Cette application est conçue pour Linux, Windows et Mac OS. La facilité d'utilisation est une
considération de conception principale. Ce projet a commencé comme une expérience avec Cursor (outil de codage par IA) pour voir ce qu'il
serait capable de créer comme application de ce type, avec une utilisation continue du développement par IA ainsi
que du développement logiciel traditionnel.

Il y a des années, j'ai créé un programme d'enregistrement musical multipiste à l'université (avec le même nom) ; ceci est une tentative
de créer quelque chose de mieux.

## Fonctionnalités

- **Arrangement multipiste** : Ajouter et supprimer plusieurs pistes pour composer des chansons
- **Pistes nommées** : Chaque piste a un nom modifiable utilisé comme nom de fichier pour l'audio enregistré
- **Types de piste** : Configurer chaque piste pour l'enregistrement audio (depuis un microphone/entrée ligne) ou l'enregistrement MIDI (depuis un clavier/contrôleur) ; cliquez sur l'icône de type de piste pour changer rapidement la source d'entrée
- **Armement de piste** : Cochez "Arm" sur une piste pour la sélectionner comme cible d'enregistrement ; une seule piste peut être armée à la fois. Une piste unique non enregistrée est automatiquement armée par commodité
- **Compte à rebours d'enregistrement** : Un compte à rebours visuel de 3 secondes avant le début de l'enregistrement, donnant à l'interprète le temps de se préparer
- **Enregistrement overdub** : Lors de l'enregistrement d'une nouvelle piste avec des pistes existantes activées, les pistes existantes sont mixées et lues en temps réel pour que vous puissiez les entendre pendant l'enregistrement. La lecture et la capture sont synchronisées pour garder toutes les pistes alignées
- **Retour visuel** : Affichage de forme d'onde audio pour les pistes audio (avec vumètre en direct pendant l'enregistrement), vue piano roll MIDI pour les pistes MIDI
- **Synthétiseur MIDI intégré** : Effectue le rendu des pistes MIDI en audio en utilisant FluidSynth avec un SoundFont configurable
- **Mixage vers un seul fichier audio** : Exporter toutes les pistes activées vers un seul fichier WAV ou FLAC mixé en utilisant la bibliothèque [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)
- **Sauvegarder / Ouvrir un projet** : Sérialiser et restaurer le projet complet (pistes, noms, types, notes MIDI, références de fichiers audio) vers/depuis un fichier JSON, avec détection des modifications non sauvegardées à la fermeture
- **Paramètres spécifiques au projet** : Remplacer les valeurs par défaut globales MIDI et audio par projet (fréquence d'échantillonnage, SoundFont, périphérique MIDI)
- **Capture PortAudio (compilation optionnelle)** : Lorsque le projet est compilé avec PortAudio (`HAVE_PORTAUDIO`), l'enregistrement peut utiliser un chemin d'entrée natif PortAudio (similaire en esprit à Audacity) au lieu de Qt Multimedia. **Project → Project Settings → Audio** permet de choisir **PortAudio** ou **Qt Multimedia** et de sélectionner un périphérique d'entrée PortAudio. Si PortAudio n'est pas installé, la compilation réussit quand même et l'enregistrement utilise uniquement Qt Multimedia.
- **Conversion de fréquence d'échantillonnage de haute qualité** : Enregistre à la fréquence native du périphérique audio et convertit vers la fréquence du projet en utilisant l'interpolation sinc fenêtrée (fenêtre de Kaiser, ~96 dB d'atténuation de bande d'arrêt), la même famille d'algorithmes utilisée par Audacity / libsoxr. Cela permet l'enregistrement à n'importe quelle fréquence d'échantillonnage du projet (8000 Hz à 192000 Hz) indépendamment de la fréquence native du périphérique, sans changement de hauteur ou de durée.
- **Gestion automatique mono/stéréo** : Détecte les périphériques physiquement mono annoncés comme stéréo (courant avec les microphones de webcam USB sur PipeWire) et convertit entre mono et stéréo selon les besoins (duplication ou moyenne), suivant l'approche de routage de canaux d'Audacity
- **Effets d'insertion à l'enregistrement (pistes audio)** : Utilisez le bouton **Effects** (sous **Options** sur chaque piste audio) pour ouvrir la boîte de dialogue **Track effects**. Ajoutez et configurez **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion** et **Amp & cabinet** (modélisation ampli et baffle), faites glisser la poignée **≡** pour réordonner la chaîne (le haut s'exécute en premier) et enregistrez avec le projet. Les effets sont appliqués à la prise lorsque vous arrêtez l'enregistrement ; les paramètres sont en unités réelles (ms, Hz) pour un comportement cohérent en mono ou stéréo et pour les fréquences d'échantillonnage de projet courantes (8 kHz–192 kHz) après normalisation de la capture. Voir le [manuel utilisateur](docs/MusiciansCanvas_User_Manual_fr.md). Les traducteurs peuvent mettre à jour les chaînes en masse via `scripts/effect_i18n.tsv` (généré par `scripts/build_effect_tsv.py`) et `scripts/fill_effect_i18n.py` après `lupdate`.
- **Écoute à l'enregistrement** : La case **Écouter l'audio pendant l'enregistrement** (à droite de l'horloge) envoie l'entrée **live** vers la **sortie audio du projet** pendant la capture. **Audio** : entendre ce qui est enregistré, en plus de l'overdub. **MIDI** : avec **restituer le MIDI en audio pour la lecture** et un SoundFont, synthé logiciel. Réglage **enregistré dans le projet** (`monitorWhileRecording`). Décochez pour limiter le larsen.
- **Audio à faible latence** : Sous Windows, la détection du pilote ASIO assure un audio à faible latence ; sous Linux, la priorité de planification du processus est élevée pour un jitter réduit avec PipeWire / PulseAudio / ALSA
- **Virtual MIDI Keyboard** : Une application compagnon pour envoyer des notes MIDI via un clavier piano logiciel, avec synthétiseur FluidSynth intégré, gain principal ajustable, mappage clavier d'ordinateur vers piano, sélection d'instrument/programme, contrôle de chorus/effet et décalage d'octave
- **Configuration** : Sélection du périphérique d'entrée audio, du périphérique MIDI et du fichier SoundFont (valeurs par défaut globales et remplacements par projet)
- **Thème sombre / clair** : Configurable via Settings → Configuration
- **Localisation** : Les deux applications sont localisées en 18 langues : anglais, allemand, espagnol, français, japonais, portugais (Brésil), chinois traditionnel, chinois simplifié, russe, suédois, finnois, danois, norvégien, polonais, grec, irlandais, gallois et pirate. La langue par défaut correspond au paramètre du système d'exploitation et peut être changée dans **Settings → Configuration → Language**. Le système standard i18n de Qt (`QTranslator` / `.ts` / `.qm`) est utilisé, et l'interface se met à jour immédiatement lorsque la langue est changée.
- **Manuel utilisateur** : Documentation HTML et PDF générée à partir de sources Markdown, disponible dans toutes les langues prises en charge (voir [docs/](docs/))

## Captures d'écran

<p align="center">
	<a href="screenshots/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/MusiciansCanvas_1_MainWin.png" alt="Fenêtre principale" width="800"></a>
	<a href="screenshots/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/MusiciansCanvas_2_TrackConfig.png" alt="Configuration de piste" width="800"></a>
	<a href="screenshots/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_3_GeneralSettings.png" alt="Paramètres généraux" width="800"></a>
	<a href="screenshots/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_4_MIDISettings.png" alt="Paramètres MIDI par défaut/généraux" width="800"></a>
	<a href="screenshots/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_5_AudioSettings.png" alt="Paramètres audio par défaut/généraux" width="800"></a>
	<a href="screenshots/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_DisplaySettings.png" alt="Paramètres d'affichage" width="800"></a>
	<a href="screenshots/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_LanguageSettings.png" alt="Paramètres de langue" width="800"></a>
	<a href="screenshots/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Paramètres MIDI spécifiques au projet" width="800"></a>
	<a href="screenshots/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Paramètres audio spécifiques au projet" width="800"></a>
	<a href="screenshots/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/Track_Effects_Dialog.png" alt="Boîte de dialogue Effets de piste" width="800"></a>
	<a href="screenshots/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Dépendances

### Compilateur

Un compilateur compatible **C++17** est requis (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (requis sur toutes les plateformes)

La fonctionnalité de mixage et d'exportation FLAC dépend de la bibliothèque
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp).
Clonez-la comme répertoire frère de ce dépôt avant de compiler :

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

La compilation CMake s'attend à la trouver à `../audio_mixer_cpp` par rapport à la racine de ce projet.

---

### Qt6 Multimedia (optionnel mais fortement recommandé)

`qt6-multimedia` est optionnel. Sans lui, l'application se compile et s'exécute, mais
**l'enregistrement et la lecture audio sont désactivés** — seuls l'édition MIDI et la gestion de projet
fonctionneront. Installez-le avec les bibliothèques Qt6 principales en utilisant les commandes de plateforme ci-dessous.

---

### PortAudio (optionnel)

Si la bibliothèque et les en-têtes PortAudio sont installés, CMake active **`HAVE_PORTAUDIO`** et
le lie à **musicians_canvas**. L'enregistrement utilise alors par défaut le chemin PortAudio à moins que
le projet ne soit configuré sur **Qt Multimedia** dans **Project → Project Settings → Audio**. Si PortAudio
n'est pas trouvé, la compilation continue sans lui (le wrapper se compile comme stubs).

---

### SoundFont pour la synthèse MIDI

Un fichier SoundFont (`.sf2`) est requis pour que les pistes MIDI produisent de l'audio. Sans un tel fichier,
les pistes MIDI seront rendues comme du silence. Sous **Linux**, le synthétiseur FluidSynth intégré
détectera automatiquement un SoundFont si un est installé dans un chemin système standard
(voir les noms de paquets ci-dessous). Sous **macOS et Windows**, il n'y a pas de chemin système standard,
vous devez donc configurer le SoundFont manuellement dans
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

> `fluid-soundfont-gm` installe `FluidR3_GM.sf2` dans `/usr/share/sounds/sf2/` et est
> détecté automatiquement au démarrage. `timgm6mb-soundfont` est une alternative plus légère.
>
> `libpipewire-0.3-dev` est requis sur les systèmes basés sur PipeWire pour que Virtual MIDI
> Keyboard puisse appeler `pw_init()` avant que FluidSynth ne crée ses ressources audio.
> La compilation se poursuit sans lui ; le drapeau `HAVE_PIPEWIRE` n'est simplement pas défini.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` installe `FluidR3_GM.sf2` dans `/usr/share/soundfonts/` et est
> détecté automatiquement au démarrage.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` installe `FluidR3_GM.sf2` dans `/usr/share/soundfonts/` et est
> détecté automatiquement au démarrage. `pipewire` est généralement déjà installé sur les systèmes
> Arch modernes ; les en-têtes de développement sont inclus dans le paquet principal.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac portaudio
```

> PipeWire est un sous-système exclusivement Linux et n'est **pas** requis sur macOS. FluidSynth
> utilisera automatiquement CoreAudio via le backend Qt Multimedia.
>
> Téléchargez un SoundFont General MIDI (par exemple
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) ou
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> et configurez son chemin dans **Settings → Configuration → MIDI**.

---

### Windows

**musicians_canvas** nécessite un **pilote audio ASIO** pour l'enregistrement et la lecture à faible latence.
L'application ne démarrera pas si aucun pilote ASIO n'est détecté.
**virtual_midi_keyboard** ne nécessite pas ASIO et utilise directement le backend WASAPI de Qt Multimedia.

**Installation d'un pilote ASIO pour musicians_canvas (choisissez-en un) :**

| Pilote | Notes |
|--------|-------|
| [ASIO4ALL](https://asio4all.org/) | Gratuit, fonctionne avec la plupart du matériel audio intégré et USB |
| Pilote du fabricant | Meilleure latence pour les interfaces audio dédiées (par exemple Focusrite, PreSonus, RME) |

**Chaîne d'outils — MSYS2 (recommandé pour les compilations MinGW) :**

Téléchargez et installez MSYS2 depuis <https://www.msys2.org>, puis ajoutez
`C:\msys64\mingw64\bin` à votre `PATH` système. Installez les paquets requis :

```bash
pacman -S mingw-w64-x86_64-qt6-base
pacman -S mingw-w64-x86_64-qt6-multimedia
pacman -S mingw-w64-x86_64-fluidsynth
pacman -S mingw-w64-ucrt-x86_64-rtmidi
pacman -S mingw-w64-x86_64-flac
pacman -S mingw-w64-x86_64-portaudio
pacman -S mingw-w64-x86_64-soundfont-fluid
```

> `mingw-w64-x86_64-soundfont-fluid` installe `FluidR3_GM.sf2` dans
> `C:\msys64\mingw64\share\soundfonts\` (ajustez si MSYS2 est installé ailleurs).
> Comme Windows n'a pas de chemin de recherche standard pour les SoundFont, vous devez configurer ce chemin
> manuellement dans **Settings → Configuration → MIDI** après le premier lancement.
>
> PipeWire est un sous-système exclusivement Linux ; aucun paquet PipeWire n'est nécessaire sous Windows.

Pages de référence des paquets :
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Chaîne d'outils — Visual Studio 2022 :**

Installez Qt 6 via le [Qt Online Installer](https://www.qt.io/download) et obtenez
les binaires FluidSynth, RtMidi et libFLAC (ou compilez-les à partir du code source).
La compilation CMake les localisera via `find_library` / `find_path` tant que les
répertoires appropriés sont dans `CMAKE_PREFIX_PATH`.

Un SoundFont doit être téléchargé séparément (par exemple
[GeneralUser GS](https://schristiancollins.com/generaluser.php) ou
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)) et son
chemin configuré dans **Settings → Configuration → MIDI** après le premier lancement.

> **Remarque :** `advapi32` (registre Windows) et `winmm` (Windows Multimedia) sont
> liés automatiquement par CMake ; aucune installation supplémentaire n'est requise pour ceux-ci.

---

## Compilation

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

**Windows (Visual Studio 2022) :**

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

Si les DLL requises ne sont pas trouvées à l'exécution, lancez le script auxiliaire depuis le répertoire de sortie de compilation :

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Génération de la documentation

Le manuel utilisateur peut être généré en HTML et PDF à partir de la source Markdown :

```bash
cd docs
./generate_docs.sh          # Générer HTML et PDF
./generate_docs.sh html     # Générer uniquement HTML
./generate_docs.sh pdf      # Générer uniquement PDF
```

**Prérequis :**

- **Python (préféré) :** `pip3 install markdown weasyprint`
- **Alternative :** `pandoc` et `wkhtmltopdf` (via le gestionnaire de paquets système)

Le HTML généré est écrit dans `docs/html/` et le PDF dans `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Exécution

```bash
./build/musicians_canvas
# ou
./build/virtual_midi_keyboard   # application compagnon Virtual MIDI Keyboard
```

---

## Utilisation

### musicians_canvas

1. **Définir le répertoire du projet** : Entrez ou naviguez vers un dossier où les enregistrements et le fichier de projet seront stockés
2. **Ajouter des pistes** : Cliquez sur "+ Add Track" ; nommez chaque piste dans le champ de texte à côté de "Options"
3. **Configurer le type de piste** : Cliquez sur "Options" sur une piste (ou cliquez sur l'icône de type de piste entre "Options" et le champ de nom) pour choisir Audio ou MIDI et définir la source d'entrée
4. **Supprimer une piste** : Cliquez sur le bouton "×" sur le côté droit de la ligne de la piste
5. **Paramètres globaux** : Utilisez **Settings → Configuration** pour définir les valeurs par défaut globales :
   - Onglet *General* : Thème (sombre/clair)
   - Onglet *MIDI* : Périphérique de sortie MIDI par défaut (synthétiseur FluidSynth intégré ou un périphérique MIDI externe) et SoundFont par défaut (`.sf2`) pour la synthèse
   - Onglet *Audio* : Périphérique d'entrée/sortie audio par défaut pour l'enregistrement et la lecture
6. **Paramètres du projet** : Utilisez **Project → Project Settings** (Ctrl+P) pour remplacer les paramètres MIDI et audio uniquement pour le projet actuel (par exemple une fréquence d'échantillonnage ou un SoundFont différent par chanson)
7. **Enregistrer** : Cochez "Arm" sur la piste cible, puis cliquez sur le bouton d'enregistrement (cercle rouge). Une seule piste peut être armée à la fois
8. **Lecture** : Cliquez sur le bouton de lecture pour mixer et lire toutes les pistes activées
9. **Mixer vers un fichier** : Utilisez **Tools → Mix tracks to file** (Ctrl+M) pour exporter en WAV ou FLAC
10. **Sauvegarder / Ouvrir** : Utilisez **File → Save Project** (Ctrl+S) et **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Ouvrir la configuration** : Utilisez le bouton ou le menu **Configuration** (Ctrl+,) pour ouvrir le dialogue des paramètres
2. **Onglet MIDI** :
   - Sélectionnez un périphérique de sortie MIDI (un synthétiseur externe matériel/logiciel) ou laissez vide pour utiliser le synthétiseur FluidSynth intégré
   - Sélectionnez un périphérique d'entrée MIDI pour transmettre les notes MIDI entrantes à l'affichage du clavier
   - Ajustez le **Synthesizer Volume (Master Gain)** pour contrôler le niveau de sortie du synthétiseur intégré (10 %–200 %)
3. **Onglet Audio** : Sélectionnez le périphérique de sortie audio utilisé par le synthétiseur intégré
4. **SoundFont** : Sélectionnez un fichier SoundFont `.sf2` pour le synthétiseur intégré (détecté automatiquement sous Linux si un SoundFont système est installé)
5. **Jouer des notes** : Cliquez sur les touches du clavier piano à l'écran, utilisez un contrôleur MIDI connecté ou utilisez le clavier de l'ordinateur :
   - Octave inférieure : Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = dièses/bémols
   - Octave supérieure : Q/W/E/R/T/Y/U/I/O/P = C à E, 2/3/5/6/7/9/0 = dièses/bémols
6. **Contrôles de la barre d'outils** : Ajuster le volume MIDI (0–127), l'octave (-3 à +5), le niveau de chorus/effet et sélectionner les instruments

---

## Raccourcis clavier

**musicians_canvas :**

| Raccourci | Action |
|-----------|--------|
| Ctrl+S | Sauvegarder le projet |
| Ctrl+O | Ouvrir un projet |
| Ctrl+M | Mixer les pistes vers un fichier |
| Ctrl+P | Paramètres du projet |
| Ctrl+, | Paramètres / Configuration |
| Ctrl+Q / Alt+F4 | Quitter |

**virtual_midi_keyboard :**

| Raccourci | Action |
|-----------|--------|
| Ctrl+, | Dialogue de configuration |
| Ctrl+U | Aide / Informations d'utilisation |
| Ctrl+Q | Fermer |

---

## Licence

Ce projet est fourni tel quel pour un usage éducatif et personnel.
