# Musician's Canvas

Jest to wielościeżkowa aplikacja do nagrywania muzyki na komputery stacjonarne. Została napisana w C++ i używa Qt jako
biblioteki interfejsu graficznego. Aplikacja jest przeznaczona do budowania na systemy Linux, Windows i Mac OS. Łatwość użycia jest
głównym aspektem projektowania. Ten projekt rozpoczął się jako eksperyment z Cursor (narzędzie do kodowania z AI), aby zobaczyć, co
byłoby w stanie stworzyć jako aplikację tego typu, z kontynuacją wykorzystania rozwoju z AI oraz
tradycyjnego tworzenia oprogramowania.

Lata temu stworzyłem wielościeżkowy program do nagrywania muzyki na uczelni (o tej samej nazwie); jest to próba
stworzenia czegoś lepszego.

## Funkcje

- **Wielościeżkowa aranżacja**: Dodawanie i usuwanie wielu ścieżek do komponowania piosenek
- **Nazwane ścieżki**: Każda ścieżka ma edytowalną nazwę używaną jako nazwa pliku dla nagranego dźwięku
- **Typy ścieżek**: Konfiguracja każdej ścieżki do nagrywania audio (z mikrofonu/wejścia liniowego) lub nagrywania MIDI (z klawiatury/kontrolera); kliknij ikonę typu ścieżki, aby szybko zmienić źródło wejściowe
- **Uzbrajanie ścieżki**: Zaznacz "Arm" na ścieżce, aby wybrać ją jako cel nagrywania; tylko jedna ścieżka może być uzbrojona jednocześnie. Pojedyncza nienagrana ścieżka jest automatycznie uzbrajana dla wygody
- **Odliczanie nagrywania**: 3-sekundowe wizualne odliczanie przed rozpoczęciem nagrywania, dające wykonawcy czas na przygotowanie
- **Nagrywanie overdub**: Podczas nagrywania nowej ścieżki, gdy istniejące ścieżki są włączone, istniejące ścieżki są miksowane i odtwarzane w czasie rzeczywistym, abyś mógł je słyszeć podczas nagrywania. Odtwarzanie i przechwytywanie są synchronizowane, aby utrzymać wyrównanie wszystkich ścieżek
- **Wizualne sprzężenie zwrotne**: Wyświetlanie fali dźwiękowej dla ścieżek audio (z miernikiem poziomu na żywo podczas nagrywania), widok MIDI piano roll dla ścieżek MIDI
- **Wbudowany syntezator MIDI**: Renderuje ścieżki MIDI do dźwięku przy użyciu FluidSynth z konfigurowalnym SoundFont
- **Miksowanie do pojedynczego pliku audio**: Eksport wszystkich włączonych ścieżek do pojedynczego zmiksowanego pliku WAV lub FLAC przy użyciu biblioteki [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)
- **Zapisz / Otwórz projekt**: Serializacja i przywracanie pełnego projektu (ścieżki, nazwy, typy, nuty MIDI, odniesienia do plików audio) do/z pliku JSON, z wykrywaniem niezapisanych zmian przy wyjściu
- **Ustawienia specyficzne dla projektu**: Nadpisywanie globalnych domyślnych ustawień MIDI i audio per projekt (częstotliwość próbkowania, SoundFont, urządzenie MIDI)
- **Przechwytywanie PortAudio (opcjonalna kompilacja)**: Gdy projekt jest zbudowany z PortAudio (`HAVE_PORTAUDIO`), nagrywanie może używać natywnej ścieżki wejściowej PortAudio (podobnej w duchu do Audacity) zamiast Qt Multimedia. **Project → Project Settings → Audio** pozwala wybrać **PortAudio** lub **Qt Multimedia** i wybrać urządzenie wejściowe PortAudio. Jeśli PortAudio nie jest zainstalowane, kompilacja nadal się powiedzie, a nagrywanie używa tylko Qt Multimedia.
- **Wysokiej jakości konwersja częstotliwości próbkowania**: Nagrywa z natywną częstotliwością urządzenia audio i konwertuje do częstotliwości projektu za pomocą okienkowanej interpolacji sinc (okno Kaisera, ~96 dB tłumienia pasma zaporowego), ta sama rodzina algorytmów, która jest używana przez Audacity / libsoxr. Pozwala to na nagrywanie z dowolną częstotliwością próbkowania projektu (8000 Hz do 192000 Hz) niezależnie od natywnej częstotliwości urządzenia, bez zmiany wysokości tonu lub czasu trwania.
- **Automatyczna obsługa mono/stereo**: Wykrywa fizycznie mono urządzenia reklamowane jako stereo (częste w przypadku mikrofonów kamer internetowych USB w PipeWire) i konwertuje między mono a stereo w razie potrzeby (duplikacja lub uśrednianie), zgodnie z podejściem routingu kanałów Audacity
- **Efekty wstawiane przy nagrywaniu (ścieżki audio)**: Przycisk **Effects** (pod **Options** na każdej ścieżce audio) otwiera okno **Track effects**. Dodawaj i konfiguruj **Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion** i **Amp & cabinet** (modelowanie wzmacniacza i kolumny), przeciągaj **≡**, by zmieniać kolejność łańcucha (góra działa pierwsza), i zapisuj z projektem. Efekty są stosowane do nagrania po zatrzymaniu nagrywania; parametry w jednostkach rzeczywistych (ms, Hz) zapewniają spójne działanie dla mono lub stereo i typowych częstotliwości próbkowania projektu (8 kHz–192 kHz) po normalizacji przechwycenia. Zobacz [podręcznik użytkownika](docs/MusiciansCanvas_User_Manual_pl.md). Tłumacze mogą masowo aktualizować ciągi przez `scripts/effect_i18n.tsv` (generuje `scripts/build_effect_tsv.py`) i `scripts/fill_effect_i18n.py` po `lupdate`.
- **Efekty miksu (szyna główna)**: **Project → Project Settings → Mix Effects** ustawia łańcuch na **całym mikście** (te same typy i kolejność co na ścieżce). Stosowane przy **odtwarzaniu wszystkich ścieżek** i **eksporcie miksu** (WAV/FLAC). Zapis w `project.json` jako `mixEffectChain`.
- **Miękkie ograniczenie przed PCM 16-bit**: wbudowany DSP ogranicza twarde przycinanie cyfrowe. **EffectWidget**: `guardFloatSampleForInt16Pcm()` i `softLimitFloatSampleForInt16Pcm()`.
- **Monitorowanie nagrywania**: Pole **Monitoruj dźwięk podczas nagrywania** (po prawej od zegara) kieruje **na żywo wejście** na **wyjście audio projektu** podczas przechwytywania. **Audio** — ten sam sygnał co nagranie; **MIDI** — przy renderze do audio i SoundFont przez syntezator programowy. Ustawienie w **projekcie** (`monitorWhileRecording`). Wyłącz, by ograniczyć sprzężenie.
- **Dźwięk o niskim opóźnieniu**: W systemie Windows wykrywanie sterownika ASIO zapewnia dźwięk o niskim opóźnieniu; w systemie Linux priorytet planowania procesów jest podnoszony dla mniejszego jittera z PipeWire / PulseAudio / ALSA
- **Dodaj ścieżkę perkusji**: **Tools → Dodaj ścieżkę perkusji** (skrót **D**) dodaje ścieżkę MIDI na kanale 10 General MIDI (perkusja) i zapisuje krótki plik `.mid` na dwa takty w folderze projektu. Gdy w **Metronom** włączone jest **Włącz metronom podczas nagrywania**, używane jest to BPM; w przeciwnym razie tempo jest szacowane z włączonego audio lub 120 BPM. Zobacz [podręcznik użytkownika](docs/MusiciansCanvas_User_Manual_pl.md).
- **Virtual MIDI Keyboard**: Towarzysząca aplikacja do wysyłania nut MIDI przez programową klawiaturę pianina, z wbudowanym syntezatorem FluidSynth, regulowanym wzmocnieniem głównym, mapowaniem klawiatury komputerowej na pianino, wyborem instrumentu/programu, sterowaniem chorus/efektem i przesunięciem oktawy
- **Konfiguracja**: Wybór urządzenia wejściowego audio, urządzenia MIDI i pliku SoundFont (globalne wartości domyślne i nadpisania per projekt)
- **Ciemny / jasny motyw**: Konfigurowalny przez Settings → Configuration
- **Lokalizacja**: Obie aplikacje są zlokalizowane w 18 językach: angielski, niemiecki, hiszpański, francuski, japoński, portugalski (Brazylia), chiński tradycyjny, chiński uproszczony, rosyjski, szwedzki, fiński, duński, norweski, polski, grecki, irlandzki, walijski i piracki. Język jest domyślnie ustawiany według systemu operacyjnego i może być zmieniony w **Settings → Configuration → Language**. Używany jest standardowy system i18n Qt (`QTranslator` / `.ts` / `.qm`), a interfejs aktualizuje się natychmiast po zmianie języka.
- **Podręcznik użytkownika**: Dokumentacja HTML i PDF wygenerowana ze źródeł Markdown, dostępna we wszystkich obsługiwanych językach (zobacz [docs/](docs/))

## Zrzuty ekranu

<p align="center">
	<a href="screenshots/i18n/pl/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/i18n/pl/MusiciansCanvas_1_MainWin.png" alt="Okno główne" width="800"></a>
	<a href="screenshots/i18n/pl/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/i18n/pl/MusiciansCanvas_2_TrackConfig.png" alt="Konfiguracja ścieżki" width="800"></a>
	<a href="screenshots/i18n/pl/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/i18n/pl/MusiciansCanvas_3_GeneralSettings.png" alt="Ustawienia ogólne" width="800"></a>
	<a href="screenshots/i18n/pl/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/i18n/pl/MusiciansCanvas_4_MIDISettings.png" alt="Domyślne/ogólne ustawienia MIDI" width="800"></a>
	<a href="screenshots/i18n/pl/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/i18n/pl/MusiciansCanvas_5_AudioSettings.png" alt="Domyślne/ogólne ustawienia audio" width="800"></a>
	<a href="screenshots/i18n/pl/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/i18n/pl/MusiciansCanvas_DisplaySettings.png" alt="Ustawienia wyświetlania" width="800"></a>
	<a href="screenshots/i18n/pl/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/i18n/pl/MusiciansCanvas_LanguageSettings.png" alt="Ustawienia języka" width="800"></a>
	<a href="screenshots/i18n/pl/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/i18n/pl/MusiciansCanvas_6_ProjectMIDISettings.png" alt="Ustawienia MIDI specyficzne dla projektu" width="800"></a>
	<a href="screenshots/i18n/pl/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/i18n/pl/MusiciansCanvas_7_ProjectAudioSettings.png" alt="Ustawienia audio specyficzne dla projektu" width="800"></a>
	<a href="screenshots/i18n/pl/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/i18n/pl/Track_Effects_Dialog.png" alt="Efekty ścieżki" width="800"></a>
	<a href="screenshots/i18n/pl/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/i18n/pl/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/i18n/pl/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/i18n/pl/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/i18n/pl/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/i18n/pl/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/i18n/pl/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/i18n/pl/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## Zależności

### Kompilator

Wymagany jest kompilator obsługujący **C++17** (GCC 8+, Clang 7+, MSVC 2017+).

---

### audio_mixer_cpp (wymagane na wszystkich platformach)

Funkcjonalność miksowania i eksportu FLAC zależy od biblioteki
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp).
Sklonuj ją jako katalog równorzędny tego repozytorium przed kompilacją:

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

Kompilacja CMake oczekuje, że znajdzie ją w `../audio_mixer_cpp` względem katalogu głównego tego projektu.

---

### Qt6 Multimedia (opcjonalne, ale zdecydowanie zalecane)

`qt6-multimedia` jest opcjonalne. Bez niego aplikacja nadal się kompiluje i uruchamia, ale
**nagrywanie i odtwarzanie dźwięku są wyłączone** — działać będzie tylko edycja MIDI i zarządzanie projektami. Zainstaluj go razem z głównymi bibliotekami Qt6, korzystając z poniższych poleceń dla danej platformy.

---

### PortAudio (opcjonalne)

Jeśli biblioteka i pliki nagłówkowe PortAudio są zainstalowane, CMake włącza **`HAVE_PORTAUDIO`** i
linkuje go do **musicians_canvas**. Nagrywanie domyślnie używa ścieżki PortAudio, chyba że
projekt jest ustawiony na **Qt Multimedia** w **Project → Project Settings → Audio**. Jeśli PortAudio
nie zostanie znalezione, kompilacja kontynuuje bez niego (wrapper kompiluje się jako stuby).

---

### SoundFont do syntezy MIDI

Plik SoundFont (`.sf2`) jest wymagany, aby ścieżki MIDI produkowały dźwięk. Bez niego
ścieżki MIDI będą renderowane jako cisza. W systemie **Linux** wbudowany syntezator FluidSynth
automatycznie wykryje SoundFont, jeśli jest zainstalowany w standardowej ścieżce systemowej
(zobacz nazwy pakietów poniżej). W systemach **macOS i Windows** nie ma standardowej ścieżki systemowej,
więc musisz skonfigurować SoundFont ręcznie w
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

> `fluid-soundfont-gm` instaluje `FluidR3_GM.sf2` do `/usr/share/sounds/sf2/` i jest
> automatycznie wykrywany przy uruchomieniu. `timgm6mb-soundfont` jest mniejszą alternatywą.
>
> `libpipewire-0.3-dev` jest wymagany w systemach opartych na PipeWire, aby Virtual MIDI
> Keyboard mógł wywołać `pw_init()` przed utworzeniem zasobów audio przez FluidSynth.
> Kompilacja kontynuuje bez niego; flaga `HAVE_PIPEWIRE` po prostu nie jest definiowana.

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel libsndfile-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` instaluje `FluidR3_GM.sf2` do `/usr/share/soundfonts/` i jest
> automatycznie wykrywany przy uruchomieniu.

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac libsndfile portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` instaluje `FluidR3_GM.sf2` do `/usr/share/soundfonts/` i jest
> automatycznie wykrywany przy uruchomieniu. `pipewire` jest zwykle już zainstalowany w nowoczesnych
> systemach Arch; nagłówki deweloperskie są zawarte w głównym pakiecie.

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac libsndfile portaudio
```

> PipeWire jest podsystemem wyłącznie dla Linuxa i **nie** jest wymagany na macOS. FluidSynth
> automatycznie użyje CoreAudio przez backend Qt Multimedia.
>
> Pobierz SoundFont General MIDI (np.
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) lub
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html))
> i skonfiguruj jego ścieżkę w **Settings → Configuration → MIDI**.

---

### Windows

**musicians_canvas** wymaga **sterownika audio ASIO** do nagrywania i odtwarzania o niskim opóźnieniu.
Aplikacja nie uruchomi się, jeśli nie zostanie wykryty sterownik ASIO.
**virtual_midi_keyboard** nie wymaga ASIO i używa bezpośrednio backendu WASAPI Qt Multimedia.

**Instalacja sterownika ASIO dla musicians_canvas (wybierz jeden):**

| Sterownik | Uwagi |
|-----------|-------|
| [ASIO4ALL](https://asio4all.org/) | Darmowy, działa z większością wbudowanych i USB urządzeń audio |
| Sterownik producenta | Najlepsza latencja dla dedykowanych interfejsów audio (np. Focusrite, PreSonus, RME) |

**Łańcuch narzędzi — MSYS2 (zalecany dla kompilacji MinGW):**

Pobierz i zainstaluj MSYS2 z <https://www.msys2.org>, następnie dodaj
`C:\msys64\mingw64\bin` do systemowej zmiennej `PATH`. Zainstaluj wymagane pakiety:

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

> `mingw-w64-x86_64-soundfont-fluid` instaluje `FluidR3_GM.sf2` do
> `C:\msys64\mingw64\share\soundfonts\` (dostosuj, jeśli MSYS2 jest zainstalowany gdzie indziej).
> Ponieważ Windows nie ma standardowej ścieżki wyszukiwania SoundFont, musisz skonfigurować tę ścieżkę
> ręcznie w **Settings → Configuration → MIDI** po pierwszym uruchomieniu.
>
> PipeWire jest podsystemem wyłącznie dla Linuxa; pakiet PipeWire nie jest potrzebny w systemie Windows.

Strony referencyjne pakietów:
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**Łańcuch narzędzi — Visual Studio 2022:**

Zainstaluj Qt 6 za pomocą [Qt Online Installer](https://www.qt.io/download) i uzyskaj
pliki binarne FluidSynth, RtMidi i libFLAC (lub skompiluj je ze źródeł).
Kompilacja CMake zlokalizuje je za pomocą `find_library` / `find_path`, pod warunkiem że
odpowiednie katalogi znajdują się w `CMAKE_PREFIX_PATH`.

SoundFont musi być pobrany osobno (np.
[GeneralUser GS](https://schristiancollins.com/generaluser.php) lub
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)), a jego
ścieżka skonfigurowana w **Settings → Configuration → MIDI** po pierwszym uruchomieniu.

> **Uwaga:** `advapi32` (rejestr Windows) i `winmm` (Windows Multimedia) są
> automatycznie linkowane przez CMake; nie jest wymagana dodatkowa instalacja dla tych bibliotek.

---

## Kompilacja

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

Jeśli wymagane pliki DLL nie zostaną znalezione w czasie wykonania, uruchom skrypt pomocniczy z katalogu wyjściowego kompilacji:

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## Generowanie dokumentacji

Podręcznik użytkownika może być wygenerowany jako HTML i PDF ze źródła Markdown:

```bash
cd docs
./generate_docs.sh          # Generuj zarówno HTML, jak i PDF
./generate_docs.sh html     # Generuj tylko HTML
./generate_docs.sh pdf      # Generuj tylko PDF
```

**Wymagania wstępne:**

- **Python (preferowany):** `pip3 install markdown weasyprint`
- **Alternatywa:** `pandoc` i `wkhtmltopdf` (przez menedżer pakietów systemu)

Wygenerowany HTML jest zapisywany do `docs/html/`, a PDF do `docs/MusiciansCanvas_User_Manual.pdf`.

---

## Uruchamianie

```bash
./build/musicians_canvas
# lub
./build/virtual_midi_keyboard   # towarzysząca aplikacja Virtual MIDI Keyboard
```

---

## Użytkowanie

### musicians_canvas

1. **Ustaw katalog projektu**: Wprowadź lub przejdź do folderu, w którym będą przechowywane nagrania i plik projektu
2. **Dodaj ścieżki**: Kliknij "+ Add Track"; nadaj nazwę każdej ścieżce w polu tekstowym obok "Options"
3. **Skonfiguruj typ ścieżki**: Kliknij "Options" na ścieżce (lub kliknij ikonę typu ścieżki między "Options" a polem nazwy), aby wybrać Audio lub MIDI i ustawić źródło wejściowe
4. **Usuń ścieżkę**: Kliknij przycisk "×" po prawej stronie wiersza ścieżki
5. **Ustawienia globalne**: Użyj **Settings → Configuration**, aby ustawić globalne wartości domyślne:
   - Zakładka *General*: Motyw (ciemny/jasny)
   - Zakładka *MIDI*: Domyślne urządzenie wyjściowe MIDI (wbudowany syntezator FluidSynth lub zewnętrzne urządzenie MIDI) i domyślny SoundFont (`.sf2`) do syntezy
   - Zakładka *Audio*: Domyślne urządzenie wejścia/wyjścia audio do nagrywania i odtwarzania
6. **Ustawienia projektu**: Użyj **Project → Project Settings** (Ctrl+P), aby nadpisać ustawienia MIDI i audio tylko dla bieżącego projektu (np. inna częstotliwość próbkowania lub SoundFont per utwór)
7. **Nagrywaj**: Zaznacz "Arm" na docelowej ścieżce, następnie kliknij przycisk nagrywania (czerwone kółko). Tylko jedna ścieżka może być uzbrojona jednocześnie
8. **Odtwarzaj**: Kliknij przycisk odtwarzania, aby zmiksować i odtworzyć wszystkie włączone ścieżki
9. **Miksuj do pliku**: Użyj **Tools → Mix tracks to file** (Ctrl+M), aby wyeksportować do WAV lub FLAC
10. **Zapisz / Otwórz**: Użyj **File → Save Project** (Ctrl+S) i **File → Open Project** (Ctrl+O)

### virtual_midi_keyboard

1. **Otwórz konfigurację**: Użyj przycisku lub menu **Configuration** (Ctrl+,), aby otworzyć okno ustawień
2. **Zakładka MIDI**:
   - Wybierz urządzenie wyjściowe MIDI (zewnętrzny syntezator sprzętowy/programowy) lub pozostaw puste, aby użyć wbudowanego syntezatora FluidSynth
   - Wybierz urządzenie wejściowe MIDI, aby przekazywać przychodzące nuty MIDI do wyświetlacza klawiatury
   - Dostosuj **Synthesizer Volume (Master Gain)**, aby kontrolować poziom wyjściowy wbudowanego syntezatora (10%–200%)
3. **Zakładka Audio**: Wybierz urządzenie wyjściowe audio używane przez wbudowany syntezator
4. **SoundFont**: Wybierz plik SoundFont `.sf2` dla wbudowanego syntezatora (automatycznie wykrywany w systemie Linux, jeśli zainstalowany jest systemowy SoundFont)
5. **Graj nuty**: Klikaj klawisze na ekranowej klawiaturze pianina, użyj podłączonego kontrolera MIDI lub użyj klawiatury komputera:
   - Dolna oktawa: Z/X/C/V/B/N/M = C/D/E/F/G/A/B, S/D/G/H/J = krzyżyki/bemole
   - Górna oktawa: Q/W/E/R/T/Y/U/I/O/P = C do E, 2/3/5/6/7/9/0 = krzyżyki/bemole
6. **Kontrolki paska narzędzi**: Regulacja głośności MIDI (0–127), oktawy (-3 do +5), poziomu chorus/efektu oraz wybór instrumentów

---

## Skróty klawiszowe

**musicians_canvas:**

| Skrót | Akcja |
|-------|-------|
| Ctrl+S | Zapisz projekt |
| Ctrl+O | Otwórz projekt |
| Ctrl+M | Miksuj ścieżki do pliku |
| Ctrl+P | Ustawienia projektu |
| Ctrl+, | Ustawienia / Konfiguracja |
| Ctrl+Q / Alt+F4 | Zakończ |

**virtual_midi_keyboard:**

| Skrót | Akcja |
|-------|-------|
| Ctrl+, | Okno konfiguracji |
| Ctrl+U | Pomoc / Informacje o użytkowaniu |
| Ctrl+Q | Zamknij |

---

## Licencja

Ten projekt jest udostępniany w stanie, w jakim jest, do celów edukacyjnych i osobistego użytku.
