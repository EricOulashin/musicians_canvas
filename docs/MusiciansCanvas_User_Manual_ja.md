---
title: "Musician's Canvas ユーザーマニュアル"
subtitle: "マルチトラック音楽録音アプリケーション"
version: "0.1.1"
date: "2026-04-03"
author: "Eric Oulashin"
lang: "ja"
---

# Musician's Canvas ユーザーマニュアル

## はじめに

Musician's Canvasは、デスクトップPC向けのマルチトラック音楽録音アプリケーションです。マイクやライン入力デバイスからのオーディオ録音、キーボードやコントローラーからのMIDI録音、そしてすべてのトラックを単一のWAVまたはFLACファイルにミックスダウンする機能をサポートしています。コンパニオンアプリケーションであるVirtual MIDI Keyboardは、MIDIノートを送信するためのソフトウェアピアノキーボードを提供します。

Musician's Canvasは、デジタルオーディオワークステーション（DAW）に一般的に見られる機能を提供しながらも、使いやすさを重視して設計されています：

- マルチトラックオーディオおよびMIDI録音
- 既存トラックの同期再生によるオーバーダブ録音
- SoundFontサポート付き内蔵FluidSynth MIDIシンセサイザー
- 任意のプロジェクトサンプルレートで録音するための高品質サンプルレート変換
- 自動モノラル/ステレオデバイス検出
- プロジェクト単位の設定とプロジェクトごとのオーバーライド
- WAVまたはFLACへのミックス
- ダークテーマとライトテーマ
- 18言語にローカライズ（English、Deutsch、Español、Français、Japanese、Português、Chinese、Русский、Svenska、Suomi、Dansk、Norsk、Polski、Ελληνικά、Gaeilge、Cymraeg、Pirate）
- コンパニオンアプリケーション Virtual MIDI Keyboard

## 使い方

### アプリケーションの起動

ビルドディレクトリまたはインストール先から`musicians_canvas`実行ファイルを起動します：

```
./musicians_canvas
```

初回起動時、アプリケーションは空のプロジェクトで開きます。録音を開始する前にプロジェクトディレクトリを設定する必要があります。

起動時、アプリケーションは保存済みのテーマ（ダークまたはライト）を適用し、以前使用したプロジェクトディレクトリに`project.json`ファイルが含まれている場合、プロジェクトは自動的に読み込まれます。

### プロジェクトのセットアップ

1. **プロジェクトディレクトリの設定**: ウィンドウ上部の「Project Location」フィールドにフォルダのパスを入力するか、参照して選択します。ここに録音ファイルとプロジェクトファイルが保存されます。

2. **トラックの追加**: **+ Add Track**ボタンをクリックします。デフォルト設定で新しいトラックが表示されます。プロジェクト内の唯一のトラックでまだ録音されていない場合、自動的に録音待機状態（アーム）になります。

3. **トラックの命名**: 「Options」ボタンの横にあるテキストフィールドに名前を入力します。この名前は録音されたオーディオファイルのファイル名として使用されます。

![メインウィンドウ](../screenshots/i18n/ja/MusiciansCanvas_1_MainWin.png)

### ボタンバー

メニューバーのすぐ下に、クイックアクセスボタンを備えたツールバーがあります：

![ボタンバー](../screenshots/i18n/ja/ButtonBar.png)

- **Open Project**: **File > Open Project** と同じ — 以前に保存したプロジェクトを開きます。
- **Save Project**: **File > Save Project** と同じ — 現在のプロジェクトを保存します。このボタンは、プロジェクトディレクトリが設定されている場合にのみ有効になります。
- **Project Settings**: **Project > Project Settings** と同じ — プロジェクト設定ダイアログを開きます。このボタンは、プロジェクトディレクトリが設定されている場合にのみ有効になります。
- **Configuration**: **Settings > Configuration** と同じ — グローバルアプリケーション設定ダイアログを開きます。
- **Metronome Settings**: メトロノーム設定ダイアログを開きます（下記のメトロノームセクションを参照）。

### プロジェクトの保存と読み込み

- **保存**: **File > Save Project** (Ctrl+S) を使用して、現在のプロジェクトをプロジェクトディレクトリにJSONファイルとして保存します。
- **読み込み**: **File > Open Project** (Ctrl+O) を使用して、以前保存したプロジェクトを読み込みます。

プロジェクトファイル（`project.json`）には、トラック名、タイプ、MIDIノート、オーディオファイル参照、およびすべてのプロジェクト固有の設定が保存されます。オーディオファイルは`project.json`と同じディレクトリに保存され、トラック名にちなんで命名されます（例：`My_Track.flac`）。

未保存の変更がある状態でアプリケーションを閉じると、終了前に保存するかどうかを確認するダイアログが表示されます。

## トラック管理

### トラックの追加と削除

- **+ Add Track**をクリックして、アレンジメントに新しいトラックを追加します。
- トラック行の右側にある**x**ボタンをクリックして、トラックを削除します。
- **Clear Tracks**（ツールバーの赤いボタン）をクリックして、すべてのトラックを削除します。実行前に確認ダイアログが表示されます。

### ドラッグ＆ドロップでトラックを追加

プロジェクトが開いている間、ファイルマネージャー（Windows エクスプローラー、
macOS Finder、Linux のファイルマネージャーなど）から 1 つ以上のサポートされた
オーディオファイルを Musician's Canvas のウィンドウに直接ドラッグして、新しい
オーディオトラックとして追加できます。

- **対応フォーマット：** `.wav` および `.flac`。他のフォーマットのファイルは
  自動的にスキップされ、最後にどのファイルがスキップされたかを示すダイアログ
  が表示されます。
- **ファイルのコピー：** ドロップされたファイルがまだプロジェクトディレクトリ
  内にない場合は、自動的にそこへコピーされます。同じ名前のファイルが既に
  プロジェクトディレクトリ内に存在する場合は、置き換えるかどうか尋ねられます。
- **トラック名：** ファイルのベース名（拡張子なし）が新しいトラックの名前として
  使用されます。たとえば、`Bass Line.wav` をドロップすると「Bass Line」という
  名前のオーディオトラックが作成されます。
- **複数ファイルの一括ドロップ：** 複数のファイルをまとめてドラッグできます。
  1 回のドロップで、サポートされた各ファイルがそれぞれのトラックになります。
- **ドロップが拒否される場合：** ドロップはプロジェクトが開いていて、かつ
  Musician's Canvas が再生または録音中で**ない**場合にのみ受け付けられます。
  さらにトラックをドラッグで追加したい場合は、先に再生または録音を停止して
  ください。

### トラックタイプの設定

各トラックは**Audio**（マイク/ライン入力録音用）または**MIDI**（キーボード/コントローラー録音用）のいずれかに設定できます。

トラックタイプを変更するには：

- トラックの**Options**ボタンをクリックするか、
- **トラックタイプアイコン**（「Options」と名前フィールドの間）をクリックします

これによりトラック設定ダイアログが開き、入力ソースを選択できます。

![トラック設定](../screenshots/i18n/ja/MusiciansCanvas_2_TrackConfig.png)

### トラックコントロール

各トラック行には以下のコントロールがあります：

- **有効チェックボックス**: トラックを再生およびミキシングに含めるかどうかを切り替えます。トラックを無効にすると、アーム状態であった場合は自動的にアーム解除されます。
- **アームラジオボタン**: このトラックを録音対象として選択します。一度にアームできるトラックは1つだけです。新しいトラックをアームすると、以前アームされていたトラックは自動的にアーム解除されます。
- **名前フィールド**: トラック名の編集可能なテキストフィールドです。この名前はオーディオファイル名として使用されます（ファイルシステムで無効な文字はアンダースコアに置換されます）。
- **Optionsボタン**: トラック設定ダイアログを開きます。
- **タイプアイコン**: オーディオトラックにはスピーカーアイコン、MIDIトラックにはピアノアイコンが表示されます。クリックするとトラック設定ダイアログが開きます。
- **削除ボタン (x)**: プロジェクトからトラックを削除します。

### 自動アーム

プロジェクトにトラックが1つだけあり、そのトラックがまだ録音されていない場合、自動的に録音待機状態（アーム）になります。これは、新しいプロジェクトに最初のトラックを追加した場合と、単一の空トラックを持つ既存プロジェクトを開いた場合の両方に適用されます。

### トラックの表示

- **オーディオトラック**は、録音されたオーディオの波形を表示します。オーディオが録音されていない場合、エリアには「No audio recorded」と表示されます。
- **MIDIトラック**は、A0からC8にわたるグリッド上に録音されたノートを表示するピアノロールビジュアライゼーションを表示します。ノートはベロシティに応じて色分けされます。MIDIデータが録音されていない場合、エリアには「No MIDI data recorded」と表示されます。

## 録音

### オーディオ録音

1. プロジェクトディレクトリが設定されていることを確認します。
2. 対象トラックをアームします（「Arm」ラジオボタンをチェック）。
3. **Record**ボタン（赤い丸）をクリックします。
4. トラック上に3秒のカウントダウンが表示され（「Get ready... 3」、「2」、「1」）、その後録音が開始されます。
5. 録音中、トラックの波形エリアにライブレベルメーターが表示され、現在の振幅がグラデーションバー（緑から黄色から赤へ）と「Recording」ラベルで示されます。
6. **Stop**ボタンをクリックして録音を終了します。

録音されたオーディオは、トラック名にちなんだFLACファイルとしてプロジェクトディレクトリに保存されます。

録音中および再生中は、誤操作を防ぐためにすべてのインタラクティブコントロール（トラックボタン、設定など）が無効になります。

### インサートエフェクト（オーディオトラックのみ）

オーディオトラックには **Options** の直下に **エフェクト** ボタンがあります。**トラックエフェクト**
ダイアログで、そのトラックの録音用 **順序付きエフェクトチェーン** を組み立てられます。

![トラックエフェクト](../screenshots/i18n/ja/Track_Effects_Dialog.png)

**リバーブ**、**コーラス**、**フランジャー**、**オーバードライブ／ディストーション**、**アンプ＆キャビネット**（アンプ種別、キャビネット、ゲイン、低／中／高域、エアー、ミックスなどのモデリング）に加えて、**ビブラート（トレモロ）**も追加できます。

**ビブラート（トレモロ）**は、Fenderアンプで“Vibrato”と呼ばれることの多い **音量の周期的な揺れ（トレモロ）** です。**Speed** で揺れの速さ、**Intensity**（Depth）で揺れの深さ（控えめな揺らぎ〜強い刻み）を調整します。

**アンプ＆キャビネット** の画面例：

![アンプ＆キャビネット・モデリング (1)](../screenshots/i18n/ja/Amp_And_Cabinet_Model_1.png)

![アンプ＆キャビネット・モデリング (2)](../screenshots/i18n/ja/Amp_And_Cabinet_Model_2.png)

- **エフェクトを追加…** からエフェクトの種類を選びます。複数追加でき、ヘッダーの赤い **✕** で削除します。
- **≡** をドラッグして **並べ替え** します。**上** のエフェクトが **先** にかかります。
- ミリ秒・ヘルツ表示は **プロジェクトのサンプリングレート** への変換後も意味が保たれます。**モノラル** と **ステレオ** の両方に対応（モノラルはデュアルモノで処理され再び1chにミックス）。
- **OK** でプロジェクトに反映、**キャンセル** でダイアログを開いたときのチェーンに戻します。

エフェクトは **録音停止時** に、通常のキャプチャとSRCの後に適用されます。設定は `project.json` の `audioEffectChain` に保存されます。

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

### 録音中のモニター

**タイム表示** の右にある **録音中にオーディオをモニター** で、録音中に **プロジェクトのオーディオ出力** へ **ライブ入力** を送るかどうかを切り替えます。

- **オーディオトラック**: 入力デバイスの音をリアルタイムで出力します（録音処理は従来どおり）。**オーバーダブ** で他トラックが鳴っている場合はその上に重なります。
- **MIDIトラック**: プロジェクトで **再生用にMIDIをオーディオへレンダリング** が有効で **SoundFont** があるとき、入力MIDIをソフトシンスで鳴らします。**外部MIDI出力** のみの場合は機器側のモニターを使います。

設定は **プロジェクト** に保存されます（`project.json` の `monitorWhileRecording`）。マイクフィードバックを避けたい場合はオフにしてください。

#### オーバーダブ録音

他の有効なトラックにすでにオーディオまたはMIDIデータが含まれている状態で新しいトラックを録音する場合、Musician's Canvasはオーバーダブ録音を実行します。既存のトラックがミックスされ、新しいトラックの録音中にリアルタイムで再生されます。これにより、新しいパートを録音しながら以前に録音したパートを聴くことができます。

既存トラックのミックスはキャプチャ開始前に準備されるため、録音と再生はほぼ同時に開始され、すべてのトラックが同期されます。

#### 録音バックエンド

Musician's Canvasは2つのオーディオキャプチャバックエンドをサポートしています：

- **PortAudio**（利用可能な場合のデフォルト）：信頼性の高い低レイテンシー録音を提供し、推奨バックエンドです。
- **Qt Multimedia**：Qtの内蔵オーディオキャプチャを使用するフォールバックバックエンドです。PortAudioが利用できない場合、またはProject Settingsで明示的に選択された場合に使用されます。

録音バックエンドは、**Project > Project Settings > Audio**でプロジェクトごとに設定できます。

#### サンプルレートとデバイスの処理

Musician's Canvasは、オーディオ入力デバイスのネイティブサンプルレートで録音し、高品質リサンプリングを使用してプロジェクトの設定サンプルレートに自動変換します。これにより、デバイスのネイティブレートに関係なく、任意のプロジェクトサンプルレート（例：44100 Hzまたは48000 Hz）を設定できます。変換ではピッチとデュレーションが正確に保持されます。

#### モノラルデバイスの検出

一部のオーディオデバイス（例：USBウェブカメラマイク）は物理的にモノラルですが、オペレーティングシステムによってステレオとして認識されます。Musician's Canvasはこれを自動的に検出し、チャンネル数を適切に調整します。プロジェクトがステレオに設定されている場合、モノラル信号は両チャンネルに複製されます。

### MIDI録音

1. Optionsボタンからトラックタイプを**MIDI**に設定します。
2. **Settings > Configuration > MIDI**でMIDI入力デバイスが設定されていることを確認します。
3. トラックをアームしてRecordをクリックします。
4. MIDIコントローラーで演奏します。
5. Stopをクリックして録音を終了します。

MIDIノートはトラック上のピアノロールビジュアライゼーションに表示されます。

## メトロノーム

Musician's Canvasには、録音中にテンポを保つのに役立つ内蔵メトロノームが含まれています。メニューバーの下にあるボタンバーのメトロノームボタンをクリックすると、メトロノーム設定ダイアログが開きます：

![メトロノーム設定](../screenshots/i18n/ja/MetronomeSettings.png)

ダイアログには次の項目があります：

- **Enable metronome during recording**: チェックを入れると、録音中にメトロノームがティック音を鳴らします。ティックはシステムオーディオを通じて再生され、録音されたトラックには**キャプチャされません**。
- **Beats per minute**: テンポを1分あたりのビート数（BPM）で指定する数値入力です。範囲は20〜300 BPMです。

メトロノームが有効な場合、録音が実際に開始されると（3秒のカウントダウンが完了した後）にティックが始まり、録音が終了すると停止します。

## 再生

**Play**ボタンをクリックすると、すべての有効なトラックがミックスされて再生されます。ボタンのツールチップは、トラックがアームされているかどうかに基づいて、再生か録音かを示すように変化します。無効なトラック（チェック解除）は再生から除外されます。

再生中、オーディオトラックはFLACファイルからデコードされ、MIDIトラックは内蔵のFluidSynthシンセサイザーを使用してオーディオにレンダリングされます。すべてのトラックがミックスされ、システムのオーディオ出力デバイスを通じて再生されます。

**Stop**ボタンをクリックすると、いつでも再生を終了できます。

## ファイルへのミックス

**Tools > Mix tracks to file** (Ctrl+M) を使用して、すべての有効なトラックを単一のオーディオファイルにエクスポートします。ダイアログで出力パスとフォーマットを選択できます：

- **出力ファイル**: 出力先のファイルパスを参照して選択します。
- **フォーマット**: FLAC（ロスレス圧縮、小さいファイルサイズ）またはWAV（非圧縮）から選択します。

ミックスにはプロジェクトの設定サンプルレートが使用されます。MIDIトラックは設定されたSoundFontを使用してレンダリングされます。

## 設定

### グローバル設定

**Settings > Configuration** (Ctrl+,) を使用して、すべてのプロジェクトに適用されるグローバルデフォルトを設定します：

![一般設定](../screenshots/i18n/ja/MusiciansCanvas_3_GeneralSettings.png)

#### Generalタブ

- **Theme**: ダークテーマとライトテーマから選択します。
- **録音デバッグログの書き込み**: 有効にすると、Musician's Canvasは録音に関する詳細な
  診断情報をプロジェクトディレクトリ内の`recording_debug.txt`というファイルに書き込みます。
  これにはキャプチャバックエンド、デバイス名、サンプルレート、チャンネル数、タイミング
  データが含まれます。この設定はデフォルトで無効であり、主に録音の問題をトラブル
  シューティングする際に役立ちます。

#### Displayタブ

- **数値表示LEDの色**: メインウィンドウのツールバーに表示される数値LED時刻表示に使用
  する色を選択します。アクティブな桁のセグメントは選択された色で描画され、非アクティブ
  なセグメントは同じ色の暗いバージョンで描画されます。利用可能な色は Light Red、Dark
  Red、Light Green、Dark Green、Light Blue、Dark Blue、Yellow、Orange、Light Cyan、
  Dark Cyan です。デフォルトは Light Green です。

![ディスプレイ設定](../screenshots/i18n/ja/MusiciansCanvas_DisplaySettings.png)

#### Languageタブ

- **言語**: アプリケーションの表示言語を選択します。デフォルトは「System Default」で、
  オペレーティングシステムの言語設定を使用します。利用可能な言語は English、Deutsch
  (German)、Español (Spanish)、Français (French)、Japanese、Português (Brazilian
  Portuguese)、Chinese (Traditional)、Chinese (Simplified)、Dansk (Danish)、Ελληνικά
  (Greek)、Gaeilge (Irish)、Norsk (Norwegian)、Polski (Polish)、Русский (Russian)、Suomi
  (Finnish)、Svenska (Swedish)、Cymraeg (Welsh)、Pirate です。言語を変更すると、
  インターフェースは即座に更新されます。

![言語設定](../screenshots/i18n/ja/MusiciansCanvas_LanguageSettings.png)

#### MIDIタブ

- **MIDI Output Device**: 内蔵FluidSynthシンセサイザーまたは外部MIDIデバイスを選択します。利用可能なMIDIデバイスを再スキャンするには**Refresh**ボタンを使用します。
- **SoundFont**: MIDIシンセシス用の`.sf2` SoundFontファイルを参照します。Linuxでは、`fluid-soundfont-gm`パッケージがインストールされている場合、システムSoundFontが自動検出されることがあります。WindowsとmacOSでは、SoundFontパスを手動で設定する必要があります。

![MIDI設定](../screenshots/i18n/ja/MusiciansCanvas_4_MIDISettings.png)

#### Audioタブ

- **Audio Input Device**: 録音用のマイクまたはライン入力デバイスを選択します。
- **Audio Output Device**: 再生用のスピーカーまたはヘッドフォンデバイスを選択します。

![オーディオ設定](../screenshots/i18n/ja/MusiciansCanvas_5_AudioSettings.png)

### プロジェクト設定

**Project > Project Settings** (Ctrl+P) を使用して、現在のプロジェクトのみのグローバルデフォルトをオーバーライドします。特定のサンプルレート、SoundFont、またはオーディオデバイスが必要なプロジェクトに便利です。プロジェクト固有の設定は`project.json`ファイル内に保存されます。

![プロジェクトMIDI設定](../screenshots/i18n/ja/MusiciansCanvas_6_ProjectMIDISettings.png)

#### MIDIタブ

- **MIDI Device**: このプロジェクト用のMIDIデバイスを選択するか、グローバル設定を使用する場合はデフォルトのままにします。
- **SoundFont**: このプロジェクト用のSoundFontファイルを選択します。
- **Refresh**: 利用可能なMIDIデバイスを再スキャンします。

#### Audioタブ

- **Audio Input Device**: このプロジェクト用の録音デバイスを選択します。
- **Recording capture backend**（PortAudioが利用可能な場合）：
  - **PortAudio (native input)**: 推奨。Audacityと同じオーディオライブラリを使用します。
  - **Qt Multimedia**: Qtの内蔵オーディオキャプチャを使用するフォールバックオプションです。
- **PortAudio Input Device**: PortAudioバックエンド使用時に、特定のPortAudio入力デバイスを選択します。
- **Audio Output Device**: このプロジェクト用の再生デバイスを選択します。

##### オーディオフォーマット設定

- **Sample rate**: 標準レート（8000 Hzから192000 Hz）から選択します。デバイスのネイティブレートには「(native)」とラベル表示されます。リサンプリングが必要なレートには「(resampled)」とラベル表示されます。デバイスの機能に関係なく任意のレートを選択できます。Musician's Canvasは必要に応じて自動的にリサンプリングを行います。
- **Channels**: モノラルまたはステレオ。デバイスがモノラルのみをサポートしている場合、ステレオオプションは無効になります。

![プロジェクトオーディオ設定](../screenshots/i18n/ja/MusiciansCanvas_7_ProjectAudioSettings.png)

#### Mix Effects tab

The **Mix Effects** tab is a scrollable list with the same controls as **Track effects** (**Add effect…**, drag **≡** to reorder, **✕** to remove). Processing order is **top to bottom** on the **combined** mix of all enabled tracks. These effects run during **whole-project playback** and when **mixing to a single WAV or FLAC file**; they are **not** baked into individual track files on disk. An empty list leaves the mixed signal unchanged aside from the mixer's own level handling.

#### Aux / Send Bus tab

Configure the **shared aux effect chain** (same effect types as track inserts). Each track’s **Aux** slider on the track row controls how much of that track’s signal is sent through this bus; the wet aux return is summed with the dry mix **before** **Mix Effects** are applied.

## メニュー

### Fileメニュー

| メニュー項目   | ショートカット | 説明                           |
|---------------|----------|---------------------------------|
| Save Project  | Ctrl+S   | 現在のプロジェクトをディスクに保存 |
| Open Project  | Ctrl+O   | 既存のプロジェクトファイルを開く   |
| Close         | Ctrl+Q   | アプリケーションを終了            |

### Projectメニュー

| メニュー項目                         | ショートカット | 説明                                    |
|-------------------------------------|----------|------------------------------------------|
| Project Settings                    | Ctrl+P   | プロジェクト設定（**Aux / Send Bus** を含む） |
| Tempo map                           |          | テンポ変化（秒と BPM）の編集：メトロノームと MIDI クオンタイズ |
| Add Demo Data to Selected Track     |          | デモ用のサンプルMIDIノートを追加          |

### Editメニュー

| メニュー項目 | ショートカット | 説明 |
|-------------|----------------|------|
| Undo | Ctrl+Z | ミキサー／トリムの変更を元に戻す |
| Redo | Ctrl+Shift+Z | やり直し |

### Settingsメニュー

| メニュー項目   | ショートカット | 説明                              |
|---------------|----------|-----------------------------------|
| Configuration | Ctrl+,   | グローバルアプリケーション設定を開く  |

### Toolsメニュー

| メニュー項目            | ショートカット | 説明                                    |
|-----------------------|----------|------------------------------------------|
| Mix tracks to file    | Ctrl+M   | すべての有効なトラックをファイルにエクスポート |
| Export stems to folder |          | トラックごとに WAV（ゲイン／パン／トリム、マスター Mix Effects なし） |
| Recording options     |          | オーディオの **パンチイン**；プロジェクト全体の **ループ再生** |
| Quantize MIDI         |          | MIDI ノート頭をグリッドにスナップ（全 MIDI またはアーム中のみ） |
| Add drum track        | D        | MIDIドラムトラックを追加し `.mid` を書き出す（下記） |
| Virtual MIDI Keyboard |          | コンパニオンキーボードアプリを起動          |

### Helpメニュー

| メニュー項目 | ショートカット | 説明 |
|------------|--------------|------|
| Manual     | Alt+M        | 現在選択されている言語のPDFマニュアルを開く |
| About      |              | バージョン情報とアプリ情報を表示 |

## キーボードショートカット

| ショートカット     | アクション                       |
|-----------------|--------------------------------|
| Ctrl+S          | プロジェクトを保存                |
| Ctrl+Z          | 元に戻す（ミキサー／トリム）     |
| Ctrl+Shift+Z    | やり直し                       |
| Ctrl+O          | プロジェクトを開く                |
| Ctrl+M          | トラックをファイルにミックス        |
| D               | ドラムトラックを追加（Toolsメニュー）      |
| Ctrl+P          | プロジェクト設定                  |
| Ctrl+,          | 設定 / Configuration             |
| Alt+M           | PDF マニュアルを開く（Help） |
| Ctrl+Q / Alt+F4 | 終了                            |


### ドラムトラックを追加

**Tools → Add drum track**（ショートカット **D**）は **General MIDI チャンネル10**（内部インデックス9）の **MIDI** ドラムトラックを追加します。既定名は **Drums**（重複時は番号付き）。

**プロジェクトフォルダ**に **`.mid`** をすぐ書き込みます（2小節・4/4、キック・スネア・クローズハイハット）。テンポは次のとおりです。

- **メトロノーム**で **録音中にメトロノームを有効にする** がオンのとき、その **BPM** を使用します。
- オフのときは、**有効なオーディオ**トラックから **BPM を推定**し、不十分な場合は **120 BPM** です。

**注意:** 推定は簡易的です。

**参考:** [Audient](https://audient.com/tutorial/how-to-program-realistic-midi-drum-tracks/)、[MDrummer](https://www.meldaproduction.com/MDrummer)、[Reddit](https://www.reddit.com/r/ableton/comments/1e51a7g/generating_midi_drum_patterns_based_on_audio_input/)、[CS229 PDF](https://cs229.stanford.edu/proj2014/Louis%20Eugene,%20Guillaume%20Rostaing,%20Automated%20Music%20Track%20Generation.pdf)。

## Virtual MIDI Keyboard

Virtual MIDI Keyboardは、MIDIノートを送信するための画面上のピアノキーボードを提供するコンパニオンアプリケーション（`virtual_midi_keyboard`）です。メインアプリケーションの**Tools > Virtual MIDI Keyboard**メニューから起動するか、独立して実行できます。

![Virtual MIDI Keyboard](../screenshots/i18n/ja/VMIDIKeyboard1.png)

### 機能

- 画面上のピアノのキーをクリックしてノートを演奏
- コンピューターキーボードをピアノキーボードとして使用（以下のキーマッピングを参照）
- 外部MIDI出力デバイスまたは内蔵FluidSynthシンセサイザーに接続
- MIDI入力デバイスに接続して、受信ノートをキーボード上に表示
- シンセサイザー音量の調整（マスターゲイン、10%--200%）
- 内蔵シンセサイザー用のSoundFont選択
- MIDIインストゥルメント/プログラム選択（General MIDIまたはSoundFontプリセット）
- コーラス/エフェクトコントロールノブ
- オクターブシフト（-3から+5）
- MIDIボリュームコントロール（CC#7、0--127）

### ツールバーコントロール

- **Volume**: MIDIボリューム（CC#7）、スピンボックスで0から127まで調整可能です。
- **Octave**: **<**および**>**ボタンまたはスピンボックスを使用して、キーボードのオクターブをシフトします。範囲は-3から+5です。
- **Chorus/Effect**: コーラス/エフェクトレベル（MIDI CC#93）を設定するためのロータリーノブとテキストフィールド（1--127）です。**Apply**をクリックして値を送信します。
- **MIDI チャンネル**: 数値入力（1--16）で、ノート、プログラムチェンジ、およびコントロールチェンジの送信先となる MIDI チャンネルを選択します。デフォルトはチャンネル 1 です。選択されたチャンネルは変更後すぐに有効になります。特定のチャンネルが選択されている状態で SoundFont プリセットを選ぶと、その bank/program がそのチャンネルに適用されるため、異なるチャンネルで異なる音色を演奏できます。
- **ドラム**: MIDI チャンネルセレクターのすぐ上にあるチェックボックスです。チェックが外れているとき（デフォルト）、MIDI チャンネルはユーザーが選択したチャンネルになります。チェックを入れると、MIDI チャンネルは自動的にチャンネル 10（General MIDI 標準のドラムチャンネル）に切り替わり、演奏したものがすべてパーカッションとして鳴ります。チェックを外すと、ドラムを有効にする前に選択していたチャンネルに戻ります。
- **インストゥルメントセレクター**: MIDIインストゥルメントを選択します。内蔵FluidSynthシンセサイザー使用時はSoundFontプリセット（Bank:Program Name）が表示されます。外部MIDIデバイスに接続している場合は、128種類のGeneral MIDIインストゥルメントが一覧表示されます。

### コンピューターキーボードでの演奏

コンピューターキーボードは2オクターブにわたってピアノキーにマッピングされています：

**下のオクターブ（現在のオクターブから開始）：**

| キー | ノート  |
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

**上のオクターブ（1オクターブ上）：**

| キー | ノート  |
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
| I   | C（次のオクターブ） |
| 9   | C#/Db |
| O   | D     |
| 0   | D#/Eb |
| P   | E     |

キーを押すと視覚的にハイライトされます（白鍵は水色に、黒鍵は暗くなります）。

### 設定

設定ダイアログ（**Settings > Configuration**、Ctrl+,）を開いて、MIDIおよびオーディオデバイスをセットアップします：

![Virtual MIDI Keyboard設定](../screenshots/i18n/ja/VMIDIKeyboard2.png)

#### MIDIタブ

- **MIDI Output Device**: 外部デバイスを選択するか、内蔵FluidSynthシンセサイザーを使用する場合は空のままにします。
- **MIDI Input Device**: キーボードディスプレイにノートを転送するコントローラーを選択します。
- **Synthesizer Volume (Master Gain)**: 内蔵シンセサイザーの出力レベルを調整します（10%--200%）。
- **SoundFont**: 内蔵シンセサイザー用の`.sf2` SoundFontファイルを参照します。
- **Refresh**: 利用可能なMIDIデバイスを再スキャンします。

#### Audioタブ

- **Audio Output Device**: 内蔵シンセサイザーの出力デバイスを選択します。

#### Languageタブ

- **言語**: 表示言語を選択します。Musician's Canvasと同じ言語が利用可能です。言語を
  変更すると、インターフェースは即座に更新されます。

### Virtual MIDI Keyboardショートカット

| ショートカット | アクション              |
|----------|------------------------|
| Ctrl+,   | 設定ダイアログ           |
| Ctrl+U   | ヘルプ / 使用方法        |
| Ctrl+Q   | 閉じる                  |

## FAQ（よくある質問）

### MIDIトラックを録音するには？

1. **プロジェクトフォルダ**を指定します（保存に必須）。
2. トラックを追加または選択し、**Options**を開きます（またはトラックタイプのアイコンをクリック）。
3. トラックタイプを**MIDI**にしてダイアログを閉じます。
4. **Settings > Configuration > MIDI**で**MIDI入力**（ハードウェアポートまたは仮想ケーブル）と、後で再生するための**SoundFont**（`.sf2`）を選びます。
5. そのトラックを**Arm**します（同時にArmできるのは1トラックだけです）。
6. **Record**をクリック、カウントダウン後にコントローラで演奏し、**Stop**で終了します。

ノートはピアノロールに表示されます。**File > Save Project**でプロジェクトフォルダに`.mid`と`project.json`が書き出されます。

### 再生するとMIDIトラックが無音なのはなぜ？

再生は設定の**SoundFont**を使った**FluidSynth**で行われます。**Settings > Configuration > MIDI**（プロジェクト単位の上書きがある場合は**Project > Project Settings**）で、有効な`.sf2`パスが設定されているか確認してください。LinuxではシステムSoundFontが自動検出されることがあります。WindowsとmacOSでは手動でSoundFontを指定することが多いです。

### Virtual MIDI KeyboardとMusician's Canvasの関係は？

**別アプリケーション**です。**Tools > Virtual MIDI Keyboard**から起動するか、単体で実行します。画面上のキーボードからMusician's CanvasのMIDIトラックへ録音するには、OSがキーボードの**MIDI出力**をMusician's Canvasが使う**入力**にルーティングする必要があります（仮想MIDIケーブルや、両方でポートを合わせるなど）。自動接続はされません。

### ConfigurationとProject Settingsの違いは？

**Settings > Configuration**はアプリ全体の**既定値**（テーマ、言語、MIDI/オーディオデバイス、SoundFontなど）です。**Project > Project Settings**は**現在のプロジェクトだけ**上書きし、`project.json`に保存されます。プロジェクト側がデフォルトのままなら、Configurationのグローバル値が使われます。

### ドラッグ＆ドロップでオーディオが追加されないのはなぜ？

**プロジェクトフォルダが設定されており**、かつ**再生・録音中でない**ときだけ受け付けます。対応形式は**`.wav`**と**`.flac`**だけです。それ以外はスキップされ、ダイアログに一覧表示されます。各ファイルは拡張子を除いた名前の新しい**オーディオ**トラックになります。

### 録音データはどこに保存されますか？

オーディオは**プロジェクトフォルダ**に**`<トラック名>.flac`**として保存されます（`/ \ : * ? " < > |`などはアンダースコアに置き換え）。プロジェクトファイルは同じフォルダの**`project.json`**です。MIDIトラックはプロジェクト保存時に**`<トラック名>.mid`**も書き出されます（ノート情報は`project.json`にも含まれます）。

### ミキス時にMIDIをレンダリングするときのファイル名は？

**ミキス**や**再生**では、まず内部で一時WAVにレンダリングします。プロジェクトパスが分かっている場合、プロジェクトフォルダに**`<サニタイズしたトラック名>.flac`**という**キャッシュ用FLAC**も書き込みます（他のトラックファイルと同じサニタイズ規則）。名前は**トラック名**に基づき、内部IDではありません。

### 2トラック同時に録音できますか？

できません。**Arm**できるのは常に**1トラック**だけで、次の録音の対象になります。**オーバーダブ**で既存トラックを聞きながら、1本ずつ重ねて録音します。

### メトロノームは録音に入りますか？

いいえ。有効にすると**システムオーディオ**で聞こえるだけです。録音ファイルには**ミックスされません**。

### WindowsでASIOが必要と言われるのはなぜ？

Windowsでは本体アプリが低遅延・安定したオーディオのために**ASIO**ドライバを想定しています。起動やオーディオで問題があれば**ASIO4ALL**やインターフェース製造元のドライバを入れてください。

### macOSではVirtual MIDI Keyboardはどこにありますか？

**`.app`バンドル**では、Virtual MIDI Keyboardの実行ファイルが`Musician's Canvas.app`の**中**（**Contents/MacOS/**）にコピーされ、フォルダ1つで配布しやすくなっています。メニューの**Tools > Virtual MIDI Keyboard**から起動できます。

## トラブルシューティング

### オーディオ出力がない

- Settings > Configuration > Audioで正しいオーディオ出力デバイスが選択されていることを確認してください。
- Linuxでは、PipeWireまたはPulseAudioが実行中で、出力がミュートされていないことを確認してください。`amixer`またはデスクトップのサウンド設定を使用して音量レベルを確認してください。
- Windowsでは、ASIOオーディオドライバがインストールされていることを確認してください（例：[ASIO4ALL](https://asio4all.org/)またはオーディオインターフェースのメーカー提供ASIOドライバ）。Musician's CanvasはWindows上で低レイテンシーオーディオのためにASIOを必要とします。

### MIDIトラックの音が出ない

- Settings > Configuration > MIDIでSoundFont（`.sf2`ファイル）が設定されていることを確認してください。
- Linuxでは、`fluid-soundfont-gm`パッケージがインストールされている場合、システムSoundFontが自動検出されることがあります。
- WindowsとmacOSでは、SoundFontパスを手動で設定する必要があります。

### 録音が歪んでいるまたはピッチがおかしい

- これは、オーディオ入力デバイスのネイティブサンプルレートがプロジェクトの設定レートと異なる場合に発生することがあります。Musician's Canvasはリサンプリングによって自動的にこれを処理しますが、問題が解決しない場合は、プロジェクトのサンプルレートをデバイスのネイティブレートに合わせてみてください。
- USBウェブカメラマイクは、異常なネイティブレート（例：32000 Hz）を持つことがよくあります。アプリケーションはこれらを自動的に検出します。
- Qt Multimediaバックエンドを使用していて問題が発生している場合は、Project Settings > AudioでPortAudioバックエンドに切り替えてみてください。

### Virtual MIDI Keyboardの音が出ない

- PipeWireを使用しているLinuxでは、`libpipewire-0.3-dev`パッケージがインストールされていることを確認してください（FluidSynthシンセサイザーとのPipeWire統合に必要です）。
- SoundFontが読み込まれていることを確認してください（設定ダイアログのMIDIタブを参照）。
- オーディオ出力デバイスが選択されていて、システムの音量がミュートされていないことを確認してください。

## ソースからのビルド

Linux、macOS、Windowsの完全なビルド手順（必要な依存関係すべてを含む）については、[README](../README.md)を参照してください。
