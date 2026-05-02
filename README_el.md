# Musician's Canvas

Αυτή είναι μια εφαρμογή πολυκάναλης ηχογράφησης μουσικής για επιτραπέζιους υπολογιστές. Είναι γραμμένη σε C++ και χρησιμοποιεί το Qt ως
βιβλιοθήκη γραφικής διεπαφής. Αυτή η εφαρμογή προορίζεται για κατασκευή σε Linux, Windows και Mac OS. Η ευκολία χρήσης είναι
μια κύρια σχεδιαστική προτεραιότητα. Αυτό το έργο ξεκίνησε ως πείραμα με το Cursor (εργαλείο κωδικοποίησης AI) για να δούμε τι
θα μπορούσε να δημιουργήσει ως εφαρμογή αυτού του τύπου, με συνεχή χρήση ανάπτυξης AI καθώς
και παραδοσιακής ανάπτυξης λογισμικού.

Πριν από χρόνια, δημιούργησα ένα πρόγραμμα πολυκάναλης ηχογράφησης μουσικής στο πανεπιστήμιο (με το ίδιο όνομα)· αυτή είναι μια προσπάθεια
να δημιουργήσω κάτι καλύτερο.

## Χαρακτηριστικά

- **Πολυκάναλη ενορχήστρωση**: Προσθήκη και αφαίρεση πολλαπλών καναλιών για σύνθεση τραγουδιών
- **Ονομασμένα κανάλια**: Κάθε κανάλι έχει ένα επεξεργάσιμο όνομα που χρησιμοποιείται ως όνομα αρχείου για τον εγγεγραμμένο ήχο
- **Τύποι καναλιών**: Διαμόρφωση κάθε καναλιού για εγγραφή ήχου (από μικρόφωνο/είσοδο γραμμής) ή εγγραφή MIDI (από πλήκτρα/ελεγκτή)· κάντε κλικ στο εικονίδιο τύπου καναλιού για γρήγορη αλλαγή πηγής εισόδου
- **Οπλισμός καναλιού**: Επιλέξτε "Arm" σε ένα κανάλι για να το ορίσετε ως στόχο εγγραφής· μόνο ένα κανάλι μπορεί να είναι οπλισμένο τη φορά. Ένα μεμονωμένο μη εγγεγραμμένο κανάλι οπλίζεται αυτόματα για ευκολία
- **Αντίστροφη μέτρηση εγγραφής**: Μια οπτική αντίστροφη μέτρηση 3 δευτερολέπτων πριν ξεκινήσει η εγγραφή, δίνοντας στον εκτελεστή χρόνο να προετοιμαστεί
- **Εγγραφή overdub**: Κατά την εγγραφή νέου καναλιού ενώ τα υπάρχοντα κανάλια είναι ενεργοποιημένα, τα υπάρχοντα κανάλια αναμειγνύονται και αναπαράγονται σε πραγματικό χρόνο ώστε να μπορείτε να τα ακούτε κατά την εγγραφή. Η αναπαραγωγή και η καταγραφή συγχρονίζονται για να κρατούν όλα τα κανάλια ευθυγραμμισμένα
- **Οπτική ανατροφοδότηση**: Εμφάνιση κυματομορφής ήχου για κανάλια ήχου (με μετρητή στάθμης σε πραγματικό χρόνο κατά την εγγραφή), προβολή MIDI piano roll για κανάλια MIDI
- **Ενσωματωμένος συνθεσάιζερ MIDI**: Αποδίδει κανάλια MIDI σε ήχο χρησιμοποιώντας FluidSynth με διαμορφώσιμο SoundFont
- **Μίξη σε ενιαίο αρχείο ήχου**: Εξαγωγή όλων των ενεργοποιημένων καναλιών σε ένα ενιαίο μεικτό αρχείο WAV ή FLAC χρησιμοποιώντας τη βιβλιοθήκη [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)
- **Αποθήκευση / Άνοιγμα έργου**: Σειριοποίηση και επαναφορά του πλήρους έργου (κανάλια, ονόματα, τύποι, νότες MIDI, αναφορές αρχείων ήχου) σε/από αρχείο JSON, με ανίχνευση μη αποθηκευμένων αλλαγών κατά την έξοδο
- **Ρυθμίσεις ειδικές ανά έργο**: Παράκαμψη καθολικών προεπιλογών MIDI και ήχου ανά έργο (ρυθμός δειγματοληψίας, SoundFont, συσκευή MIDI)
- **Καταγραφή PortAudio (προαιρετική κατασκευή)**: Όταν το έργο κατασκευάζεται με PortAudio (`HAVE_PORTAUDIO`), η εγγραφή μπορεί να χρησιμοποιήσει μια εγγενή διαδρομή εισόδου PortAudio (παρόμοια σε πνεύμα με το Audacity) αντί του Qt Multimedia. Το **Project → Project Settings → Audio** επιτρέπει την επιλογή **PortAudio** ή **Qt Multimedia** και την επιλογή συσκευής εισόδου PortAudio. Εάν το PortAudio δεν είναι εγκατεστημένο, η κατασκευή πετυχαίνει ακόμα και η εγγραφή χρησιμοποιεί μόνο Qt Multimedia.
- **Υψηλής ποιότητας μετατροπή ρυθμού δειγματοληψίας**: Εγγράφει στον εγγενή ρυθμό της συσκευής ήχου και μετατρέπει στον ρυθμό του έργου χρησιμοποιώντας παραθυρική sinc παρεμβολή (παράθυρο Kaiser, ~96 dB απόσβεσης ζώνης απόρριψης), η ίδια οικογένεια αλγορίθμων που χρησιμοποιείται από το Audacity / libsoxr. Αυτό επιτρέπει εγγραφή σε οποιονδήποτε ρυθμό δειγματοληψίας έργου (8000 Hz έως 192000 Hz) ανεξάρτητα από τον εγγενή ρυθμό της συσκευής, χωρίς αλλαγή τόνου ή διάρκειας.
- **Αυτόματη διαχείριση μονοφωνικού/στερεοφωνικού**: Ανιχνεύει φυσικά μονοφωνικές συσκευές που διαφημίζονται ως στερεοφωνικές (κοινό με μικρόφωνα USB κάμερας σε PipeWire) και μετατρέπει μεταξύ μονοφωνικού και στερεοφωνικού κατά ανάγκη (αντιγραφή ή μέσος όρος), ακολουθώντας την προσέγγιση δρομολόγησης καναλιών του Audacity
- **Εφέ εισαγωγής κατά την εγγραφή (κανάλια ήχου)**: Χρησιμοποιήστε το κουμπί **Effects** (κάτω από **Options** σε κάθε κανάλι ήχου) για να ανοίξετε το παράθυρο **Track effects**. Προσθέστε και ρυθμίστε **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion** και **Amp & cabinet** (μοντελοποίηση ενισχυτή και καμπίνας), σύρετε τη λαβή **≡** για αναδιάταξη της αλυσίδας (το επάνω τρέχει πρώτο) και αποθηκεύστε με το έργο. Τα εφέ εφαρμόζονται στη λήψη όταν σταματάτε την εγγραφή· οι παράμετροι σε πραγματικές μονάδες (ms, Hz) διατηρούν τη συμπεριφορά συνεπή για μονοφωνικό ή στερεοφωνικό και τυπικούς ρυθμούς δειγματοληψίας έργου (8 kHz–192 kHz) μετά την κανονικοποίηση της λήψης. Δείτε το [εγχειρίδιο χρήστη](docs/MusiciansCanvas_User_Manual_el.md). Οι μεταφραστές μπορούν να ενημερώνουν μαζικά τις συμβολοσειρές μέσω `scripts/effect_i18n.tsv` (παράγεται από `scripts/build_effect_tsv.py`) και `scripts/fill_effect_i18n.py` μετά το `lupdate`.
- **Εφέ μίξης (κύρια διαδρομή)**: **Project → Project Settings → Mix Effects** ρυθμίζει αλυσίδα στο **συνολικό μίγμα** (ίδιοι τύποι και σειρά με τα κανάλια). Εφαρμόζεται κατά την **αναπαραγωγή όλων των καναλιών** και την **εξαγωγή μίξης** (WAV/FLAC). Αποθηκεύεται στο `project.json` ως `mixEffectChain`.
- **Μαλακός περιορισμός πριν το PCM 16-bit**: ενσωματωμένο DSP μειώνει το σκληρό ψηφιακό κλιπάρισμα. **EffectWidget**: `guardFloatSampleForInt16Pcm()` και `softLimitFloatSampleForInt16Pcm()`.
- **Παρακολούθηση κατά την εγγραφή**: Το **Παρακολούθηση ήχου κατά την εγγραφή** (δεξιά από την ώρα) στέλνει **ζωντανή είσοδο** στην **έξοδο ήχου του έργου** στη λήψη. **Ήχος** – ό,τι καταγράφεται· **MIDI** – λογισμικό synth με απόδοση MIDI σε ήχο και SoundFont. Αποθηκεύεται στο **έργο** (`monitorWhileRecording`). Απενεργοποιήστε για λιγότερο larsen.
- **Ήχος χαμηλής καθυστέρησης**: Στα Windows, η ανίχνευση προγράμματος οδήγησης ASIO εξασφαλίζει ήχο χαμηλής καθυστέρησης· στο Linux, η προτεραιότητα χρονοπρογραμματισμού διεργασίας αυξάνεται για μικρότερη αστάθεια με PipeWire / PulseAudio / ALSA
- **Προσθήκη κομματιού τύμπανων**: **Tools → Προσθήκη κομματιού τύμπανων** (συντόμευση **D**) προσθέτει MIDI στο κανάλι 10 General MIDI (τύμπανα) και γράφει σύντομο αρχείο `.mid` δύο μέτρων στον φάκελο έργου. Αν το **Μετρονόμος** έχει ενεργοποιημένο **Ενεργοποίηση μετρονόμου κατά την εγγραφή**, χρησιμοποιείται αυτό το BPM· αλλιώς εκτιμάται από ενεργό ήχο ή 120 BPM. Δείτε το [εγχειρίδιο χρήστη](docs/MusiciansCanvas_User_Manual_el.md).
- **Virtual MIDI Keyboard**: Μια συνοδευτική εφαρμογή για αποστολή νοτών MIDI μέσω πληκτρολογίου πιάνου λογισμικού, με ενσωματωμένο συνθεσάιζερ FluidSynth, ρυθμιζόμενο κύριο κέρδος, αντιστοίχιση πληκτρολογίου υπολογιστή σε πιάνο, επιλογή οργάνου/προγράμματος, έλεγχο χορωδίας/εφέ και μετατόπιση οκτάβας
- **Διαμόρφωση**: Επιλογή συσκευής εισόδου ήχου, συσκευής MIDI και αρχείου SoundFont (καθολικές προεπιλογές και παρακάμψεις ανά έργο)
- **Σκοτεινό / φωτεινό θέμα**: Ρυθμιζόμενο μέσω Settings → Configuration
- **Τοπικοποίηση**: Και οι δύο εφαρμογές είναι τοπικοποιημένες σε 18 γλώσσες: αγγλικά, γερμανικά, ισπανικά, γαλλικά, ιαπωνικά, πορτογαλικά (Βραζιλία), παραδοσιακά κινεζικά, απλοποιημένα κινεζικά, ρωσικά, σουηδικά, φινλανδικά, δανικά, νορβηγικά, πολωνικά, ελληνικά, ιρλανδικά, ουαλικά και πειρατικά. Η γλώσσα ορίζεται κατά προεπιλογή από τη ρύθμιση του λειτουργικού συστήματος και μπορεί να αλλαχθεί στο **Settings → Configuration → Language**. Χρησιμοποιείται το τυπικό σύστημα i18n του Qt (`QTranslator` / `.ts` / `.qm`) και η διεπαφή ενημερώνεται αμέσως όταν αλλάξει η γλώσσα.
- **Εγχειρίδιο χρήστη**: Τεκμηρίωση HTML και PDF που δημιουργείται από πηγή Markdown, διαθέσιμη σε όλες τις υποστηριζόμενες γλώσσες (βλ. [docs/](docs/))

## Στιγμιότυπα οθόνης

<p align="center">
	<a href="screenshots/i18n/el/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/i18n/el/MusiciansCanvas_1_MainWin.png" alt="Κύριο παράθυρο" width="800"></a>
	<a href="screenshots/i18n/el/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/i18n/el/MusiciansCanvas_2_TrackConfig.png" alt="Διαμόρφωση καναλιού" width="800"></a>
	<a href="screenshots/i18n/el/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/i18n/el/MusiciansCanvas_3_GeneralSettings.png" alt="Γενικές ρυθμίσεις" width="800"></a>
	<a href="screenshots/i18n/el/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/i18n/el/MusiciansCanvas_4_MIDISettings.png" alt="Προεπιλεγμένες/γενικές ρυθμίσεις MIDI" width="800"></a>
	<a href="screenshots/i18n/el/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/i18n/el/MusiciansCanvas_5_AudioSettings.png" alt="Προεπιλεγμένες/γενικές ρυθμίσεις ήχου" width="800"></a>
	<a href="screenshots/i18n/el/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/i18n/el/MusiciansCanvas_DisplaySettings.png" alt="Ρυθμίσεις οθόνης" width="800"></a>
	<a href="screenshots/i18n/el/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/i18n/el/MusiciansCanvas_LanguageSettings.png" alt="Ρυθμίσεις γλώσσας" width="800"></a>
	<a href="screenshots/i18n/el/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/i18n/el/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Ρυθμίσεις MIDI ειδικές ανά έργο" width="800"></a>
	<a href="screenshots/i18n/el/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/i18n/el/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Ρυθμίσεις ήχου ειδικές ανά έργο" width="800"></a>
	<a href="screenshots/i18n/el/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/i18n/el/Track_Effects_Dialog.png" alt="Εφέ κομματιού" width="800"></a>
	<a href="screenshots/i18n/el/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/i18n/el/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/i18n/el/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/i18n/el/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/i18n/el/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/i18n/el/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/i18n/el/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/i18n/el/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Εξαρτήσεις

### Μεταγλωττιστής

Απαιτείται μεταγλωττιστής συμβατός με **C++17** (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (απαιτείται σε όλες τις πλατφόρμες)

Η λειτουργικότητα μίξης και εξαγωγής FLAC εξαρτάται από τη βιβλιοθήκη
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp).
Κλωνοποιήστε τη ως αδελφό κατάλογο αυτού του αποθετηρίου πριν από την κατασκευή:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

Η κατασκευή CMake αναμένει να τη βρει στο `../audio_mixer_cpp` σχετικά με τον ριζικό κατάλογο αυτού του έργου.

---

### Qt6 Multimedia (προαιρετικό αλλά συνιστάται ανεπιφύλακτα)

Το `qt6-multimedia` είναι προαιρετικό. Χωρίς αυτό η εφαρμογή κατασκευάζεται και εκτελείται ακόμα, αλλά
**η εγγραφή και αναπαραγωγή ήχου είναι απενεργοποιημένες** — μόνο η επεξεργασία MIDI και η διαχείριση έργων
θα λειτουργούν. Εγκαταστήστε το μαζί με τις βασικές βιβλιοθήκες Qt6 χρησιμοποιώντας τις παρακάτω εντολές πλατφόρμας.

---

### PortAudio (προαιρετικό)

Εάν η βιβλιοθήκη και τα αρχεία κεφαλίδων PortAudio είναι εγκατεστημένα, το CMake ενεργοποιεί **`HAVE_PORTAUDIO`** και
το συνδέει στο **musicians_canvas**. Η εγγραφή χρησιμοποιεί τότε κατά προεπιλογή τη διαδρομή PortAudio εκτός εάν
το έργο έχει οριστεί σε **Qt Multimedia** στο **Project → Project Settings → Audio**. Εάν το PortAudio
δεν βρεθεί, η κατασκευή συνεχίζεται χωρίς αυτό (ο wrapper μεταγλωττίζεται ως stubs).

---

### SoundFont για σύνθεση MIDI

Ένα αρχείο SoundFont (`.sf2`) απαιτείται για να παράγουν ήχο τα κανάλια MIDI. Χωρίς αυτό,
τα κανάλια MIDI θα αποδοθούν ως σιωπή. Στο **Linux**, ο ενσωματωμένος συνθεσάιζερ FluidSynth
θα ανιχνεύσει αυτόματα ένα SoundFont εάν είναι εγκατεστημένο σε τυπική διαδρομή συστήματος
(δείτε τα ονόματα πακέτων παρακάτω). Στα **macOS και Windows** δεν υπάρχει τυπική διαδρομή συστήματος,
οπότε πρέπει να διαμορφώσετε το SoundFont χειροκίνητα στο
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

> Το `fluid-soundfont-gm` εγκαθιστά το `FluidR3_GM.sf2` στο `/usr/share/sounds/sf2/` και
> ανιχνεύεται αυτόματα κατά την εκκίνηση. Το `timgm6mb-soundfont` είναι μια μικρότερη εναλλακτική.
>
> Το `libpipewire-0.3-dev` απαιτείται σε συστήματα βασισμένα στο PipeWire ώστε το Virtual MIDI
> Keyboard να μπορεί να καλέσει `pw_init()` πριν το FluidSynth δημιουργήσει τους πόρους ήχου του.
> Η κατασκευή συνεχίζεται χωρίς αυτό· η σημαία `HAVE_PIPEWIRE` απλά δεν ορίζεται.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel libsndfile-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> Το `fluid-soundfont-gm` εγκαθιστά το `FluidR3_GM.sf2` στο `/usr/share/soundfonts/` και
> ανιχνεύεται αυτόματα κατά την εκκίνηση.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac libsndfile portaudio \
  pipewire \
  soundfont-fluid
```

> Το `soundfont-fluid` εγκαθιστά το `FluidR3_GM.sf2` στο `/usr/share/soundfonts/` και
> ανιχνεύεται αυτόματα κατά την εκκίνηση. Το `pipewire` είναι συνήθως ήδη εγκατεστημένο σε σύγχρονα
> συστήματα Arch· οι κεφαλίδες ανάπτυξης περιλαμβάνονται στο κύριο πακέτο.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac libsndfile portaudio
```

> Το PipeWire είναι υποσύστημα αποκλειστικά για Linux και **δεν** απαιτείται στο macOS. Το FluidSynth
> θα χρησιμοποιήσει αυτόματα το CoreAudio μέσω του backend Qt Multimedia.
>
> Κατεβάστε ένα General MIDI SoundFont (π.χ.
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) ή
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> και διαμορφώστε τη διαδρομή του στο **Settings → Configuration → MIDI**.

---

### Windows

Το **musicians_canvas** απαιτεί **πρόγραμμα οδήγησης ήχου ASIO** για εγγραφή και αναπαραγωγή χαμηλής καθυστέρησης.
Η εφαρμογή δεν θα ξεκινήσει εάν δεν ανιχνευθεί πρόγραμμα οδήγησης ASIO.
Το **virtual_midi_keyboard** δεν απαιτεί ASIO και χρησιμοποιεί απευθείας το backend WASAPI του Qt Multimedia.

**Εγκατάσταση προγράμματος οδήγησης ASIO για musicians_canvas (επιλέξτε ένα):**

| Πρόγραμμα οδήγησης | Σημειώσεις |
|---------------------|------------|
| [ASIO4ALL](https://asio4all.org/) | Δωρεάν, λειτουργεί με τις περισσότερες ενσωματωμένες και USB συσκευές ήχου |
| Πρόγραμμα οδήγησης κατασκευαστή | Καλύτερη καθυστέρηση για αφοσιωμένες διεπαφές ήχου (π.χ. Focusrite, PreSonus, RME) |

**Αλυσίδα εργαλείων — MSYS2 (συνιστάται για κατασκευές MinGW):**

Κατεβάστε και εγκαταστήστε το MSYS2 από <https://www.msys2.org>, στη συνέχεια προσθέστε
`C:\msys64\mingw64\bin` στο `PATH` του συστήματός σας. Εγκαταστήστε τα απαιτούμενα πακέτα:

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

> Το `mingw-w64-x86_64-soundfont-fluid` εγκαθιστά το `FluidR3_GM.sf2` στο
> `C:\msys64\mingw64\share\soundfonts\` (προσαρμόστε εάν το MSYS2 είναι εγκατεστημένο αλλού).
> Επειδή τα Windows δεν έχουν τυπική διαδρομή αναζήτησης SoundFont, πρέπει να διαμορφώσετε αυτή τη διαδρομή
> χειροκίνητα στο **Settings → Configuration → MIDI** μετά την πρώτη εκκίνηση.
>
> Το PipeWire είναι υποσύστημα αποκλειστικά για Linux· δεν χρειάζεται πακέτο PipeWire στα Windows.

Σελίδες αναφοράς πακέτων:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Αλυσίδα εργαλείων — Visual Studio 2022:**

Εγκαταστήστε το Qt 6 μέσω του [Qt Online Installer](https://www.qt.io/download) και αποκτήστε
τα δυαδικά αρχεία FluidSynth, RtMidi και libFLAC (ή κατασκευάστε τα από πηγαίο κώδικα).
Η κατασκευή CMake θα τα εντοπίσει μέσω `find_library` / `find_path` εφόσον οι
κατάλληλοι κατάλογοι βρίσκονται στο `CMAKE_PREFIX_PATH`.

Ένα SoundFont πρέπει να ληφθεί ξεχωριστά (π.χ.
[GeneralUser GS](https://schristiancollins.com/generaluser.php) ή
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)) και η
διαδρομή του να διαμορφωθεί στο **Settings → Configuration → MIDI** μετά την πρώτη εκκίνηση.

> **Σημείωση:** Τα `advapi32` (μητρώο Windows) και `winmm` (Windows Multimedia)
> συνδέονται αυτόματα από το CMake· δεν απαιτείται πρόσθετη εγκατάσταση γι' αυτά.

---

## Κατασκευή

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

Εάν τα απαιτούμενα DLL δεν βρεθούν κατά την εκτέλεση, εκτελέστε το βοηθητικό σενάριο από τον κατάλογο εξόδου κατασκευής:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Δημιουργία τεκμηρίωσης

Το εγχειρίδιο χρήστη μπορεί να δημιουργηθεί ως HTML και PDF από την πηγή Markdown:

```bash
cd docs
./generate_docs.sh          # Δημιουργία HTML και PDF
./generate_docs.sh html     # Δημιουργία μόνο HTML
./generate_docs.sh pdf      # Δημιουργία μόνο PDF
```

**Προϋποθέσεις:**

- **Python (προτιμώμενο):** `pip3 install markdown weasyprint`
- **Εναλλακτικά:** `pandoc` και `wkhtmltopdf` (μέσω διαχειριστή πακέτων συστήματος)

Το δημιουργημένο HTML γράφεται στο `docs/html/` και το PDF στο `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Εκτέλεση

```bash
./build/musicians_canvas
# ή
./build/virtual_midi_keyboard   # συνοδευτική εφαρμογή Virtual MIDI Keyboard
```

---

## Χρήση

### musicians_canvas

1. **Ορισμός καταλόγου έργου**: Εισάγετε ή περιηγηθείτε σε έναν φάκελο όπου θα αποθηκευτούν οι εγγραφές και το αρχείο έργου
2. **Προσθήκη καναλιών**: Κάντε κλικ στο "+ Add Track"· ονομάστε κάθε κανάλι στο πεδίο κειμένου δίπλα στο "Options"
3. **Διαμόρφωση τύπου καναλιού**: Κάντε κλικ στο "Options" σε ένα κανάλι (ή κάντε κλικ στο εικονίδιο τύπου καναλιού μεταξύ "Options" και του πεδίου ονόματος) για να επιλέξετε Audio ή MIDI και να ορίσετε την πηγή εισόδου
4. **Αφαίρεση καναλιού**: Κάντε κλικ στο κουμπί "×" στη δεξιά πλευρά της γραμμής καναλιού
5. **Καθολικές ρυθμίσεις**: Χρησιμοποιήστε **Settings → Configuration** για να ορίσετε καθολικές προεπιλογές:
   - Καρτέλα *General*: Θέμα (σκοτεινό/φωτεινό)
   - Καρτέλα *MIDI*: Προεπιλεγμένη συσκευή εξόδου MIDI (ενσωματωμένος συνθεσάιζερ FluidSynth ή εξωτερική συσκευή MIDI) και προεπιλεγμένο SoundFont (`.sf2`) για σύνθεση
   - Καρτέλα *Audio*: Προεπιλεγμένη συσκευή εισόδου/εξόδου ήχου για εγγραφή και αναπαραγωγή
6. **Ρυθμίσεις έργου**: Χρησιμοποιήστε **Project → Project Settings** (Ctrl+P) για να παρακάμψετε τις ρυθμίσεις MIDI και ήχου μόνο για το τρέχον έργο (π.χ. διαφορετικός ρυθμός δειγματοληψίας ή SoundFont ανά τραγούδι)
7. **Εγγραφή**: Επιλέξτε "Arm" στο κανάλι-στόχο, στη συνέχεια κάντε κλικ στο κουμπί εγγραφής (κόκκινος κύκλος). Μόνο ένα κανάλι μπορεί να είναι οπλισμένο τη φορά
8. **Αναπαραγωγή**: Κάντε κλικ στο κουμπί αναπαραγωγής για μίξη και αναπαραγωγή όλων των ενεργοποιημένων καναλιών
9. **Μίξη σε αρχείο**: Χρησιμοποιήστε **Tools → Mix tracks to file** (Ctrl+M) για εξαγωγή σε WAV ή FLAC
10. **Αποθήκευση / Άνοιγμα**: Χρησιμοποιήστε **File → Save Project** (Ctrl+S) και **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Άνοιγμα διαμόρφωσης**: Χρησιμοποιήστε το κουμπί ή μενού **Configuration** (Ctrl+,) για να ανοίξετε τον διάλογο ρυθμίσεων
2. **Καρτέλα MIDI**:
   - Επιλέξτε μια συσκευή εξόδου MIDI (εξωτερικός συνθεσάιζερ υλικού/λογισμικού) ή αφήστε κενό για χρήση του ενσωματωμένου συνθεσάιζερ FluidSynth
   - Επιλέξτε μια συσκευή εισόδου MIDI για προώθηση εισερχόμενων νοτών MIDI στην οθόνη πληκτρολογίου
   - Ρυθμίστε **Synthesizer Volume (Master Gain)** για έλεγχο του επιπέδου εξόδου του ενσωματωμένου συνθεσάιζερ (10%–200%)
3. **Καρτέλα Audio**: Επιλέξτε τη συσκευή εξόδου ήχου που χρησιμοποιείται από τον ενσωματωμένο συνθεσάιζερ
4. **SoundFont**: Επιλέξτε ένα αρχείο SoundFont `.sf2` για τον ενσωματωμένο συνθεσάιζερ (ανιχνεύεται αυτόματα στο Linux εάν είναι εγκατεστημένο ένα SoundFont συστήματος)
5. **Παίξτε νότες**: Κάντε κλικ στα πλήκτρα του πληκτρολογίου πιάνου στην οθόνη, χρησιμοποιήστε έναν συνδεδεμένο ελεγκτή MIDI ή χρησιμοποιήστε το πληκτρολόγιο του υπολογιστή:
   - Κάτω οκτάβα: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = διέσεις/υφέσεις
   - Πάνω οκτάβα: Q/W/E/R/T/Y/U/I/O/P = C έως E, 2/3/5/6/7/9/0 = διέσεις/υφέσεις
6. **Χειριστήρια γραμμής εργαλείων**: Ρύθμιση έντασης MIDI (0–127), οκτάβας (-3 έως +5), επιπέδου χορωδίας/εφέ και επιλογή οργάνων

---

## Συντομεύσεις πληκτρολογίου

**musicians_canvas:**

| Συντόμευση | Ενέργεια |
|------------|----------|
| Ctrl+S | Αποθήκευση έργου |
| Ctrl+O | Άνοιγμα έργου |
| Ctrl+M | Μίξη καναλιών σε αρχείο |
| Ctrl+P | Ρυθμίσεις έργου |
| Ctrl+, | Ρυθμίσεις / Διαμόρφωση |
| Ctrl+Q / Alt+F4 | Έξοδος |

**virtual_midi_keyboard:**

| Συντόμευση | Ενέργεια |
|------------|----------|
| Ctrl+, | Διάλογος διαμόρφωσης |
| Ctrl+U | Βοήθεια / Πληροφορίες χρήσης |
| Ctrl+Q | Κλείσιμο |

---

## Άδεια

Αυτό το έργο παρέχεται ως έχει για εκπαιδευτική και προσωπική χρήση.
