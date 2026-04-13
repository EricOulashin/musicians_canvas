---
title: "Musician's Canvas Podręcznik Użytkownika"
subtitle: "Wielościeżkowa aplikacja do nagrywania muzyki"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "pl"
---

# Musician's Canvas Podręcznik Użytkownika

## Wprowadzenie

Musician's Canvas to wielościeżkowa aplikacja do nagrywania muzyki na komputery stacjonarne. Obsługuje nagrywanie dźwięku z mikrofonów i urządzeń line-in, nagrywanie MIDI z klawiatur i kontrolerów oraz miksowanie wszystkich ścieżek do jednego pliku WAV lub FLAC. Aplikacja towarzysząca, Virtual MIDI Keyboard, udostępnia programową klawiaturę fortepianową do wysyłania nut MIDI.

Musician's Canvas został zaprojektowany z myślą o łatwości użytkowania, oferując jednocześnie funkcje powszechnie spotykane w cyfrowych stacjach roboczych audio (DAW):

- Wielościeżkowe nagrywanie audio i MIDI
- Nagrywanie overdub z zsynchronizowanym odtwarzaniem istniejących ścieżek
- Wbudowany syntezator MIDI FluidSynth z obsługą SoundFont
- Wysokiej jakości konwersja częstotliwości próbkowania przy nagrywaniu z dowolną częstotliwością próbkowania projektu
- Automatyczne wykrywanie urządzeń mono/stereo
- Ustawienia oparte na projektach z nadpisywaniem ustawień per projekt
- Miksowanie do WAV lub FLAC
- Ciemne i jasne motywy
- Lokalizacja w wielu językach (angielski, niemiecki, hiszpański, francuski, japoński, portugalski, chiński i piracki)
- Aplikacja towarzysząca Virtual MIDI Keyboard

## Pierwsze kroki

### Uruchamianie aplikacji

Uruchom plik wykonywalny `musicians_canvas` z katalogu kompilacji lub lokalizacji instalacji:

```
./musicians_canvas
```

Przy pierwszym uruchomieniu aplikacja otwiera się z pustym projektem. Przed nagrywaniem należy ustawić katalog projektu.

Przy uruchomieniu aplikacja stosuje zapisany motyw (ciemny lub jasny), a jeśli katalog projektu był wcześniej używany i zawiera plik `project.json`, projekt jest automatycznie ładowany.

### Konfiguracja projektu

1. **Ustaw katalog projektu**: Wpisz lub przejdź do folderu w polu "Project Location" u góry okna. Tutaj będą przechowywane nagrania i plik projektu.

2. **Dodaj ścieżkę**: Kliknij przycisk **+ Add Track**. Pojawi się nowa ścieżka z domyślnymi ustawieniami. Jeśli jest jedyną ścieżką w projekcie i nie została jeszcze nagrana, zostaje automatycznie uzbrojona do nagrywania.

3. **Nazwij ścieżkę**: Wpisz nazwę w polu tekstowym obok przycisku "Options". Ta nazwa jest używana jako nazwa pliku nagrywanego pliku audio.

![Okno główne](../screenshots/MusiciansCanvas_1_MainWin.png)

### Pasek przycisków

Tuż pod paskiem menu znajduje się pasek narzędzi z przyciskami szybkiego dostępu:

![Pasek przycisków](../screenshots/ButtonBar.png)

- **Open Project**: To samo co **File > Open Project** — otwiera wcześniej zapisany projekt.
- **Save Project**: To samo co **File > Save Project** — zapisuje bieżący projekt. Ten przycisk
  jest włączony tylko wtedy, gdy ustawiony jest katalog projektu.
- **Project Settings**: To samo co **Project > Project Settings** — otwiera okno dialogowe
  ustawień projektu. Ten przycisk jest włączony tylko wtedy, gdy ustawiony jest katalog projektu.
- **Configuration**: To samo co **Settings > Configuration** — otwiera okno dialogowe globalnych
  ustawień aplikacji.
- **Metronome Settings**: Otwiera okno dialogowe ustawień metronomu (zobacz sekcję Metronom poniżej).

### Zapisywanie i otwieranie projektów

- **Zapisz**: Użyj **File > Save Project** (Ctrl+S), aby zapisać bieżący projekt jako plik JSON w katalogu projektu.
- **Otwórz**: Użyj **File > Open Project** (Ctrl+O), aby załadować wcześniej zapisany projekt.

Plik projektu (`project.json`) przechowuje nazwy ścieżek, typy, nuty MIDI, odniesienia do plików audio oraz wszystkie ustawienia specyficzne dla projektu. Pliki audio są przechowywane w tym samym katalogu co `project.json` i noszą nazwy odpowiadające ścieżkom (np. `My_Track.flac`).

Jeśli zamkniesz aplikację z niezapisanymi zmianami, dialog potwierdzenia zapyta, czy chcesz zapisać przed wyjściem.

## Zarządzanie ścieżkami

### Dodawanie i usuwanie ścieżek

- Kliknij **+ Add Track**, aby dodać nową ścieżkę do aranżacji.
- Kliknij przycisk **x** po prawej stronie wiersza ścieżki, aby ją usunąć.
- Kliknij **Clear Tracks** (czerwony przycisk na pasku narzędzi), aby usunąć wszystkie ścieżki. Przed wykonaniem wyświetlany jest dialog potwierdzenia.

### Dodawanie ścieżek metodą przeciągnij i upuść

Gdy projekt jest otwarty, możesz przeciągnąć jeden lub więcej obsługiwanych
plików audio z menedżera plików (Eksplorator Windows, Finder macOS, menedżer
plików Linux itp.) bezpośrednio na okno Musician's Canvas, aby dodać je jako
nowe ścieżki audio.

- **Obsługiwane formaty:** `.wav` i `.flac`. Pliki w dowolnym innym formacie
  są po cichu pomijane, a na końcu okno dialogowe wyświetla listę pominiętych
  plików.
- **Kopiowanie plików:** Jeśli upuszczony plik nie znajduje się jeszcze w
  katalogu projektu, jest do niego automatycznie kopiowany. Jeśli w katalogu
  projektu istnieje już plik o tej samej nazwie, zostaniesz zapytany, czy go
  zastąpić.
- **Nazwa ścieżki:** Podstawowa nazwa pliku (bez rozszerzenia) jest używana
  jako nazwa nowej ścieżki. Na przykład upuszczenie `Bass Line.wav` tworzy
  ścieżkę audio o nazwie "Bass Line".
- **Wiele plików naraz:** Można jednocześnie przeciągać kilka plików; każdy
  obsługiwany plik staje się własną ścieżką w ramach jednego upuszczenia.
- **Gdy upuszczenie zostaje odrzucone:** Upuszczenia są akceptowane tylko
  wtedy, gdy projekt jest otwarty, a Musician's Canvas **nie** odtwarza ani
  nie nagrywa w danej chwili. Najpierw zatrzymaj odtwarzanie lub nagrywanie,
  jeśli chcesz przeciągnąć dodatkowe ścieżki.

### Konfiguracja typu ścieżki

Każda ścieżka może być skonfigurowana jako **Audio** (do nagrywania z mikrofonu/line-in) lub **MIDI** (do nagrywania z klawiatury/kontrolera).

Aby zmienić typ ścieżki:

- Kliknij przycisk **Options** na ścieżce, lub
- Kliknij **ikonę typu ścieżki** (między "Options" a polem nazwy)

Otworzy się dialog Konfiguracja ścieżki, w którym można wybrać źródło wejściowe.

![Konfiguracja ścieżki](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### Kontrolki ścieżki

Każdy wiersz ścieżki udostępnia następujące kontrolki:

- **Pole wyboru Włącz**: Przełącza, czy ścieżka jest uwzględniona w odtwarzaniu i miksowaniu. Wyłączenie ścieżki automatycznie ją rozbraja, jeśli była uzbrojona.
- **Przycisk radiowy Uzbrojenie**: Wybiera tę ścieżkę jako cel nagrywania. Tylko jedna ścieżka może być uzbrojona w danym momencie; uzbrojenie nowej ścieżki automatycznie rozbraja wcześniej uzbrojoną ścieżkę.
- **Pole nazwy**: Edytowalne pole tekstowe na nazwę ścieżki. Ta nazwa jest używana jako nazwa pliku audio (z nieprawidłowymi znakami systemu plików zastąpionymi podkreśleniami).
- **Przycisk Options**: Otwiera dialog Konfiguracja ścieżki.
- **Ikona typu**: Wyświetla ikonę głośnika dla ścieżek audio lub ikonę fortepianu dla ścieżek MIDI. Kliknięcie otwiera dialog Konfiguracja ścieżki.
- **Przycisk Usuń (x)**: Usuwa ścieżkę z projektu.

### Automatyczne uzbrajanie

Gdy projekt ma dokładnie jedną ścieżkę i ta ścieżka nie została jeszcze nagrana, jest automatycznie uzbrajana do nagrywania. Dotyczy to zarówno dodawania pierwszej ścieżki do nowego projektu, jak i otwierania istniejącego projektu z jedną pustą ścieżką.

### Wizualizacja ścieżki

- **Ścieżki audio** wyświetlają wizualizację przebiegu fali nagranego dźwięku. Gdy nie nagrano żadnego dźwięku, w obszarze wyświetlany jest napis "No audio recorded".
- **Ścieżki MIDI** wyświetlają wizualizację piano roll pokazującą nagrane nuty na siatce obejmującej zakres od A0 do C8. Nuty są kolorowane według velocity. Gdy nie nagrano żadnych danych MIDI, w obszarze wyświetlany jest napis "No MIDI data recorded".

## Nagrywanie

### Nagrywanie audio

1. Upewnij się, że katalog projektu jest ustawiony.
2. Uzbrój docelową ścieżkę (zaznacz przycisk radiowy "Arm").
3. Kliknij przycisk **Record** (czerwone kółko).
4. Na ścieżce pojawi się 3-sekundowe odliczanie ("Get ready... 3", "2", "1"), a następnie rozpocznie się nagrywanie.
5. Podczas nagrywania w obszarze przebiegu fali ścieżki wyświetlany jest miernik poziomu na żywo, pokazujący bieżącą amplitudę jako pasek gradientowy (zielony do żółtego do czerwonego) z etykietą "Recording".
6. Kliknij przycisk **Stop**, aby zakończyć nagrywanie.

Nagrany dźwięk jest zapisywany jako plik FLAC w katalogu projektu, nazwany po ścieżce.

Podczas nagrywania i odtwarzania wszystkie interaktywne kontrolki (przyciski ścieżek, ustawienia itp.) są wyłączone, aby zapobiec przypadkowym zmianom.

### Efekty insert (tylko ścieżki audio)

Ścieżki audio mają przycisk **Efekty** tuż pod **Options**. Otwiera on okno **Efekty ścieżki**, gdzie
budujesz **uporządkowany łańcuch** efektów insert dla nagrań na tej ścieżce:

![Okno efektów ścieżki](../screenshots/Track_Effects_Dialog.png)

Dostępne są m.in. **Pogłos**, **Chorus**, **Flanger**, **Overdrive / distortion**, **Amp & cabinet** (modelowanie wzmacniacza i kolumny: typ wzm., kolumna, przester, bas/średnie/wysokie, „powietrze” i mieszanie) oraz **Vibrato (Tremolo)**.

**Vibrato (Tremolo)** to klasyczne fenderowskie „Vibrato” (w praktyce tremolo): **okresowa modulacja głośności**. **Speed** ustawia szybkość pulsowania, a **Intensity** (Depth) jego głębokość (od subtelnego „shimmer” do pełnego „chop”).

Zrzuty ekranu efektu **Amp & cabinet**:

![Modelowanie Amp & cabinet (1)](../screenshots/Amp_And_Cabinet_Model_1.png)

![Modelowanie Amp & cabinet (2)](../screenshots/Amp_And_Cabinet_Model_2.png)

- Kliknij **Dodaj efekt…** i wybierz typ efektu. Możesz dodać wiele instancji; czerwone **✕**
  w nagłówku usuwa efekt.
- Przeciągnij **≡**, aby **zmienić kolejność**. **Górny** efekt działa **pierwszy**.
- Jednostki ms i Hz pozostają spójne po konwersji do **częstotliwości próbkowania projektu**. Obsługiwane są
  **mono** i **stereo** (mono jest przetwarzane dual-mono i sumowane z powrotem do jednego kanału).
- **OK** zapisuje zmiany w projekcie; **Anuluj** przywraca łańcuch z momentu otwarcia okna.

Efekty stosuje się po **zatrzymaniu nagrywania**, po zwykłym przechwyceniu i resamplingu. Konfiguracja jest
w `project.json` w `audioEffectChain`.

### Mix effects (full project)

**Project → Project Settings → Mix Effects** lets you build the same kind of ordered effect chain as **Track effects** (**Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion**, **Amp & cabinet**, **Vibrato (Tremolo)**), but applied to the **entire mixed program**: when you press **Play** to hear all enabled tracks together, and when you export with **Mix tracks to file** (toolbar or **Tools** menu). The chain is saved in `project.json` under `projectSettings` → `mixEffectChain`.

To reduce harsh [digital clipping](https://en.wikipedia.org/wiki/Clipping_%28audio%29) when processing pushes peaks toward full scale, the effect engine applies a **soft limiter** to normalized float samples immediately before conversion to 16-bit PCM. The **EffectWidget** base class documents `guardFloatSampleForInt16Pcm()` and `softLimitFloatSampleForInt16Pcm()` for any new real-time code that writes to 16-bit audio.

### Monitorowanie podczas nagrywania

Obok **wyświetlacza czasu** pole **Monitoruj dźwięk podczas nagrywania** włącza lub wyłącza wysyłanie **żywego wejścia**
do **wyjścia audio projektu** w trakcie nagrywania:

- **Ścieżki audio**: sygnał wejściowy jest odtwarzany na żywo (zapis bez zmian). Dodatkowo może grać **overdub** innych ścieżek.
- **Ścieżki MIDI**: przy włączeniu **renderuj MIDI do audio przy odtwarzaniu** i ustawionym **SoundFont** usłyszysz grę przez syntezator programowy. Przy **zewnętrznym wyjściu MIDI** użyj monitorowania instrumentu.

Ustawienie jest **zapisywane w projekcie** (`monitorWhileRecording` w `project.json`). Wyłącz, aby ograniczyć sprzężenie mikrofonowe.

#### Nagrywanie overdub

Podczas nagrywania nowej ścieżki, gdy inne włączone ścieżki zawierają już dane audio lub MIDI, Musician's Canvas wykonuje nagrywanie overdub: istniejące ścieżki są miksowane i odtwarzane w czasie rzeczywistym podczas nagrywania nowej ścieżki. Pozwala to słyszeć wcześniej nagrane partie podczas nagrywania nowej.

Miks istniejących ścieżek jest przygotowywany przed rozpoczęciem przechwytywania, dzięki czemu nagrywanie i odtwarzanie rozpoczynają się w przybliżeniu w tym samym momencie, utrzymując wszystkie ścieżki zsynchronizowane.

#### Backendy nagrywania

Musician's Canvas obsługuje dwa backendy przechwytywania dźwięku:

- **PortAudio** (domyślny, gdy dostępny): Zapewnia niezawodne nagrywanie z niskim opóźnieniem i jest zalecanym backendem.
- **Qt Multimedia**: Zapasowy backend wykorzystujący wbudowane przechwytywanie dźwięku Qt. Używany, gdy PortAudio nie jest dostępny lub gdy jest jawnie wybrany w Ustawieniach projektu.

Backend nagrywania można skonfigurować osobno dla każdego projektu w **Project > Project Settings > Audio**.

#### Częstotliwość próbkowania i obsługa urządzeń

Musician's Canvas nagrywa z natywną częstotliwością próbkowania urządzenia wejściowego audio, a następnie automatycznie konwertuje do skonfigurowanej częstotliwości próbkowania projektu przy użyciu wysokiej jakości resamplingu. Oznacza to, że można ustawić dowolną częstotliwość próbkowania projektu (np. 44100 Hz lub 48000 Hz) niezależnie od natywnej częstotliwości urządzenia. Konwersja zachowuje wysokość dźwięku i czas trwania dokładnie.

#### Wykrywanie urządzeń mono

Niektóre urządzenia audio (np. mikrofony kamer internetowych USB) są fizycznie mono, ale są zgłaszane jako stereo przez system operacyjny. Musician's Canvas automatycznie to wykrywa i odpowiednio dostosowuje liczbę kanałów. Jeśli projekt jest skonfigurowany na stereo, sygnał mono jest duplikowany do obu kanałów.

### Nagrywanie MIDI

1. Ustaw typ ścieżki na **MIDI** za pomocą przycisku Options.
2. Upewnij się, że urządzenie wejściowe MIDI jest skonfigurowane w **Settings > Configuration > MIDI**.
3. Uzbrój ścieżkę i kliknij Record.
4. Graj nuty na kontrolerze MIDI.
5. Kliknij Stop, aby zakończyć nagrywanie.

Nuty MIDI są wyświetlane w wizualizacji piano roll na ścieżce.

## Metronom

Musician's Canvas zawiera wbudowany metronom, którego można używać podczas nagrywania, aby
pomóc utrzymać rytm. Kliknij przycisk metronomu na pasku przycisków (pod paskiem menu), aby
otworzyć okno dialogowe ustawień metronomu:

![Ustawienia metronomu](../screenshots/MetronomeSettings.png)

Okno dialogowe udostępnia:

- **Enable metronome during recording**: Po zaznaczeniu metronom odtwarza dźwięk tykania,
  gdy trwa nagrywanie. Tykanie jest odtwarzane przez dźwięk systemowy i **nie** jest
  rejestrowane na nagrywanej ścieżce.
- **Beats per minute**: Numeryczne wejście tempa, w uderzeniach na minutę (BPM). Zakres to
  20–300 BPM.

Gdy metronom jest włączony, zaczyna tykać, gdy nagrywanie faktycznie się rozpocznie (po
zakończeniu 3-sekundowego odliczania), i zatrzymuje się po zakończeniu nagrywania.

## Odtwarzanie

Kliknij przycisk **Play**, aby zmiksować i odtworzyć wszystkie włączone ścieżki. Podpowiedź przycisku zmienia się, wskazując, czy będzie odtwarzać, czy nagrywać, w zależności od tego, czy ścieżka jest uzbrojona. Wyłączone ścieżki (niezaznaczone) są pomijane podczas odtwarzania.

Podczas odtwarzania ścieżki audio są dekodowane z plików FLAC, a ścieżki MIDI są renderowane do audio przy użyciu wbudowanego syntezatora FluidSynth. Wszystkie ścieżki są miksowane razem i odtwarzane przez urządzenie wyjściowe audio systemu.

Kliknij przycisk **Stop**, aby zakończyć odtwarzanie w dowolnym momencie.

## Miksowanie do pliku

Użyj **Tools > Mix tracks to file** (Ctrl+M), aby wyeksportować wszystkie włączone ścieżki do pojedynczego pliku audio. Dialog pozwala wybrać ścieżkę wyjściową i format:

- **Plik wyjściowy**: Przejdź, aby wybrać docelową ścieżkę pliku.
- **Format**: Wybierz między FLAC (bezstratna kompresja, mniejsze pliki) lub WAV (nieskompresowany).

Miks używa skonfigurowanej częstotliwości próbkowania projektu. Ścieżki MIDI są renderowane przy użyciu skonfigurowanego SoundFont.

## Ustawienia

### Ustawienia globalne

Użyj **Settings > Configuration** (Ctrl+,), aby ustawić globalne wartości domyślne, które mają zastosowanie do wszystkich projektów:

![Ustawienia ogólne](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### Karta Ogólne

- **Motyw**: Wybierz między ciemnym a jasnym motywem.

#### Karta Wyświetlanie

- **Kolor numerycznego wyświetlacza LED**: Wybierz kolor używany dla numerycznego wyświetlacza LED czasu pokazywanego na pasku narzędzi głównego okna. Aktywne segmenty cyfr są rysowane w wybranym kolorze, a nieaktywne segmenty są rysowane jako przyciemniona wersja tego samego koloru. Dostępne kolory to Light Red, Dark Red, Light Green, Dark Green, Light Blue, Dark Blue, Yellow, Orange, Light Cyan i Dark Cyan. Domyślny to Light Green.

![Ustawienia wyświetlania](../screenshots/MusiciansCanvas_DisplaySettings.png)

#### Karta Język

- **Język**: Wybierz język wyświetlania aplikacji. Domyślnie jest to "System Default", który używa ustawień językowych systemu operacyjnego. Dostępne języki to angielski, Deutsch (niemiecki), Español (hiszpański), Français (francuski), japoński, Português (portugalski brazylijski), chiński (tradycyjny), chiński (uproszczony) i piracki. Interfejs aktualizuje się natychmiast po zmianie języka.

![Ustawienia języka](../screenshots/MusiciansCanvas_LanguageSettings.png)

#### Karta MIDI

- **Urządzenie wyjściowe MIDI**: Wybierz wbudowany syntezator FluidSynth lub zewnętrzne urządzenie MIDI. Użyj przycisku **Refresh**, aby ponownie przeskanować dostępne urządzenia MIDI.
- **SoundFont**: Przejdź do pliku `.sf2` SoundFont do syntezy MIDI. W systemie Linux systemowy SoundFont może zostać automatycznie wykryty, jeśli jest zainstalowany pakiet `fluid-soundfont-gm`. W systemach Windows i macOS należy ręcznie skonfigurować ścieżkę SoundFont.

![Ustawienia MIDI](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Karta Audio

- **Urządzenie wejściowe audio**: Wybierz mikrofon lub urządzenie line-in do nagrywania.
- **Urządzenie wyjściowe audio**: Wybierz głośnik lub słuchawki do odtwarzania.

![Ustawienia audio](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### Ustawienia projektu

Użyj **Project > Project Settings** (Ctrl+P), aby nadpisać globalne wartości domyślne tylko dla bieżącego projektu. Jest to przydatne w przypadku projektów wymagających określonej częstotliwości próbkowania, SoundFont lub urządzenia audio. Ustawienia specyficzne dla projektu są zapisywane w pliku `project.json`.

![Ustawienia MIDI projektu](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### Karta MIDI

- **Urządzenie MIDI**: Wybierz urządzenie MIDI dla tego projektu lub pozostaw domyślne, aby użyć ustawienia globalnego.
- **SoundFont**: Wybierz plik SoundFont dla tego projektu.
- **Refresh**: Ponownie przeskanuj dostępne urządzenia MIDI.

#### Karta Audio

- **Urządzenie wejściowe audio**: Wybierz urządzenie nagrywające dla tego projektu.
- **Backend przechwytywania nagrywania** (gdy PortAudio jest dostępny):
  - **PortAudio (native input)**: Zalecany. Wykorzystuje tę samą bibliotekę audio co Audacity.
  - **Qt Multimedia**: Opcja zapasowa wykorzystująca wbudowane przechwytywanie audio Qt.
- **Urządzenie wejściowe PortAudio**: Gdy używany jest backend PortAudio, wybierz konkretne urządzenie wejściowe PortAudio.
- **Urządzenie wyjściowe audio**: Wybierz urządzenie odtwarzające dla tego projektu.

##### Ustawienia formatu audio

- **Częstotliwość próbkowania**: Wybierz spośród standardowych częstotliwości (8000 Hz do 192000 Hz). Natywna częstotliwość urządzenia jest oznaczona "(native)". Częstotliwości wymagające resamplingu są oznaczone "(resampled)". Możesz wybrać dowolną częstotliwość niezależnie od możliwości urządzenia; Musician's Canvas automatycznie wykonuje resampling w razie potrzeby.
- **Kanały**: Mono lub Stereo. Jeśli urządzenie obsługuje tylko mono, opcja Stereo jest wyłączona.

![Ustawienia audio projektu](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

#### Mix Effects tab

The **Mix Effects** tab is a scrollable list with the same controls as **Track effects** (**Add effect…**, drag **≡** to reorder, **✕** to remove). Processing order is **top to bottom** on the **combined** mix of all enabled tracks. These effects run during **whole-project playback** and when **mixing to a single WAV or FLAC file**; they are **not** baked into individual track files on disk. An empty list leaves the mixed signal unchanged aside from the mixer's own level handling.

## Menu

### Menu Plik

| Element menu  | Skrót    | Opis                                  |
|---------------|----------|---------------------------------------|
| Save Project  | Ctrl+S   | Zapisz bieżący projekt na dysk       |
| Open Project  | Ctrl+O   | Otwórz istniejący plik projektu      |
| Close         | Ctrl+Q   | Zamknij aplikację                     |

### Menu Projekt

| Element menu                        | Skrót    | Opis                                            |
|-------------------------------------|----------|-------------------------------------------------|
| Project Settings                    | Ctrl+P   | Konfiguruj ustawienia specyficzne dla projektu  |
| Add Demo Data to Selected Track     |          | Dodaj przykładowe nuty MIDI do demonstracji     |

### Menu Ustawienia

| Element menu  | Skrót    | Opis                                       |
|---------------|----------|--------------------------------------------|
| Configuration | Ctrl+,   | Otwórz globalne ustawienia aplikacji       |

### Menu Narzędzia

| Element menu          | Skrót    | Opis                                            |
|-----------------------|----------|-------------------------------------------------|
| Mix tracks to file    | Ctrl+M   | Eksportuj wszystkie włączone ścieżki do pliku  |
| Add drum track        | D        | Ścieżka MIDI perkusji i plik `.mid` (patrz niżej) |
| Virtual MIDI Keyboard |          | Uruchom aplikację towarzyszącą klawiatury       |

### Menu Help

| Element menu | Skrót | Opis |
|-------------|------|------|
| Manual      | Alt+M | Otwórz podręcznik PDF w aktualnie wybranym języku |
| About       |       | Pokaż informacje o wersji i aplikacji |

## Skróty klawiaturowe

| Skrót           | Działanie                          |
|-----------------|------------------------------------|
| Ctrl+S          | Zapisz projekt                     |
| Ctrl+O          | Otwórz projekt                     |
| Ctrl+M          | Miksuj ścieżki do pliku           |
| D               | Dodaj ścieżkę perkusji (menu Tools)    |
| Ctrl+P          | Ustawienia projektu                |
| Ctrl+,          | Ustawienia / Konfiguracja          |
| Ctrl+Q / Alt+F4 | Zamknij                           |


### Dodaj ścieżkę perkusji

**Tools → Add drum track** (skrót **D**) dodaje **MIDI** na **kanale 10** General MIDI (indeks 9). Domyślna nazwa **Drums**.

Od razu zapisywany jest **`.mid`** w **folderze projektu**: dwa takty 4/4. Tempo:

- Przy włączonym **Włącz metronom podczas nagrywania** używane jest **BPM** z metronomu.
- W przeciwnym razie **szacowane BPM** z włączonych ścieżek **audio**; inaczej **120 BPM**.

**Linki:** [Audient](https://audient.com/tutorial/how-to-program-realistic-midi-drum-tracks/), [MDrummer](https://www.meldaproduction.com/MDrummer), [Reddit](https://www.reddit.com/r/ableton/comments/1e51a7g/generating_midi_drum_patterns_based_on_audio_input/), [CS229 PDF](https://cs229.stanford.edu/proj2014/Louis%20Eugene,%20Guillaume%20Rostaing,%20Automated%20Music%20Track%20Generation.pdf).

## Virtual MIDI Keyboard

Virtual MIDI Keyboard to aplikacja towarzysząca (`virtual_midi_keyboard`), która udostępnia klawiaturę ekranową do wysyłania nut MIDI. Można ją uruchomić z menu **Tools > Virtual MIDI Keyboard** w głównej aplikacji lub uruchomić niezależnie.

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### Funkcje

- Klikaj klawisze na klawiaturze ekranowej, aby grać nuty
- Używaj klawiatury komputera jako klawiatury fortepianowej (patrz mapowanie klawiszy poniżej)
- Łącz się z zewnętrznymi urządzeniami wyjściowymi MIDI lub używaj wbudowanego syntezatora FluidSynth
- Łącz się z urządzeniem wejściowym MIDI, aby wyświetlać przychodzące nuty na klawiaturze
- Regulowana głośność syntezatora (master gain, 10%--200%)
- Wybór SoundFont dla wbudowanego syntezatora
- Wybór instrumentu/programu MIDI (General MIDI lub presety SoundFont)
- Pokrętło kontroli chorus/efekt
- Przesunięcie oktawy (-3 do +5)
- Kontrola głośności MIDI (CC#7, 0--127)

### Kontrolki paska narzędzi

- **Volume**: Głośność MIDI (CC#7), regulowana od 0 do 127 za pomocą pola liczbowego.
- **Octave**: Przesuwaj oktawę klawiatury za pomocą przycisków **<** i **>** lub pola liczbowego. Zakres wynosi od -3 do +5.
- **Chorus/Effect**: Pokrętło obrotowe i pole tekstowe (1--127) do ustawiania poziomu chorus/efektu (MIDI CC#93). Kliknij **Apply**, aby wysłać wartość.
- **Kanał MIDI**: Pole numeryczne (1--16) wybiera kanał MIDI, na którym wysyłane są nuty, zmiany programu i zmiany kontroli. Domyślnie jest to kanał 1. Wybrany kanał zaczyna obowiązywać natychmiast po zmianie. Wybranie presetu SoundFont, gdy wybrany jest określony kanał, przypisuje ten bank/program do tego kanału, dzięki czemu możesz grać różne dźwięki na różnych kanałach.
- **Perkusja**: Pole wyboru znajdujące się tuż nad selektorem kanału MIDI. Gdy nie jest zaznaczone (domyślnie), kanał MIDI jest taki, jaki wybrał użytkownik. Gdy jest zaznaczone, kanał MIDI jest automatycznie przełączany na kanał 10 — standardowy kanał perkusji General MIDI — dzięki czemu wszystko, co jest grane, brzmi jak perkusja. Odznaczenie pola przywraca kanał wybrany przed włączeniem Perkusji.
- **Selektor instrumentów**: Wybierz instrument MIDI. Gdy używany jest wbudowany syntezator FluidSynth, wyświetlane są presety SoundFont (Bank:Program Nazwa). Po podłączeniu do zewnętrznego urządzenia MIDI wyświetlana jest lista 128 instrumentów General MIDI.

### Granie na klawiaturze komputera

Klawiatura komputera jest zmapowana na klawisze fortepianowe w dwóch oktawach:

**Dolna oktawa (zaczynając od bieżącej oktawy):**

| Klawisz | Nuta  |
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

**Górna oktawa (jedna oktawa wyżej):**

| Klawisz | Nuta  |
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
| I       | C (następna oktawa) |
| 9       | C#/Db |
| O       | D     |
| 0       | D#/Eb |
| P       | E     |

Klawisze podświetlają się wizualnie po naciśnięciu (białe klawisze stają się jasnoniebieskie, czarne klawisze ciemnieją).

### Konfiguracja

Otwórz dialog konfiguracji (**Settings > Configuration**, Ctrl+,), aby skonfigurować urządzenia MIDI i audio:

![Konfiguracja Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard2.png)

#### Karta MIDI

- **Urządzenie wyjściowe MIDI**: Wybierz urządzenie zewnętrzne lub pozostaw puste dla wbudowanego syntezatora FluidSynth.
- **Urządzenie wejściowe MIDI**: Wybierz kontroler do przekazywania nut na wyświetlacz klawiatury.
- **Głośność syntezatora (Master Gain)**: Dostosuj poziom wyjściowy wbudowanego syntezatora (10%--200%).
- **SoundFont**: Przejdź do pliku `.sf2` SoundFont dla wbudowanego syntezatora.
- **Refresh**: Ponownie przeskanuj dostępne urządzenia MIDI.

#### Karta Audio

- **Urządzenie wyjściowe audio**: Wybierz urządzenie wyjściowe dla wbudowanego syntezatora.

#### Karta Język

- **Język**: Wybierz język wyświetlania. Dostępne są te same języki co w Musician's Canvas. Interfejs aktualizuje się natychmiast po zmianie języka.

### Skróty Virtual MIDI Keyboard

| Skrót    | Działanie                       |
|----------|---------------------------------|
| Ctrl+,   | Dialog konfiguracji             |
| Ctrl+U   | Pomoc / Informacje o użyciu     |
| Ctrl+Q   | Zamknij                         |

## FAQ (Często zadawane pytania)

### Jak nagrać ścieżkę MIDI?

1. Ustaw **katalog projektu** (wymagany do zapisu).
2. Dodaj lub wybierz ścieżkę i otwórz **Options** (lub kliknij ikonę typu ścieżki).
3. Ustaw typ **MIDI** i zamknij okno.
4. W **Settings > Configuration > MIDI** wybierz **wejście MIDI** (port sprzętowy lub wirtualny kabel) oraz **SoundFont** (`.sf2`) do późniejszego odtwarzania.
5. **Uzbrój (Arm)** tę ścieżkę (tylko jedna może być uzbrojona naraz).
6. Kliknij **Record**, poczekaj na odliczanie, graj na kontrolerze, potem **Stop**.

Nuty widać w piano roll. **File > Save Project** zapisuje plik `.mid` oraz `project.json` w folderze projektu.

### Dlaczego moja ścieżka MIDI jest bez dźwięku przy odtwarzaniu?

Odtwarzanie używa **FluidSynth** z **SoundFont** z ustawień. Sprawdź **Settings > Configuration > MIDI** (lub **Project > Project Settings** przy nadpisaniach projektu): potrzebna jest poprawna ścieżka `.sf2`. W Linuksie systemowy SoundFont bywa wykrywany automatycznie; w Windows i macOS zwykle trzeba wskazać plik ręcznie.

### Jak Virtual MIDI Keyboard łączy się z Musician's Canvas?

To **dwa osobne programy**. Uruchom Virtual MIDI Keyboard z **Tools > Virtual MIDI Keyboard** (lub samodzielnie). Aby klawiatura na ekranie **trafiała** do Musician's Canvas podczas nagrywania MIDI, system musi skierować **wyjście MIDI** klawiatury na **wejście** używane przez Musician's Canvas — często przez wirtualny kabel MIDI lub zgodne porty w obu aplikacjach. Nie łączą się automatycznie.

### Czym różnią się Configuration i Project Settings?

**Settings > Configuration** ustawia **globalne domyślne** (motyw, język, urządzenia MIDI/audio, SoundFont itd.). **Project > Project Settings** nadpisuje część tego **tylko dla bieżącego projektu** i zapisuje się w `project.json`. Jeśli pole pozostaje przy domyślnym ustawieniu projektu, obowiązuje wartość globalna z Configuration.

### Dlaczego przeciąganie plików nie dodaje audio?

Pliki są przyjmowane tylko przy ustawionym **katalogu projektu** i gdy Musician's Canvas **nie** odtwarza ani **nie** nagrywa. Obsługiwane są **`.wav`** i **`.flac`**; inne rozszerzenia są pomijane i wymienione w oknie. Każdy plik staje się nową ścieżką **audio** o nazwie z podstawowej nazwy pliku.

### Gdzie zapisywane są nagrania?

Audio trafia do **katalogu projektu** jako **`<nazwa_ścieżki>.flac`** (znaki takie jak `/ \ : * ? " < > |` zamieniane są na podkreślenie). Plik projektu to **`project.json`** w tym samym folderze. Ścieżki MIDI zapisują się jako **`<nazwa_ścieżki>.mid`** przy zapisie projektu (nuty także w `project.json`).

### Jak nazywa się plik po wyrenderowaniu MIDI do miksu?

Przy **miksowaniu** lub **odtwarzaniu** MIDI jest najpierw renderowane do tymczasowego WAV. Jeśli znana jest ścieżka projektu, Musician's Canvas zapisuje też **pamięć podręczną FLAC** w folderze projektu: **`<oczyszczona_nazwa_ścieżki>.flac`** (te same zasady co dla innych plików ścieżek). Nazwa wynika z **nazwy ścieżki**, nie z wewnętrznego ID.

### Czy mogę nagrywać dwie ścieżki naraz?

Nie. Tylko **jedna** ścieżka może być **uzbrojona**; ona dostaje następne nagranie. Buduj utwór, nagrywając **kolejno** (**overdub** odtwarza istniejące ścieżki podczas nowej).

### Czy metronom trafia na nagranie?

Nie. Po włączeniu metronom brzmi przez **audio systemowe** tylko dla Ciebie. **Nie** jest miksowany do pliku nagrania.

### Dlaczego Musician's Canvas wymaga ASIO w Windows?

W Windows główna aplikacja oczekuje sterownika **ASIO** dla stabilnego audio z niskim opóźnieniem. Przy błędach zainstaluj **ASIO4ALL** lub sterownik producenta interfejsu.

### Gdzie na macOS jest Virtual MIDI Keyboard?

W **pakiecie `.app`** plik wykonywalny Virtual MIDI Keyboard jest **kopiowany do środka** `Musician's Canvas.app` (**Contents/MacOS/**), aby dystrybuować jeden folder aplikacji. Nadal uruchomisz go z **Tools > Virtual MIDI Keyboard**.

## Rozwiązywanie problemów

### Brak wyjścia audio

- Sprawdź, czy prawidłowe urządzenie wyjściowe audio jest wybrane w Settings > Configuration > Audio.
- W systemie Linux sprawdź, czy PipeWire lub PulseAudio działa i czy wyjście nie jest wyciszone. Użyj `amixer` lub ustawień dźwięku pulpitu, aby sprawdzić poziomy głośności.
- W systemie Windows upewnij się, że zainstalowany jest sterownik audio ASIO (np. [ASIO4ALL](https://asio4all.org/) lub sterownik ASIO dostarczony przez producenta interfejsu audio). Musician's Canvas wymaga ASIO do audio o niskim opóźnieniu w systemie Windows.

### Ścieżki MIDI są ciche

- Upewnij się, że SoundFont (plik `.sf2`) jest skonfigurowany w Settings > Configuration > MIDI.
- W systemie Linux systemowy SoundFont może zostać automatycznie wykryty, jeśli jest zainstalowany pakiet `fluid-soundfont-gm`.
- W systemach Windows i macOS należy ręcznie skonfigurować ścieżkę SoundFont.

### Nagranie brzmi zniekształcone lub ma złą wysokość dźwięku

- Może się to zdarzyć, gdy natywna częstotliwość próbkowania urządzenia wejściowego audio różni się od skonfigurowanej częstotliwości projektu. Musician's Canvas obsługuje to automatycznie poprzez resampling, ale jeśli problemy się utrzymują, spróbuj ustawić częstotliwość próbkowania projektu na wartość odpowiadającą natywnej częstotliwości urządzenia.
- Mikrofony kamer internetowych USB często mają nietypowe natywne częstotliwości (np. 32000 Hz). Aplikacja wykrywa je automatycznie.
- Jeśli używasz backendu Qt Multimedia i masz problemy, spróbuj przełączyć się na backend PortAudio w Project Settings > Audio.

### Virtual MIDI Keyboard nie ma dźwięku

- W systemie Linux z PipeWire upewnij się, że zainstalowany jest pakiet `libpipewire-0.3-dev` (wymagany do integracji PipeWire z syntezatorem FluidSynth).
- Sprawdź, czy SoundFont jest załadowany (patrz karta MIDI w dialogu konfiguracji).
- Sprawdź, czy urządzenie wyjściowe audio jest wybrane i czy głośność systemu nie jest wyciszona.

## Kompilacja ze źródeł

Zobacz [README](../README.md), aby uzyskać kompletne instrukcje kompilacji dla systemów Linux, macOS i Windows, w tym wszystkie wymagane zależności.
