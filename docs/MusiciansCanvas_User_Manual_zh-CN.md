---
title: "Musician's Canvas 用户手册"
subtitle: "多轨音乐录音应用程序"
version: "0.1.0"
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
| Virtual MIDI Keyboard |          | 启动配套键盘应用程序                   |

## 键盘快捷键

| 快捷键          | 操作                           |
|-----------------|--------------------------------|
| Ctrl+S          | 保存项目                       |
| Ctrl+O          | 打开项目                       |
| Ctrl+M          | 混合导出轨道到文件             |
| Ctrl+P          | 项目设置                       |
| Ctrl+,          | 设置 / 配置                    |
| Ctrl+Q / Alt+F4 | 退出                          |

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

### Virtual MIDI Keyboard 快捷键

| 快捷键   | 操作                   |
|----------|------------------------|
| Ctrl+,   | 配置对话框             |
| Ctrl+U   | 帮助 / 使用信息        |
| Ctrl+Q   | 关闭                   |

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
