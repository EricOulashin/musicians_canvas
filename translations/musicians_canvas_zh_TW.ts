<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_TW">
<context>
    <name>AudioStartup</name>
    <message>
        <location filename="../src/audiostartup.cpp" line="45"/>
        <source>No ASIO audio drivers were detected on this system.

ASIO (Audio Stream Input/Output) drivers are required for the low-latency audio recording and playback this application relies on.

Please install an ASIO driver and then restart the application:

  •  ASIO4ALL (free, compatible with most audio hardware):
       https://asio4all.org/

  •  Your audio interface manufacturer&apos;s own ASIO driver
       (provides the best latency for dedicated hardware)</source>
        <translation>在此系統上未偵測到 ASIO 音訊驅動程式。

本應用程式需要 ASIO（Audio Stream Input/Output）驅動程式來進行低延遲的錄音與播放。

請安裝 ASIO 驅動程式後重新啟動應用程式：

  •  ASIO4ALL（免費，相容大多數音訊硬體）：
       https://asio4all.org/

  •  您的音訊介面製造商提供的 ASIO 驅動程式
       （為專用硬體提供最佳延遲）</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="182"/>
        <location filename="../src/mainwindow.cpp" line="216"/>
        <source>Musician&apos;s Canvas</source>
        <translation>Musician&apos;s Canvas</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="245"/>
        <source>&amp;File</source>
        <translation>檔案(&amp;F)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="246"/>
        <source>&amp;Save Project</source>
        <translation>儲存專案(&amp;S)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="249"/>
        <source>&amp;Open Project...</source>
        <translation>開啟專案(&amp;O)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="253"/>
        <source>&amp;Close</source>
        <translation>關閉(&amp;C)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="257"/>
        <source>&amp;Project</source>
        <translation>專案(&amp;P)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="258"/>
        <source>&amp;Project Settings</source>
        <translation>專案設定(&amp;P)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="262"/>
        <source>Add Demo &amp;Data to Selected Track</source>
        <translation>將示範資料加入選取的音軌(&amp;D)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="265"/>
        <source>&amp;Settings</source>
        <translation>設定(&amp;S)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="266"/>
        <source>&amp;Configuration</source>
        <translation>組態設定(&amp;C)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="270"/>
        <source>&amp;Tools</source>
        <translation>工具(&amp;T)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="271"/>
        <source>&amp;Mix tracks to file...</source>
        <translation>混音至檔案(&amp;M)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="274"/>
        <source>&amp;Virtual MIDI Keyboard</source>
        <translation>虛擬 MIDI 鍵盤(&amp;V)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="222"/>
        <location filename="../src/mainwindow.cpp" line="343"/>
        <source>Project location:</source>
        <translation>專案位置：</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="346"/>
        <source>Select a project directory...</source>
        <translation>選擇專案目錄...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="224"/>
        <location filename="../src/mainwindow.cpp" line="361"/>
        <source>Browse...</source>
        <translation>瀏覽...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="374"/>
        <location filename="../src/mainwindow.cpp" line="633"/>
        <source>Play all tracks</source>
        <translation>播放所有音軌</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="226"/>
        <location filename="../src/mainwindow.cpp" line="379"/>
        <source>+ Add Track</source>
        <translation>+ 新增音軌</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="228"/>
        <location filename="../src/mainwindow.cpp" line="386"/>
        <location filename="../src/mainwindow.cpp" line="1580"/>
        <source>Clear Tracks</source>
        <translation>清除音軌</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="232"/>
        <location filename="../src/mainwindow.cpp" line="301"/>
        <source>Project Settings</source>
        <translation>專案設定</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="233"/>
        <location filename="../src/mainwindow.cpp" line="307"/>
        <source>Configuration</source>
        <translation>組態設定</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="234"/>
        <location filename="../src/mainwindow.cpp" line="316"/>
        <source>Metronome Settings</source>
        <translation>節拍器設定</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="496"/>
        <source>Mix Complete</source>
        <translation>混音完成</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="497"/>
        <source>Audio exported successfully to:
%1</source>
        <translation>音訊已成功匯出至：
%1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="501"/>
        <source>Mix Failed</source>
        <translation>混音失敗</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="502"/>
        <source>Could not export audio. Please check that you have at least one enabled track with content, and a SoundFont configured for MIDI tracks.</source>
        <translation>無法匯出音訊。請確認至少有一個已啟用且包含內容的音軌，並且 MIDI 音軌已設定 SoundFont。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="512"/>
        <source>Select Project Directory</source>
        <translation>選擇專案目錄</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="621"/>
        <source>Stop</source>
        <translation>停止</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="627"/>
        <source>Record armed track</source>
        <translation>錄製已備錄的音軌</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="651"/>
        <location filename="../src/mainwindow.cpp" line="661"/>
        <source>Playback Error</source>
        <translation>播放錯誤</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="651"/>
        <source>Could not create temporary file.</source>
        <translation>無法建立暫存檔案。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="662"/>
        <source>Could not mix tracks for playback. Make sure tracks have content.</source>
        <translation>無法混音進行播放。請確認音軌包含內容。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="694"/>
        <source>Playback</source>
        <translation>播放</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="695"/>
        <source>Qt Multimedia is not available. Build with Qt6::Multimedia for playback support.</source>
        <translation>Qt Multimedia 無法使用。請使用 Qt6::Multimedia 建置以支援播放功能。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="708"/>
        <source>Project Directory Required</source>
        <translation>需要專案目錄</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="709"/>
        <source>You must choose a project directory before recording.
Please select a directory to store your recorded files.</source>
        <translation>錄音前必須選擇專案目錄。
請選擇一個目錄來儲存您的錄音檔案。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="734"/>
        <source>Recording Error</source>
        <translation>錄音錯誤</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="735"/>
        <source>No audio input device found. Configure one in Settings.</source>
        <translation>找不到音訊輸入裝置。請在設定中設定一個裝置。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="753"/>
        <location filename="../src/mainwindow.cpp" line="763"/>
        <source>Get ready...  %1</source>
        <translation>準備好...  %1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="777"/>
        <source>Recording</source>
        <translation>錄音</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="778"/>
        <source>Qt Multimedia is not available. Build with Qt6::Multimedia for recording support.</source>
        <translation>Qt Multimedia 無法使用。請使用 Qt6::Multimedia 建置以支援錄音功能。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1126"/>
        <source>PortAudio</source>
        <translation>PortAudio</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1127"/>
        <source>Could not start PortAudio recording (%1). Falling back to Qt Multimedia.</source>
        <translation>無法啟動 PortAudio 錄音（%1）。將改用 Qt Multimedia。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="231"/>
        <location filename="../src/mainwindow.cpp" line="293"/>
        <location filename="../src/mainwindow.cpp" line="1303"/>
        <source>Save Project</source>
        <translation>儲存專案</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1304"/>
        <source>Please choose a project directory before saving.</source>
        <translation>儲存前請先選擇專案目錄。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1378"/>
        <source>Save Failed</source>
        <translation>儲存失敗</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1379"/>
        <source>Could not write project file:
%1</source>
        <translation>無法寫入專案檔案：
%1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1385"/>
        <source>Project Saved</source>
        <translation>專案已儲存</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1386"/>
        <source>Project saved to:
%1</source>
        <translation>專案已儲存至：
%1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="230"/>
        <location filename="../src/mainwindow.cpp" line="287"/>
        <location filename="../src/mainwindow.cpp" line="1395"/>
        <source>Open Project</source>
        <translation>開啟專案</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1396"/>
        <source>Project files (*.json);;All files (*)</source>
        <translation>專案檔案 (*.json);;所有檔案 (*)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1407"/>
        <location filename="../src/mainwindow.cpp" line="1415"/>
        <source>Open Failed</source>
        <translation>開啟失敗</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1408"/>
        <source>Could not read project file:
%1</source>
        <translation>無法讀取專案檔案：
%1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1415"/>
        <source>Invalid project file format.</source>
        <translation>無效的專案檔案格式。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1581"/>
        <source>Remove all %1 track(s) from the project?</source>
        <translation>要從專案中移除所有 %1 個音軌嗎？</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1599"/>
        <source>Unsaved Changes</source>
        <translation>未儲存的變更</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1600"/>
        <source>The project has unsaved changes. Save before closing?</source>
        <translation>專案有未儲存的變更。要在關閉前儲存嗎？</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1672"/>
        <source>No Tracks</source>
        <translation>沒有音軌</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1672"/>
        <source>Add a track first, then select it.</source>
        <translation>請先新增音軌，然後選取它。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1684"/>
        <location filename="../src/mainwindow.cpp" line="1689"/>
        <source>Demo Data</source>
        <translation>示範資料</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1685"/>
        <source>Demo MIDI notes added. Try Mix → Mix tracks to file to export.</source>
        <translation>已加入示範 MIDI 音符。請嘗試「混音 → 混音至檔案」來匯出。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1690"/>
        <source>Switch the track to MIDI type (Options) to add demo notes.</source>
        <translation>將音軌切換為 MIDI 類型（選項）以加入示範音符。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1698"/>
        <source>Track %1</source>
        <translation>音軌 %1</translation>
    </message>
</context>
<context>
    <name>MetronomeDialog</name>
    <message>
        <location filename="../src/metronomedialog.cpp" line="13"/>
        <source>Metronome Settings</source>
        <translation>節拍器設定</translation>
    </message>
    <message>
        <location filename="../src/metronomedialog.cpp" line="18"/>
        <source>Metronome</source>
        <translation>節拍器</translation>
    </message>
    <message>
        <location filename="../src/metronomedialog.cpp" line="21"/>
        <source>Enable metronome during recording</source>
        <translation>錄音時啟用節拍器</translation>
    </message>
    <message>
        <location filename="../src/metronomedialog.cpp" line="25"/>
        <source>Beats per minute:</source>
        <translation>每分鐘節拍數：</translation>
    </message>
    <message>
        <location filename="../src/metronomedialog.cpp" line="38"/>
        <source>OK</source>
        <translation>確定</translation>
    </message>
    <message>
        <location filename="../src/metronomedialog.cpp" line="41"/>
        <source>Cancel</source>
        <translation>取消</translation>
    </message>
</context>
<context>
    <name>MidiVisualizationWidget</name>
    <message>
        <location filename="../src/midivisualizationwidget.cpp" line="48"/>
        <source>No MIDI data recorded</source>
        <translation>尚未錄製 MIDI 資料</translation>
    </message>
</context>
<context>
    <name>MixDialog</name>
    <message>
        <location filename="../src/mixdialog.cpp" line="15"/>
        <source>Mix Tracks to File</source>
        <translation>混音至檔案</translation>
    </message>
    <message>
        <location filename="../src/mixdialog.cpp" line="24"/>
        <source>Output file:</source>
        <translation>輸出檔案：</translation>
    </message>
    <message>
        <location filename="../src/mixdialog.cpp" line="26"/>
        <source>Choose output filename...</source>
        <translation>選擇輸出檔案名稱...</translation>
    </message>
    <message>
        <location filename="../src/mixdialog.cpp" line="28"/>
        <source>Browse...</source>
        <translation>瀏覽...</translation>
    </message>
    <message>
        <location filename="../src/mixdialog.cpp" line="35"/>
        <source>Mix</source>
        <translation>混音</translation>
    </message>
    <message>
        <location filename="../src/mixdialog.cpp" line="51"/>
        <source>Choose output file</source>
        <translation>選擇輸出檔案</translation>
    </message>
    <message>
        <location filename="../src/mixdialog.cpp" line="52"/>
        <source>FLAC files (*.flac);;WAV files (*.wav)</source>
        <translation>FLAC 檔案 (*.flac);;WAV 檔案 (*.wav)</translation>
    </message>
    <message>
        <location filename="../src/mixdialog.cpp" line="67"/>
        <source>No file chosen</source>
        <translation>未選擇檔案</translation>
    </message>
    <message>
        <location filename="../src/mixdialog.cpp" line="67"/>
        <source>Please choose an output file.</source>
        <translation>請選擇輸出檔案。</translation>
    </message>
</context>
<context>
    <name>ProjectSettingsDialog</name>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="42"/>
        <source>Project Settings</source>
        <translation>專案設定</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="86"/>
        <source>MIDI Device</source>
        <translation>MIDI 裝置</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="92"/>
        <source>Refresh</source>
        <translation>重新整理</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="98"/>
        <source>SoundFont (for MIDI synthesis)</source>
        <translation>SoundFont（用於 MIDI 合成）</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="101"/>
        <source>Path to .sf2 SoundFont file (empty = app default)</source>
        <translation>.sf2 SoundFont 檔案路徑（空白 = 應用程式預設值）</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="104"/>
        <source>Browse...</source>
        <translation>瀏覽...</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="111"/>
        <source>MIDI</source>
        <translation>MIDI</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="120"/>
        <source>Audio Input Device</source>
        <translation>音訊輸入裝置</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="129"/>
        <source>Recording capture</source>
        <translation>錄音擷取</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="132"/>
        <source>PortAudio (native input, recommended when built-in)</source>
        <translation>PortAudio（原生輸入，內建時建議使用）</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="133"/>
        <source>Qt Multimedia</source>
        <translation>Qt Multimedia</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="140"/>
        <source>PortAudio input device:</source>
        <translation>PortAudio 輸入裝置：</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="156"/>
        <source>Audio Output Device</source>
        <translation>音訊輸出裝置</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="164"/>
        <source>Audio Format (applies to all audio tracks)</source>
        <translation>音訊格式（套用至所有音訊音軌）</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="169"/>
        <source>Sample rate:</source>
        <translation>取樣率：</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="177"/>
        <source>Channels:</source>
        <translation>聲道：</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="178"/>
        <source>Mono</source>
        <translation>單聲道</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="179"/>
        <source>Stereo</source>
        <translation>立體聲</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="196"/>
        <source>Audio</source>
        <translation>音訊</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="203"/>
        <location filename="../src/projectsettingsdialog.cpp" line="224"/>
        <location filename="../src/projectsettingsdialog.cpp" line="229"/>
        <location filename="../src/projectsettingsdialog.cpp" line="234"/>
        <location filename="../src/projectsettingsdialog.cpp" line="235"/>
        <source>(App default)</source>
        <translation>（應用程式預設值）</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="204"/>
        <source>Built-in FluidSynth synthesizer</source>
        <translation>內建 FluidSynth 合成器</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="306"/>
        <source>Saved device %1 (not in current list)</source>
        <translation>已儲存的裝置 %1（不在目前清單中）</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="496"/>
        <location filename="../src/projectsettingsdialog.cpp" line="513"/>
        <source> (native)</source>
        <translation> （原生）</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="498"/>
        <source> (resampled)</source>
        <translation> （重新取樣）</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="547"/>
        <source>Select SoundFont</source>
        <translation>選擇 SoundFont</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="549"/>
        <source>SoundFont files (*.sf2 *.SF2);;All files (*)</source>
        <translation>SoundFont 檔案 (*.sf2 *.SF2);;所有檔案 (*)</translation>
    </message>
    <message>
        <location filename="../src/projectsettingsdialog.cpp" line="558"/>
        <source>(Default audio input)</source>
        <translation>（預設音訊輸入）</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../src/musicians_canvas_main.cpp" line="89"/>
        <source>Audio System Error</source>
        <translation>音訊系統錯誤</translation>
    </message>
    <message>
        <location filename="../src/musicians_canvas_main.cpp" line="101"/>
        <source>PortAudio</source>
        <translation>PortAudio</translation>
    </message>
    <message>
        <location filename="../src/musicians_canvas_main.cpp" line="102"/>
        <source>PortAudio could not be initialized. Recording will use Qt Multimedia only.
%1</source>
        <translation>無法初始化 PortAudio。錄音將僅使用 Qt Multimedia。
%1</translation>
    </message>
    <message>
        <location filename="../src/portaudiorecorder.cpp" line="22"/>
        <source>PortAudio was not enabled in this build.</source>
        <translation>此版本未啟用 PortAudio。</translation>
    </message>
    <message>
        <location filename="../src/portaudiorecorder.cpp" line="145"/>
        <source>No PortAudio devices available.</source>
        <translation>沒有可用的 PortAudio 裝置。</translation>
    </message>
    <message>
        <location filename="../src/portaudiorecorder.cpp" line="154"/>
        <source>No default PortAudio input device.</source>
        <translation>沒有預設的 PortAudio 輸入裝置。</translation>
    </message>
    <message>
        <location filename="../src/portaudiorecorder.cpp" line="161"/>
        <source>Invalid PortAudio device index.</source>
        <translation>無效的 PortAudio 裝置索引。</translation>
    </message>
    <message>
        <location filename="../src/portaudiorecorder.cpp" line="168"/>
        <source>Device does not support %1 input channel(s).</source>
        <translation>裝置不支援 %1 個輸入聲道。</translation>
    </message>
    <message>
        <location filename="../src/portaudiorecorder.cpp" line="228"/>
        <source>Could not open PortAudio input at a supported sample rate for this device.</source>
        <translation>無法以此裝置支援的取樣率開啟 PortAudio 輸入。</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <location filename="../src/settingsdialog.cpp" line="32"/>
        <source>Configuration</source>
        <translation>組態設定</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="47"/>
        <source>Apply</source>
        <translation>套用</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="52"/>
        <source>Close</source>
        <translation>關閉</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="86"/>
        <source>Here, you can configure general application settings.</source>
        <translation>您可以在此設定一般應用程式設定。</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="88"/>
        <source>Appearance</source>
        <translation>外觀</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="90"/>
        <source>Theme:</source>
        <translation>佈景主題：</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="92"/>
        <source>Dark</source>
        <translation>深色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="93"/>
        <source>Light</source>
        <translation>淺色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="98"/>
        <source>Diagnostics</source>
        <translation>診斷</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="100"/>
        <source>Write recording debug log</source>
        <translation>寫入錄音除錯記錄</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="105"/>
        <source>General</source>
        <translation>一般</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="116"/>
        <source>Configure the appearance of display elements.</source>
        <translation>設定顯示元素的外觀。</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="118"/>
        <source>Numeric Display</source>
        <translation>數字顯示</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="120"/>
        <source>Numeric display LED color:</source>
        <translation>數字顯示 LED 顏色：</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="123"/>
        <source>Light Red</source>
        <translation>淺紅色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="124"/>
        <source>Dark Red</source>
        <translation>深紅色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="125"/>
        <source>Light Green</source>
        <translation>淺綠色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="126"/>
        <source>Dark Green</source>
        <translation>深綠色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="127"/>
        <source>Light Blue</source>
        <translation>淺藍色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="128"/>
        <source>Dark Blue</source>
        <translation>深藍色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="129"/>
        <source>Yellow</source>
        <translation>黃色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="130"/>
        <source>Orange</source>
        <translation>橙色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="131"/>
        <source>Light Cyan</source>
        <translation>淺青色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="132"/>
        <source>Dark Cyan</source>
        <translation>深青色</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="137"/>
        <source>Display</source>
        <translation>顯示</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="148"/>
        <source>Here, you can configure settings related to MIDI.</source>
        <translation>您可以在此設定與 MIDI 相關的設定。</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="150"/>
        <source>MIDI Device</source>
        <translation>MIDI 裝置</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="156"/>
        <source>Refresh</source>
        <translation>重新整理</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="162"/>
        <source>SoundFont (for MIDI synthesis)</source>
        <translation>SoundFont（用於 MIDI 合成）</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="165"/>
        <source>Path to .sf2 SoundFont file</source>
        <translation>.sf2 SoundFont 檔案路徑</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="168"/>
        <source>Browse...</source>
        <translation>瀏覽...</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="174"/>
        <source>MIDI</source>
        <translation>MIDI</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="185"/>
        <source>Here, you can configure audio settings for the application.</source>
        <translation>您可以在此設定應用程式的音訊設定。</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="187"/>
        <source>Audio Input Device</source>
        <translation>音訊輸入裝置</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="194"/>
        <source>Audio Output Device</source>
        <translation>音訊輸出裝置</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="202"/>
        <source>Audio</source>
        <translation>音訊</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="213"/>
        <source>Choose the display language for the application.</source>
        <translation>選擇應用程式的顯示語言。</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="215"/>
        <location filename="../src/settingsdialog.cpp" line="242"/>
        <source>Language</source>
        <translation>語言</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="219"/>
        <source>System Default</source>
        <translation>系統預設</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="250"/>
        <source>Built-in FluidSynth synthesizer</source>
        <translation>內建 FluidSynth 合成器</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="271"/>
        <location filename="../src/settingsdialog.cpp" line="277"/>
        <source>(Default)</source>
        <translation>（預設）</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="283"/>
        <location filename="../src/settingsdialog.cpp" line="284"/>
        <source>(Default - install qt6-multimedia-dev for device list)</source>
        <translation>（預設 - 安裝 qt6-multimedia-dev 以取得裝置清單）</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="372"/>
        <source>Select SoundFont</source>
        <translation>選擇 SoundFont</translation>
    </message>
    <message>
        <location filename="../src/settingsdialog.cpp" line="374"/>
        <source>SoundFont files (*.sf2 *.SF2);;All files (*)</source>
        <translation>SoundFont 檔案 (*.sf2 *.SF2);;所有檔案 (*)</translation>
    </message>
</context>
<context>
    <name>TrackConfigDialog</name>
    <message>
        <location filename="../src/trackconfigdialog.cpp" line="10"/>
        <source>Track Configuration</source>
        <translation>音軌設定</translation>
    </message>
    <message>
        <location filename="../src/trackconfigdialog.cpp" line="16"/>
        <source>Track Type</source>
        <translation>音軌類型</translation>
    </message>
    <message>
        <location filename="../src/trackconfigdialog.cpp" line="19"/>
        <source>Audio - Record from audio interface</source>
        <translation>音訊 - 從音訊介面錄製</translation>
    </message>
    <message>
        <location filename="../src/trackconfigdialog.cpp" line="20"/>
        <source>MIDI - Record from MIDI device</source>
        <translation>MIDI - 從 MIDI 裝置錄製</translation>
    </message>
    <message>
        <location filename="../src/trackconfigdialog.cpp" line="28"/>
        <source>Capture audio from microphone or line-in.</source>
        <translation>從麥克風或線路輸入擷取音訊。</translation>
    </message>
    <message>
        <location filename="../src/trackconfigdialog.cpp" line="31"/>
        <source>Capture MIDI notes from keyboard or controller.</source>
        <translation>從鍵盤或控制器擷取 MIDI 音符。</translation>
    </message>
</context>
<context>
    <name>TrackWidget</name>
    <message>
        <location filename="../src/trackwidget.cpp" line="37"/>
        <location filename="../src/trackwidget.cpp" line="228"/>
        <source>Options</source>
        <translation>選項</translation>
    </message>
    <message>
        <location filename="../src/trackwidget.cpp" line="63"/>
        <source>Track name</source>
        <translation>音軌名稱</translation>
    </message>
    <message>
        <location filename="../src/trackwidget.cpp" line="69"/>
        <source>Invalid Name</source>
        <translation>無效的名稱</translation>
    </message>
    <message>
        <location filename="../src/trackwidget.cpp" line="70"/>
        <source>Track name cannot be empty.</source>
        <translation>音軌名稱不能為空。</translation>
    </message>
    <message>
        <location filename="../src/trackwidget.cpp" line="87"/>
        <location filename="../src/trackwidget.cpp" line="229"/>
        <source>Remove track</source>
        <translation>移除音軌</translation>
    </message>
    <message>
        <location filename="../src/trackwidget.cpp" line="117"/>
        <location filename="../src/trackwidget.cpp" line="230"/>
        <source>Enable</source>
        <translation>啟用</translation>
    </message>
    <message>
        <location filename="../src/trackwidget.cpp" line="121"/>
        <location filename="../src/trackwidget.cpp" line="231"/>
        <source>Arm</source>
        <translation>備錄</translation>
    </message>
    <message>
        <location filename="../src/trackwidget.cpp" line="241"/>
        <source>MIDI track — click to change input type</source>
        <translation>MIDI 音軌 — 按一下以變更輸入類型</translation>
    </message>
    <message>
        <location filename="../src/trackwidget.cpp" line="246"/>
        <source>Audio track — click to change input type</source>
        <translation>音訊音軌 — 按一下以變更輸入類型</translation>
    </message>
</context>
<context>
    <name>WaveformWidget</name>
    <message>
        <location filename="../src/waveformwidget.cpp" line="125"/>
        <source>● Recording</source>
        <translation>● 錄音中</translation>
    </message>
    <message>
        <location filename="../src/waveformwidget.cpp" line="134"/>
        <source>No audio recorded</source>
        <translation>尚未錄製音訊</translation>
    </message>
</context>
</TS>
