---
title: "Musician's Canvas 使用手冊"
subtitle: "多軌音樂錄製應用程式"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "zh-TW"
---

# Musician's Canvas 使用手冊

## 簡介

Musician's Canvas 是一款適用於桌上型電腦的多軌音樂錄製應用程式。它支援從麥克風和線路輸入裝置進行音訊錄製、從鍵盤和控制器進行 MIDI 錄製，並可將所有音軌混音為單一 WAV 或 FLAC 檔案。附屬應用程式 Virtual MIDI Keyboard 提供了一個軟體鋼琴鍵盤，用於傳送 MIDI 音符。

Musician's Canvas 的設計注重易用性，同時提供數位音訊工作站（DAW）中常見的功能：

- 多軌音訊和 MIDI 錄製
- 疊錄功能，可同步播放現有音軌
- 內建 FluidSynth MIDI 合成器，支援 SoundFont
- 高品質取樣率轉換，支援以任何專案取樣率進行錄製
- 自動單聲道/立體聲裝置偵測
- 基於專案的設定，可針對個別專案進行覆寫
- 混音匯出為 WAV 或 FLAC
- 深色和淺色主題
- 支援 18 種語言本地化，包括 English、Deutsch、Español、Français、Japanese、Português、Chinese、Русский、Svenska、Suomi、Dansk、Norsk、Polski、Ελληνικά、Gaeilge、Cymraeg 和 Pirate
- 附屬 Virtual MIDI Keyboard 應用程式

## 入門指南

### 啟動應用程式

從建置目錄或安裝位置執行 `musicians_canvas` 可執行檔：

```
./musicians_canvas
```

首次啟動時，應用程式會開啟一個空白專案。您需要在錄製前設定專案目錄。

啟動時，應用程式會套用已儲存的主題（深色或淺色），如果先前使用的專案目錄中包含 `project.json` 檔案，則會自動載入該專案。

### 設定專案

1. **設定專案目錄**：在視窗頂部的「Project Location」欄位中輸入或瀏覽至一個資料夾。錄音檔案和專案檔案將儲存在此處。

2. **新增音軌**：點選 **+ Add Track** 按鈕。一個具有預設設定的新音軌將會出現。如果它是專案中唯一的音軌且尚未錄製，則會自動啟用錄製待命。

3. **為音軌命名**：在「Options」按鈕旁的文字欄位中輸入名稱。此名稱將作為錄製音訊檔案的檔案名稱。

![主視窗](../screenshots/MusiciansCanvas_1_MainWin.png)

### 按鈕列

選單列正下方有一個帶有快速存取按鈕的工具列：

![按鈕列](../screenshots/ButtonBar.png)

- **Open Project**：與 **File > Open Project** 相同 — 開啟先前儲存的專案。
- **Save Project**：與 **File > Save Project** 相同 — 儲存目前專案。此按鈕僅在設定專案目錄時啟用。
- **Project Settings**：與 **Project > Project Settings** 相同 — 開啟專案設定對話框。此按鈕僅在設定專案目錄時啟用。
- **Configuration**：與 **Settings > Configuration** 相同 — 開啟全域應用程式設定對話框。
- **Metronome Settings**：開啟節拍器設定對話框（請參閱下方的節拍器章節）。

### 儲存和開啟專案

- **儲存**：使用 **File > Save Project**（Ctrl+S）將目前專案儲存為專案目錄中的 JSON 檔案。
- **開啟**：使用 **File > Open Project**（Ctrl+O）載入先前儲存的專案。

專案檔案（`project.json`）儲存音軌名稱、類型、MIDI 音符、音訊檔案參照及所有專案特定設定。音訊檔案儲存在與 `project.json` 相同的目錄中，並以其音軌名稱命名（例如 `My_Track.flac`）。

如果您在有未儲存變更的情況下關閉應用程式，系統會顯示確認對話框，詢問是否要在退出前儲存。

## 音軌管理

### 新增和移除音軌

- 點選 **+ Add Track** 以新增音軌至編曲中。
- 點選音軌列右側的 **x** 按鈕以移除該音軌。
- 點選 **Clear Tracks**（工具列中的紅色按鈕）以移除所有音軌。在執行前會顯示確認對話框。

### 透過拖放新增音軌

當專案開啟時，您可以從檔案管理員（Windows 檔案總管、macOS Finder、Linux 檔案
管理員等）將一個或多個支援的音訊檔案直接拖放到 Musician's Canvas 視窗上，
將它們新增為新的音訊音軌。

- **支援的格式：** `.wav` 和 `.flac`。任何其他格式的檔案會被靜默略過，結束時
  會有一個對話框列出哪些檔案被略過。
- **檔案複製：** 如果拖放的檔案尚未位於專案目錄中，它會自動被複製到該處。
  如果專案目錄中已經存在同名檔案，系統會詢問您是否要取代它。
- **音軌名稱：** 檔案的基本名稱（不含副檔名）會被用作新音軌的名稱。例如，
  拖放 `Bass Line.wav` 會建立一個名為「Bass Line」的音訊音軌。
- **一次拖放多個檔案：** 可以一起拖放多個檔案；在單次拖放中，每個支援的檔案
  都會成為獨立的音軌。
- **拖放被拒絕時：** 只有在專案開啟且 Musician's Canvas **未**正在播放或
  錄製時才會接受拖放。如果您想拖入更多音軌，請先停止播放或錄製。

### 設定音軌類型

每個音軌可設定為 **Audio**（用於麥克風/線路輸入錄製）或 **MIDI**（用於鍵盤/控制器錄製）。

若要變更音軌類型：

- 點選音軌上的 **Options** 按鈕，或
- 點選 **音軌類型圖示**（位於「Options」和名稱欄位之間）

這將開啟「Track Configuration」對話框，您可以在其中選擇輸入來源。

![音軌設定](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### 音軌控制項

每個音軌列提供以下控制項：

- **Enable 核取方塊**：切換音軌是否包含在播放和混音中。停用音軌也會自動取消其錄製待命狀態。
- **Arm 選項按鈕**：選擇此音軌作為錄製目標。一次只能有一個音軌處於待命狀態；啟用新音軌的待命會自動取消先前待命的音軌。
- **名稱欄位**：可編輯的文字欄位，用於設定音軌名稱。此名稱作為音訊檔案名稱使用（無效的檔案系統字元會被替換為底線）。
- **Options 按鈕**：開啟「Track Configuration」對話框。
- **類型圖示**：音訊音軌顯示喇叭圖示，MIDI 音軌顯示鋼琴圖示。點選它會開啟「Track Configuration」對話框。
- **移除按鈕 (x)**：從專案中移除該音軌。

### 自動待命

當專案只有一個音軌且該音軌尚未錄製時，它會自動進入錄製待命狀態。這適用於在新專案中新增第一個音軌，以及開啟僅含單一空白音軌的現有專案。

### 音軌視覺化

- **音訊音軌** 顯示已錄製音訊的波形視覺化。當沒有錄製音訊時，該區域顯示「No audio recorded」。
- **MIDI 音軌** 顯示鋼琴捲軸視覺化，在跨越 A0 至 C8 的網格上顯示已錄製的音符。音符依力度著色。當沒有錄製 MIDI 資料時，該區域顯示「No MIDI data recorded」。

## 錄製

### 音訊錄製

1. 確保已設定專案目錄。
2. 將目標音軌設為待命（勾選「Arm」選項按鈕）。
3. 點選 **Record** 按鈕（紅色圓圈）。
4. 音軌上會出現 3 秒倒數計時（「Get ready... 3」、「2」、「1」），然後開始錄製。
5. 錄製期間，音軌的波形區域會顯示即時電平表，以漸層條（綠色到黃色到紅色）顯示目前振幅，並標有「Recording」標籤。
6. 點選 **Stop** 按鈕結束錄製。

錄製的音訊以 FLAC 檔案格式儲存在專案目錄中，以音軌名稱命名。

在錄製和播放期間，所有互動控制項（音軌按鈕、設定等）都會被停用，以防止意外變更。

### 插入式效果（僅音訊音軌）

音訊音軌在 **Options** 下方有 **效果** 按鈕，可開啟 **軌道效果** 對話方塊，為該音軌的錄音建立 **有序效果链**：

![軌道效果對話框](../screenshots/Track_Effects_Dialog.png)

可加入 **殘響**、**合唱**、**鑲邊（Flanger）**、**過載／失真（Overdrive / distortion）**、**音箱頭與箱體建模（Amp & cabinet）**（箱頭類型、箱體、增益、低／中／高頻、空氣感與溼乾比等），以及 **顫音（Vibrato / Tremolo）**。

**顫音（Vibrato / Tremolo）** 是 Fender 許多音箱上標示為 “Vibrato” 的經典效果（其實是 **音量隨時間週期性起伏** 的 tremolo）。使用 **Speed** 調整起伏速度，使用 **Intensity**（Depth）調整深度（從細微閃動到強烈斷奏）。

以下為 **Amp & cabinet** 的介面範例：

![Amp & cabinet 建模 (1)](../screenshots/Amp_And_Cabinet_Model_1.png)

![Amp & cabinet 建模 (2)](../screenshots/Amp_And_Cabinet_Model_2.png)

- 按 **加入效果…** 選擇一種效果類型。可新增多個；效果標題列上的紅色 **✕** 可移除。
- 拖曳 **≡** 可 **重新排序**；**最上方** 的效果 **最先** 處理。
- 以毫秒、赫茲為單位，在轉換到 **專案取樣率** 後仍一致。支援 **單聲道** 與 **立體聲**（單聲道內部以雙單聲道處理後再混回單聲道）。
- **確定** 寫入專案；**取消** 還原開啟對話方塊前的鏈。

效果在 **停止錄製** 後套用（經一般擷取與重取樣）。設定儲存在 `project.json` 的 `audioEffectChain`。

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

### 錄製時監聽

在 **時間顯示** 右側，**錄音時監聽音訊** 控制錄製期間是否將 **即時輸入** 送到 **專案的音訊輸出**：

- **音軌**：輸入裝置的聲音會即時播出（錄製管線不變）。若同時有 **疊錄** 播放其他軌道，會疊加在一起。
- **MIDI 軌**：若專案啟用 **為播放將 MIDI 算繪成音訊** 且已設定 **SoundFont**，可透過軟體合成器聽到彈奏；僅 **外部 MIDI 輸出** 時請用樂器本身的監聽。

此設定儲存在 **專案**（`project.json` 的 `monitorWhileRecording`）。若要避免麥克風回授可關閉。

#### 疊錄

當錄製新音軌時，如果其他已啟用的音軌已包含音訊或 MIDI 資料，Musician's Canvas 會執行疊錄：現有音軌會混合在一起，並在錄製新音軌的同時即時播放。這讓您可以在錄製新段落時聽到先前錄製的部分。

現有音軌的混音會在擷取開始前準備好，因此錄製和播放幾乎同時開始，保持所有音軌同步。

#### 錄製後端

Musician's Canvas 支援兩種音訊擷取後端：

- **PortAudio**（可用時為預設）：提供穩定的低延遲錄製，是建議使用的後端。
- **Qt Multimedia**：使用 Qt 內建音訊擷取的備用後端。當 PortAudio 不可用或在 Project Settings 中明確選擇時使用。

錄製後端可在 **Project > Project Settings > Audio** 中針對每個專案進行設定。

#### 取樣率與裝置處理

Musician's Canvas 以音訊輸入裝置的原生取樣率進行錄製，然後使用高品質重新取樣自動轉換為專案設定的取樣率。這意味著您可以設定任何專案取樣率（例如 44100 Hz 或 48000 Hz），而不受裝置原生取樣率的限制。轉換過程會精確保留音高和時間長度。

#### 單聲道裝置偵測

某些音訊裝置（例如 USB 網路攝影機麥克風）實際上是單聲道，但被作業系統報告為立體聲。Musician's Canvas 會自動偵測此情況並相應調整聲道數。如果專案設定為立體聲，單聲道訊號會被複製到兩個聲道。

### MIDI 錄製

1. 透過 Options 按鈕將音軌類型設定為 **MIDI**。
2. 確保在 **Settings > Configuration > MIDI** 中已設定 MIDI 輸入裝置。
3. 將音軌設為待命並點選 Record。
4. 在您的 MIDI 控制器上演奏音符。
5. 點選 Stop 結束錄製。

MIDI 音符會以鋼琴捲軸視覺化顯示在音軌上。

## 節拍器

Musician's Canvas 內建節拍器，可在錄製期間用於協助保持節奏。點選選單列下方按鈕列上的節拍器按鈕，以開啟節拍器設定對話框：

![節拍器設定](../screenshots/MetronomeSettings.png)

該對話框提供：

- **Enable metronome during recording**：勾選時，節拍器會在錄製進行中播放滴答聲。滴答聲會透過系統音訊播放，**不會**被擷取到錄製的音軌中。
- **Beats per minute**：速度的數字輸入，以每分鐘拍數（BPM）為單位。範圍為 20–300 BPM。若 **Project → Tempo map** 已定義速度變化，錄製時節拍器會跟隨這些標記（在尚無適用標記前，仍以本對話方塊的 BPM 作為起始速度）。

當節拍器啟用時，它會在錄製真正開始後（3 秒倒數計時結束後）開始滴答，並在錄製結束時停止。

## 播放

點選 **Play** 按鈕以混音並播放所有已啟用的音軌。按鈕的工具提示會根據是否有音軌處於待命狀態，指示將執行播放或錄製。已停用的音軌（未勾選）會從播放中排除。

播放期間，音訊音軌會從其 FLAC 檔案中解碼，MIDI 音軌會使用內建的 FluidSynth 合成器轉譯為音訊。所有音軌會混合在一起，並透過系統的音訊輸出裝置播放。

點選 **Stop** 按鈕可隨時結束播放。

## 混音匯出為檔案

使用 **Tools > Mix tracks to file**（Ctrl+M）將所有已啟用的音軌匯出為單一音訊檔案。對話框可讓您選擇輸出路徑和格式：

- **輸出檔案**：瀏覽以選擇目的地檔案路徑。
- **格式**：選擇 FLAC（無損壓縮，檔案較小）或 WAV（未壓縮）。

混音使用專案設定的取樣率。MIDI 音軌使用設定的 SoundFont 進行轉譯。每軌的 **增益**、**聲像**、**靜音／獨奏**、**Aux 送出** 與 **修剪** 與播放時相同套用。

**Tools → Export stems to folder** 會為參與混音的已啟用音軌各寫出一個 **WAV** 分軌；每個分軌反映該軌的混音與修剪設定；全專案的 **Mix Effects** **不會**套用在個別分軌上。

## 設定

### 全域設定

使用 **Settings > Configuration**（Ctrl+,）設定適用於所有專案的全域預設值：

![一般設定](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### General 分頁

- **主題**：選擇深色或淺色主題。
- **寫入錄製除錯日誌**：啟用時，Musician's Canvas 會將詳細的錄製診斷資訊寫入專案目錄中
  名為 `recording_debug.txt` 的檔案。這包括擷取後端、裝置名稱、取樣率、聲道數和計時
  資料。此選項預設為停用，主要用於排解錄製問題。

#### Display 分頁

- **數字顯示 LED 顏色**：選擇主視窗工具列上顯示的數字 LED 時間顯示所使用的顏色。
  作用中的數字區段以所選顏色繪製，非作用中的區段則以相同顏色的暗淡版本繪製。
  可用的顏色為 Light Red、Dark Red、Light Green、Dark Green、Light Blue、Dark
  Blue、Yellow、Orange、Light Cyan 和 Dark Cyan。預設值為 Light Green。

![顯示設定](../screenshots/MusiciansCanvas_DisplaySettings.png)

#### Language 分頁

- **語言**：選擇應用程式的顯示語言。預設值為「System Default」，使用作業系統的語言
  設定。可用語言包括 English、Deutsch (German)、Español (Spanish)、Français (French)、
  Japanese、Português (Brazilian Portuguese)、Chinese (Traditional)、Chinese
  (Simplified)、Dansk (Danish)、Ελληνικά (Greek)、Gaeilge (Irish)、Norsk (Norwegian)、
  Polski (Polish)、Русский (Russian)、Suomi (Finnish)、Svenska (Swedish)、Cymraeg (Welsh)
  和 Pirate。變更語言後，介面會立即更新。

![語言設定](../screenshots/MusiciansCanvas_LanguageSettings.png)

#### MIDI 分頁

- **MIDI Output Device**：選擇內建的 FluidSynth 合成器或外部 MIDI 裝置。使用 **Refresh** 按鈕重新掃描可用的 MIDI 裝置。
- **SoundFont**：瀏覽至 `.sf2` SoundFont 檔案以進行 MIDI 合成。在 Linux 上，如果已安裝 `fluid-soundfont-gm` 套件，系統可能會自動偵測 SoundFont。在 Windows 和 macOS 上，您需要手動設定 SoundFont 路徑。

![MIDI 設定](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Audio 分頁

- **Audio Input Device**：選擇用於錄製的麥克風或線路輸入裝置。
- **Audio Output Device**：選擇用於播放的喇叭或耳機裝置。

![音訊設定](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### 專案設定

使用 **Project > Project Settings**（Ctrl+P）為目前專案覆寫全域預設值。這對於需要特定取樣率、SoundFont 或音訊裝置的專案非常有用。專案特定設定儲存在 `project.json` 檔案中。

![專案 MIDI 設定](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### MIDI 分頁

- **MIDI Device**：為此專案選擇 MIDI 裝置，或保持預設以使用全域設定。
- **SoundFont**：為此專案選擇 SoundFont 檔案。
- **Refresh**：重新掃描可用的 MIDI 裝置。

#### Audio 分頁

- **Audio Input Device**：為此專案選擇錄製裝置。
- **Recording capture backend**（當 PortAudio 可用時）：
  - **PortAudio (native input)**：建議使用。使用與 Audacity 相同的音訊函式庫。
  - **Qt Multimedia**：使用 Qt 內建音訊擷取的備用選項。
- **PortAudio Input Device**：使用 PortAudio 後端時，選擇特定的 PortAudio 輸入裝置。
- **Audio Output Device**：為此專案選擇播放裝置。

##### 音訊格式設定

- **取樣率**：從標準取樣率（8000 Hz 至 192000 Hz）中選擇。裝置的原生取樣率標記為「(native)」。需要重新取樣的取樣率標記為「(resampled)」。您可以選擇任何取樣率，不受裝置能力限制；Musician's Canvas 會根據需要自動重新取樣。
- **聲道**：單聲道或立體聲。如果裝置僅支援單聲道，則立體聲選項會被停用。

![專案音訊設定](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

#### Mix Effects tab

The **Mix Effects** tab is a scrollable list with the same controls as **Track effects** (**Add effect…**, drag **≡** to reorder, **✕** to remove). Processing order is **top to bottom** on the **combined** mix of all enabled tracks. These effects run during **whole-project playback** and when **mixing to a single WAV or FLAC file**; they are **not** baked into individual track files on disk. An empty list leaves the mixed signal unchanged aside from the mixer's own level handling.

#### Aux / Send Bus 索引標籤

設定 **共用 Aux 效果鏈**（與音軌插入效果相同類型）。每軌的 **Aux** 滑桿控制送入此匯流排的量；濕聲回送會在套用 **Mix Effects** 之前加回乾聲總和。

## 選單

### File 選單

| 選單項目      | 快捷鍵   | 說明                           |
|---------------|----------|--------------------------------|
| Save Project  | Ctrl+S   | 將目前專案儲存至磁碟           |
| Open Project  | Ctrl+O   | 開啟現有的專案檔案             |
| Close         | Ctrl+Q   | 結束應用程式                   |

### Project 選單

| 選單項目                            | 快捷鍵   | 說明                                   |
|-------------------------------------|----------|----------------------------------------|
| Project Settings                    | Ctrl+P   | 設定專案（含 **Aux / Send Bus**）     |
| Tempo map                           |          | 編輯速度變化（秒與 BPM）：節拍器與 MIDI 量化 |
| Add Demo Data to Selected Track     |          | 為選取的音軌新增範例 MIDI 音符         |

### Edit 選單

| 選單項目 | 快捷鍵 | 說明 |
|----------|--------|------|
| Undo | Ctrl+Z | 復原混音器／修剪變更 |
| Redo | Ctrl+Shift+Z | 重做 |

### Settings 選單

| 選單項目      | 快捷鍵   | 說明                           |
|---------------|----------|--------------------------------|
| Configuration | Ctrl+,   | 開啟全域應用程式設定           |

### Tools 選單

| 選單項目              | 快捷鍵   | 說明                                   |
|-----------------------|----------|----------------------------------------|
| Mix tracks to file    | Ctrl+M   | 將所有已啟用的音軌匯出為檔案           |
| Export stems to folder |          | 每音軌一個 WAV（增益／聲像／修剪；無主程式 Mix Effects） |
| Recording options     |          | 音訊 **切入錄音**；整個專案的 **循環播放** |
| Quantize MIDI         |          | 將 MIDI 音符開頭對齊格線（全部 MIDI 或僅待命音軌） |
| Add drum track        | D        | 新增鼓 MIDI 音軌並寫入 `.mid`（見下文）   |
| Virtual MIDI Keyboard |          | 啟動附屬鍵盤應用程式                   |

### Help 選單

| 選單項目 | 快捷鍵 | 說明 |
|--------|------|------|
| Manual | Alt+M | 開啟目前所選語言的 PDF 使用手冊 |
| About  |      | 顯示版本與應用資訊 |

## 鍵盤快捷鍵

| 快捷鍵          | 動作                           |
|-----------------|--------------------------------|
| Ctrl+S          | 儲存專案                       |
| Ctrl+Z          | 復原（混音器／修剪）           |
| Ctrl+Shift+Z    | 重做                           |
| Ctrl+O          | 開啟專案                       |
| Ctrl+M          | 混音匯出為檔案                 |
| D               | 新增鼓音軌（Tools 選單）                |
| Ctrl+P          | 專案設定                       |
| Ctrl+,          | 設定 / Configuration           |
| Alt+M           | 開啟 PDF 使用手冊（Help）      |
| Ctrl+Q / Alt+F4 | 結束應用程式                   |


### 新增鼓音軌

**Tools → Add drum track**（快速鍵 **D**）會新增 **General MIDI 第10通道**（通道索引 9）的 **MIDI** 鼓音軌。預設名稱 **Drums**（必要時加序號）。

會在**專案資料夾**立即寫入 **`.mid`**（兩小節 4/4：大鼓、小鼓、閉合嗨嗨鑔）。速度：

- 若**節拍器**啟用**錄音時啟用節拍器**，則使用該 **BPM**。
- 否則從已**啟用**且有內容的**音訊**軌估計 **BPM**；失敗則 **120 BPM**。

**參考：** [Audient](https://audient.com/tutorial/how-to-program-realistic-midi-drum-tracks/)、[MDrummer](https://www.meldaproduction.com/MDrummer)、[Reddit](https://www.reddit.com/r/ableton/comments/1e51a7g/generating_midi_drum_patterns_based_on_audio_input/)、[CS229 PDF](https://cs229.stanford.edu/proj2014/Louis%20Eugene,%20Guillaume%20Rostaing,%20Automated%20Music%20Track%20Generation.pdf)。

## Virtual MIDI Keyboard

Virtual MIDI Keyboard 是一款附屬應用程式（`virtual_midi_keyboard`），提供螢幕上的鋼琴鍵盤，用於傳送 MIDI 音符。它可以從主應用程式的 **Tools > Virtual MIDI Keyboard** 選單中啟動，也可以獨立執行。

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### 功能

- 點選螢幕上的鋼琴鍵來演奏音符
- 使用電腦鍵盤作為鋼琴鍵盤（請參閱下方的按鍵對應表）
- 連接至外部 MIDI 輸出裝置或使用內建的 FluidSynth 合成器
- 連接至 MIDI 輸入裝置，在鍵盤上顯示傳入的音符
- 可調整的合成器音量（主增益，10%--200%）
- 為內建合成器選擇 SoundFont
- MIDI 樂器/音色選擇（General MIDI 或 SoundFont 預設音色）
- 合唱/效果控制旋鈕
- 八度移調（-3 至 +5）
- MIDI 音量控制（CC#7，0--127）

### 工具列控制項

- **Volume**：MIDI 音量（CC#7），可透過微調方塊從 0 調整到 127。
- **Octave**：使用 **<** 和 **>** 按鈕或微調方塊來移動鍵盤八度。範圍為 -3 至 +5。
- **Chorus/Effect**：旋轉旋鈕和文字欄位（1--127），用於設定合唱/效果等級（MIDI CC#93）。點選 **Apply** 傳送該值。
- **MIDI 通道**：數字輸入欄位（1--16）用於選擇傳送音符、音色切換和控制變化的 MIDI 通道。預設為通道 1。變更後所選的通道會立即生效。當某個通道被選取時挑選 SoundFont 預設音色，會將該 bank/program 套用到該通道，因此您可以在不同通道上演奏不同的聲音。
- **鼓組**：位於 MIDI 通道選擇器正上方的核取方塊。未勾選時（預設），MIDI 通道為使用者所選的通道。勾選後，MIDI 通道會自動切換到通道 10 — General MIDI 標準的鼓組通道 — 因此所演奏的任何內容都會以打擊樂器的方式發聲。取消勾選則會還原為啟用鼓組之前所選的通道。
- **樂器選擇器**：選擇 MIDI 樂器。使用內建 FluidSynth 合成器時，會顯示 SoundFont 預設音色（Bank:Program Name）。連接至外部 MIDI 裝置時，會列出 128 種 General MIDI 樂器。

### 使用電腦鍵盤演奏

電腦鍵盤對應至跨越兩個八度的鋼琴鍵：

**低八度（從目前八度開始）：**

| 按鍵 | 音符  |
|------|-------|
| Z    | C     |
| S    | C#/Db |
| X    | D     |
| D    | D#/Eb |
| C    | E     |
| V    | F     |
| G    | F#/Gb |
| B    | G     |
| H    | G#/Ab |
| N    | A     |
| J    | A#/Bb |
| M    | B     |

**高八度（高一個八度）：**

| 按鍵 | 音符  |
|------|-------|
| Q    | C     |
| 2    | C#/Db |
| W    | D     |
| 3    | D#/Eb |
| E    | E     |
| R    | F     |
| 5    | F#/Gb |
| T    | G     |
| 6    | G#/Ab |
| Y    | A     |
| 7    | A#/Bb |
| U    | B     |
| I    | C（下一個八度）|
| 9    | C#/Db |
| O    | D     |
| 0    | D#/Eb |
| P    | E     |

按下按鍵時，鍵盤會有視覺反應（白鍵變為淺藍色，黑鍵變深）。

### 設定

開啟 Configuration 對話框（**Settings > Configuration**，Ctrl+,）以設定 MIDI 和音訊裝置：

![Virtual MIDI Keyboard 設定](../screenshots/VMIDIKeyboard2.png)

#### MIDI 分頁

- **MIDI Output Device**：選擇外部裝置，或留空以使用內建的 FluidSynth 合成器。
- **MIDI Input Device**：選擇控制器以將音符轉發至鍵盤顯示。
- **Synthesizer Volume (Master Gain)**：調整內建合成器的輸出音量（10%--200%）。
- **SoundFont**：瀏覽至 `.sf2` SoundFont 檔案以供內建合成器使用。
- **Refresh**：重新掃描可用的 MIDI 裝置。

#### Audio 分頁

- **Audio Output Device**：選擇內建合成器的輸出裝置。

#### Language 分頁

- **語言**：選擇顯示語言。與 Musician's Canvas 相同的語言可供選擇。變更語言後，介面
  會立即更新。

### Virtual MIDI Keyboard 快捷鍵

| 快捷鍵   | 動作                 |
|----------|----------------------|
| Ctrl+,   | Configuration 對話框 |
| Ctrl+U   | 說明 / 使用資訊      |
| Ctrl+Q   | 關閉                 |

## 常見問題（FAQ）

### 如何錄製 MIDI 音軌？

1. 設定**專案目錄**（儲存時必填）。
2. 新增或選取音軌，開啟 **Options**（或點選音軌類型圖示）。
3. 將音軌類型設為 **MIDI**，關閉對話框。
4. 在 **Settings > Configuration > MIDI** 選擇 **MIDI 輸入**（硬體連接埠或虛擬連線）以及稍後播放用的 **SoundFont**（`.sf2`）。
5. **武裝（Arm）**該音軌（同一時間只能武裝一軌）。
6. 按 **Record**，倒數後在控制器上演奏，再按 **Stop**。

音符顯示在鋼琴捲簾。**File > Save Project** 會在專案資料夾寫入 `.mid` 與 `project.json`。

### 為什麼播放時 MIDI 音軌沒聲音？

播放使用設定中的 **SoundFont** 經 **FluidSynth** 發聲。請檢查 **Settings > Configuration > MIDI**（若依專案覆寫則看 **Project > Project Settings**）：必須有有效的 `.sf2` 路徑。Linux 有時會自動找到系統 SoundFont；Windows 與 macOS 通常需手動選擇 SoundFont。

### Virtual MIDI Keyboard 與 Musician's Canvas 的關係？

它們是**兩個獨立程式**。從 **Tools > Virtual MIDI Keyboard** 啟動（亦可單獨執行）。若要在錄製 MIDI 時讓螢幕鍵盤**送入** Musician's Canvas，作業系統必須把鍵盤的 **MIDI 輸出**路由到 Musician's Canvas 使用的**輸入**（例如虛擬 MIDI 線或在兩邊選一致連接埠）。不會自動連線。

### Configuration 與 Project Settings 有何不同？

**Settings > Configuration** 設定應用程式的**全域預設值**（主題、語言、MIDI/音訊裝置、SoundFont 等）。**Project > Project Settings** 只**針對目前專案**覆寫部分設定，並存在 `project.json`。欄位若維持專案預設，則使用 Configuration 的全域值。

### 為什麼拖放無法加入音訊檔？

僅在已設定**專案目錄**，且 Musician's Canvas **未在播放或錄製**時才接受拖放。支援 **`.wav`** 與 **`.flac`**；其他副檔名會略過並在對話框列出。每個檔案成為新**音訊**音軌，名稱取自檔名（不含副檔名）。

### 錄音存在哪裡？

音訊存在**專案目錄**，檔名為 **`<音軌名稱>.flac`**（`/ \ : * ? " < > |` 等字元會改成底線）。專案檔為同資料夾的 **`project.json`**。儲存專案時，MIDI 音軌還會寫出 **`<音軌名稱>.mid`**（音符亦在 `project.json`）。

### 混音時 MIDI 渲染的檔名是什麼？

**混音**或**播放**時，MIDI 會先在內部渲染成暫存 WAV。若知道專案路徑，Musician's Canvas 也會在專案資料夾寫入快取用 **`<清理後的音軌名稱>.flac`**（與其他音軌檔案相同的檔名規則）。名稱依**音軌名稱**，不是內部 ID。

### 可以同時錄兩軌嗎？

不行。同一時間只能 **Arm** **一**軌，下一筆錄音只進該軌。請**一軌一軌**疊錄（**overdub** 會在錄新軌時播放既有軌）。

### 節拍器會錄進音軌嗎？

不會。啟用時節拍器只從**系統音訊**播放給您聽，**不會**混進錄音檔。

### Windows 為何要求 ASIO？

在 Windows 上，主程式預期使用 **ASIO** 驅動以取得可靠、低延遲音訊。若出現啟動或音訊錯誤，請安裝 **ASIO4ALL** 或介面廠商提供的驅動。

### 在 macOS 上 Virtual MIDI Keyboard 在哪？

在 **`.app` 套件**中，Virtual MIDI Keyboard 執行檔會**複製到** `Musician's Canvas.app` 內（**Contents/MacOS/**），方便只散佈一個應用程式資料夾。仍可由選單 **Tools > Virtual MIDI Keyboard** 啟動。

## 疑難排解

### 沒有音訊輸出

- 檢查 Settings > Configuration > Audio 中是否選擇了正確的音訊輸出裝置。
- 在 Linux 上，確認 PipeWire 或 PulseAudio 正在執行，且輸出未被靜音。使用 `amixer` 或您桌面的音效設定來檢查音量。
- 在 Windows 上，確保已安裝 ASIO 音訊驅動程式（例如 [ASIO4ALL](https://asio4all.org/) 或音訊介面製造商提供的 ASIO 驅動程式）。Musician's Canvas 在 Windows 上需要 ASIO 才能實現低延遲音訊。

### MIDI 音軌沒有聲音

- 確保在 Settings > Configuration > MIDI 中已設定 SoundFont（`.sf2` 檔案）。
- 在 Linux 上，如果已安裝 `fluid-soundfont-gm` 套件，系統可能會自動偵測 SoundFont。
- 在 Windows 和 macOS 上，您需要手動設定 SoundFont 路徑。

### 錄音聽起來失真或音高不正確

- 當音訊輸入裝置的原生取樣率與專案設定的取樣率不同時，可能會發生此問題。Musician's Canvas 會透過重新取樣自動處理此問題，但如果問題持續存在，請嘗試將專案取樣率設定為與裝置的原生取樣率相符。
- USB 網路攝影機麥克風通常具有不常見的原生取樣率（例如 32000 Hz）。應用程式會自動偵測這些情況。
- 如果使用 Qt Multimedia 後端時遇到問題，請嘗試在 Project Settings > Audio 中切換至 PortAudio 後端。

### Virtual MIDI Keyboard 沒有音訊

- 在使用 PipeWire 的 Linux 上，確保已安裝 `libpipewire-0.3-dev` 套件（FluidSynth 合成器與 PipeWire 整合所需）。
- 檢查是否已載入 SoundFont（請參閱 Configuration 對話框中的 MIDI 分頁）。
- 確認已選擇音訊輸出裝置且系統音量未被靜音。

## 從原始碼建置

請參閱 [README](../README.md) 以取得 Linux、macOS 和 Windows 的完整建置說明，包括所有必要的相依套件。
