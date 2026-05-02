# Musician's Canvas

这是一款专为桌面电脑设计的、支持多轨录音的音乐应用程序。该程序使用 C++ 语言编写，并采用 Qt 作为图形用户界面（GUI）库。本项目旨在支持 Linux、Windows 和 Mac OS 三大操作系统平台。易用性是其核心的设计考量之一。本项目最初始于一项实验，旨在借助 Cursor 和 Claude（两款 AI 编程辅助工具），探索 AI 在构建此类应用程序方面的潜力；在后续的开发过程中，项目持续融合了 AI 辅助开发与传统的软件开发模式。

多年前，我在大学时制作了一个多轨音乐录音程序（同名）；这是一次
尝试制作更好版本的努力。

## 功能

- **多轨编排**：添加和移除多个轨道来作曲
- **命名轨道**：每个轨道都有一个可编辑的名称，用作录制音频的文件名
- **轨道类型**：将每个轨道配置为音频录音（从麦克风/线路输入）或 MIDI 录音（从键盘/控制器）；点击轨道类型图标可快速更改输入源
- **轨道预备录音**：勾选轨道上的"Arm"以将其选为录音目标；一次只能有一个轨道处于预备录音状态。单一未录制的轨道会自动设为预备录音以方便使用
- **录音倒计时**：录音开始前有 3 秒的视觉倒计时，让演奏者有时间准备
- **叠录**：在现有轨道启用的情况下录制新轨道时，现有轨道会被混音并实时播放，让您在录音时能够听到它们。播放和录制是同步的，以保持所有轨道对齐
- **视觉反馈**：音频轨道的音频波形显示（录音期间有实时电平表），MIDI 轨道的 MIDI 钢琴卷帘
- **内置 MIDI 合成器**：使用 FluidSynth 搭配可配置的 SoundFont 将 MIDI 轨道渲染为音频
- **混音为单一音频文件**：使用 [audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) 库（**Tools → Mix tracks to file**）将所有启用的轨道导出为单个混音文件（WAV、FLAC、MP3、Ogg Vorbis、AIFF）。编码使用同源项目的相同类（`MP3File`、`OggFile`、`AiffFile`）以及 **libsndfile**。若目标格式为 **MP3** 或 **Ogg Vorbis**，混音对话框会显示 **Encoding** 分区，用于设置比特率模式与音质（例如 MP3 的 CBR/ABR/VBR，以及 Ogg 的音质型 VBR 或近似标称比特率）；**WAV**、**FLAC**、**AIFF** 不在该对话框中提供比特率选项。
- **保存/打开项目**：将完整项目（轨道、名称、类型、MIDI 音符、音频文件引用）序列化到 JSON 文件并从中恢复，退出时可检测未保存的更改
- **项目特定设置**：按项目覆盖全局 MIDI 和音频默认值（采样率、SoundFont、MIDI 设备）
- **PortAudio 捕获（可选构建）**：当项目使用 PortAudio 构建时（`HAVE_PORTAUDIO`），录音可以使用原生 PortAudio 输入路径（精神上类似 Audacity）而非 Qt Multimedia。**Project → Project Settings → Audio** 可让您选择 **PortAudio** 或 **Qt Multimedia** 并选取 PortAudio 输入设备。若未安装 PortAudio，构建仍会成功，录音仅使用 Qt Multimedia。
- **高品质采样率转换**：以音频设备的原生速率录音，并使用加窗 sinc 插值（Kaiser 窗，~96 dB 阻带衰减）转换为项目速率，这与 Audacity / libsoxr 使用的算法系列相同。这允许以任何项目采样率（8000 Hz 至 192000 Hz）录音，不受设备原生速率限制，且无音调或时长变化。
- **自动单声道/立体声处理**：检测被标识为立体声的物理单声道设备（在 PipeWire 上的 USB 网络摄像头麦克风常见此情况），并根据需要在单声道和立体声之间转换（复制或平均），符合 Audacity 的声道路由方法
- **录音插入效果（仅音轨）**：在每个音轨的 **Options** 下方使用 **Effects** 按钮打开 **Track effects** 对话框。添加并配置 **Reverb**、**Chorus**、**Flanger**、**Overdrive / distortion** 与 **Amp & cabinet**（音箱头与箱体建模），拖动 **≡** 重排序效果链（最上方最先处理），并与项目一并保存。停止录音后将效果应用于已录片段；参数使用实际单位（ms、Hz），在捕获归一化后，单声道与立体声及常见项目采样率（8 kHz–192 kHz）下行为一致。详见[用户手册](docs/MusiciansCanvas_User_Manual_zh-CN.md)。翻译人员可在 `lupdate` 后通过 `scripts/effect_i18n.tsv`（由 `scripts/build_effect_tsv.py` 生成）与 `scripts/fill_effect_i18n.py` 批量更新效果相关字符串。
- **混音效果（总线）**：在 **Project → Project Settings → Mix Effects** 中为**整首混音**配置效果链（类型与顺序与音轨效果相同）。在**播放全部音轨**与**混音导出为文件**（WAV/FLAC）时应用；保存在 `project.json` 的 `mixEffectChain` 字段。
- **防削波输出**：内置 DSP 在转为 16 位 PCM 前进行软限制。**EffectWidget** 提供 `guardFloatSampleForInt16Pcm()` 与 `softLimitFloatSampleForInt16Pcm()` 供新处理代码使用。
- **录制时监听**：**时间显示**右侧的 **录音时监听音频** 可在录制期间将**实时输入**送往**项目的音频输出**。音轨为正在录制的信号；MIDI 在启用「为回放将 MIDI 渲染为音频」且配置 SoundFont 时经软件合成器播出。选项保存在**项目**（`project.json` 的 `monitorWhileRecording`）。可关闭以减少麦克风回授。
- **低延迟音频**：在 Windows 上，ASIO 驱动程序检测确保低延迟音频；在 Linux 上，提升进程调度优先级以降低 PipeWire / PulseAudio / ALSA 的抖动
- **添加鼓音轨**：**Tools → 添加鼓音轨**（快捷键 **D**）会添加 General MIDI 第10通道（鼓组）的 MIDI 音轨，并在项目文件夹中写入两小节的 `.mid`（底鼓、军鼓、踩镲）。若**节拍器**中启用了**录音时启用节拍器**，则使用该 BPM；否则从已启用的音频估算节奏，失败则默认 120 BPM。详见[用户手册](docs/MusiciansCanvas_User_Manual_zh-CN.md)。
- **Virtual MIDI Keyboard**：一款通过软件钢琴键盘发送 MIDI 音符的配套应用程序，具有内置 FluidSynth 合成器、可调主增益、计算机键盘到钢琴映射、乐器/程序选择、合唱/效果控制和八度移位
- **配置**：选择音频输入设备、MIDI 设备和 SoundFont 文件（全局默认值和按项目覆盖）
- **深色/浅色主题**：可通过 Settings → Configuration 配置
- **本地化**：两个应用程序都有 18 种语言的本地化：英语、德语、西班牙语、法语、日语、葡萄牙语（巴西）、繁体中文、简体中文、俄语、瑞典语、芬兰语、丹麦语、挪威语、波兰语、希腊语、爱尔兰语、威尔士语和海盗语。语言默认为操作系统设置，可在 **Settings → Configuration → Language** 中更改。使用 Qt 的标准 i18n 系统（`QTranslator` / `.ts` / `.qm`），更改语言时 UI 会立即更新。
- **用户手册**：从 Markdown 源码生成的 HTML 和 PDF 文档，提供所有支持语言的版本（请参阅 [docs/](docs/)）

## 屏幕截图

<p align="center">
	<a href="screenshots/i18n/zh_CN/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/i18n/zh_CN/MusiciansCanvas_1_MainWin.png" alt="主窗口" width="800"></a>
	<a href="screenshots/i18n/zh_CN/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/i18n/zh_CN/MusiciansCanvas_2_TrackConfig.png" alt="轨道配置" width="800"></a>
	<a href="screenshots/i18n/zh_CN/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/i18n/zh_CN/MusiciansCanvas_3_GeneralSettings.png" alt="常规设置" width="800"></a>
	<a href="screenshots/i18n/zh_CN/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/i18n/zh_CN/MusiciansCanvas_4_MIDISettings.png" alt="默认/常规 MIDI 设置" width="800"></a>
	<a href="screenshots/i18n/zh_CN/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/i18n/zh_CN/MusiciansCanvas_5_AudioSettings.png" alt="默认/常规音频设置" width="800"></a>
	<a href="screenshots/i18n/zh_CN/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/i18n/zh_CN/MusiciansCanvas_DisplaySettings.png" alt="显示设置" width="800"></a>
	<a href="screenshots/i18n/zh_CN/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/i18n/zh_CN/MusiciansCanvas_LanguageSettings.png" alt="语言设置" width="800"></a>
	<a href="screenshots/i18n/zh_CN/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/i18n/zh_CN/MusiciansCanvas_6_ProjectMIDISettings.png" alt="项目特定 MIDI 设置" width="800"></a>
	<a href="screenshots/i18n/zh_CN/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/i18n/zh_CN/MusiciansCanvas_7_ProjectAudioSettings.png" alt="项目特定音频设置" width="800"></a>
	<a href="screenshots/i18n/zh_CN/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/i18n/zh_CN/Track_Effects_Dialog.png" alt="轨道效果对话框" width="800"></a>
	<a href="screenshots/i18n/zh_CN/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/i18n/zh_CN/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/i18n/zh_CN/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/i18n/zh_CN/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/i18n/zh_CN/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/i18n/zh_CN/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/i18n/zh_CN/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/i18n/zh_CN/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## 依赖

### 编译器

需要支持 **C++17** 的编译器（GCC 8+、Clang 7+、MSVC 2017+）。

---

### audio_mixer_cpp（所有平台均需要）

混音和 FLAC 导出功能依赖
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp) 库。
在构建前，将其克隆为此仓库的兄弟目录：

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

CMake 构建期望在此项目根目录的相对路径 `../audio_mixer_cpp` 找到它。

---

### Qt6 Multimedia（可选但强烈建议）

`qt6-multimedia` 是可选的。没有它，应用程序仍可构建和运行，但
**音频录音和播放将被禁用** — 只有 MIDI 编辑和项目管理可以使用。
使用以下平台命令将其与核心 Qt6 库一起安装。

---

### PortAudio（可选）

如果已安装 PortAudio 库和头文件，CMake 会启用 **`HAVE_PORTAUDIO`** 并
将其链接到 **musicians_canvas**。除非项目在 **Project → Project Settings → Audio** 中设置为
**Qt Multimedia**，否则录音默认使用 PortAudio 路径。如果找不到 PortAudio，
构建会在没有它的情况下继续（包装器编译为存根）。

---

### MIDI 合成用 SoundFont

MIDI 轨道需要 SoundFont（`.sf2`）文件才能产生音频。没有的话，
MIDI 轨道将渲染为静音。在 **Linux** 上，内置的 FluidSynth 合成器
会自动检测安装在标准系统路径的 SoundFont
（请参阅以下包名）。在 **macOS 和 Windows** 上没有标准系统路径，
因此您必须在 **Settings → Configuration → MIDI** 中手动配置 SoundFont。

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

> `fluid-soundfont-gm` 将 `FluidR3_GM.sf2` 安装到 `/usr/share/sounds/sf2/`，
> 启动时会自动检测。`timgm6mb-soundfont` 是较小的替代方案。
>
> `libpipewire-0.3-dev` 在基于 PipeWire 的系统上需要，以便 Virtual MIDI
> Keyboard 可以在 FluidSynth 创建其音频资源之前调用 `pw_init()`。
> 没有它构建仍会继续；只是不会定义 `HAVE_PIPEWIRE` 标志。

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel libsndfile-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm` 将 `FluidR3_GM.sf2` 安装到 `/usr/share/soundfonts/`，
> 启动时会自动检测。

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac libsndfile portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid` 将 `FluidR3_GM.sf2` 安装到 `/usr/share/soundfonts/`，
> 启动时会自动检测。`pipewire` 通常已安装在现代 Arch 系统上；
> 开发头文件包含在主包中。

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac libsndfile portaudio
```

> PipeWire 是纯 Linux 子系统，在 macOS 上**不**需要。FluidSynth
> 将通过 Qt Multimedia 后端自动使用 CoreAudio。
>
> 下载 General MIDI SoundFont（例如
> [GeneralUser GS](https://schristiancollins.com/generaluser.php) 或
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)）
> 并在 **Settings → Configuration → MIDI** 中配置其路径。

---

### Windows

**musicians_canvas** 需要 **ASIO 音频驱动程序**才能进行低延迟录音和播放。
如果未检测到 ASIO 驱动程序，应用程序将无法启动。
**virtual_midi_keyboard** 不需要 ASIO，直接使用 Qt Multimedia 的 WASAPI 后端。

**为 musicians_canvas 安装 ASIO 驱动程序（择一）：**

| 驱动程序 | 备注 |
|----------|------|
| [ASIO4ALL](https://asio4all.org/) | 免费，适用于大多数内置和 USB 音频硬件 |
| 制造商驱动程序 | 专用音频接口的最佳延迟（例如 Focusrite、PreSonus、RME） |

**工具链 — MSYS2（建议用于 MinGW 构建）：**

从 <https://www.msys2.org> 下载并安装 MSYS2，然后将
`C:\msys64\mingw64\bin` 添加到系统 `PATH`。安装所需包：

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

> `mingw-w64-x86_64-soundfont-fluid` 将 `FluidR3_GM.sf2` 安装到
> `C:\msys64\mingw64\share\soundfonts\`（如果 MSYS2 安装在其他位置请调整）。
> 由于 Windows 没有标准的 SoundFont 搜索路径，您必须在首次启动后
> 在 **Settings → Configuration → MIDI** 中手动配置此路径。
>
> PipeWire 是纯 Linux 子系统；在 Windows 上不需要 PipeWire 包。

包参考页面：
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**工具链 — Visual Studio 2022：**

通过 [Qt Online Installer](https://www.qt.io/download) 安装 Qt 6，并获取
FluidSynth、RtMidi 和 libFLAC 二进制文件（或从源码构建）。
只要适当的目录在 `CMAKE_PREFIX_PATH` 中，CMake 构建将通过
`find_library` / `find_path` 找到它们。

必须另外下载 SoundFont（例如
[GeneralUser GS](https://schristiancollins.com/generaluser.php) 或
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)），
并在首次启动后于 **Settings → Configuration → MIDI** 中配置其路径。

> **注意：** `advapi32`（Windows 注册表）和 `winmm`（Windows Multimedia）由
> CMake 自动链接；不需要额外安装。

---

## 构建

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

如果在运行时找不到所需的 DLL，请从构建输出目录运行辅助脚本：

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## 生成文档

用户手册可从 Markdown 源码生成为 HTML 和 PDF：

```bash
cd docs
./generate_docs.sh          # 生成 HTML 和 PDF
./generate_docs.sh html     # 仅生成 HTML
./generate_docs.sh pdf      # 仅生成 PDF
```

**先决条件：**

- **Python（首选）：** `pip3 install markdown weasyprint`
- **备选方案：** `pandoc` 和 `wkhtmltopdf`（通过系统包管理器）

生成的 HTML 写入 `docs/html/`，PDF 写入 `docs/MusiciansCanvas_User_Manual.pdf`。

---

## 运行

```bash
./build/musicians_canvas
# 或
./build/virtual_midi_keyboard   # 配套应用程序 Virtual MIDI Keyboard
```

---

## 使用方式

### musicians_canvas

1. **设置项目目录**：输入或浏览到一个文件夹，录音和项目文件将存储在此
2. **添加轨道**：点击"+ Add Track"；在"Options"旁边的文本字段中为每个轨道命名
3. **配置轨道类型**：点击轨道上的"Options"（或点击"Options"和名称字段之间的轨道类型图标）以选择 Audio 或 MIDI 并设置输入源
4. **移除轨道**：点击轨道行右侧的"×"按钮
5. **全局设置**：使用 **Settings → Configuration** 设置全局默认值：
   - *General* 选项卡：主题（深色/浅色）
   - *MIDI* 选项卡：默认 MIDI 输出设备（内置 FluidSynth 合成器或外部 MIDI 设备）和默认 SoundFont（`.sf2`）
   - *Audio* 选项卡：用于录音和播放的默认音频输入/输出设备
6. **项目设置**：使用 **Project → Project Settings**（Ctrl+P）仅为当前项目覆盖 MIDI 和音频设置（例如每首歌不同的采样率或 SoundFont）
7. **录音**：在目标轨道上勾选"Arm"，然后点击录音按钮（红色圆圈）。一次只能有一个轨道处于预备录音状态
8. **播放**：点击播放按钮以混音并播放所有启用的轨道
9. **混音为文件**：使用 **Tools → Mix tracks to file**（Ctrl+M）导出为 WAV、FLAC、MP3、Ogg Vorbis、AIFF。**MP3** 与 **Ogg** 请在混音前于对话框的 **Encoding** 分区设置比特率或音质。
10. **保存/打开**：使用 **File → Save Project**（Ctrl+S）和 **File → Open Project**（Ctrl+O）

### virtual_midi_keyboard

1. **打开配置**：使用 **Configuration** 按钮或菜单（Ctrl+,）打开设置对话框
2. **MIDI 选项卡**：
   - 选择 MIDI 输出设备（外部硬件/软件合成器）或留空以使用内置 FluidSynth 合成器
   - 选择 MIDI 输入设备以将传入的 MIDI 音符转发到键盘显示
   - 调整 **Synthesizer Volume (Master Gain)** 以控制内置合成器的输出电平（10%–200%）
3. **Audio 选项卡**：选择内置合成器使用的音频输出设备
4. **SoundFont**：为内置合成器选择 `.sf2` SoundFont 文件（在 Linux 上若已安装系统 SoundFont 则自动检测）
5. **演奏音符**：点击屏幕上钢琴键盘的琴键、使用连接的 MIDI 控制器或使用计算机键盘：
   - 下方八度：Z/X/C/V/B/N/M = C/D/E/F/G/A/B，S/D/G/H/J = 升号/降号
   - 上方八度：Q/W/E/R/T/Y/U/I/O/P = C 到 E，2/3/5/6/7/9/0 = 升号/降号
6. **工具栏控制**：调整 MIDI 音量（0–127）、八度（-3 到 +5）、合唱/效果电平，以及选择乐器

---

## 键盘快捷键

**musicians_canvas：**

| 快捷键 | 操作 |
|--------|------|
| Ctrl+S | 保存项目 |
| Ctrl+O | 打开项目 |
| Ctrl+M | 混音轨道为文件 |
| Ctrl+P | 项目设置 |
| Ctrl+, | 设置 / 配置 |
| Ctrl+Q / Alt+F4 | 退出 |

**virtual_midi_keyboard：**

| 快捷键 | 操作 |
|--------|------|
| Ctrl+, | 配置对话框 |
| Ctrl+U | 帮助 / 使用信息 |
| Ctrl+Q | 关闭 |

---

## 许可证

本项目按原样提供，仅供教育和个人使用。
