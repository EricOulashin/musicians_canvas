# Musician's Canvas

これはデスクトップPC向けのマルチトラック音楽録音アプリケーションです。C++で書かれており、GUIライブラリとしてQtを使用しています。このアプリケーションはLinux、Windows、Mac OS向けに構築されることを意図しています。使いやすさが主要な設計上の考慮事項です。このプロジェクトは、Cursor（AIコーディングツール）を使った実験として始まりました。このようなアプリケーションとしてどのようなものを作成できるかを確認するためで、AI開発の継続的な使用と従来のソフトウェア開発を組み合わせています。

何年も前に、大学でマルチトラック音楽録音プログラムを作成しました（同じ名前で）。これはより良いものを作る試みです。

## 機能

- **マルチトラックアレンジメント**：曲を作成するために複数のトラックを追加・削除
- **名前付きトラック**：各トラックには、録音されたオーディオのファイル名として使用される編集可能な名前があります
- **トラックタイプ**：各トラックをオーディオ録音（マイク/ライン入力から）またはMIDI録音（キーボード/コントローラーから）に設定。トラックタイプアイコンをクリックして入力ソースを素早く変更できます
- **トラックのアーム**：トラックの「Arm」をチェックして録音ターゲットとして選択。一度にアームできるトラックは1つだけです。未録音のトラックが1つだけの場合、自動的にアームされます
- **録音カウントダウン**：録音開始前の3秒間のビジュアルカウントダウンで、演奏者に準備時間を与えます
- **オーバーダブ録音**：既存のトラックが有効な状態で新しいトラックを録音すると、既存のトラックがミックスされてリアルタイムで再生されるため、録音中に聴くことができます。再生とキャプチャは同期され、すべてのトラックが揃った状態を維持します
- **ビジュアルフィードバック**：オーディオトラック用のオーディオ波形表示（録音中のライブレベルメーター付き）、MIDIトラック用のMIDIピアノロール
- **内蔵MIDIシンセサイザー**：設定可能なSoundFontを使用してFluidSynthでMIDIトラックをオーディオにレンダリング
- **単一オーディオファイルへのミックス**：[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)ライブラリを使用して、有効なすべてのトラックを単一のミックスされたWAVまたはFLACファイルにエクスポート
- **プロジェクトの保存/開く**：完全なプロジェクト（トラック、名前、タイプ、MIDIノート、オーディオファイル参照）をJSONファイルにシリアライズおよび復元。終了時に未保存の変更を検出
- **プロジェクト固有の設定**：プロジェクトごとにグローバルMIDIおよびオーディオのデフォルトを上書き（サンプルレート、SoundFont、MIDIデバイス）
- **PortAudioキャプチャ（オプションビルド）**：プロジェクトがPortAudioでビルドされている場合（`HAVE_PORTAUDIO`）、録音はQt Multimediaの代わりにネイティブPortAudio入力パスを使用できます（Audacityと同様の精神）。**Project → Project Settings → Audio** で **PortAudio** または **Qt Multimedia** を選択し、PortAudio入力デバイスを選択できます。PortAudioがインストールされていない場合でもビルドは成功し、録音はQt Multimediaのみを使用します。
- **高品質サンプルレート変換**：オーディオデバイスのネイティブレートで録音し、ウィンドウ付きsinc補間（Kaiser窓、~96 dBの阻止帯域減衰）を使用してプロジェクトレートに変換します。これはAudacity / libsoxrが使用するのと同じアルゴリズムファミリーです。これにより、デバイスのネイティブレートに関係なく、任意のプロジェクトサンプルレート（8000 Hzから192000 Hz）で録音でき、ピッチや長さの変更はありません。
- **自動モノ/ステレオ処理**：ステレオとして広告されている物理的にモノのデバイスを検出し（PipeWireでのUSBウェブカムマイクによく見られる）、必要に応じてモノとステレオ間で変換（複製または平均化）します。Audacityのチャンネルルーティングアプローチに対応
- **録音インサートエフェクト（オーディオトラック）**：各オーディオトラックの **Options** の下にある **Effects** ボタンで **Track effects** ダイアログを開きます。**Reverb**、**Chorus**、**Flanger**、**Overdrive / distortion**、**Amp & cabinet**（アンプ＆キャビネット・モデリング）を追加・設定し、**≡** をドラッグしてチェーンを並べ替え（上が先）、プロジェクトに保存します。エフェクトは録音停止時にテイクへ適用されます。パラメータは実単位（ms、Hz）で、キャプチャ正規化後もモノ／ステレオおよび一般的なプロジェクトサンプルレート（8 kHz～192 kHz）で一貫した挙動になります。詳細は[ユーザーマニュアル](docs/MusiciansCanvas_User_Manual_ja.md)を参照。翻訳者は `lupdate` 後に `scripts/effect_i18n.tsv`（`scripts/build_effect_tsv.py` で生成）と `scripts/fill_effect_i18n.py` で一括更新できます。
- **ミックスエフェクト（マスターバス）**：**Project → Project Settings → Mix Effects** で**全体のミックス**にエフェクトチェーンを設定します（トラックと同じ種類と順序）。**全トラック再生**と**ミックス書き出し**（WAV/FLAC）に適用。`project.json` の `mixEffectChain` に保存されます。
- **クリッピング対策**：組み込み DSP が 16 ビット PCM 変換前にソフトリミットします。**EffectWidget** の `guardFloatSampleForInt16Pcm()` と `softLimitFloatSampleForInt16Pcm()` を新規処理で利用できます。
- **録音中のモニター**：数値時刻表示の右の **録音中にオーディオをモニター** で、キャプチャ中にライブ入力を**プロジェクトのオーディオ出力**へ送るか切り替えます。オーディオトラックは録音と同じ信号、MIDI は「再生用に MIDI をオーディオへレンダリング」と SoundFont 利用時にソフトシンスで聴取。設定は**プロジェクト**に保存（`monitorWhileRecording`）。フィードバック回避のためオフにできます。
- **低レイテンシーオーディオ**：WindowsではASIOドライバー検出により低レイテンシーオーディオを実現。LinuxではPipeWire / PulseAudio / ALSAでのジッター低減のためにプロセススケジューリング優先度を上げます
- **ドラムトラックを追加**：**Tools → ドラムトラックを追加**（ショートカット **D**）で General MIDI チャンネル10（ドラム）の MIDI トラックを追加し、2小節の `.mid`（キック、スネア、ハイハット）をプロジェクトフォルダに書き込みます。**メトロノーム**で **録音中にメトロノームを有効にする** がオンのときはその BPM を使用し、オフのときは有効なオーディオから推定、失敗時は 120 BPM です。詳細は[ユーザーマニュアル](docs/MusiciansCanvas_User_Manual_ja.md)を参照。
- **Virtual MIDI Keyboard**：ソフトウェアピアノキーボードを介してMIDIノートを送信するコンパニオンアプリケーション。内蔵FluidSynthシンセサイザー、調整可能なマスターゲイン、コンピューターキーボードからピアノへのマッピング、楽器/プログラム選択、コーラス/エフェクト制御、オクターブシフト機能付き
- **設定**：オーディオ入力デバイス、MIDIデバイス、SoundFontファイルの選択（グローバルデフォルトとプロジェクトごとの上書き）
- **ダーク/ライトテーマ**：Settings → Configurationで設定可能
- **ローカライゼーション**：両アプリケーションは18言語にローカライズ：英語、ドイツ語、スペイン語、フランス語、日本語、ポルトガル語（ブラジル）、繁体字中国語、簡体字中国語、ロシア語、スウェーデン語、フィンランド語、デンマーク語、ノルウェー語、ポーランド語、ギリシャ語、アイルランド語、ウェールズ語、海賊語。言語はOSの設定がデフォルトで、**Settings → Configuration → Language** で変更できます。Qtの標準i18nシステム（`QTranslator` / `.ts` / `.qm`）が使用され、言語を変更するとUIが即座に更新されます。
- **ユーザーマニュアル**：Markdownソースから生成されたHTMLおよびPDFドキュメント。サポートされているすべての言語で利用可能（[docs/](docs/)を参照）

## スクリーンショット

<p align="center">
	<a href="screenshots/i18n/ja/MusiciansCanvas_1_MainWin.png" target='_blank'><img src="screenshots/i18n/ja/MusiciansCanvas_1_MainWin.png" alt="メインウィンドウ" width="800"></a>
	<a href="screenshots/i18n/ja/MusiciansCanvas_2_TrackConfig.png" target='_blank'><img src="screenshots/i18n/ja/MusiciansCanvas_2_TrackConfig.png" alt="トラック設定" width="800"></a>
	<a href="screenshots/i18n/ja/MusiciansCanvas_3_GeneralSettings.png" target='_blank'><img src="screenshots/i18n/ja/MusiciansCanvas_3_GeneralSettings.png" alt="一般設定" width="800"></a>
	<a href="screenshots/i18n/ja/MusiciansCanvas_4_MIDISettings.png" target='_blank'><img src="screenshots/i18n/ja/MusiciansCanvas_4_MIDISettings.png" alt="デフォルト/一般MIDI設定" width="800"></a>
	<a href="screenshots/i18n/ja/MusiciansCanvas_5_AudioSettings.png" target='_blank'><img src="screenshots/i18n/ja/MusiciansCanvas_5_AudioSettings.png" alt="デフォルト/一般オーディオ設定" width="800"></a>
	<a href="screenshots/i18n/ja/MusiciansCanvas_DisplaySettings.png" target='_blank'><img src="screenshots/i18n/ja/MusiciansCanvas_DisplaySettings.png" alt="表示設定" width="800"></a>
	<a href="screenshots/i18n/ja/MusiciansCanvas_LanguageSettings.png" target='_blank'><img src="screenshots/i18n/ja/MusiciansCanvas_LanguageSettings.png" alt="言語設定" width="800"></a>
	<a href="screenshots/i18n/ja/MusiciansCanvas_6_ProjectMIDISettings.png" target='_blank'><img src="screenshots/i18n/ja/MusiciansCanvas_6_ProjectMIDISettings.png" alt="プロジェクト固有のMIDI設定" width="800"></a>
	<a href="screenshots/i18n/ja/MusiciansCanvas_7_ProjectAudioSettings.png" target='_blank'><img src="screenshots/i18n/ja/MusiciansCanvas_7_ProjectAudioSettings.png" alt="プロジェクト固有のオーディオ設定" width="800"></a>
	<a href="screenshots/i18n/ja/Track_Effects_Dialog.png" target='_blank'><img src="screenshots/i18n/ja/Track_Effects_Dialog.png" alt="トラックエフェクト" width="800"></a>
	<a href="screenshots/i18n/ja/Amp_And_Cabinet_Model_1.png" target='_blank'><img src="screenshots/i18n/ja/Amp_And_Cabinet_Model_1.png" alt="Amp & cabinet modeling (1)" width="800"></a>
	<a href="screenshots/i18n/ja/Amp_And_Cabinet_Model_2.png" target='_blank'><img src="screenshots/i18n/ja/Amp_And_Cabinet_Model_2.png" alt="Amp & cabinet modeling (2)" width="800"></a>
	<a href="screenshots/i18n/ja/VMIDIKeyboard1.png" target='_blank'><img src="screenshots/i18n/ja/VMIDIKeyboard1.png" alt="Virtual MIDI Keyboard" width="800"></a>
	<a href="screenshots/i18n/ja/VMIDIKeyboard2.png" target='_blank'><img src="screenshots/i18n/ja/VMIDIKeyboard2.png" alt="Virtual MIDI Keyboard" width="800"></a>
</p>

## 依存関係

### コンパイラ

**C++17**対応のコンパイラが必要です（GCC 8+、Clang 7+、MSVC 2017+）。

---

### audio_mixer_cpp（全プラットフォームで必須）

ミキシングおよびFLACエクスポート機能は
[audio_mixer_cpp](https://github.com/EricOulashin/audio_mixer_cpp)ライブラリに依存します。
ビルド前に、このリポジトリの兄弟ディレクトリとしてクローンしてください：

```bash
git clone https://github.com/EricOulashin/audio_mixer_cpp.git
```

CMakeビルドは、このプロジェクトのルートからの相対パス `../audio_mixer_cpp` で見つけることを期待します。

---

### Qt6 Multimedia（オプションだが強く推奨）

`qt6-multimedia`はオプションです。なくてもアプリケーションはビルドおよび実行されますが、
**オーディオ録音と再生は無効になります** — MIDI編集とプロジェクト管理のみが動作します。
以下のプラットフォームコマンドを使用して、コアQt6ライブラリと一緒にインストールしてください。

---

### PortAudio（オプション）

PortAudioライブラリとヘッダーがインストールされている場合、CMakeは**`HAVE_PORTAUDIO`**を有効にし、
**musicians_canvas**にリンクします。プロジェクトが**Project → Project Settings → Audio**で**Qt Multimedia**に設定されていない限り、
録音はデフォルトでPortAudioパスを使用します。PortAudioが見つからない場合、ビルドはそれなしで続行されます（ラッパーはスタブとしてコンパイルされます）。

---

### MIDI合成用SoundFont

MIDIトラックがオーディオを生成するにはSoundFont（`.sf2`）ファイルが必要です。ファイルがない場合、
MIDIトラックは無音としてレンダリングされます。**Linux**では、内蔵のFluidSynthシンセサイザーが
標準的なシステムパスにSoundFontがインストールされている場合は自動的に検出します
（以下のパッケージ名を参照）。**macOSとWindows**には標準的なシステムパスがないため、
**Settings → Configuration → MIDI**でSoundFontを手動で設定する必要があります。

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

> `fluid-soundfont-gm`は`FluidR3_GM.sf2`を`/usr/share/sounds/sf2/`にインストールし、
> 起動時に自動検出されます。`timgm6mb-soundfont`はより小さな代替パッケージです。
>
> `libpipewire-0.3-dev`はPipeWireベースのシステムで必要で、Virtual MIDI
> KeyboardがFluidSynthのオーディオリソース作成前に`pw_init()`を呼び出せるようにします。
> なくてもビルドは続行されます。`HAVE_PIPEWIRE`フラグが定義されないだけです。

### Linux — Fedora

```bash
sudo dnf install cmake gcc-c++ \
  qt6-qtbase-devel qt6-qtmultimedia-devel \
  fluidsynth-devel rtmidi-devel flac-devel \
  portaudio-devel \
  pipewire-devel \
  fluid-soundfont-gm
```

> `fluid-soundfont-gm`は`FluidR3_GM.sf2`を`/usr/share/soundfonts/`にインストールし、
> 起動時に自動検出されます。

### Linux — Arch Linux

```bash
sudo pacman -S base-devel cmake \
  qt6-base qt6-multimedia \
  fluidsynth rtmidi flac portaudio \
  pipewire \
  soundfont-fluid
```

> `soundfont-fluid`は`FluidR3_GM.sf2`を`/usr/share/soundfonts/`にインストールし、
> 起動時に自動検出されます。`pipewire`は通常、最新のArchシステムにはすでにインストールされています。
> 開発ヘッダーはメインパッケージに含まれています。

### macOS

```bash
brew install cmake qt fluidsynth rtmidi flac portaudio
```

> PipeWireはLinux専用のサブシステムであり、macOSでは**不要**です。FluidSynthは
> Qt MultimediaバックエンドでCoreAudioを自動的に使用します。
>
> General MIDI SoundFontをダウンロードしてください（例：
> [GeneralUser GS](https://schristiancollins.com/generaluser.php)または
> [FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)）。
> そのパスを**Settings → Configuration → MIDI**で設定してください。

---

### Windows

**musicians_canvas**は低レイテンシーの録音と再生のために**ASIOオーディオドライバー**が必要です。
ASIOドライバーが検出されない場合、アプリケーションは起動しません。
**virtual_midi_keyboard**はASIOを必要とせず、Qt MultimediaのWASAPIバックエンドを直接使用します。

**musicians_canvas用ASIOドライバーのインストール（1つ選択）：**

| ドライバー | 備考 |
|------------|------|
| [ASIO4ALL](https://asio4all.org/) | 無料、ほとんどの内蔵およびUSBオーディオハードウェアで動作 |
| メーカードライバー | 専用オーディオインターフェース向けの最高のレイテンシー（例：Focusrite、PreSonus、RME） |

**ツールチェーン — MSYS2（MinGWビルドに推奨）：**

<https://www.msys2.org>からMSYS2をダウンロードしてインストールし、
`C:\msys64\mingw64\bin`をシステムの`PATH`に追加してください。必要なパッケージをインストールしてください：

```bash
pacman -S mingw-w64-x86_64-qt6-base
pacman -S mingw-w64-x86_64-qt6-multimedia
pacman -S mingw-w64-x86_64-fluidsynth
pacman -S mingw-w64-ucrt-x86_64-rtmidi
pacman -S mingw-w64-x86_64-flac
pacman -S mingw-w64-x86_64-portaudio
pacman -S mingw-w64-x86_64-soundfont-fluid
```

> `mingw-w64-x86_64-soundfont-fluid`は`FluidR3_GM.sf2`を
> `C:\msys64\mingw64\share\soundfonts\`にインストールします（MSYS2が別の場所にインストールされている場合は調整してください）。
> Windowsには標準的なSoundFont検索パスがないため、初回起動後に
> **Settings → Configuration → MIDI**でこのパスを手動で設定する必要があります。
>
> PipeWireはLinux専用のサブシステムです。WindowsではPipeWireパッケージは不要です。

パッケージ参照ページ：
- <https://packages.msys2.org/packages/mingw-w64-x86_64-fluidsynth>
- <https://packages.msys2.org/packages/mingw-w64-ucrt-x86_64-rtmidi>
- <https://packages.msys2.org/packages/mingw-w64-x86_64-soundfont-fluid>

**ツールチェーン — Visual Studio 2022：**

[Qt Online Installer](https://www.qt.io/download)でQt 6をインストールし、
FluidSynth、RtMidi、libFLACのバイナリを入手してください（またはソースからビルド）。
適切なディレクトリが`CMAKE_PREFIX_PATH`にある限り、
CMakeビルドは`find_library` / `find_path`で検索します。

SoundFontは別途ダウンロードする必要があります（例：
[GeneralUser GS](https://schristiancollins.com/generaluser.php)または
[FluidR3_GM](https://member.keymusician.com/Member/FluidR3_GM/index.html)）。
初回起動後に**Settings → Configuration → MIDI**でパスを設定してください。

> **注意：** `advapi32`（Windowsレジストリ）と`winmm`（Windows Multimedia）は
> CMakeによって自動的にリンクされます。追加のインストールは不要です。

---

## ビルド

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

必要なDLLが実行時に見つからない場合、ビルド出力ディレクトリからヘルパースクリプトを実行してください：

```cmd
..\..\copyRequiredWinDLLs.bat
```

---

## ドキュメント生成

ユーザーマニュアルはMarkdownソースからHTMLおよびPDFとして生成できます：

```bash
cd docs
./generate_docs.sh          # HTMLとPDFの両方を生成
./generate_docs.sh html     # HTMLのみ生成
./generate_docs.sh pdf      # PDFのみ生成
```

**前提条件：**

- **Python（推奨）：** `pip3 install markdown weasyprint`
- **代替手段：** `pandoc`と`wkhtmltopdf`（システムパッケージマネージャーで入手）

生成されたHTMLは`docs/html/`に、PDFは`docs/MusiciansCanvas_User_Manual.pdf`に出力されます。

---

## 実行

```bash
./build/musicians_canvas
# または
./build/virtual_midi_keyboard   # コンパニオンアプリ Virtual MIDI Keyboard
```

---

## 使い方

### musicians_canvas

1. **プロジェクトディレクトリを設定**：録音とプロジェクトファイルを保存するフォルダを入力または参照
2. **トラックを追加**：「+ Add Track」をクリック。「Options」の横のテキストフィールドで各トラックに名前を付ける
3. **トラックタイプを設定**：トラックの「Options」をクリック（または「Options」と名前フィールドの間のトラックタイプアイコンをクリック）してAudioまたはMIDIを選択し、入力ソースを設定
4. **トラックを削除**：トラック行の右側にある「×」ボタンをクリック
5. **グローバル設定**：**Settings → Configuration**を使用してグローバルデフォルトを設定：
   - *General*タブ：テーマ（ダーク/ライト）
   - *MIDI*タブ：デフォルトMIDI出力デバイス（内蔵FluidSynthシンセサイザーまたは外部MIDIデバイス）とデフォルトSoundFont（`.sf2`）
   - *Audio*タブ：録音と再生のデフォルトオーディオ入出力デバイス
6. **プロジェクト設定**：**Project → Project Settings**（Ctrl+P）を使用して、現在のプロジェクトのMIDIおよびオーディオ設定のみを上書き（例：曲ごとに異なるサンプルレートやSoundFont）
7. **録音**：ターゲットトラックで「Arm」をチェックし、録音ボタン（赤い丸）をクリック。一度にアームできるトラックは1つだけです
8. **再生**：再生ボタンをクリックして、有効なすべてのトラックをミックスして再生
9. **ファイルにミックス**：**Tools → Mix tracks to file**（Ctrl+M）を使用してWAVまたはFLACにエクスポート
10. **保存/開く**：**File → Save Project**（Ctrl+S）と**File → Open Project**（Ctrl+O）を使用

### virtual_midi_keyboard

1. **設定を開く**：**Configuration**ボタンまたはメニュー（Ctrl+,）を使用して設定ダイアログを開く
2. **MIDIタブ**：
   - MIDI出力デバイスを選択（外部ハードウェア/ソフトウェアシンセサイザー）するか、空白のままにして内蔵FluidSynthシンセサイザーを使用
   - MIDI入力デバイスを選択して、受信MIDIノートをキーボード表示に転送
   - **Synthesizer Volume (Master Gain)**を調整して内蔵シンセサイザーの出力レベルを制御（10%〜200%）
3. **Audioタブ**：内蔵シンセサイザーが使用するオーディオ出力デバイスを選択
4. **SoundFont**：内蔵シンセサイザー用の`.sf2` SoundFontファイルを選択（LinuxでシステムSoundFontがインストールされている場合は自動検出）
5. **ノートを演奏**：画面上のピアノキーボードのキーをクリック、接続されたMIDIコントローラーを使用、またはコンピューターキーボードを使用：
   - 下のオクターブ：Z/X/C/V/B/N/M = C/D/E/F/G/A/B、S/D/G/H/J = シャープ/フラット
   - 上のオクターブ：Q/W/E/R/T/Y/U/I/O/P = CからE、2/3/5/6/7/9/0 = シャープ/フラット
6. **ツールバーコントロール**：MIDIボリューム（0〜127）、オクターブ（-3〜+5）、コーラス/エフェクトレベルの調整、楽器の選択

---

## キーボードショートカット

**musicians_canvas：**

| ショートカット | アクション |
|----------------|------------|
| Ctrl+S | プロジェクトを保存 |
| Ctrl+O | プロジェクトを開く |
| Ctrl+M | トラックをファイルにミックス |
| Ctrl+P | プロジェクト設定 |
| Ctrl+, | 設定 / コンフィグレーション |
| Ctrl+Q / Alt+F4 | 終了 |

**virtual_midi_keyboard：**

| ショートカット | アクション |
|----------------|------------|
| Ctrl+, | 設定ダイアログ |
| Ctrl+U | ヘルプ / 使用方法 |
| Ctrl+Q | 閉じる |

---

## ライセンス

このプロジェクトは教育目的および個人使用のためにそのまま提供されます。
