# Musician's Canvas

這是一款適用於桌上型電腦的多軌音樂錄音應用程式。它使用 C++ 編寫，並使用 Qt 作為
GUI 函式庫。此應用程式旨在為 Linux、Windows 和 Mac OS 構建。易用性是
主要的設計考量。這個專案最初是作為使用 Cursor（AI 程式設計工具）的實驗而開始的，旨在了解
它能夠建立什麼樣的此類應用程式，並持續使用 AI 開發以及
傳統軟體開發。

多年前，我在大學時製作了一個多軌音樂錄音程式（同名）；這是一次
嘗試製作更好版本的努力。

## 功能

- **多軌編排**：新增和移除多個軌道來作曲
- **命名軌道**：每個軌道都有一個可編輯的名稱，用作錄製音訊的檔案名稱
- **軌道類型**：將每個軌道配置為音訊錄音（從麥克風/線路輸入）或 MIDI 錄音（從鍵盤/控制器）；點擊軌道類型圖示可快速更改輸入源
- **軌道待錄**：勾選軌道上的「Arm」以將其選為錄音目標；一次只能有一個軌道處於待錄狀態。單一未錄製的軌道會自動設為待錄以方便使用
- **錄音倒數計時**：錄音開始前有 3 秒的視覺倒數計時，讓演奏者有時間準備
- **疊錄**：在現有軌道啟用的情況下錄製新軌道時，現有軌道會被混音並即時播放，讓您在錄音時能夠聽到它們。播放和錄製是同步的，以保持所有軌道對齊
- **視覺回饋**：音訊軌道的音訊波形顯示（錄音期間有即時電平表），MIDI 軌道的 MIDI 鋼琴捲簾
- **內建 MIDI 合成器**：使用 FluidSynth 搭配可設定的 SoundFont 將 MIDI 軌道渲染為音訊
- **混音為單一音訊檔案**：使用 [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) 函式庫將所有啟用的軌道匯出為單一混音的 WAV 或 FLAC 檔案
- **儲存/開啟專案**：將完整專案（軌道、名稱、類型、MIDI 音符、音訊檔案參照）序列化到 JSON 檔案並從中還原，退出時可偵測未儲存的變更
- **專案特定設定**：按專案覆寫全域 MIDI 和音訊預設值（取樣率、SoundFont、MIDI 裝置）
- **PortAudio 擷取（選用構建）**：當專案使用 PortAudio 構建時（`HAVE_PORTAUDIO`），錄音可以使用原生 PortAudio 輸入路徑（精神上類似 Audacity）而非 Qt Multimedia。**Project → Project Settings → Audio** 可讓您選擇 **PortAudio** 或 **Qt Multimedia** 並選取 PortAudio 輸入裝置。若未安裝 PortAudio，構建仍會成功，錄音僅使用 Qt Multimedia。
- **高品質取樣率轉換**：以音訊裝置的原生速率錄音，並使用加窗 sinc 內插法（Kaiser 窗，~96 dB 阻帶衰減）轉換為專案速率，這與 Audacity / libsoxr 使用的演算法系列相同。這允許以任何專案取樣率（8000 Hz 至 192000 Hz）錄音，不受裝置原生速率限制，且無音調或時長變化。
- **自動單聲道/立體聲處理**：偵測被標示為立體聲的物理單聲道裝置（在 PipeWire 上的 USB 網路攝影機麥克風常見此情況），並根據需要在單聲道和立體聲之間轉換（複製或平均），符合 Audacity 的聲道路由方法
- **錄音插入效果（僅音軌）**：在每個音軌的 **Options** 下方使用 **Effects** 按鈕開啟 **Track effects** 對話方塊。新增並設定 **Reverb**、**Chorus**、**Flanger**、**Overdrive / distortion** 與 **Amp & cabinet**（音箱頭與箱體建模），拖曳 **≡** 重新排序效果鏈（最上方最先處理），並與專案一併儲存。停止錄音後將效果套用至該次收音；參數採實際單位（ms、Hz），在擷取正規化後，單聲道與立體聲及常見專案取樣率（8 kHz–192 kHz）下行為一致。詳見[使用者手冊](docs/MusiciansCanvas_User_Manual_zh-TW.md)。譯者可於 `lupdate` 後透過 `scripts/effect_i18n.tsv`（由 `scripts/build_effect_tsv.py` 產生）與 `scripts/fill_effect_i18n.py` 批次更新效果字串。
- **混音效果（主匯流排）**：在 **Project → Project Settings → Mix Effects** 為**整首混音**設定效果鏈（類型與順序與音軌效果相同）。於**播放全部音軌**與**混音匯出為檔案**（WAV/FLAC）時套用；儲存在 `project.json` 的 `mixEffectChain`。
- **防削波輸出**：內建 DSP 在轉為 16 位元 PCM 前做軟限制。**EffectWidget** 提供 `guardFloatSampleForInt16Pcm()` 與 `softLimitFloatSampleForInt16Pcm()`。
- **錄製時監聽**：**時間顯示**右側的 **錄音時監聽音訊** 可於錄製期間將**即時輸入**送至**專案的音訊輸出**。音軌為正在錄下的訊號；MIDI 在啟用「為播放將 MIDI 算繪成音訊」且設定 SoundFont 時由軟體合成器播出。設定存於**專案**（`project.json` 的 `monitorWhileRecording`）。可關閉以降低麥克風回授。
- **低延遲音訊**：在 Windows 上，ASIO 驅動程式偵測確保低延遲音訊；在 Linux 上，提升程序排程優先權以降低 PipeWire / PulseAudio / ALSA 的抖動
- **新增鼓音軌**：**Tools → 新增鼓音軌**（快速鍵 **D**）會新增 General MIDI 第10通道（鼓組）的 MIDI 音軌，並在專案資料夾寫入兩小節的 `.mid`（大鼓、小鼓、嗨嗨鑔）。若**節拍器**啟用**錄音時啟用節拍器**，則使用該 BPM；否則從已啟用的音訊估算，失敗則預設 120 BPM。詳見[使用者手冊](docs/MusiciansCanvas_User_Manual_zh-TW.md)。
- **Virtual MIDI Keyboard**：一款透過軟體鋼琴鍵盤傳送 MIDI 音符的配套應用程式，具有內建 FluidSynth 合成器、可調主增益、電腦鍵盤到鋼琴對映、樂器/程式選擇、合唱/效果控制和八度移位
- **組態**：選擇音訊輸入裝置、MIDI 裝置和 SoundFont 檔案（全域預設值和按專案覆寫）
- **深色/淺色主題**：可透過 Settings → Configuration 設定
- **本地化**：兩個應用程式都有 18 種語言的本地化：英語、德語、西班牙語、法語、日語、葡萄牙語（巴西）、繁體中文、簡體中文、俄語、瑞典語、芬蘭語、丹麥語、挪威語、波蘭語、希臘語、愛爾蘭語、威爾斯語和海盜語。語言預設為作業系統設定，可在 **Settings → Configuration → Language** 中更改。使用 Qt 的標準 i18n 系統（`QTranslator` / `.ts` / `.qm`），更改語言時 UI 會立即更新。
- **使用手冊**：從 Markdown 原始碼產生的 HTML 和 PDF 文件，提供所有支援語言的版本（請參閱 [docs/](docs/)）

## 螢幕截圖

<p align="center">
	<a href="screenshots/i18n/zh_TW/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/i18n/zh_TW/MusiciansCanvas_1_MainWin.png" alt="主視窗" width="800"></a>
	<a href="screenshots/i18n/zh_TW/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/i18n/zh_TW/MusiciansCanvas_2_TrackConfig.png" alt="軌道設定" width="800"></a>
	<a href="screenshots/i18n/zh_TW/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/i18n/zh_TW/MusiciansCanvas_3_GeneralSettings.png" alt="一般設定" width="800"></a>
	<a href="screenshots/i18n/zh_TW/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/i18n/zh_TW/MusiciansCanvas_4_MIDISettings.png" alt="預設/一般 MIDI 設定" width="800"></a>
	<a href="screenshots/i18n/zh_TW/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/i18n/zh_TW/MusiciansCanvas_5_AudioSettings.png" alt="預設/一般音訊設定" width="800"></a>
	<a href="screenshots/i18n/zh_TW/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/i18n/zh_TW/MusiciansCanvas_DisplaySettings.png" alt="顯示設定" width="800"></a>
	<a href="screenshots/i18n/zh_TW/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/i18n/zh_TW/MusiciansCanvas_LanguageSettings.png" alt="語言設定" width="800"></a>
	<a href="screenshots/i18n/zh_TW/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/i18n/zh_TW/MusiciansCanvas_6_ProjectMIDISettings.png" alt="專案特定 MIDI 設定" width="800"></a>
	<a href="screenshots/i18n/zh_TW/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/i18n/zh_TW/MusiciansCanvas_7_ProjectAudioSettings.png" alt="專案特定音訊設定" width="800"></a>
	<a href="screenshots/i18n/zh_TW/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/i18n/zh_TW/Track_Effects_Dialog.png" alt="軌道效果對話框" width="800"></a>
	<a href="screenshots/i18n/zh_TW/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/i18n/zh_TW/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/i18n/zh_TW/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/i18n/zh_TW/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/i18n/zh_TW/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/i18n/zh_TW/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/i18n/zh_TW/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/i18n/zh_TW/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## 相依性

### 編譯器

需要支援 **C++17** 的編譯器（GCC 8+、Clang 7+、MSVC 2017+）。

---

### audio_mixer_cpp（所有平台皆需要）

混音和 FLAC 匯出功能依賴
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) 函式庫。
在構建前，將其克隆為此儲存庫的兄弟目錄：

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

CMake 構建期望在此專案根目錄的相對路徑 `../audio_mixer_cpp` 找到它。

---

### Qt6 Multimedia（選用但強烈建議）

`qt6-multimedia` 是選用的。沒有它，應用程式仍可構建和執行，但
**音訊錄音和播放將被停用** — 只有 MIDI 編輯和專案管理可以使用。
使用以下平台命令將其與核心 Qt6 函式庫一起安裝。

---

### PortAudio（選用）

如果已安裝 PortAudio 函式庫和標頭，CMake 會啟用 **`HAVE_PORTAUDIO`** 並
將其連結到 **musicians_canvas**。除非專案在 **Project → Project Settings → Audio** 中設定為
**Qt Multimedia**，否則錄音預設使用 PortAudio 路徑。如果找不到 PortAudio，
構建會在沒有它的情況下繼續（包裝器編譯為存根）。

---

### MIDI 合成用 SoundFont

MIDI 軌道需要 SoundFont（`.sf2`）檔案才能產生音訊。沒有的話，
MIDI 軌道將渲染為靜音。在 **Linux** 上，內建的 FluidSynth 合成器
會自動偵測安裝在標準系統路徑的 SoundFont
（請參閱以下套件名稱）。在 **macOS 和 Windows** 上沒有標準系統路徑，
因此您必須在 **Settings → Configuration → MIDI** 中手動設定 SoundFont。

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

> `fluid-soundfont-gm` 將 `FluidR3_GM.sf2` 安裝到 `/usr/share/sounds/sf2/`，
> 啟動時會自動偵測。`timgm6mb-soundfont` 是較小的替代方案。
>
> `libpipewire-0.3-dev` 在基於 PipeWire 的系統上需要，以便 Virtual MIDI
> Keyboard 可以在 FluidSynth 建立其音訊資源之前呼叫 `pw_init()`。
> 沒有它構建仍會繼續；只是不會定義 `HAVE_PIPEWIRE` 旗標。

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel libsndfile-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` 將 `FluidR3_GM.sf2` 安裝到 `/usr/share/soundfonts/`，
> 啟動時會自動偵測。

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac libsndfile portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` 將 `FluidR3_GM.sf2` 安裝到 `/usr/share/soundfonts/`，
> 啟動時會自動偵測。`pipewire` 通常已安裝在現代 Arch 系統上；
> 開發標頭包含在主套件中。

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac libsndfile portaudio
```

> PipeWire 是純 Linux 子系統，在 macOS 上**不**需要。FluidSynth
> 將透過 Qt Multimedia 後端自動使用 CoreAudio。
>
> 下載 General MIDI SoundFont（例如
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) 或
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)）
> 並在 **Settings → Configuration → MIDI** 中設定其路徑。

---

### Windows

**musicians_canvas** 需要 **ASIO 音訊驅動程式**才能進行低延遲錄音和播放。
如果未偵測到 ASIO 驅動程式，應用程式將無法啟動。
**virtual_midi_keyboard** 不需要 ASIO，直接使用 Qt Multimedia 的 WASAPI 後端。

**為 musicians_canvas 安裝 ASIO 驅動程式（擇一）：**

| 驅動程式 | 備註 |
|----------|------|
| [ASIO4ALL](https://asio4all.org/) | 免費，適用於大多數內建和 USB 音訊硬體 |
| 製造商驅動程式 | 專用音訊介面的最佳延遲（例如 Focusrite、PreSonus、RME） |

**工具鏈 — MSYS2（建議用於 MinGW 構建）：**

從 <https://www.msys2.org> 下載並安裝 MSYS2，然後將
`C:\msys64\mingw64\bin` 新增到系統 `PATH`。安裝所需套件：

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

> `mingw-w64-x86_64-soundfont-fluid` 將 `FluidR3_GM.sf2` 安裝到
> `C:\msys64\mingw64\share\soundfonts\`（如果 MSYS2 安裝在其他位置請調整）。
> 由於 Windows 沒有標準的 SoundFont 搜尋路徑，您必須在首次啟動後
> 在 **Settings → Configuration → MIDI** 中手動設定此路徑。
>
> PipeWire 是純 Linux 子系統；在 Windows 上不需要 PipeWire 套件。

套件參考頁面：
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**工具鏈 — Visual Studio 2022：**

透過 [Qt Online Installer](https://www.qt.io/download) 安裝 Qt 6，並取得
FluidSynth、RtMidi 和 libFLAC 二進位檔（或從原始碼構建）。
只要適當的目錄在 `CMAKE_PREFIX_PATH` 中，CMake 構建將透過
`find_library` / `find_path` 找到它們。

必須另外下載 SoundFont（例如
[GeneralUser GS](https://schristiancollins.com/generaluser.php) 或
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)），
並在首次啟動後於 **Settings → Configuration → MIDI** 中設定其路徑。

> **注意：** `advapi32`（Windows 登錄檔）和 `winmm`（Windows Multimedia）由
> CMake 自動連結；不需要額外安裝。

---

## 構建

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

**Windows（Visual Studio 2022）：**

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

如果在執行時找不到所需的 DLL，請從構建輸出目錄執行輔助腳本：

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## 產生文件

使用手冊可從 Markdown 原始碼產生為 HTML 和 PDF：

```bash
cd docs
./generate_docs.sh          # 產生 HTML 和 PDF
./generate_docs.sh html     # 僅產生 HTML
./generate_docs.sh pdf      # 僅產生 PDF
```

**先決條件：**

- **Python（偏好）：** `pip3 install markdown weasyprint`
- **備選方案：** `pandoc` 和 `wkhtmltopdf`（透過系統套件管理員）

產生的 HTML 寫入 `docs/html/`，PDF 寫入 `docs/MusiciansCanvas_User_Manual.pdf`。

---

## 執行

```bash
./build/musicians_canvas
# 或
./build/virtual_midi_keyboard   # 配套應用程式 Virtual MIDI Keyboard
```

---

## 使用方式

### musicians_canvas

1. **設定專案目錄**：輸入或瀏覽到一個資料夾，錄音和專案檔案將儲存在此
2. **新增軌道**：點擊「+ Add Track」；在「Options」旁邊的文字欄位中為每個軌道命名
3. **設定軌道類型**：點擊軌道上的「Options」（或點擊「Options」和名稱欄位之間的軌道類型圖示）以選擇 Audio 或 MIDI 並設定輸入源
4. **移除軌道**：點擊軌道列右側的「×」按鈕
5. **全域設定**：使用 **Settings → Configuration** 設定全域預設值：
   - *General* 分頁：主題（深色/淺色）
   - *MIDI* 分頁：預設 MIDI 輸出裝置（內建 FluidSynth 合成器或外部 MIDI 裝置）和預設 SoundFont（`.sf2`）
   - *Audio* 分頁：用於錄音和播放的預設音訊輸入/輸出裝置
6. **專案設定**：使用 **Project → Project Settings**（Ctrl+P）僅為目前專案覆寫 MIDI 和音訊設定（例如每首歌不同的取樣率或 SoundFont）
7. **錄音**：在目標軌道上勾選「Arm」，然後點擊錄音按鈕（紅色圓圈）。一次只能有一個軌道處於待錄狀態
8. **播放**：點擊播放按鈕以混音並播放所有啟用的軌道
9. **混音為檔案**：使用 **Tools → Mix tracks to file**（Ctrl+M）匯出為 WAV 或 FLAC
10. **儲存/開啟**：使用 **File → Save Project**（Ctrl+S）和 **File → Open Project**（Ctrl+O）

### virtual_midi_keyboard

1. **開啟組態**：使用 **Configuration** 按鈕或選單（Ctrl+,）開啟設定對話方塊
2. **MIDI 分頁**：
   - 選擇 MIDI 輸出裝置（外部硬體/軟體合成器）或留空以使用內建 FluidSynth 合成器
   - 選擇 MIDI 輸入裝置以將傳入的 MIDI 音符轉發到鍵盤顯示
   - 調整 **Synthesizer Volume (Master Gain)** 以控制內建合成器的輸出電平（10%–200%）
3. **Audio 分頁**：選擇內建合成器使用的音訊輸出裝置
4. **SoundFont**：為內建合成器選擇 `.sf2` SoundFont 檔案（在 Linux 上若已安裝系統 SoundFont 則自動偵測）
5. **演奏音符**：點擊螢幕上鋼琴鍵盤的琴鍵、使用連接的 MIDI 控制器或使用電腦鍵盤：
   - 下方八度：Z/X/C/V/B/N/M = C/D/E/F/G/A/B，S/D/G/H/J = 升號/降號
   - 上方八度：Q/W/E/R/T/Y/U/I/O/P = C 到 E，2/3/5/6/7/9/0 = 升號/降號
6. **工具列控制**：調整 MIDI 音量（0–127）、八度（-3 到 +5）、合唱/效果電平，以及選擇樂器

---

## 鍵盤快捷鍵

**musicians_canvas：**

| 快捷鍵 | 動作 |
|--------|------|
| Ctrl+S | 儲存專案 |
| Ctrl+O | 開啟專案 |
| Ctrl+M | 混音軌道為檔案 |
| Ctrl+P | 專案設定 |
| Ctrl+, | 設定 / 組態 |
| Ctrl+Q / Alt+F4 | 退出 |

**virtual_midi_keyboard：**

| 快捷鍵 | 動作 |
|--------|------|
| Ctrl+, | 組態對話方塊 |
| Ctrl+U | 說明 / 使用資訊 |
| Ctrl+Q | 關閉 |

---

## 授權條款

本專案按原樣提供，僅供教育和個人使用。
