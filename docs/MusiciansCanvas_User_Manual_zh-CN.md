---
title: "Musician's Canvas 用户手册"
subtitle: "多轨音乐录音应用程序"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "zh-CN"
---

# Musician's Canvas 用户手册

## 简介

Musician's Canvas 是一款适用于桌面电脑的多轨音乐录音应用程序。它支持通过麦克风和线路输入设备进行音频录制，通过键盘和控制器进行 MIDI 录制，并可将所有轨道混合导出为单个 WAV 或 FLAC 文件。配套应用程序 Virtual MIDI Keyboard 提供了一个软件钢琴键盘，用于发送 MIDI 音符。

Musician's Canvas 设计简单易用，同时提供数字音频工作站（DAW）中常见的功能：

- 多轨音频和 MIDI 录制
- 叠录功能，可在录制时同步播放已有轨道
- 内置 FluidSynth MIDI 合成器，支持 SoundFont
- 高质量采样率转换，支持任意项目采样率录制
- 自动检测单声道/立体声设备
- 基于项目的设置，支持按项目覆盖
- 混合导出为 WAV 或 FLAC
- 深色和浅色主题
- 支持 18 种语言本地化，包括 English、Deutsch、Español、Français、Japanese、Português、Chinese、Русский、Svenska、Suomi、Dansk、Norsk、Polski、Ελληνικά、Gaeilge、Cymraeg 和 Pirate
- 配套 Virtual MIDI Keyboard 应用程序

## 快速入门

### 启动应用程序

从构建目录或安装位置运行 `musicians_canvas` 可执行文件：

```
./musicians_canvas
```

首次启动时，应用程序会打开一个空项目。您需要先设置项目目录才能开始录制。

启动时，应用程序会应用已保存的主题（深色或浅色），如果之前使用的项目目录中包含 `project.json` 文件，则会自动加载该项目。

### 设置项目

1. **设置项目目录**：在窗口顶部的"Project Location"字段中输入或浏览到一个文件夹。录音文件和项目文件将存储在此处。

2. **添加轨道**：点击 **+ Add Track** 按钮。一个新轨道将以默认设置出现。如果该轨道是项目中唯一的轨道且尚未录制，它将自动处于录音待命状态。

3. **命名轨道**：在"Options"按钮旁边的文本字段中输入名称。此名称将用作录制音频文件的文件名。

![主窗口](../screenshots/MusiciansCanvas_1_MainWin.png)

### 按钮栏

菜单栏正下方有一个带有快速访问按钮的工具栏：

![按钮栏](../screenshots/ButtonBar.png)

- **Open Project**：与 **File > Open Project** 相同 — 打开之前保存的项目。
- **Save Project**：与 **File > Save Project** 相同 — 保存当前项目。此按钮仅在已设置项目目录时启用。
- **Project Settings**：与 **Project > Project Settings** 相同 — 打开项目设置对话框。此按钮仅在已设置项目目录时启用。
- **Configuration**：与 **Settings > Configuration** 相同 — 打开全局应用程序设置对话框。
- **Metronome Settings**：打开节拍器设置对话框（参见下方的节拍器部分）。

### 保存和打开项目

- **保存**：使用 **File > Save Project**（Ctrl+S）将当前项目保存为项目目录中的 JSON 文件。
- **打开**：使用 **File > Open Project**（Ctrl+O）加载之前保存的项目。

项目文件（`project.json`）存储轨道名称、类型、MIDI 音符、音频文件引用以及所有项目特定设置。音频文件与 `project.json` 存储在同一目录中，以其轨道名称命名（例如 `My_Track.flac`）。

如果您在有未保存更改的情况下关闭应用程序，将弹出确认对话框询问是否在退出前保存。

## 轨道管理

### 添加和删除轨道

- 点击 **+ Add Track** 向编曲中添加新轨道。
- 点击轨道行右侧的 **x** 按钮删除该轨道。
- 点击 **Clear Tracks**（工具栏中的红色按钮）删除所有轨道。执行前会显示确认对话框。

### 通过拖放添加轨道

当项目打开时，您可以从文件管理器（Windows 资源管理器、macOS Finder、
Linux 文件管理器等）将一个或多个受支持的音频文件直接拖放到 Musician's Canvas
窗口上，将它们添加为新的音频轨道。

- **支持的格式：** `.wav` 和 `.flac`。任何其他格式的文件会被静默跳过，
  结束时会有一个对话框列出哪些文件被跳过。
- **文件复制：** 如果拖放的文件尚未位于项目目录中，它会自动被复制到那里。
  如果项目目录中已经存在同名文件，系统会询问您是否要替换它。
- **轨道名称：** 文件的基本名称（不含扩展名）会被用作新轨道的名称。例如，
  拖放 `Bass Line.wav` 会创建一个名为"Bass Line"的音频轨道。
- **一次拖放多个文件：** 可以一起拖放多个文件；在单次拖放中，每个受支持的
  文件都会成为独立的轨道。
- **拖放被拒绝时：** 仅当项目打开且 Musician's Canvas **未**正在播放或
  录制时才接受拖放。如果您想拖入更多轨道，请先停止播放或录制。

### 配置轨道类型

每个轨道可以配置为 **Audio**（用于麦克风/线路输入录制）或 **MIDI**（用于键盘/控制器录制）。

要更改轨道类型：

- 点击轨道上的 **Options** 按钮，或
- 点击**轨道类型图标**（位于"Options"和名称字段之间）

这将打开轨道配置对话框，您可以在其中选择输入源。

![轨道配置](../screenshots/MusiciansCanvas_2_TrackConfig.png)

### 轨道控件

每个轨道行提供以下控件：

- **启用复选框**：切换该轨道是否包含在播放和混音中。禁用轨道也会自动取消其录音待命状态。
- **录音待命单选按钮**：选择此轨道作为录音目标。同一时间只能有一个轨道处于录音待命状态；对新轨道启用录音待命会自动取消之前已启用的轨道。
- **名称字段**：可编辑的轨道名称文本字段。此名称用作音频文件名（无效的文件系统字符会被替换为下划线）。
- **Options 按钮**：打开轨道配置对话框。
- **类型图标**：音频轨道显示扬声器图标，MIDI 轨道显示钢琴图标。点击可打开轨道配置对话框。
- **删除按钮 (x)**：从项目中删除该轨道。

### 自动录音待命

当项目中只有一个轨道且该轨道尚未录制时，它会自动进入录音待命状态。这适用于在新项目中添加第一个轨道和打开包含单个空轨道的现有项目两种情况。

### 轨道可视化

- **音频轨道**显示已录制音频的波形可视化。未录制音频时，该区域显示"No audio recorded"。
- **MIDI 轨道**显示钢琴卷帘可视化，在 A0 到 C8 的网格上显示已录制的音符。音符按力度着色。未录制 MIDI 数据时，该区域显示"No MIDI data recorded"。

## 录制

### 音频录制

1. 确保已设置项目目录。
2. 将目标轨道设为录音待命状态（选中"Arm"单选按钮）。
3. 点击 **Record** 按钮（红色圆圈）。
4. 轨道上会出现 3 秒倒计时（"Get ready... 3"、"2"、"1"），然后开始录制。
5. 录制期间，轨道的波形区域会显示实时电平表，以渐变条（绿色到黄色到红色）显示当前振幅，并带有"Recording"标签。
6. 点击 **Stop** 按钮结束录制。

录制的音频以 FLAC 文件格式保存在项目目录中，以轨道名称命名。

在录制和播放期间，所有交互控件（轨道按钮、设置等）均被禁用，以防止意外更改。

### 插入式效果（仅音频轨道）

音频轨道在 **Options** 下方有 **效果** 按钮，可打开 **轨道效果** 对话框，为该轨道的录音搭建 **有序效果链**：

![轨道效果对话框](../screenshots/Track_Effects_Dialog.png)

可用效果包括 **混响**、**合唱**、**镶边（Flanger）**、**过载／失真（Overdrive / distortion）** 与 **音箱头与箱体建模（Amp & cabinet）**（箱头类型、箱体、增益、低／中／高频、空气感与湿干比等）。以下为 **Amp & cabinet** 的界面示例：

![Amp & cabinet 建模 (1)](../screenshots/Amp_And_Cabinet_Model_1.png)

![Amp & cabinet 建模 (2)](../screenshots/Amp_And_Cabinet_Model_2.png)

- 点击 **添加效果…**，选择一种效果类型。可添加多个实例；效果标题栏上的红色 **✕** 可移除。
- 拖动 **≡** 可 **重排** 顺序：**最上方** 的效果 **最先** 处理音频。
- 控件以毫秒、赫兹为单位，在转换到 **项目采样率** 后仍然一致。支持 **单声道** 与 **立体声**（单声道在内部以双单声道处理后再混回单声道）。
- **确定** 将更改写入项目；**取消** 恢复打开对话框前的链。

效果在 **停止录制** 后应用（经过常规采集与重采样）。配置保存在 `project.json` 的 `audioEffectChain` 字段。

### Mix effects (full project)

**Project → Project Settings → Mix Effects** lets you build the same kind of ordered effect chain as **Track effects** (**Reverb**, **Chorus**, **Flanger**, **Overdrive / distortion**, **Amp & cabinet**), but applied to the **entire mixed program**: when you press **Play** to hear all enabled tracks together, and when you export with **Mix tracks to file** (toolbar or **Tools** menu). The chain is saved in `project.json` under `projectSettings` → `mixEffectChain`.

To reduce harsh [digital clipping](https://en.wikipedia.org/wiki/Clipping_%28audio%29) when processing pushes peaks toward full scale, the effect engine applies a **soft limiter** to normalized float samples immediately before conversion to 16-bit PCM. The **EffectWidget** base class documents `guardFloatSampleForInt16Pcm()` and `softLimitFloatSampleForInt16Pcm()` for any new real-time code that writes to 16-bit audio.

### 录制时监听

在 **时间显示** 右侧，**录音时监听音频** 决定在录制期间是否把 **实时输入** 送到 **项目的音频输出**：

- **音轨**：输入设备的声音会同步播放（录制流程不变）。若正在 **叠录** 播放其他轨道，会与之叠加。
- **MIDI 轨**：当启用 **为回放将 MIDI 渲染为音频** 且已配置 **SoundFont** 时，通过软件合成器听到弹奏；仅 **外部 MIDI 输出** 时请使用设备自带监听。

该选项保存在 **项目** 中（`project.json` 的 `monitorWhileRecording`）。为避免麦克风反馈可关闭。

#### 叠录

当录制新轨道时，如果其他已启用的轨道已包含音频或 MIDI 数据，Musician's Canvas 会执行叠录：已有轨道会被混合并实时播放，同时录制新轨道。这样您可以在录制新内容时听到之前录制的部分。

在采集开始前会先准备好已有轨道的混音，因此录制和播放几乎同时开始，保持所有轨道同步。

#### 录制后端

Musician's Canvas 支持两种音频采集后端：

- **PortAudio**（可用时为默认后端）：提供可靠、低延迟的录制，是推荐的后端。
- **Qt Multimedia**：使用 Qt 内置音频采集的备用后端。在 PortAudio 不可用或在项目设置中明确选择时使用。

录制后端可在 **Project > Project Settings > Audio** 中按项目配置。

#### 采样率和设备处理

Musician's Canvas 以音频输入设备的原生采样率进行录制，然后使用高质量重采样自动转换为项目配置的采样率。这意味着您可以设置任意项目采样率（例如 44100 Hz 或 48000 Hz），无论设备的原生采样率是多少。转换过程精确保持音高和时长。

#### 单声道设备检测

某些音频设备（例如 USB 网络摄像头麦克风）物理上是单声道的，但操作系统将其识别为立体声。Musician's Canvas 会自动检测此情况并相应调整声道数。如果项目配置为立体声，单声道信号将被复制到两个声道。

### MIDI 录制

1. 通过 Options 按钮将轨道类型设置为 **MIDI**。
2. 确保在 **Settings > Configuration > MIDI** 中配置了 MIDI 输入设备。
3. 将轨道设为录音待命状态并点击 Record。
4. 在 MIDI 控制器上演奏音符。
5. 点击 Stop 结束录制。

MIDI 音符以钢琴卷帘可视化的形式显示在轨道上。

## 节拍器

Musician's Canvas 内置节拍器，可在录制期间用于帮助保持节奏。点击菜单栏下方按钮栏上的节拍器按钮以打开节拍器设置对话框：

![节拍器设置](../screenshots/MetronomeSettings.png)

该对话框提供：

- **Enable metronome during recording**：勾选时，节拍器会在录制过程中播放滴答声。滴答声通过系统音频播放，**不会**被采集到录制的轨道中。
- **Beats per minute**：节拍速度的数值输入，以每分钟拍数（BPM）为单位。范围为 20–300 BPM。

当节拍器启用时，它会在录制实际开始后（3 秒倒计时结束后）开始滴答，并在录制结束时停止。

## 播放

点击 **Play** 按钮混合并播放所有已启用的轨道。按钮的工具提示会根据是否有轨道处于录音待命状态而变化，指示将执行播放还是录制。已禁用（未勾选）的轨道不参与播放。

播放期间，音频轨道从其 FLAC 文件解码，MIDI 轨道使用内置 FluidSynth 合成器渲染为音频。所有轨道混合后通过系统的音频输出设备播放。

点击 **Stop** 按钮可随时结束播放。

## 混合导出到文件

使用 **Tools > Mix tracks to file**（Ctrl+M）将所有已启用的轨道导出为单个音频文件。对话框允许您选择输出路径和格式：

- **输出文件**：浏览并选择目标文件路径。
- **格式**：在 FLAC（无损压缩，文件较小）或 WAV（未压缩）之间选择。

混音使用项目配置的采样率。MIDI 轨道使用配置的 SoundFont 进行渲染。

## 设置

### 全局设置

使用 **Settings > Configuration**（Ctrl+,）设置适用于所有项目的全局默认值：

![常规设置](../screenshots/MusiciansCanvas_3_GeneralSettings.png)

#### General 标签页

- **Theme**：在深色和浅色主题之间选择。
- **写入录制调试日志**：启用后，Musician's Canvas 会将详细的录制诊断信息写入项目目录中
  名为 `recording_debug.txt` 的文件。这包括采集后端、设备名称、采样率、声道数和计时
  数据。此选项默认禁用，主要用于排查录制问题。

#### Display 标签页

- **数字显示 LED 颜色**：选择主窗口工具栏上显示的数字 LED 时间显示所使用的颜色。
  活动的数字段以所选颜色绘制，非活动的段则以相同颜色的暗淡版本绘制。可用的颜色
  为 Light Red、Dark Red、Light Green、Dark Green、Light Blue、Dark Blue、Yellow、
  Orange、Light Cyan 和 Dark Cyan。默认值为 Light Green。

![显示设置](../screenshots/MusiciansCanvas_DisplaySettings.png)

#### Language 标签页

- **语言**：选择应用程序的显示语言。默认值为"System Default"，使用操作系统的语言
  设置。可用语言包括 English、Deutsch (German)、Español (Spanish)、Français (French)、
  Japanese、Português (Brazilian Portuguese)、Chinese (Traditional)、Chinese
  (Simplified)、Dansk (Danish)、Ελληνικά (Greek)、Gaeilge (Irish)、Norsk (Norwegian)、
  Polski (Polish)、Русский (Russian)、Suomi (Finnish)、Svenska (Swedish)、Cymraeg (Welsh)
  和 Pirate。更改语言后，界面会立即更新。

![语言设置](../screenshots/MusiciansCanvas_LanguageSettings.png)

#### MIDI 标签页

- **MIDI Output Device**：选择内置 FluidSynth 合成器或外部 MIDI 设备。使用 **Refresh** 按钮重新扫描可用的 MIDI 设备。
- **SoundFont**：浏览到 `.sf2` SoundFont 文件用于 MIDI 合成。在 Linux 上，如果安装了 `fluid-soundfont-gm` 软件包，系统可能会自动检测到 SoundFont。在 Windows 和 macOS 上，您需要手动配置 SoundFont 路径。

![MIDI 设置](../screenshots/MusiciansCanvas_4_MIDISettings.png)

#### Audio 标签页

- **Audio Input Device**：选择用于录制的麦克风或线路输入设备。
- **Audio Output Device**：选择用于播放的扬声器或耳机设备。

![音频设置](../screenshots/MusiciansCanvas_5_AudioSettings.png)

### 项目设置

使用 **Project > Project Settings**（Ctrl+P）仅为当前项目覆盖全局默认值。这对于需要特定采样率、SoundFont 或音频设备的项目非常有用。项目特定设置保存在 `project.json` 文件中。

![项目 MIDI 设置](../screenshots/MusiciansCanvas_6_ProjectMIDISettings.png)

#### MIDI 标签页

- **MIDI Device**：为此项目选择 MIDI 设备，或保留默认值以使用全局设置。
- **SoundFont**：为此项目选择 SoundFont 文件。
- **Refresh**：重新扫描可用的 MIDI 设备。

#### Audio 标签页

- **Audio Input Device**：选择此项目的录制设备。
- **Recording capture backend**（PortAudio 可用时）：
  - **PortAudio (native input)**：推荐。使用与 Audacity 相同的音频库。
  - **Qt Multimedia**：使用 Qt 内置音频采集的备用选项。
- **PortAudio Input Device**：使用 PortAudio 后端时，选择特定的 PortAudio 输入设备。
- **Audio Output Device**：选择此项目的播放设备。

##### 音频格式设置

- **Sample rate**：从标准采样率（8000 Hz 到 192000 Hz）中选择。设备的原生采样率标记为"(native)"。需要重采样的采样率标记为"(resampled)"。您可以选择任意采样率，无论设备能力如何；Musician's Canvas 会根据需要自动重采样。
- **Channels**：单声道或立体声。如果设备仅支持单声道，立体声选项将被禁用。

![项目音频设置](../screenshots/MusiciansCanvas_7_ProjectAudioSettings.png)

#### Mix Effects tab

The **Mix Effects** tab is a scrollable list with the same controls as **Track effects** (**Add effect…**, drag **≡** to reorder, **✕** to remove). Processing order is **top to bottom** on the **combined** mix of all enabled tracks. These effects run during **whole-project playback** and when **mixing to a single WAV or FLAC file**; they are **not** baked into individual track files on disk. An empty list leaves the mixed signal unchanged aside from the mixer's own level handling.

## 菜单

### File 菜单

| 菜单项        | 快捷键   | 描述                          |
|---------------|----------|---------------------------------|
| Save Project  | Ctrl+S   | 将当前项目保存到磁盘          |
| Open Project  | Ctrl+O   | 打开现有项目文件              |
| Close         | Ctrl+Q   | 退出应用程序                  |

### Project 菜单

| 菜单项                              | 快捷键   | 描述                                   |
|-------------------------------------|----------|------------------------------------------|
| Project Settings                    | Ctrl+P   | 配置项目特定设置                       |
| Add Demo Data to Selected Track     |          | 向选定轨道添加示例 MIDI 音符           |

### Settings 菜单

| 菜单项        | 快捷键   | 描述                            |
|---------------|----------|-----------------------------------|
| Configuration | Ctrl+,   | 打开全局应用程序设置              |

### Tools 菜单

| 菜单项                | 快捷键   | 描述                                   |
|-----------------------|----------|------------------------------------------|
| Mix tracks to file    | Ctrl+M   | 将所有已启用轨道导出到文件             |
| Add drum track        | D        | 添加鼓 MIDI 音轨并写入 `.mid`（见下文）   |
| Virtual MIDI Keyboard |          | 启动配套键盘应用程序                   |

## 键盘快捷键

| 快捷键          | 操作                           |
|-----------------|--------------------------------|
| Ctrl+S          | 保存项目                       |
| Ctrl+O          | 打开项目                       |
| Ctrl+M          | 混合导出轨道到文件             |
| D               | 添加鼓音轨（Tools 菜单）                |
| Ctrl+P          | 项目设置                       |
| Ctrl+,          | 设置 / 配置                    |
| Ctrl+Q / Alt+F4 | 退出                          |


### 添加鼓音轨

**Tools → Add drum track**（快捷键 **D**）会添加 **General MIDI 第10通道**（通道索引 9）的 **MIDI** 鼓轨。默认名称 **Drums**（必要时加序号）。

会在**项目文件夹**立即写入 **`.mid`**（两小节 4/4：底鼓、军鼓、闭镲）。速度选择：

- 若**节拍器**中启用**录音时启用节拍器**，则使用该 **BPM**。
- 否则从已**启用**且含内容的**音频**轨估计 **BPM**；失败则用 **120 BPM**。

**参考链接：** [Audient](https://audient.com/tutorial/how-to-program-realistic-midi-drum-tracks/)、[MDrummer](https://www.meldaproduction.com/MDrummer)、[Reddit](https://www.reddit.com/r/ableton/comments/1e51a7g/generating_midi_drum_patterns_based_on_audio_input/)、[CS229 PDF](https://cs229.stanford.edu/proj2014/Louis%20Eugene,%20Guillaume%20Rostaing,%20Automated%20Music%20Track%20Generation.pdf)。

## Virtual MIDI Keyboard

Virtual MIDI Keyboard 是一款配套应用程序（`virtual_midi_keyboard`），提供屏幕上的钢琴键盘用于发送 MIDI 音符。可以从主应用程序的 **Tools > Virtual MIDI Keyboard** 菜单启动，也可以独立运行。

![Virtual MIDI Keyboard](../screenshots/VMIDIKeyboard1.png)

### 功能特性

- 点击屏幕上的钢琴键演奏音符
- 使用电脑键盘作为钢琴键盘（参见下方按键映射）
- 连接外部 MIDI 输出设备或使用内置 FluidSynth 合成器
- 连接 MIDI 输入设备以在键盘上显示传入的音符
- 可调节合成器音量（主增益，10%--200%）
- 为内置合成器选择 SoundFont
- MIDI 乐器/音色选择（General MIDI 或 SoundFont 预设）
- 合唱/效果控制旋钮
- 八度移位（-3 到 +5）
- MIDI 音量控制（CC#7，0--127）

### 工具栏控件

- **Volume**：MIDI 音量（CC#7），可通过数值框在 0 到 127 之间调节。
- **Octave**：使用 **<** 和 **>** 按钮或数值框移动键盘八度。范围为 -3 到 +5。
- **Chorus/Effect**：旋转旋钮和文本字段（1--127），用于设置合唱/效果级别（MIDI CC#93）。点击 **Apply** 发送该值。
- **MIDI 通道**：数字输入字段（1--16）用于选择发送音符、音色切换和控制变化的 MIDI 通道。默认为通道 1。更改后所选通道会立即生效。当某个通道被选中时挑选 SoundFont 预设，会将该 bank/program 应用到该通道，因此您可以在不同通道上演奏不同的声音。
- **鼓组**：位于 MIDI 通道选择器正上方的复选框。未勾选时（默认），MIDI 通道为用户所选的通道。勾选后，MIDI 通道会自动切换到通道 10 — General MIDI 标准的鼓组通道 — 因此所演奏的任何内容都会以打击乐器的方式发声。取消勾选将恢复为启用鼓组之前所选的通道。
- **乐器选择器**：选择 MIDI 乐器。使用内置 FluidSynth 合成器时，显示 SoundFont 预设（Bank:Program Name）。连接到外部 MIDI 设备时，列出 128 种 General MIDI 乐器。

### 使用电脑键盘演奏

电脑键盘映射到两个八度的钢琴键：

**低八度（从当前八度开始）：**

| 按键 | 音符  |
|-----|-------|
| Z   | C     |
| S   | C#/Db |
| X   | D     |
| D   | D#/Eb |
| C   | E     |
| V   | F     |
| G   | F#/Gb |
| B   | G     |
| H   | G#/Ab |
| N   | A     |
| J   | A#/Bb |
| M   | B     |

**高八度（高一个八度）：**

| 按键 | 音符  |
|-----|-------|
| Q   | C     |
| 2   | C#/Db |
| W   | D     |
| 3   | D#/Eb |
| E   | E     |
| R   | F     |
| 5   | F#/Gb |
| T   | G     |
| 6   | G#/Ab |
| Y   | A     |
| 7   | A#/Bb |
| U   | B     |
| I   | C（下一个八度） |
| 9   | C#/Db |
| O   | D     |
| 0   | D#/Eb |
| P   | E     |

按下时按键会产生视觉高亮效果（白键变为浅蓝色，黑键变暗）。

### 配置

打开配置对话框（**Settings > Configuration**，Ctrl+,）以设置 MIDI 和音频设备：

![Virtual MIDI Keyboard 配置](../screenshots/VMIDIKeyboard2.png)

#### MIDI 标签页

- **MIDI Output Device**：选择外部设备或留空以使用内置 FluidSynth 合成器。
- **MIDI Input Device**：选择控制器以将音符转发到键盘显示。
- **Synthesizer Volume (Master Gain)**：调节内置合成器的输出音量（10%--200%）。
- **SoundFont**：浏览到 `.sf2` SoundFont 文件用于内置合成器。
- **Refresh**：重新扫描可用的 MIDI 设备。

#### Audio 标签页

- **Audio Output Device**：选择内置合成器的输出设备。

#### Language 标签页

- **语言**：选择显示语言。与 Musician's Canvas 相同的语言可供选择。更改语言后，界面
  会立即更新。

### Virtual MIDI Keyboard 快捷键

| 快捷键   | 操作                   |
|----------|------------------------|
| Ctrl+,   | 配置对话框             |
| Ctrl+U   | 帮助 / 使用信息        |
| Ctrl+Q   | 关闭                   |

## 常见问题（FAQ）

### 如何录制 MIDI 轨道？

1. 设置**项目目录**（保存所必需）。
2. 添加或选择轨道，打开 **Options**（或点击轨道类型图标）。
3. 将轨道类型设为 **MIDI**，关闭对话框。
4. 在 **Settings > Configuration > MIDI** 中选择 **MIDI 输入**（硬件端口或虚拟连线）以及用于稍后播放的 **SoundFont**（`.sf2`）。
5. **武装（Arm）**该轨道（同一时间只能武装一条轨道）。
6. 点击 **Record**，倒计时结束后在控制器上演奏，然后点击 **Stop**。

音符显示在钢琴卷帘中。**File > Save Project** 会在项目文件夹中写入 `.mid` 和 `project.json`。

### 为什么回放时 MIDI 轨道没有声音？

回放使用设置中的 **SoundFont** 通过 **FluidSynth** 发声。请检查 **Settings > Configuration > MIDI**（若按项目覆盖则查看 **Project > Project Settings**）：必须配置有效的 `.sf2` 路径。Linux 上有时会自动找到系统 SoundFont；Windows 和 macOS 上通常需要手动选择 SoundFont 文件。

### Virtual MIDI Keyboard 与 Musician's Canvas 是什么关系？

它们是**两个独立程序**。通过 **Tools > Virtual MIDI Keyboard** 启动（也可单独运行）。若要在录制 MIDI 轨道时让屏幕键盘**进入** Musician's Canvas，需要由操作系统将键盘的 **MIDI 输出**路由到 Musician's Canvas 使用的**输入**（例如虚拟 MIDI 线缆或在两个程序中选择一致的端口）。两者不会自动连接。

### Configuration 与 Project Settings 有何区别？

**Settings > Configuration** 设置应用程序的**全局默认值**（主题、语言、MIDI/音频设备、SoundFont 等）。**Project > Project Settings** 仅**针对当前项目**覆盖部分设置，并保存在 `project.json` 中。若某字段保持项目默认，则使用 Configuration 中的全局值。

### 为什么拖放无法添加音频文件？

仅在已设置**项目目录**，且 Musician's Canvas **未在播放或录制**时才能接受拖放。支持的格式为 **`.wav`** 和 **`.flac`**；其他扩展名会被跳过并在对话框中列出。每个文件会成为一个新的**音频**轨道，名称取自文件名（不含扩展名）。

### 录音保存在哪里？

音频保存在**项目目录**中，文件名为 **`<轨道名>.flac`**（`/ \ : * ? " < > |` 等字符会替换为下划线）。项目文件为同目录下的 **`project.json`**。保存项目时，MIDI 轨道还会生成 **`<轨道名>.mid`**（音符数据也会写入 `project.json`）。

### 混音时渲染 MIDI 使用什么文件名？

**混音**或**播放**时，MIDI 会先在内部渲染为临时 WAV。若已知项目路径，Musician's Canvas 还会在项目文件夹中写入缓存用的 **`<清理后的轨道名>.flac`**（与其他轨道文件使用相同的文件名清理规则）。名称依据**轨道名**，而非内部 ID。

### 可以同时录制两条轨道吗？

不可以。同一时间只能 **Arm** **一条**轨道，下一段录音只录到该轨道。通过**逐条叠加**录制来制作（**叠录 overdub** 会在录制新轨道时播放已有轨道）。

### 节拍器会录进轨道吗？

不会。启用后节拍器仅通过**系统音频**播放给您听，**不会**混入录音文件。

### Windows 上为何提示需要 ASIO？

在 Windows 上，主程序期望使用 **ASIO** 驱动以实现可靠、低延迟的音频。若出现启动或音频错误，请安装 **ASIO4ALL** 或声卡/接口厂商提供的驱动。

### 在 macOS 上 Virtual MIDI Keyboard 在哪里？

在 **`.app` 应用程序包**中，Virtual MIDI Keyboard 可执行文件会被**复制到** `Musician's Canvas.app` 内部（**Contents/MacOS/**），便于只分发一个应用文件夹。仍可通过菜单 **Tools > Virtual MIDI Keyboard** 启动。

## 故障排除

### 无音频输出

- 检查是否在 Settings > Configuration > Audio 中选择了正确的音频输出设备。
- 在 Linux 上，验证 PipeWire 或 PulseAudio 正在运行且输出未静音。使用 `amixer` 或桌面环境的声音设置检查音量级别。
- 在 Windows 上，确保已安装 ASIO 音频驱动程序（例如 [ASIO4ALL](https://asio4all.org/) 或音频接口制造商提供的 ASIO 驱动程序）。Musician's Canvas 在 Windows 上需要 ASIO 才能实现低延迟音频。

### MIDI 轨道无声

- 确保在 Settings > Configuration > MIDI 中配置了 SoundFont（`.sf2` 文件）。
- 在 Linux 上，如果安装了 `fluid-soundfont-gm` 软件包，系统可能会自动检测到 SoundFont。
- 在 Windows 和 macOS 上，您需要手动配置 SoundFont 路径。

### 录制声音失真或音高错误

- 当音频输入设备的原生采样率与项目配置的采样率不同时，可能会发生此问题。Musician's Canvas 通过重采样自动处理此情况，但如果问题持续存在，请尝试将项目采样率设置为与设备的原生采样率匹配。
- USB 网络摄像头麦克风通常具有不常见的原生采样率（例如 32000 Hz）。应用程序会自动检测这些情况。
- 如果使用 Qt Multimedia 后端时遇到问题，请尝试在 Project Settings > Audio 中切换到 PortAudio 后端。

### Virtual MIDI Keyboard 无音频

- 在使用 PipeWire 的 Linux 上，确保已安装 `libpipewire-0.3-dev` 软件包（FluidSynth 合成器与 PipeWire 集成所需）。
- 检查是否已加载 SoundFont（参见配置对话框中的 MIDI 标签页）。
- 验证是否已选择音频输出设备且系统音量未静音。

## 从源代码构建

有关 Linux、macOS 和 Windows 的完整构建说明（包括所有必需的依赖项），请参阅 [README](../README.md)。
