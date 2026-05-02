#include "mainwindow.h"
#include "trackmixerundocommand.h"
#include "workflowdialogs.h"
#include "trackwidget.h"
#include "trackconfigdialog.h"
#include "settingsdialog.h"
#include "projectsettingsdialog.h"
#include "audioutils.h"
#include "recordingpostprocess.h"
#if defined(HAVE_PORTAUDIO)
#include "portaudiorecorder.h"
#endif
#include "mixdialog.h"
#include "audioformats.h"
#include "metronomedialog.h"
#include "midirecorder.h"
#include "midifile.h"
#include "drumtrackgenerator.h"
#include "segmentdisplay.h"
#include "appsettings.h"
#include "midiplayer.h"
#include "audiostartup.h"
#include "effectdsp.h"
#include "effectsdialog.h"
#ifdef QT_MULTIMEDIA_AVAILABLE
#include "recordingwritedevice.h"
#include "recordingaudiomonitor.h"
#include "midirecordingmonitor.h"
#endif
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QSignalBlocker>
#include <QLineEdit>
#include <QLabel>
#include <QIODevice>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QProgressDialog>
#include <QFutureWatcher>
#include <QEventLoop>
#include <QtConcurrent/QtConcurrentRun>
#include <QApplication>
#include <QEvent>
#include <QStyle>
#include <QToolBar>
#include <QToolButton>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QSet>
#include <QFileInfo>
#include <QCoreApplication>
#include <QFrame>
#include <QProcess>
#include <QDir>
#include <QDesktopServices>
#include <QIcon>
#include <QPixmap>
#include <QTemporaryFile>
#include <QCloseEvent>
#include <QRegularExpression>
#include <QTimer>
#include <algorithm>
#include <cmath>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <functional>
#include <optional>
#include <vector>
#include <cstring>

#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAudioSource>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QUrl>
#endif

#ifdef QT_MULTIMEDIA_AVAILABLE
static QAudioDevice resolveAudioOutputDevice(const QByteArray& projectId,
                                             const QByteArray& appId)
{
    const QByteArray id = !projectId.isEmpty() ? projectId : appId;
    if (!id.isEmpty())
    {
        const auto outs = QMediaDevices::audioOutputs();
        for (const auto& d : outs)
        {
            if (d.id() == id)
                return d;
        }
    }
    return QMediaDevices::defaultAudioOutput();
}
#endif

namespace
{
void applyMixBusEffectsToPathIfSet(const QString& path, const QJsonArray& chain)
{
    if (path.isEmpty() || chain.isEmpty())
        return;
    if (!QFile::exists(path))
        return;
    static_cast<void>(AudioUtils::applyMixEffectChainToAudioFile(path, chain));
}

QString normalizedManualTag(const QString& lang)
{
    if (lang.isEmpty())
        return {};

    QString l = lang;
    l.replace(QLatin1Char('-'), QLatin1Char('_'));
    const QStringList parts = l.split(QLatin1Char('_'), Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return {};

    const QString language = parts.value(0).toLower();
    const QString region   = parts.value(1).toUpper();
    if (!region.isEmpty())
        return language + QLatin1Char('-') + region;
    return language;
}

QString resolveManualPdfPathForLanguage(const QString& langSetting)
{
    const QString effective =
        !langSetting.isEmpty() ? langSetting : QLocale::system().name();

    const QString tagExact = normalizedManualTag(effective);
    const QString tagShort = normalizedManualTag(effective.left(2));

    const QString baseName = QStringLiteral("MusiciansCanvas_User_Manual");
    const QString fileBase = baseName + QStringLiteral(".pdf");

    QStringList candidates;
    candidates.reserve(6);
    if (!tagExact.isEmpty() && tagExact != QStringLiteral("en"))
        candidates.append(baseName + QLatin1Char('_') + tagExact + QStringLiteral(".pdf"));
    if (!tagShort.isEmpty() && tagShort != QStringLiteral("en") && tagShort != tagExact)
        candidates.append(baseName + QLatin1Char('_') + tagShort + QStringLiteral(".pdf"));
    candidates.append(fileBase);

    // Search for docs/ in and above the executable directory (supports running from build/).
    QDir dir(QCoreApplication::applicationDirPath());
    for (int up = 0; up < 6; ++up)
    {
        const QString docsDir = dir.absoluteFilePath(QStringLiteral("docs"));
        for (const QString& f : candidates)
        {
            const QString full = QDir(docsDir).absoluteFilePath(f);
            if (QFile::exists(full))
                return full;
        }
        if (!dir.cdUp())
            break;
    }
    return {};
}
}  // namespace

namespace
{
QByteArray spliceAudioPunchReplace(const QByteArray& oldPcm, const QByteArray& newPcm, int sampleRate,
                                 int channelCount, double punchInSec, double punchOutSec)
{
    if (oldPcm.isEmpty() || newPcm.isEmpty() || sampleRate <= 0 || channelCount <= 0)
        return newPcm;
    const int bps = channelCount * static_cast<int>(sizeof(qint16));
    const int oldFrames = oldPcm.size() / bps;
    const int newFrames = newPcm.size() / bps;
    int i0 = static_cast<int>(std::llround(std::max(0.0, punchInSec) * sampleRate));
    int i1 = static_cast<int>(std::llround(std::max(0.0, punchOutSec) * sampleRate));
    i0 = std::clamp(i0, 0, oldFrames);
    i1 = std::clamp(i1, i0, oldFrames);
    const int replaceFrames = i1 - i0;
    const int takeNew = std::min(replaceFrames, newFrames);
    QByteArray out = oldPcm;
    const int newSizeBytes = (i0 + takeNew) * bps;
    if (newSizeBytes > out.size())
        out.resize(newSizeBytes);
    if (takeNew > 0)
        std::memcpy(out.data() + i0 * bps, newPcm.constData(), takeNew * bps);
    return out;
}
}  // namespace

// Map LED color setting name to QColor
static QColor ledColorFromName(const QString& name)
{
    if (name == "dark_red")    return QColor(180, 20, 20);
    else if (name == "light_green") return QColor(80, 255, 80);
    else if (name == "dark_green")  return QColor(20, 160, 20);
    else if (name == "light_blue")  return QColor(100, 140, 255);
    else if (name == "dark_blue")   return QColor(30, 60, 200);
    else if (name == "yellow")      return QColor(255, 255, 40);
    else if (name == "orange")      return QColor(255, 160, 20);
    else if (name == "light_cyan")  return QColor(80, 255, 255);
    else if (name == "dark_cyan")   return QColor(20, 180, 180);
    // default: light_red
    else return QColor(255, 60, 60);
}

#ifdef QT_MULTIMEDIA_AVAILABLE
// Generate a short metronome click sound (a short sine burst with exponential decay).
// Returns raw Int16 PCM data at 44100 Hz mono.
static QByteArray generateClickPcm()
{
    const int sampleRate = 44100;
    const int durationMs = 30;
    const int numSamples = sampleRate * durationMs / 1000;
    const double freq = 1000.0;  // 1 kHz tick
    QByteArray pcm;
    pcm.resize(numSamples * 2);
    auto* samples = reinterpret_cast<qint16*>(pcm.data());
    for (int i = 0; i < numSamples; ++i)
    {
        double t = static_cast<double>(i) / sampleRate;
        double envelope = std::exp(-t * 150.0);  // fast decay
        double value = std::sin(2.0 * M_PI * freq * t) * envelope * 0.7;
        samples[i] = static_cast<qint16>(value * 32767.0);
    }
    return pcm;
}
#endif

// Single dispatch point for loading any supported audio file format.  When
// adding a new format, register its extension in src/audioformats.h and add
// a new branch here that calls the appropriate decoder.
static bool loadAudioFile(const QString& path, QByteArray& audioData,
                          int& sampleRate, int& channelCount)
{
    const QString ext = QStringLiteral(".") + QFileInfo(path).suffix().toLower();
    if (ext == QStringLiteral(".wav"))
        return AudioUtils::readWavInt16Pcm(path, audioData, sampleRate, channelCount);
    return AudioUtils::readEncodedAudioFile(path, audioData, sampleRate, channelCount);
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle(tr("Musician's Canvas"));
    setWindowIcon(QIcon(":/icons/MusiciansCanvas.ico"));
    setMinimumSize(800, 540);
    resize(900, 640);

    m_playIcon   = QIcon(":/images/play_button.png");
    m_recordIcon = QIcon(":/images/record_button.png");
    m_stopIcon   = QIcon(":/images/stop_button.png");

    setupMenuBar();
    setupToolBar();
    setupUi();

    // Accept drag-and-drop of supported audio files onto the main window
    setAcceptDrops(true);

    // Auto-load the last project if one exists in the stored project directory
    const QString lastDir = AppSettings::instance().projectLocation();
    if (!lastDir.isEmpty())
    {
        const QString projectFile = lastDir + QDir::separator() + QStringLiteral("project.json");
        if (QFile::exists(projectFile))
            loadProjectFromFile(projectFile);
    }
}

MainWindow::~MainWindow() = default;

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    QMainWindow::changeEvent(event);
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("Musician's Canvas"));
    // Rebuild menus with new translations
    menuBar()->clear();
    setupMenuBar();
    // Update toolbar and label texts
    if (m_projectLocationLabel)
        m_projectLocationLabel->setText(tr("Project location:"));
    if (m_browseButton)
        m_browseButton->setText(tr("Browse..."));
    if (m_addTrackButton)
        m_addTrackButton->setText(tr("+ Add Track"));
    if (m_clearTracksButton)
        m_clearTracksButton->setText(tr("Clear Tracks"));
    // Update toolbar button tooltips
    if (m_tbOpen) m_tbOpen->setToolTip(tr("Open Project"));
    if (m_tbSave) m_tbSave->setToolTip(tr("Save Project"));
    if (m_tbProjectSettings) m_tbProjectSettings->setToolTip(tr("Project Settings"));
    if (m_tbConfig) m_tbConfig->setToolTip(tr("Configuration"));
    if (m_tbMetronome) m_tbMetronome->setToolTip(tr("Metronome Settings"));
    if (m_monitorWhileRecordingCheck)
        m_monitorWhileRecordingCheck->setText(tr("Monitor audio while recording"));
    updatePlayRecordButton();
    // Update track widgets
    for (auto* tw : m_trackWidgets)
        tw->retranslateUi();
}

void MainWindow::setupMenuBar()
{
    auto* menuBar = this->menuBar();

    auto* fileMenu = menuBar->addMenu(tr("&File"));
    auto* editMenu = menuBar->addMenu(tr("&Edit"));
    editMenu->addAction(m_undoStack.createUndoAction(this, tr("&Undo")));
    editMenu->addAction(m_undoStack.createRedoAction(this, tr("&Redo")));

    auto* saveAction = fileMenu->addAction(tr("&Save Project"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveProject);
    auto* openAction = fileMenu->addAction(tr("&Open Project..."));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::onOpenProject);
    fileMenu->addSeparator();
    auto* closeAction = fileMenu->addAction(tr("&Close"));
    closeAction->setShortcut(QKeySequence::Quit);
    connect(closeAction, &QAction::triggered, this, &MainWindow::onClose);

    auto* projectMenu = menuBar->addMenu(tr("&Project"));
    auto* projectSettingsAction = projectMenu->addAction(tr("&Project Settings"));
    projectSettingsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
    connect(projectSettingsAction, &QAction::triggered, this, &MainWindow::onProjectSettings);
    auto* tempoMapAction = projectMenu->addAction(tr("&Tempo map..."));
    connect(tempoMapAction, &QAction::triggered, this, &MainWindow::onTempoMapEditor);
    projectMenu->addSeparator();
    auto* demoAction = projectMenu->addAction(tr("Add Demo &Data to Selected Track"));
    connect(demoAction, &QAction::triggered, this, &MainWindow::onAddDemoData);

    auto* settingsMenu = menuBar->addMenu(tr("&Settings"));
    auto* configAction = settingsMenu->addAction(tr("&Configuration"));
    configAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Comma));
    connect(configAction, &QAction::triggered, this, &MainWindow::onSettingsConfiguration);

    auto* toolsMenu = menuBar->addMenu(tr("&Tools"));
    auto* mixAction = toolsMenu->addAction(tr("&Mix tracks to file..."));
    mixAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    connect(mixAction, &QAction::triggered, this, &MainWindow::onMix);
    auto* stemsAction = toolsMenu->addAction(tr("Export &stems to folder..."));
    connect(stemsAction, &QAction::triggered, this, &MainWindow::onExportStems);
    auto* recOptAction = toolsMenu->addAction(tr("&Recording options..."));
    connect(recOptAction, &QAction::triggered, this, &MainWindow::onRecordingOptions);
    auto* quantAction = toolsMenu->addAction(tr("&Quantize MIDI..."));
    connect(quantAction, &QAction::triggered, this, &MainWindow::onQuantizeMidi);
    toolsMenu->addSeparator();
    auto* drumAction = toolsMenu->addAction(tr("Add &drum track"));
    drumAction->setShortcut(QKeySequence(Qt::Key_D));
    connect(drumAction, &QAction::triggered, this, &MainWindow::onAddDrumTrack);
    auto* vkAction = toolsMenu->addAction(tr("&Virtual MIDI Keyboard"));
    connect(vkAction, &QAction::triggered, this, &MainWindow::onVirtualMidiKeyboard);

    auto* helpMenu = menuBar->addMenu(tr("&Help"));
    auto* manualAction = helpMenu->addAction(tr("&Manual"));
    connect(manualAction, &QAction::triggered, this, &MainWindow::onManual);
    helpMenu->addSeparator();
    auto* aboutAction = helpMenu->addAction(tr("&About"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::setupToolBar()
{
    m_toolBar = new QToolBar(this);
    m_toolBar->setMovable(false);
    m_toolBar->setIconSize(QSize(24, 24));
    m_toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    m_tbOpen = new QToolButton();
    m_tbOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    m_tbOpen->setToolTip(tr("Open Project"));
    connect(m_tbOpen, &QToolButton::clicked, this, &MainWindow::onOpenProject);
    m_toolBar->addWidget(m_tbOpen);

    m_tbSave = new QToolButton();
    m_tbSave->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    m_tbSave->setToolTip(tr("Save Project"));
    connect(m_tbSave, &QToolButton::clicked, this, &MainWindow::onSaveProject);
    m_toolBar->addWidget(m_tbSave);

    m_toolBar->addSeparator();

    m_tbProjectSettings = new QToolButton();
    m_tbProjectSettings->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    m_tbProjectSettings->setToolTip(tr("Project Settings"));
    connect(m_tbProjectSettings, &QToolButton::clicked, this, &MainWindow::onProjectSettings);
    m_toolBar->addWidget(m_tbProjectSettings);

    m_tbConfig = new QToolButton();
    m_tbConfig->setIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    m_tbConfig->setToolTip(tr("Configuration"));
    connect(m_tbConfig, &QToolButton::clicked, this, &MainWindow::onSettingsConfiguration);
    m_toolBar->addWidget(m_tbConfig);

    m_toolBar->addSeparator();

    m_tbMetronome = new QToolButton();
    m_tbMetronome->setIcon(QIcon(QStringLiteral(":/images/metronome.png")));
    m_tbMetronome->setToolTip(tr("Metronome Settings"));
    connect(m_tbMetronome, &QToolButton::clicked, this, &MainWindow::onMetronomeSettings);
    m_toolBar->addWidget(m_tbMetronome);

    addToolBar(m_toolBar);
    updateToolBarState();
}

void MainWindow::updateToolBarState()
{
    const bool hasProject = !m_projectLocationEdit
                            || !m_projectLocationEdit->text().trimmed().isEmpty();
    if (m_tbSave) m_tbSave->setEnabled(hasProject);
    if (m_tbProjectSettings) m_tbProjectSettings->setEnabled(hasProject);
}

void MainWindow::setupUi()
{
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(8);

    // Project location row
    auto* projectRow = new QHBoxLayout();
    projectRow->setSpacing(6);
    m_projectLocationLabel = new QLabel(tr("Project location:"));
    projectRow->addWidget(m_projectLocationLabel);
    m_projectLocationEdit = new QLineEdit();
    m_projectLocationEdit->setPlaceholderText(tr("Select a project directory..."));
    m_projectLocationEdit->setText(AppSettings::instance().projectLocation());
    connect(m_projectLocationEdit, &QLineEdit::editingFinished, this, [this]()
    {
        const QString oldDir = AppSettings::instance().projectLocation();
        const QString newDir = m_projectLocationEdit->text().trimmed();
        if (newDir != oldDir)
        {
            moveProjectFiles(oldDir, newDir);
            AppSettings::instance().setProjectLocation(newDir);
            AppSettings::instance().save();
            updateToolBarState();
        }
    });
    projectRow->addWidget(m_projectLocationEdit, 1);
    m_browseButton = new QPushButton(tr("Browse..."));
    m_browseButton->setFixedHeight(28);
    connect(m_browseButton, &QPushButton::clicked, this, &MainWindow::onBrowseProjectLocation);
    projectRow->addWidget(m_browseButton);
    mainLayout->addLayout(projectRow);

    // Toolbar row: play/record button + add track button
    auto* toolbarLayout = new QHBoxLayout();
    toolbarLayout->setSpacing(6);

    m_playRecordButton = new QPushButton();
    m_playRecordButton->setIconSize(QSize(28, 28));
    m_playRecordButton->setFixedSize(40, 36);
    m_playRecordButton->setToolTip(tr("Play all tracks"));
    connect(m_playRecordButton, &QPushButton::clicked, this, &MainWindow::onPlayRecord);
    updatePlayRecordButton();
    toolbarLayout->addWidget(m_playRecordButton);

    m_addTrackButton = new QPushButton(QIcon(":/images/addTrack.png"), tr("+ Add Track"));
    m_addTrackButton->setObjectName("addTrackBtn");
    m_addTrackButton->setIconSize(QSize(20, 20));
    m_addTrackButton->setFixedHeight(36);
    connect(m_addTrackButton, &QPushButton::clicked, this, &MainWindow::onAddTrack);
    toolbarLayout->addWidget(m_addTrackButton);

    m_clearTracksButton = new QPushButton(tr("Clear Tracks"));
    m_clearTracksButton->setFixedHeight(36);
    m_clearTracksButton->setStyleSheet(
        "QPushButton { background-color: #e8521e; color: white; font-weight: bold; }"
        "QPushButton:hover { background-color: #f07040; }"
        "QPushButton:pressed { background-color: #c03010; }");
    connect(m_clearTracksButton, &QPushButton::clicked, this, &MainWindow::onClearTracks);
    toolbarLayout->addWidget(m_clearTracksButton);

    // LED time display — sized to exactly fit HH:MM:SS.hh digits
    m_timeDisplay = new SegmentDisplay();
    m_timeDisplay->setActiveColor(ledColorFromName(AppSettings::instance().ledColor()));
    m_timeDisplay->setFixedHeight(36);
    m_timeDisplay->setFixedWidth(m_timeDisplay->sizeHint().width());
    toolbarLayout->addWidget(m_timeDisplay);

    m_monitorWhileRecordingCheck = new QCheckBox(
        tr("Monitor audio while recording"));
    m_monitorWhileRecordingCheck->setChecked(m_projectSettings.monitorWhileRecording);
    connect(m_monitorWhileRecordingCheck, &QCheckBox::toggled, this, [this](bool on)
    {
        m_projectSettings.monitorWhileRecording = on;
        markDirty();
#ifdef QT_MULTIMEDIA_AVAILABLE
        applyLiveRecordingMonitorState();
#endif
    });
    toolbarLayout->addWidget(m_monitorWhileRecordingCheck);

    toolbarLayout->addStretch();

    mainLayout->addLayout(toolbarLayout);

    // Tracks scroll area
    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    m_tracksContainer = new QWidget();
    m_tracksLayout = new QVBoxLayout(m_tracksContainer);
    m_tracksLayout->setContentsMargins(0, 0, 0, 0);
    m_tracksLayout->setSpacing(10);
    m_tracksLayout->addStretch();

    m_scrollArea->setWidget(m_tracksContainer);
    mainLayout->addWidget(m_scrollArea);
}

void MainWindow::wireTrackWidget(TrackWidget* widget)
{
    connect(widget, &TrackWidget::configurationRequested, this, &MainWindow::onTrackConfigRequested);
    connect(widget, &TrackWidget::effectsRequested, this, &MainWindow::onTrackEffectsRequested);
    connect(widget, &TrackWidget::armChanged, this, &MainWindow::onArmChanged);
    connect(widget, &TrackWidget::dataChanged, this, [this](TrackWidget*)
    {
        markDirty();
        updatePlayRecordButton();
    });
    connect(widget, &TrackWidget::nameChanged, this, &MainWindow::onTrackNameChanged);
    connect(widget, &TrackWidget::removeRequested, this, &MainWindow::onTrackRemoveRequested);
    connect(widget, &TrackWidget::mixerInteractionStarted, this,
            &MainWindow::onTrackMixerInteractionStarted);
    connect(widget, &TrackWidget::mixerInteractionEnded, this,
            &MainWindow::onTrackMixerInteractionEnded);
}

void MainWindow::onTrackMixerInteractionStarted(TrackWidget* w)
{
    if (w)
        m_mixerUndoBaseline[w] = w->trackData();
}

static bool mixerFieldsEqual(const TrackData& a, const TrackData& b)
{
    return a.gainDb == b.gainDb && a.pan == b.pan && a.mute == b.mute && a.solo == b.solo
 && a.auxSend == b.auxSend && a.trimStartSec == b.trimStartSec
           && a.trimEndSec == b.trimEndSec;
}

void MainWindow::onTrackMixerInteractionEnded(TrackWidget* w)
{
    if (!w)
        return;
    auto it = m_mixerUndoBaseline.find(w);
    if (it == m_mixerUndoBaseline.end())
        return;
    const TrackData before = it.value();
    m_mixerUndoBaseline.erase(it);
    const TrackData after = w->trackData();
    if (mixerFieldsEqual(before, after))
        return;
    m_undoStack.push(new TrackMixerUndoCommand(w, before, after));
    markDirty();
}

void MainWindow::onAddTrack()
{
    TrackData data = createNewTrack();
    auto* widget = new TrackWidget(data, this);
    wireTrackWidget(widget);
    m_trackWidgets.append(widget);
    m_tracksLayout->insertWidget(m_tracksLayout->count() - 1, widget);

    // If this is the only track and it hasn't been recorded yet, arm it
    // automatically so the user can start recording immediately.
    if (m_trackWidgets.size() == 1 && data.audioData.isEmpty() && data.midiNotes.isEmpty())
        widget->setArmed(true);

    markDirty();
    updatePlayRecordButton();
}

void MainWindow::onAddDrumTrack()
{
    const QString projectDir = m_projectLocationEdit->text().trimmed();
    if (projectDir.isEmpty())
    {
        QMessageBox::warning(this, tr("Add drum track"),
            tr("Please choose a project directory before adding a drum track."));
        return;
    }

    AppSettings& app = AppSettings::instance();
    const double defaultBpm =
        static_cast<double>(qBound(20, app.metronomeBpm(), 300));
    double bpm = tempoBpmAtTime(m_projectSettings.tempoMarkers, 0.0, defaultBpm);
    if (app.metronomeEnabled())
    {
        bpm = tempoBpmAtTime(m_projectSettings.tempoMarkers, 0.0,
                             static_cast<double>(qBound(20, app.metronomeBpm(), 300)));
    }
    else
    {
        QVector<TrackData> list;
        list.reserve(m_trackWidgets.size());
        for (auto* w : m_trackWidgets)
            list.append(w->trackData());
        const int sr = m_projectSettings.sampleRate > 0 ? m_projectSettings.sampleRate : 44100;
        const QByteArray mix = DrumTrackGenerator::mixEnabledAudioTracksForAnalysis(list, sr);
        const int est =
            mix.isEmpty()
                ? 0
                : DrumTrackGenerator::estimateBpmFromInt16Pcm(mix, sr, 2);
        if (est > 0)
            bpm = static_cast<double>(est);
    }

    constexpr int kBars = 2;
    QVector<MidiNote> notes = DrumTrackGenerator::createGroovePattern(bpm, kBars);
    double lengthSeconds = DrumTrackGenerator::patternLengthSeconds(bpm, kBars);
    for (MidiNote& n : notes)
        n.channel = 9;  // General MIDI drums (MIDI channel 10)

    const QString trackName = allocateUniqueDrumTrackName();
    const QString midiBase   = AudioUtils::sanitizedTrackFilesystemName(trackName);
    const QString midiFullPath =
        projectDir + QDir::separator() + midiBase + QStringLiteral(".mid");
    if (!MidiFile::write(midiFullPath, notes, lengthSeconds))
    {
        QMessageBox::warning(this, tr("Add drum track"),
            tr("Could not write drum MIDI file:\n%1").arg(midiFullPath));
        return;
    }

    TrackData data;
    data.id            = m_nextTrackId++;
    data.name          = trackName;
    data.type          = TrackType::MIDI;
    data.enabled       = true;
    data.midiNotes     = std::move(notes);
    data.lengthSeconds = lengthSeconds;

    auto* widget = new TrackWidget(data, this);
    wireTrackWidget(widget);
    m_trackWidgets.append(widget);
    m_tracksLayout->insertWidget(m_tracksLayout->count() - 1, widget);

    markDirty();
    updatePlayRecordButton();
}

QString MainWindow::allocateUniqueDrumTrackName() const
{
    const QString base = tr("Drums");
    QSet<QString> existing;
    for (auto* w : m_trackWidgets)
        existing.insert(w->trackData().name);
    QString candidate = base;
    int     n           = 2;
    while (existing.contains(candidate))
        candidate = base + QLatin1Char(' ') + QString::number(n++);
    return candidate;
}

// ─── Drag-and-drop of audio files ─────────────────────────────────────────
//
// While a project is open, the user may drag one or more supported audio
// files — see src/audioformats.h for the central
// list) onto the main window.  Each accepted file is copied into the project
// directory if it isn't already there, and a new audio track is created with
// the file's base name as the track name and the file's audio data preloaded.

static bool mimeHasSupportedAudioFile(const QMimeData* mime)
{
    if (!mime || !mime->hasUrls())
        return false;
    for (const QUrl& url : mime->urls())
    {
        if (!url.isLocalFile())
            continue;
        if (AudioFormats::isSupported(url.toLocalFile()))
            return true;
    }
    return false;
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    // Only accept drops while a project is open AND at least one of the
    // dragged files is a supported audio format.
    if (!m_projectLocationEdit
        || m_projectLocationEdit->text().trimmed().isEmpty()
        || m_isActive)
    {
        event->ignore();
        return;
    }
    if (mimeHasSupportedAudioFile(event->mimeData()))
        event->acceptProposedAction();
    else
        event->ignore();
}

void MainWindow::dragMoveEvent(QDragMoveEvent* event)
{
    if (mimeHasSupportedAudioFile(event->mimeData())
        && m_projectLocationEdit
        && !m_projectLocationEdit->text().trimmed().isEmpty()
        && !m_isActive)
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mime = event->mimeData();
    if (!mime || !mime->hasUrls())
    {
        event->ignore();
        return;
    }

    bool addedAny = false;
    QStringList skipped;
    for (const QUrl& url : mime->urls())
    {
        if (!url.isLocalFile())
            continue;
        const QString path = url.toLocalFile();
        if (!AudioFormats::isSupported(path))
        {
            skipped.append(QFileInfo(path).fileName());
            continue;
        }
        if (addAudioFileAsTrack(path))
            addedAny = true;
    }

    if (addedAny)
        event->acceptProposedAction();
    else
        event->ignore();

    if (!skipped.isEmpty())
    {
        QMessageBox::information(this,
            tr("Unsupported file type"),
            tr("The following file(s) were skipped because they are not in a "
               "supported audio format:\n%1\n\nSupported formats: %2")
                .arg(skipped.join(QStringLiteral("\n")),
                     AudioFormats::extensions().join(QStringLiteral(", "))));
    }
}

bool MainWindow::addAudioFileAsTrack(const QString& sourcePath)
{
    if (!AudioFormats::isSupported(sourcePath))
        return false;

    const QString projectDir = m_projectLocationEdit
        ? m_projectLocationEdit->text().trimmed()
        : QString();
    if (projectDir.isEmpty())
        return false;

    const QFileInfo srcInfo(sourcePath);
    if (!srcInfo.exists() || !srcInfo.isFile())
        return false;

    // Use the file's base name (without extension) as the track name.
    const QString trackName = srcInfo.completeBaseName();
    const QString destFileName = AudioUtils::sanitizedTrackFilesystemName(trackName)
                                 + QStringLiteral(".")
                                 + srcInfo.suffix().toLower();
    const QString destPath = projectDir + QDir::separator() + destFileName;

    // Copy the file into the project dir if it isn't already there.
    if (QFileInfo(srcInfo.absoluteFilePath()) != QFileInfo(destPath))
    {
        QDir().mkpath(projectDir);
        if (QFile::exists(destPath))
        {
            const auto answer = QMessageBox::question(
                this,
                tr("File already exists"),
                tr("A file named \"%1\" already exists in the project "
                   "directory. Replace it?").arg(destFileName),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No);
            if (answer != QMessageBox::Yes)
                return false;
            QFile::remove(destPath);
        }
        if (!QFile::copy(sourcePath, destPath))
        {
            QMessageBox::warning(this,
                tr("Copy failed"),
                tr("Could not copy %1 into the project directory.").arg(srcInfo.fileName()));
            return false;
        }
    }

    // Build the new track and try to load the audio data so the waveform
    // shows up immediately.  If decoding fails the track is still created
    // (the file is in place; the user can investigate).
    TrackData data = createNewTrack();
    data.name = trackName;
    data.type = TrackType::Audio;
    int sr = data.sampleRate;
    int ch = data.channelCount;
    if (loadAudioFile(destPath, data.audioData, sr, ch))
    {
        data.sampleRate   = sr;
        data.channelCount = ch;
    }

    auto* widget = new TrackWidget(data, this);
    wireTrackWidget(widget);
    m_trackWidgets.append(widget);
    m_tracksLayout->insertWidget(m_tracksLayout->count() - 1, widget);
    widget->updateVisualization();

    markDirty();
    updatePlayRecordButton();
    return true;
}

void MainWindow::onArmChanged(TrackWidget* armed, bool isArmed)
{
    if (isArmed)
    {
        // Uncheck Arm on all other tracks (keep them enabled so the user can switch)
        for (auto* w : m_trackWidgets)
        {
            if (w != armed)
                w->setArmed(false);
        }
    }
    updatePlayRecordButton();
}

void MainWindow::onTrackConfigRequested(TrackWidget* widget)
{
    TrackConfigDialog dlg(this);
    dlg.setFromTrackData(widget->trackData());
    if (dlg.exec() == QDialog::Accepted)
    {
        TrackData data = widget->trackData();
        data.type = dlg.trackType();
        dlg.applyTrimToTrackData(data);
        widget->setTrackData(data);
        markDirty();
    }
}

void MainWindow::onTrackEffectsRequested(TrackWidget* widget)
{
    if (widget == nullptr || widget->trackData().type != TrackType::Audio)
        return;
    EffectsDialog dlg(widget, this);
    dlg.exec();
}

void MainWindow::onClose()
{
    close();
}

void MainWindow::onMix()
{
    QVector<TrackData> tracks;
    for (auto* w : m_trackWidgets)
        tracks.append(w->trackData());

    MixDialog dlg(effectiveProjectPath(), this);
    if (dlg.exec() != QDialog::Accepted) return;

    const QString outputPath = dlg.outputFilename();
    if (outputPath.isEmpty()) return;

    const std::optional<MixExportEncodeSettings> mixEnc = dlg.mixEncodeSettings();

    const QString projectPath = effectiveProjectPath();
    const int volPct =
        (m_projectSettings.midiVolumePercent >= 0)
            ? m_projectSettings.midiVolumePercent
            : AppSettings::instance().midiVolumePercent();
    const double midiGainMultiplier = std::clamp(volPct, 0, 200) / 100.0;

    const int sampleRate        = m_projectSettings.sampleRate;
    const QString soundFontPath = m_projectSettings.soundFontPath;
    const QJsonArray auxChain   = m_projectSettings.auxEffectChain;

    QProgressDialog progress(tr("Mixing tracks to file…"), QString(), 0, 0, this);
    progress.setWindowTitle(tr("Mix in progress"));
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setMinimumDuration(0);
    progress.setCancelButton(nullptr);
    progress.setRange(0, 0);

    QFuture<bool> future =
        QtConcurrent::run([tracks, outputPath, projectPath, sampleRate, soundFontPath,
                           midiGainMultiplier, auxChain, mixEnc]()
                            {
                                const MixExportEncodeSettings* encPtr =
                                    mixEnc.has_value() ? &mixEnc.value() : nullptr;
                                return AudioUtils::mixTracksToFile(tracks, outputPath, projectPath,
                                                                   sampleRate, soundFontPath,
                                                                   /*renderMidiToAudio=*/true,
                                                                   midiGainMultiplier, auxChain,
                                                                   encPtr);
                            });

    QEventLoop waitLoop;
    QFutureWatcher<bool> watcher;
    QObject::connect(&watcher, &QFutureWatcher<bool>::finished, &waitLoop, &QEventLoop::quit);
    watcher.setFuture(future);
    progress.show();
    waitLoop.exec();

    progress.close();

    const bool mixOk = future.result();
    if (mixOk)
    {
        applyMixBusEffectsToPathIfSet(outputPath, m_projectSettings.mixEffectChain);
        QMessageBox::information(this, tr("Mix Complete"),
            tr("Audio exported successfully to:\n%1").arg(outputPath));
    }
    else
    {
        QMessageBox::warning(this, tr("Mix Failed"),
            tr("Could not export audio. Please check that you have at least one "
               "enabled track with content, and a SoundFont configured for MIDI tracks."));
    }
}

void MainWindow::onBrowseProjectLocation()
{
    const QString current = m_projectLocationEdit->text().trimmed();
    const QString startDir = current.isEmpty() ? QDir::homePath() : current;
    const QString dir = QFileDialog::getExistingDirectory(
        this, tr("Select Project Directory"), startDir,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty()) return;
    const QString oldDir = AppSettings::instance().projectLocation();
    if (dir != oldDir)
        moveProjectFiles(oldDir, dir);
    m_projectLocationEdit->setText(dir);
    AppSettings::instance().setProjectLocation(dir);
    AppSettings::instance().save();
    updateToolBarState();
}

void MainWindow::moveProjectFiles(const QString& oldDir, const QString& newDir)
{
    if (oldDir.isEmpty() || newDir.isEmpty() || oldDir == newDir) return;
    QDir src(oldDir);
    if (!src.exists()) return;
    QDir().mkpath(newDir);
    const QStringList entries = src.entryList(AudioFormats::nameFilters(), QDir::Files);
    for (const QString& filename : entries)
    {
        const QString srcPath = oldDir + QDir::separator() + filename;
        const QString dstPath = newDir + QDir::separator() + filename;
        QFile::rename(srcPath, dstPath);
    }
}

void MainWindow::onPlayRecord()
{
    if (m_isActive)
    {
        stopPlaybackOrRecording();
    }
    else if (isAnyTrackArmed())
    {
        startRecording();
    }
    else
    {
        startPlayback();
    }
}

void MainWindow::onPlaybackFinished()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
#    if defined(HAVE_PORTAUDIO)
    // PortAudio capture does not use m_audioSource; overdub still uses m_player.
    if (m_recordingUsesPortAudio && m_portAudioRecorder != nullptr)
    {
        if (!m_playbackTempFile.isEmpty())
        {
            QFile::remove(m_playbackTempFile);
            m_playbackTempFile.clear();
        }
        return;
    }
#    endif
    // If recording is still in progress the player just finished the overdub backing track —
    // don't stop the recording; just clean up the temp file.
    if (m_audioSource != nullptr)
    {
        if (!m_playbackTempFile.isEmpty())
        {
            QFile::remove(m_playbackTempFile);
            m_playbackTempFile.clear();
        }
        return;
    }
#endif
    stopPlaybackOrRecording();
}

bool MainWindow::isAnyTrackArmed() const
{
    for (auto* w : m_trackWidgets)
        if (w->isArmed()) return true;
    return false;
}

TrackWidget* MainWindow::armedTrack() const
{
    for (auto* w : m_trackWidgets)
        if (w->isArmed()) return w;
    return nullptr;
}

QString MainWindow::effectiveProjectPath() const
{
    const QString loc = m_projectLocationEdit != nullptr
                        ? m_projectLocationEdit->text().trimmed()
                        : QString();
    return loc.isEmpty() ? QDir::tempPath() : loc;
}

static bool isAnyTrackEnabledInList(const QVector<TrackWidget*>& widgets)
{
    for (auto* w : widgets)
        if (w->trackData().enabled) return true;
    return false;
}

void MainWindow::updatePlayRecordButton()
{
    if (m_playRecordButton == nullptr) return;
    if (m_isActive)
    {
        m_playRecordButton->setIcon(m_stopIcon);
        m_playRecordButton->setToolTip(tr("Stop"));
        m_playRecordButton->setEnabled(true);
    }
    else if (isAnyTrackArmed())
    {
        m_playRecordButton->setIcon(m_recordIcon);
        m_playRecordButton->setToolTip(tr("Record armed track"));
        m_playRecordButton->setEnabled(true);
    }
    else
    {
        m_playRecordButton->setIcon(m_playIcon);
        m_playRecordButton->setToolTip(tr("Play all tracks"));
        m_playRecordButton->setEnabled(isAnyTrackEnabledInList(m_trackWidgets));
    }
}

void MainWindow::startPlayback()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    QVector<TrackData> tracks;
    for (auto* w : m_trackWidgets)
        tracks.append(w->trackData());

    const int effectiveMidiDeviceIndex =
        (m_projectSettings.midiDeviceIndex >= 0)
            ? m_projectSettings.midiDeviceIndex
            : AppSettings::instance().midiDeviceIndex();

    const int effectiveMidiVolumePercent =
        (m_projectSettings.midiVolumePercent >= 0)
            ? m_projectSettings.midiVolumePercent
            : AppSettings::instance().midiVolumePercent();
    const double midiGainMultiplier =
        std::clamp(effectiveMidiVolumePercent, 0, 200) / 100.0;

    const bool hasEnabledMidi =
        std::any_of(tracks.begin(), tracks.end(),
                    [&tracks](const TrackData& t)
                    {
                        return trackShouldMix(t, tracks) && t.type == TrackType::MIDI && (!t.midiNotes.isEmpty() || !t.midiControlChanges.isEmpty());
                    });

    const bool hasEnabledAudio =
        std::any_of(tracks.begin(), tracks.end(),
                    [&tracks](const TrackData& t)
                    {
                        return trackShouldMix(t, tracks) && t.type == TrackType::Audio
                               && !t.audioData.isEmpty();
                    });

    // Project option: by default, render MIDI to audio for playback so MIDI can be
    // heard along with audio tracks. If disabled, send MIDI out in real time.
    const bool useRealtimeMidiOut =
        (!m_projectSettings.renderMidiToAudioForPlayback)
        && (effectiveMidiDeviceIndex > 0)
        && hasEnabledMidi;

    // Mix enabled AUDIO tracks (and optionally offline-render MIDI) to a temporary file.
    QTemporaryFile* tmpFile = new QTemporaryFile(
        QDir::temp().filePath("mc_play_XXXXXX.wav"), this);
    tmpFile->setAutoRemove(false);
    if (!tmpFile->open())
    {
        QMessageBox::warning(this, tr("Playback Error"), tr("Could not create temporary file."));
        return;
    }
    m_playbackTempFile = tmpFile->fileName();
    tmpFile->close();

    const bool mixed = AudioUtils::mixTracksToFile(tracks, m_playbackTempFile, effectiveProjectPath(),
                                                   m_projectSettings.sampleRate,
                                                   m_projectSettings.soundFontPath,
                                                   /*renderMidiToAudio=*/!useRealtimeMidiOut,
                                                   /*midiGainMultiplier=*/midiGainMultiplier,
                                                   m_projectSettings.auxEffectChain);

    // If we're doing realtime MIDI-out playback and there are no audio tracks,
    // mixing will (correctly) produce no input files. In that case, proceed with
    // MIDI-only playback without an audio file.
    if (!mixed && !(useRealtimeMidiOut && !hasEnabledAudio))
    {
        QMessageBox::warning(this, tr("Playback Error"),
            tr("Could not mix tracks for playback. Make sure tracks have content."));
        QFile::remove(m_playbackTempFile);
        m_playbackTempFile.clear();
        return;
    }

    if (useRealtimeMidiOut)
    {
        m_midiPlayer = std::make_unique<MidiPlayer>(this);
        connect(m_midiPlayer.get(), &MidiPlayer::finished, this,
            [this]()
            {
                // If audio playback is active, let it drive stop; otherwise
                // end the MIDI-only playback session.
                if (m_player == nullptr || m_player->playbackState() == QMediaPlayer::StoppedState)
                    onPlaybackFinished();
            });

        if (!m_midiPlayer->start(effectiveMidiDeviceIndex, tracks, effectiveMidiVolumePercent))
        {
            // Fall back to offline MIDI render for playback if MIDI-out cannot start.
            m_midiPlayer.reset();

            // Re-mix including MIDI synthesis so playback still has MIDI audio.
            if (!AudioUtils::mixTracksToFile(tracks, m_playbackTempFile, effectiveProjectPath(),
                                             m_projectSettings.sampleRate,
                                             m_projectSettings.soundFontPath,
                                             /*renderMidiToAudio=*/true,
                                             /*midiGainMultiplier=*/midiGainMultiplier,
                                             m_projectSettings.auxEffectChain))
            {
                QMessageBox::warning(this, tr("Playback Error"),
                    tr("Could not mix tracks for playback. Make sure tracks have content."));
                QFile::remove(m_playbackTempFile);
                m_playbackTempFile.clear();
                return;
            }
        }
    }

    if (m_player == nullptr)
    {
        m_player = new QMediaPlayer(this);
        m_audioOutput = new QAudioOutput(this);
        m_player->setAudioOutput(m_audioOutput);
        connect(m_player, &QMediaPlayer::playbackStateChanged, this,
            [this](QMediaPlayer::PlaybackState state)
            {
                if (state == QMediaPlayer::StoppedState && m_isActive)
                {
                    // When realtime MIDI-out playback is active, the mixed audio file
                    // can legitimately end before the MIDI timeline (e.g. MIDI starts
                    // later than the audio). In that case, don't stop the session yet.
                    if (m_midiPlayer && m_midiPlayer->isRunning())
                        return;
                    onPlaybackFinished();
                }
            });
        connect(
            m_player, &QMediaPlayer::positionChanged, this,
            [this](qint64 posMs)
            {
                if (!m_isActive || !m_projectSettings.loopPlaybackEnabled || !m_player)
                    return;
                const double endSec = m_projectSettings.loopEndSec;
                const double startSec = m_projectSettings.loopStartSec;
                if (endSec <= startSec)
                    return;
                if (posMs >= static_cast<qint64>(endSec * 1000.0))
                    m_player->setPosition(static_cast<qint64>(startSec * 1000.0));
            },
            Qt::UniqueConnection);
    }
    if (!m_playbackTempFile.isEmpty() && QFile::exists(m_playbackTempFile))
    {
        applyMixBusEffectsToPathIfSet(m_playbackTempFile, m_projectSettings.mixEffectChain);
        m_player->setSource(QUrl::fromLocalFile(m_playbackTempFile));
        m_player->play();
    }

    m_isActive = true;
    m_activeTimer.start();
    if (!m_timeDisplayTimer)
    {
        m_timeDisplayTimer = new QTimer(this);
        connect(m_timeDisplayTimer, &QTimer::timeout, this, &MainWindow::onTimeDisplayTick);
    }
    m_timeDisplayTimer->start(10);  // ~100 fps for smooth hundredths display
    for (auto* w : m_trackWidgets) w->setInteractiveControlsEnabled(false);
    updatePlayRecordButton();
#else
    QMessageBox::information(this, tr("Playback"),
        tr("Qt Multimedia is not available. Build with Qt6::Multimedia for playback support."));
#endif
}

void MainWindow::startRecording()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    TrackWidget* armed = armedTrack();
    if (armed == nullptr) return;

    // Require an explicit project directory before recording
    if (m_projectLocationEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, tr("Project Directory Required"),
            tr("You must choose a project directory before recording.\n"
               "Please select a directory to store your recorded files."));
        onBrowseProjectLocation();
        if (m_projectLocationEdit->text().trimmed().isEmpty())
            return;  // User cancelled the directory chooser
    }

    // Branch on track type: MIDI tracks capture from the configured MIDI
    // input port; audio tracks capture from the configured audio input device.
    const bool isMidiTrack = (armed->trackData().type == TrackType::MIDI);

    if (isMidiTrack)
    {
        // For MIDI tracks, we need an RtMidiIn port name.  If none is
        // configured in the project settings, prompt the user.
        if (m_projectSettings.midiInputPortName.isEmpty())
        {
            QMessageBox::warning(this, tr("Recording Error"),
                tr("No MIDI input device is configured for this project. "
                   "Open Project Settings and choose a MIDI input device "
                   "before recording a MIDI track."));
            return;
        }
    }
    else
    {
        // Audio track: verify an audio input device is available
        QAudioDevice inputDevice;
        if (!m_projectSettings.audioInputDeviceId.isEmpty())
        {
            const auto inputs = QMediaDevices::audioInputs();
            for (const auto& dev : inputs)
            {
                if (dev.id() == m_projectSettings.audioInputDeviceId)
                {
                    inputDevice = dev;
                    break;
                }
            }
        }
        if (inputDevice.isNull())
            inputDevice = QMediaDevices::defaultAudioInput();
        if (inputDevice.isNull())
        {
            QMessageBox::warning(this, tr("Recording Error"),
                tr("No audio input device found. Configure one in Settings."));
            return;
        }
    }

    // Disable controls and show countdown on the armed track.  The LED time
    // display is NOT started here — it begins in beginRecordingAfterCountdown()
    // once recording actually starts.
    m_isActive       = true;
    m_recordingMidi  = isMidiTrack;
    for (auto* w : m_trackWidgets) w->setInteractiveControlsEnabled(false);
    updatePlayRecordButton();

    m_countdownRemaining = 3;
    armed->setRecordingStatus(tr("Get ready...  %1").arg(m_countdownRemaining));

    m_countdownTimer = new QTimer(this);
    connect(m_countdownTimer, &QTimer::timeout, this, [this]()
    {
        TrackWidget* armed = armedTrack();
        --m_countdownRemaining;
        if (m_countdownRemaining > 0)
        {
            if (armed)
                armed->setRecordingStatus(tr("Get ready...  %1").arg(m_countdownRemaining));
        }
        else
        {
            m_countdownTimer->stop();
            m_countdownTimer->deleteLater();
            m_countdownTimer = nullptr;
            if (armed)
                armed->clearRecordingStatus();
            beginRecordingAfterCountdown();
        }
    });
    m_countdownTimer->start(1000);
#else
    QMessageBox::information(this, tr("Recording"),
        tr("Qt Multimedia is not available. Build with Qt6::Multimedia for recording support."));
#endif
}

#ifdef QT_MULTIMEDIA_AVAILABLE
void MainWindow::prepareOverdubPlayback()
{
    QVector<TrackData> overdubTracks;
    for (auto* w : m_trackWidgets)
    {
        const TrackData& td = w->trackData();
        if (!w->isArmed() && td.enabled)
            overdubTracks.append(td);
    }
    if (overdubTracks.isEmpty())
        return;

    // Mix existing tracks to a temporary WAV file.  This blocking I/O
    // must complete BEFORE capture starts so that the mix is ready to
    // play the instant recording begins — otherwise the capture would
    // record dead time while the file is being written.
    QTemporaryFile* tmpFile = new QTemporaryFile(
        QDir::temp().filePath("mc_overdub_XXXXXX.wav"), this);
    tmpFile->setAutoRemove(false);
    if (!tmpFile->open())
        return;
    m_playbackTempFile = tmpFile->fileName();
    tmpFile->close();
    const int volPct =
        (m_projectSettings.midiVolumePercent >= 0)
            ? m_projectSettings.midiVolumePercent
            : AppSettings::instance().midiVolumePercent();
    const double midiGainMultiplier = std::clamp(volPct, 0, 200) / 100.0;
    const bool mixed = AudioUtils::mixTracksToFile(
        overdubTracks, m_playbackTempFile,
        effectiveProjectPath(),
        m_projectSettings.sampleRate,
        m_projectSettings.soundFontPath,
        /*renderMidiToAudio=*/true,
        /*midiGainMultiplier=*/midiGainMultiplier,
        m_projectSettings.auxEffectChain);
    if (mixed)
    {
        if (m_player == nullptr)
        {
            m_player = new QMediaPlayer(this);
            m_audioOutput = new QAudioOutput(this);
            m_player->setAudioOutput(m_audioOutput);
            connect(m_player, &QMediaPlayer::playbackStateChanged, this,
                [this](QMediaPlayer::PlaybackState state)
                {
                    if (state == QMediaPlayer::StoppedState && m_isActive)
                        onPlaybackFinished();
                });
        }

        // Pre-load the media source so QMediaPlayer buffers it.  When
        // startOverdubPlayback() calls play(), audio output will begin
        // with minimal latency.
        applyMixBusEffectsToPathIfSet(m_playbackTempFile, m_projectSettings.mixEffectChain);
        m_player->setSource(QUrl::fromLocalFile(m_playbackTempFile));
    }
    else
    {
        QFile::remove(m_playbackTempFile);
        m_playbackTempFile.clear();
    }
}

void MainWindow::startOverdubPlayback()
{
    if (m_player == nullptr || m_playbackTempFile.isEmpty())
        return;
    // Trigger playback right after capture has started so recording and
    // playback begin at approximately the same instant.
    m_player->play();
}

void MainWindow::startRecordingLevelMeter()
{
    if (m_recordingLevelTimer != nullptr)
    {
        m_recordingLevelTimer->stop();
        m_recordingLevelTimer->deleteLater();
        m_recordingLevelTimer = nullptr;
    }
    m_recordingLevelTimer = new QTimer(this);
    connect(m_recordingLevelTimer, &QTimer::timeout, this, [this]()
    {
        if (!m_isActive) return;
        TrackWidget* armed = armedTrack();
        if (armed == nullptr) return;

#if defined(HAVE_PORTAUDIO)
        if (m_recordingUsesPortAudio && m_portAudioRecorder != nullptr)
        {
            armed->setRecordingLevel(m_portAudioRecorder->recentPeak());
            return;
        }
#endif
        if (m_recordWriteDevice == nullptr) return;
        const QByteArray& buf = m_recordWriteDevice->recordedData();
        if (buf.isEmpty())
        {
            armed->setRecordingLevel(0.0f);
            return;
        }

        float peak = 0.0f;
        const auto recFmt = static_cast<QAudioFormat::SampleFormat>(m_recordingSampleFormat);
        if (recFmt == QAudioFormat::Float)
        {
            const int windowBytes = qMin(buf.size(),
                                         m_recordingChannelCount * static_cast<int>(sizeof(float)) * 2048);
            const auto* samples = reinterpret_cast<const float*>(
                buf.constData() + buf.size() - windowBytes);
            const int n = windowBytes / static_cast<int>(sizeof(float));
            for (int i = 0; i < n; ++i)
                peak = qMax(peak, qAbs(samples[i]));
        }
        else if (recFmt == QAudioFormat::Int32)
        {
            const int windowBytes = qMin(buf.size(),
                                         m_recordingChannelCount * static_cast<int>(sizeof(qint32)) * 2048);
            const auto* samples = reinterpret_cast<const qint32*>(
                buf.constData() + buf.size() - windowBytes);
            const int n = windowBytes / static_cast<int>(sizeof(qint32));
            for (int i = 0; i < n; ++i)
                peak = qMax(peak, qAbs(samples[i] / 2147483648.0f));
        }
        else if (recFmt == QAudioFormat::UInt8)
        {
            const int windowBytes = qMin(buf.size(), m_recordingChannelCount * 2048);
            const auto* samples = reinterpret_cast<const quint8*>(
                buf.constData() + buf.size() - windowBytes);
            for (int i = 0; i < windowBytes; ++i)
                peak = qMax(peak, qAbs((samples[i] - 128) / 128.0f));
        }
        else
        {
            const int windowBytes = qMin(buf.size(),
                                         m_recordingChannelCount * static_cast<int>(sizeof(qint16)) * 2048);
            const auto* samples = reinterpret_cast<const qint16*>(
                buf.constData() + buf.size() - windowBytes);
            const int n = windowBytes / static_cast<int>(sizeof(qint16));
            for (int i = 0; i < n; ++i)
                peak = qMax(peak, qAbs(samples[i] / 32768.0f));
        }
        armed->setRecordingLevel(qMin(peak, 1.0f));
    });
    m_recordingLevelTimer->start(50);
}

void MainWindow::finalizeRecordedAudio(const QByteArray& rawData, qint64 processedMicroseconds,
                                      const char* captureBackendLabel)
{
    TrackWidget* armed = armedTrack();
    if (armed == nullptr) return;

    const int reportedRate = m_recordingSampleRate;
    const int reportedCh   = m_recordingChannelCount;
    const double elapsedSec = m_recordingTimer.elapsed() / 1000.0;

    RecordingPostProcessParams rp;
    rp.rawBytes = rawData;
    rp.sampleFormat = m_recordingSampleFormat;
    rp.reportedChannels = reportedCh;
    rp.reportedSampleRate = reportedRate;
    rp.processedMicroseconds = processedMicroseconds;
    rp.wallClockSeconds = elapsedSec;
    rp.projectSampleRate = m_projectSettings.sampleRate;
    rp.projectChannelCount = m_projectSettings.channelCount;

    RecordingPostProcessResult rec = RecordingPostProcess::process(rp);

    const TrackData armedDataBeforeFx = armed->trackData();
    if (!armedDataBeforeFx.audioEffectChain.isEmpty())
        applyAudioEffectChain(rec.int16Pcm, rec.channelCount, rec.sampleRate,
                              armedDataBeforeFx.audioEffectChain);

    if (m_projectSettings.punchRecordingEnabled && !m_recordingMidi
        && m_projectSettings.punchOutSec > m_projectSettings.punchInSec
        && !armedDataBeforeFx.audioData.isEmpty())
    {
        rec.int16Pcm =
            spliceAudioPunchReplace(armedDataBeforeFx.audioData, rec.int16Pcm, rec.sampleRate,
                                    rec.channelCount, m_projectSettings.punchInSec,
                                    m_projectSettings.punchOutSec);
    }

    if (AppSettings::instance().recordingDebugLog())
    {
        QFile logFile(effectiveProjectPath() + QDir::separator()
                      + QStringLiteral("recording_debug.txt"));
        if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            QTextStream ts(&logFile);
            const int nFrames = rec.int16Pcm.size() / (rec.channelCount * 2);
            ts << "--- Recording Stop ---\n"
               << "captureBackend: " << (captureBackendLabel ? captureBackendLabel : "?") << "\n"
               << "reportedRate: " << reportedRate
               << "  reportedCh: " << reportedCh << "\n"
               << "processedUSecs: " << processedMicroseconds << "\n"
               << "throughputVsReported: " << rec.throughputVsReported << "\n"
               << "inferredCaptureRate: " << rec.inferredCaptureRate << "\n"
               << "projectRate: " << m_projectSettings.sampleRate
               << "  projectCh: " << m_projectSettings.channelCount << "\n"
               << "elapsedSec: " << elapsedSec << "\n"
               << "rawData.size: " << rawData.size() << "\n"
               << "outputFrames: " << nFrames
               << "  trackLength: "
               << (rec.sampleRate > 0 ? static_cast<double>(nFrames) / rec.sampleRate : 0)
               << " sec\n";
        }
    }

    TrackData data = armed->trackData();
    data.type         = TrackType::Audio;
    data.audioData    = rec.int16Pcm;
    data.sampleRate   = rec.sampleRate;
    data.channelCount = rec.channelCount;
    armed->setTrackData(data);
    markDirty();

    armed->setArmed(false);

    const QString flacPath = effectiveProjectPath() + QDir::separator() +
                             AudioUtils::sanitizedTrackFilesystemName(data.name) + ".flac";
    static_cast<void>(
        AudioUtils::writeAudioDataToFlac(rec.int16Pcm,
                                         rec.sampleRate,
                                         rec.channelCount,
                                         flacPath));
}

void MainWindow::applyLiveRecordingMonitorState()
{
    const bool want = m_projectSettings.monitorWhileRecording;

    if (m_recordingMidi && m_midiRecorder)
    {
        if (!want)
        {
            m_midiRecorder->setRawTap({});
            if (m_midiRecordingMonitor)
            {
                m_midiRecordingMonitor->stop();
                m_midiRecordingMonitor.reset();
            }
            return;
        }

        QString sfPath = m_projectSettings.soundFontPath;
        if (sfPath.isEmpty())
            sfPath = AppSettings::instance().soundFontPath();
        if (!m_projectSettings.renderMidiToAudioForPlayback || sfPath.isEmpty()
            || !QFile::exists(sfPath))
        {
            m_midiRecorder->setRawTap({});
            if (m_midiRecordingMonitor)
            {
                m_midiRecordingMonitor->stop();
                m_midiRecordingMonitor.reset();
            }
            return;
        }

        if (!m_midiRecordingMonitor)
        {
            m_midiRecordingMonitor = std::make_unique<MidiRecordingMonitor>(this);
            const QAudioDevice outDev = resolveAudioOutputDevice(
                m_projectSettings.audioOutputDeviceId,
                AppSettings::instance().audioOutputDeviceId());
            const int volPct =
                (m_projectSettings.midiVolumePercent >= 0)
                    ? m_projectSettings.midiVolumePercent
                    : AppSettings::instance().midiVolumePercent();
            const double gainMult = std::clamp(volPct, 0, 200) / 100.0;
            if (!m_midiRecordingMonitor->start(sfPath, m_projectSettings.sampleRate, outDev,
                                               gainMult))
            {
                m_midiRecordingMonitor.reset();
                m_midiRecorder->setRawTap({});
                return;
            }
        }

        m_midiRecorder->setRawTap(
            [this](const std::vector<unsigned char>& v)
            {
                if (m_midiRecordingMonitor)
                    m_midiRecordingMonitor->postMidiBytes(v);
            });
        return;
    }

#if defined(HAVE_PORTAUDIO)
    if (m_recordingUsesPortAudio && m_portAudioRecorder)
    {
        if (!want)
        {
            m_portAudioRecorder->clearMonitorTap();
            if (m_recordingAudioMonitor)
            {
                m_recordingAudioMonitor->stop();
                m_recordingAudioMonitor.reset();
            }
            return;
        }
        if (m_recordingAudioMonitor)
            return;

        QAudioFormat paFmt;
        paFmt.setSampleRate(m_recordingSampleRate);
        paFmt.setChannelCount(m_recordingChannelCount);
        paFmt.setSampleFormat(QAudioFormat::Int16);
        const QAudioDevice outDev = resolveAudioOutputDevice(
            m_projectSettings.audioOutputDeviceId,
            AppSettings::instance().audioOutputDeviceId());
        m_recordingAudioMonitor = std::make_unique<RecordingAudioMonitor>(this);
        if (m_recordingAudioMonitor->start(outDev, paFmt))
        {
            m_portAudioRecorder->setMonitorTap(
                [this](const char* p, int n)
                {
                    if (m_recordingAudioMonitor)
                        m_recordingAudioMonitor->enqueuePcm(p, n);
                });
        }
        else
            m_recordingAudioMonitor.reset();
        return;
    }
#endif

    if (m_audioSource != nullptr && m_recordWriteDevice != nullptr)
    {
        if (!want)
        {
            m_recordWriteDevice->setAudioMonitor(nullptr);
            if (m_recordingAudioMonitor)
            {
                m_recordingAudioMonitor->stop();
                m_recordingAudioMonitor.reset();
            }
            return;
        }
        if (m_recordingAudioMonitor)
            return;

        QAudioFormat format;
        format.setSampleRate(m_recordingSampleRate);
        format.setChannelCount(m_recordingChannelCount);
        format.setSampleFormat(static_cast<QAudioFormat::SampleFormat>(m_recordingSampleFormat));
        const QAudioDevice outDev = resolveAudioOutputDevice(
            m_projectSettings.audioOutputDeviceId,
            AppSettings::instance().audioOutputDeviceId());
        m_recordingAudioMonitor = std::make_unique<RecordingAudioMonitor>(this);
        if (m_recordingAudioMonitor->start(outDev, format))
            m_recordWriteDevice->setAudioMonitor(m_recordingAudioMonitor.get());
        else
            m_recordingAudioMonitor.reset();
    }
}

void MainWindow::stopRecordingMonitors()
{
#if defined(HAVE_PORTAUDIO)
    if (m_portAudioRecorder)
        m_portAudioRecorder->clearMonitorTap();
#endif
    if (m_recordingAudioMonitor)
    {
        m_recordingAudioMonitor->stop();
        m_recordingAudioMonitor.reset();
    }
    if (m_midiRecordingMonitor)
    {
        m_midiRecordingMonitor->stop();
        m_midiRecordingMonitor.reset();
    }
}
#endif  // QT_MULTIMEDIA_AVAILABLE

void MainWindow::syncMonitorCheckboxFromSettings()
{
    if (!m_monitorWhileRecordingCheck)
        return;
    const QSignalBlocker blocker(m_monitorWhileRecordingCheck);
    m_monitorWhileRecordingCheck->setChecked(m_projectSettings.monitorWhileRecording);
}

void MainWindow::beginRecordingAfterCountdown()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    const QString projectDir = effectiveProjectPath();
    QDir().mkpath(projectDir);

    // Start the LED time display now that the countdown is finished
    // and actual recording is about to begin.
    m_activeTimer.start();
    if (!m_timeDisplayTimer)
    {
        m_timeDisplayTimer = new QTimer(this);
        connect(m_timeDisplayTimer, &QTimer::timeout, this, &MainWindow::onTimeDisplayTick);
    }
    m_timeDisplayTimer->start(10);

    // ── MIDI recording branch ────────────────────────────────────────
    if (m_recordingMidi)
    {
        // Show overdub playback alongside MIDI capture, just like audio
        // recording does, so the user can hear existing tracks while
        // playing notes on their MIDI controller.
        prepareOverdubPlayback();

        std::function<void(const std::vector<unsigned char>&)> midiTap;
        if (m_projectSettings.monitorWhileRecording
            && m_projectSettings.renderMidiToAudioForPlayback)
        {
            QString sfPath = m_projectSettings.soundFontPath;
            if (sfPath.isEmpty())
                sfPath = AppSettings::instance().soundFontPath();
            if (!sfPath.isEmpty() && QFile::exists(sfPath))
            {
                m_midiRecordingMonitor = std::make_unique<MidiRecordingMonitor>(this);
                const QAudioDevice outDev = resolveAudioOutputDevice(
                    m_projectSettings.audioOutputDeviceId,
                    AppSettings::instance().audioOutputDeviceId());
                const int volPct =
                    (m_projectSettings.midiVolumePercent >= 0)
                        ? m_projectSettings.midiVolumePercent
                        : AppSettings::instance().midiVolumePercent();
                const double gainMult = std::clamp(volPct, 0, 200) / 100.0;
                if (m_midiRecordingMonitor->start(sfPath, m_projectSettings.sampleRate, outDev,
                                                  gainMult))
                {
                    midiTap = [this](const std::vector<unsigned char>& v)
                    {
                        if (m_midiRecordingMonitor)
                            m_midiRecordingMonitor->postMidiBytes(v);
                    };
                }
                else
                    m_midiRecordingMonitor.reset();
            }
        }

        m_midiRecorder = std::make_unique<MidiRecorder>();
        QString err;
        if (!m_midiRecorder->start(m_projectSettings.midiInputPortName, &err, midiTap))
        {
            QMessageBox::warning(this, tr("Recording Error"),
                tr("Could not open MIDI input port \"%1\":\n%2")
                    .arg(m_projectSettings.midiInputPortName, err));
            m_midiRecorder.reset();
            m_midiRecordingMonitor.reset();
            stopPlaybackOrRecording();
            return;
        }

        if (TrackWidget* armed = armedTrack())
            armed->setRecordingStatus(tr("\u25CF Recording"));

        startOverdubPlayback();

        // Metronome — for MIDI recording the audible tick is safe (no mic)
        startMetronomeIfEnabled();
        return;
    }

    // ── Audio recording branch (existing path) ───────────────────────
    // Prepare overdub playback (mix existing tracks, pre-load media)
    // BEFORE starting capture, so no dead time is recorded while the
    // mix file is being written to disk.
    prepareOverdubPlayback();

#if defined(HAVE_PORTAUDIO)
    // Always prefer PortAudio when available — it delivers reliable, accurate
    // PCM at the requested sample rate (like Audacity).  Qt Multimedia's
    // QAudioSource has known throughput issues on PipeWire/ALSA backends that
    // cause pitched-down or distorted recordings.  The useQtAudioInput flag
    // is only honored as a last-resort manual override.
    if (PortAudioRecorder::isCompiledWithPortAudio()
        && PortAudioRecorder::hasInputDevice())
    {
        int paDev = m_projectSettings.portAudioInputDeviceIndex;

        // If no PortAudio device is explicitly configured but the user
        // selected a Qt audio input device, try to find the matching
        // PortAudio device by name substring matching.
        if (paDev < 0 && !m_projectSettings.audioInputDeviceId.isEmpty())
        {
            // Decode the Qt device description for matching
            const auto inputs = QMediaDevices::audioInputs();
            QString qtDeviceName;
            for (const auto& dev : inputs)
            {
                if (dev.id() == m_projectSettings.audioInputDeviceId)
                {
                    qtDeviceName = dev.description();
                    break;
                }
            }
            // The Qt device ID on Linux/PipeWire is the ALSA source name,
            // e.g. "alsa_input.pci-0000_00_1f.3.analog-stereo".
            // PortAudio ALSA device names look like "HDA Intel PCH: ALC1220
            // Analog (hw:0,0)".  Match by checking if both refer to the
            // same ALSA card (e.g. "pci-0000_00_1f.3" ↔ "hw:0").
            const QString qtId = QString::fromUtf8(m_projectSettings.audioInputDeviceId);
            if (!qtDeviceName.isEmpty() || !qtId.isEmpty())
            {
                const int paCount = PortAudioRecorder::deviceCount();
                for (int i = 0; i < paCount; ++i)
                {
                    if (PortAudioRecorder::maxInputChannels(i) <= 0)
                        continue;
                    const QString paName = PortAudioRecorder::deviceName(i);

                    // Direct name substring match
                    if (!qtDeviceName.isEmpty()
                        && (paName.contains(qtDeviceName, Qt::CaseInsensitive)
                            || qtDeviceName.contains(paName, Qt::CaseInsensitive)))
                    {
                        paDev = i;
                        break;
                    }

                    // Match via ALSA hw: index.  The Qt ID contains the
                    // PCI path; the PortAudio name contains "(hw:N,M)".
                    // Both "analog-stereo" and "Analog" indicate the same
                    // interface.
                    if (qtId.contains(QStringLiteral("pci-"))
                        && paName.contains(QStringLiteral("(hw:"))
                        && (qtId.contains(QStringLiteral("analog"))
                            == paName.contains(QStringLiteral("Analog"), Qt::CaseInsensitive)))
                    {
                        // Extract hw:N from PortAudio name and card N from Qt ID
                        const auto hwMatch = QRegularExpression(QStringLiteral("hw:(\\d+)")).match(paName);
                        if (hwMatch.hasMatch())
                        {
                            const QString hwCard = hwMatch.captured(1);
                            // On PipeWire, card 0 maps to the first PCI audio device
                            // Prefer the first matching hw: device
                            paDev = i;
                            break;
                        }
                    }
                }
            }
        }

        m_portAudioRecorder = std::make_unique<PortAudioRecorder>();
        QString paErr;
        if (m_portAudioRecorder->start(paDev, m_projectSettings.channelCount,
                                       m_projectSettings.sampleRate, paErr))
        {
            m_recordingUsesPortAudio = true;
            m_recordingSampleRate   = m_portAudioRecorder->streamSampleRate();
            m_recordingChannelCount = m_portAudioRecorder->streamChannelCount();
            m_recordingSampleFormat = 2;  // Int16 (QAudioFormat::Int16)

            if (AppSettings::instance().recordingDebugLog())
            {
                const int logDev =
                    (paDev < 0) ? PortAudioRecorder::defaultInputDeviceIndex() : paDev;
                QFile logFile(projectDir + QDir::separator() + QStringLiteral("recording_debug.txt"));
                if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
                {
                    QTextStream ts(&logFile);
                    ts << "\n=== Recording Start ===\n"
                       << "captureBackend: PortAudio\n"
                       << "device: " << PortAudioRecorder::deviceName(logDev) << "\n"
                       << "paDeviceIndex: " << logDev << "\n"
                       << "streamSampleRate: " << m_recordingSampleRate << "\n"
                       << "streamChannels: " << m_recordingChannelCount << "\n"
                       << "sampleFormat: 2 (Int16)\n"
                       << "m_projectSettings.sampleRate: " << m_projectSettings.sampleRate << "\n";
                }
            }

            m_recordingTimer.start();

            if (m_projectSettings.monitorWhileRecording)
            {
                QAudioFormat paFmt;
                paFmt.setSampleRate(m_recordingSampleRate);
                paFmt.setChannelCount(m_recordingChannelCount);
                paFmt.setSampleFormat(QAudioFormat::Int16);
                const QAudioDevice outDev = resolveAudioOutputDevice(
                    m_projectSettings.audioOutputDeviceId,
                    AppSettings::instance().audioOutputDeviceId());
                m_recordingAudioMonitor = std::make_unique<RecordingAudioMonitor>(this);
                if (m_recordingAudioMonitor->start(outDev, paFmt))
                {
                    m_portAudioRecorder->setMonitorTap(
                        [this](const char* p, int n)
                        {
                            if (m_recordingAudioMonitor)
                                m_recordingAudioMonitor->enqueuePcm(p, n);
                        });
                }
                else
                    m_recordingAudioMonitor.reset();
            }

            startOverdubPlayback();
            startRecordingLevelMeter();

            // Metronome — silenced automatically while audio capture is
            // active so the tick can't bleed into the recording.
            startMetronomeIfEnabled();
            return;
        }

        QMessageBox::warning(
            this,
            tr("PortAudio"),
            tr("Could not start PortAudio recording (%1). Falling back to Qt Multimedia.")
                .arg(paErr));
        m_portAudioRecorder.reset();
        m_recordingUsesPortAudio = false;
    }
#endif

#if defined(HAVE_PORTAUDIO)
    m_recordingUsesPortAudio = false;
#endif
    // Use the device selected in project settings (if any), otherwise fall
    // back to the system default.  The old code always used
    // QMediaDevices::defaultAudioInput() which ignored the user's choice.
    QAudioDevice inputDevice;
    if (!m_projectSettings.audioInputDeviceId.isEmpty())
    {
        const auto inputs = QMediaDevices::audioInputs();
        for (const auto& dev : inputs)
        {
            if (dev.id() == m_projectSettings.audioInputDeviceId)
            {
                inputDevice = dev;
                break;
            }
        }
    }
    if (inputDevice.isNull())
        inputDevice = QMediaDevices::defaultAudioInput();

    // Use preferredFormat() as-is — this returns the device's native sample
    // rate, channel count, and sample type.  Recording at the device's native
    // rate is essential because some audio backends (e.g. PipeWire's PulseAudio
    // compatibility layer) report non-native rates as "supported" via
    // isFormatSupported() but do not actually resample the data.  When that
    // happens the raw audio arrives at the hardware's native rate but gets
    // labelled with the requested rate, causing playback to be too fast or too
    // slow.  Using preferredFormat() guarantees the metadata matches the data.
    //
    // The mixing pipeline already adapts to each track's actual sample rate
    // (see mixTracksToFile), so a recording at 32 000 Hz or 48 000 Hz will
    // play back correctly regardless of the project's configured rate.
    const QAudioFormat format = inputDevice.preferredFormat();

    // Capture the actual format fields that will be used.  These determine how
    // the raw bytes in m_recordBuffer must be interpreted when recording stops.
    m_recordingSampleRate   = format.sampleRate();
    m_recordingChannelCount = format.channelCount();
    m_recordingSampleFormat = static_cast<int>(format.sampleFormat());

    // Diagnostic: log the recording format
    if (AppSettings::instance().recordingDebugLog())
    {
        const QString projectDir = effectiveProjectPath();
        QFile logFile(projectDir + QDir::separator() + QStringLiteral("recording_debug.txt"));
        if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            QTextStream ts(&logFile);
            ts << "\n=== Recording Start ===\n"
               << "captureBackend: Qt Multimedia\n"
               << "device: " << inputDevice.description() << "\n"
               << "preferredFormat sampleRate: " << format.sampleRate() << "\n"
               << "preferredFormat channels: " << format.channelCount() << "\n"
               << "preferredFormat sampleFormat: " << static_cast<int>(format.sampleFormat()) << "\n"
               << "m_projectSettings.sampleRate: " << m_projectSettings.sampleRate << "\n";
        }
    }

    m_recordWriteDevice = new RecordingWriteDevice(this);
    if (m_projectSettings.monitorWhileRecording)
    {
        const QAudioDevice outDev = resolveAudioOutputDevice(
            m_projectSettings.audioOutputDeviceId,
            AppSettings::instance().audioOutputDeviceId());
        m_recordingAudioMonitor = std::make_unique<RecordingAudioMonitor>(this);
        if (m_recordingAudioMonitor->start(outDev, format))
            m_recordWriteDevice->setAudioMonitor(m_recordingAudioMonitor.get());
        else
            m_recordingAudioMonitor.reset();
    }
    m_recordWriteDevice->open(QIODevice::WriteOnly);

    m_audioSource = new QAudioSource(inputDevice, format, this);
    // Use the platform default buffer size.  Overriding with a small value
    // (e.g. 2048) causes massive data loss on devices that use 32-bit
    // sample formats (Int32/Float32), because the tiny buffer cannot hold
    // enough frames and the audio callback drops ~75% of the data.
    // Basically, DON'T DO THIS: m_audioSource->setBufferSize(AudioStartup::recommendedBufferBytes());

    m_audioSource->start(m_recordWriteDevice);
    m_recordingTimer.start();

    startOverdubPlayback();
    startRecordingLevelMeter();

    // Metronome — silenced automatically while audio capture is active
    // so the tick can't bleed into the recording.
    startMetronomeIfEnabled();
#endif
}

void MainWindow::stopPlaybackOrRecording()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    if (m_midiPlayer)
    {
        m_midiPlayer->stop();
        m_midiPlayer.reset();
    }

    // Cancel countdown if still running
    if (m_countdownTimer != nullptr)
    {
        m_countdownTimer->stop();
        m_countdownTimer->deleteLater();
        m_countdownTimer = nullptr;
    }
    for (auto* w : m_trackWidgets) w->clearRecordingStatus();

    // Stop the level-meter timer first so there are no in-flight callbacks
    if (m_recordingLevelTimer != nullptr)
    {
        m_recordingLevelTimer->stop();
        m_recordingLevelTimer->deleteLater();
        m_recordingLevelTimer = nullptr;
    }
    for (auto* w : m_trackWidgets) w->clearRecordingLevel();

    if (m_player != nullptr && m_player->playbackState() != QMediaPlayer::StoppedState)
        m_player->stop();

    // ── MIDI recording stop (before tearing down monitor sinks) ───
    if (m_recordingMidi)
    {
        if (m_midiRecorder)
        {
            double lengthSec = 0.0;
            const QVector<MidiNote> notes = m_midiRecorder->stop(&lengthSec);
            m_midiRecorder.reset();

            if (TrackWidget* armed = armedTrack())
            {
                TrackData data = armed->trackData();
                data.type          = TrackType::MIDI;
                data.midiNotes     = notes;
                data.lengthSeconds = lengthSec;
                armed->setTrackData(data);
                armed->setArmed(false);
                markDirty();
            }
        }
        m_recordingMidi = false;
    }

    stopRecordingMonitors();

#if defined(HAVE_PORTAUDIO)
    if (m_recordingUsesPortAudio && m_portAudioRecorder != nullptr)
    {
        m_portAudioRecorder->stop();
        const qint64 processedMicroseconds = m_portAudioRecorder->processedMicroseconds();
        const QByteArray rawData = m_portAudioRecorder->takeRecordedPcm();
        m_portAudioRecorder.reset();
        m_recordingUsesPortAudio = false;
        if (!rawData.isEmpty())
            finalizeRecordedAudio(rawData, processedMicroseconds, "PortAudio");
    }
#endif

    if (m_audioSource != nullptr)
    {
        m_audioSource->stop();
        const qint64 processedMicroseconds = m_audioSource->processedUSecs();

        if (m_recordWriteDevice != nullptr)
        {
            const QByteArray rawData = m_recordWriteDevice->recordedData();
            if (!rawData.isEmpty())
                finalizeRecordedAudio(rawData, processedMicroseconds, "Qt Multimedia");
            delete m_recordWriteDevice;
            m_recordWriteDevice = nullptr;
        }
        delete m_audioSource;
        m_audioSource = nullptr;
    }

    if (!m_playbackTempFile.isEmpty())
    {
        QFile::remove(m_playbackTempFile);
        m_playbackTempFile.clear();
    }
#endif
    m_isActive = false;
    // Stop time display and metronome timers
    if (m_timeDisplayTimer)
        m_timeDisplayTimer->stop();
    if (m_metronomeTimer)
    {
        m_metronomeTimer->stop();
        m_metronomeTimer->disconnect();
    }
    for (auto* w : m_trackWidgets) w->setInteractiveControlsEnabled(true);
    updatePlayRecordButton();  // reflects disarmed state after recording
}

void MainWindow::onSaveProject()
{
    const QString projectDir = m_projectLocationEdit->text().trimmed();
    if (projectDir.isEmpty())
    {
        QMessageBox::warning(this, tr("Save Project"),
            tr("Please choose a project directory before saving."));
        onBrowseProjectLocation();
        if (m_projectLocationEdit->text().trimmed().isEmpty()) return;
    }

    QJsonArray tracksArray;
    for (auto* w : m_trackWidgets)
    {
        const TrackData& d = w->trackData();
        QJsonObject obj;
        obj[QStringLiteral("id")]            = d.id;
        obj[QStringLiteral("name")]          = d.name;
        obj[QStringLiteral("type")]          = (d.type == TrackType::MIDI)
                                                ? QStringLiteral("MIDI")
                                                : QStringLiteral("Audio");
        obj[QStringLiteral("enabled")]       = d.enabled;
        obj[QStringLiteral("armed")]         = d.armed;
        obj[QStringLiteral("sampleRate")]    = d.sampleRate;
        obj[QStringLiteral("channelCount")]  = d.channelCount;
        obj[QStringLiteral("lengthSeconds")] = d.lengthSeconds;

        obj[QStringLiteral("gainDb")] = static_cast<double>(d.gainDb);
        obj[QStringLiteral("pan")] = static_cast<double>(d.pan);
        obj[QStringLiteral("mute")] = d.mute;
        obj[QStringLiteral("solo")] = d.solo;
        obj[QStringLiteral("auxSend")] = static_cast<double>(d.auxSend);
        obj[QStringLiteral("trimStartSec")] = d.trimStartSec;
        obj[QStringLiteral("trimEndSec")] = d.trimEndSec;

        obj[QStringLiteral("audioEffectChain")] = d.audioEffectChain;

        if (d.type == TrackType::Audio)
        {
            // Reference whichever supported audio file exists.  The central
            // AudioFormats::all() list is checked in order, so the preferred
            // format (currently .flac) wins when more than one exists; older
            // projects with .wav still resolve correctly.
            const QString baseName = AudioUtils::sanitizedTrackFilesystemName(d.name);
            QString audioExt = QString::fromLatin1(AudioFormats::all().first().extension);
            for (const auto& fmt : AudioFormats::all())
            {
                const QString candidate = baseName + QString::fromLatin1(fmt.extension);
                if (QFile::exists(projectDir + QDir::separator() + candidate))
                {
                    audioExt = QString::fromLatin1(fmt.extension);
                    break;
                }
            }
            obj[QStringLiteral("audioFile")] = baseName + audioExt;
        }
        else
        {
            // MIDI tracks: save the notes to a Standard MIDI File
            // (<sanitized_name>.mid) in the project directory and reference
            // it from the project JSON via the "midiFile" property.  The
            // inline midiNotes array is no longer written; older projects
            // that still have it inline are still readable on load.
            const QString midiBase = AudioUtils::sanitizedTrackFilesystemName(d.name);
            const QString midiFileName = midiBase + QStringLiteral(".mid");
            const QString midiFullPath =
                m_projectLocationEdit->text().trimmed()
                + QDir::separator() + midiFileName;
            if (MidiFile::write(midiFullPath, d.midiNotes, d.lengthSeconds, d.midiControlChanges))
            {
                obj[QStringLiteral("midiFile")] = midiFileName;
            }
            else
            {
                qWarning() << "Failed to write MIDI file" << midiFullPath
                           << "- falling back to inline midiNotes for track" << d.id;
                QJsonArray notesArray;
                for (const MidiNote& n : d.midiNotes)
                {
                    QJsonObject no;
                    no[QStringLiteral("note")]      = n.note;
                    no[QStringLiteral("velocity")]  = n.velocity;
                    no[QStringLiteral("channel")]   = n.channel;
                    no[QStringLiteral("startTime")] = n.startTime;
                    no[QStringLiteral("duration")]  = n.duration;
                    notesArray.append(no);
                }
                obj[QStringLiteral("midiNotes")] = notesArray;
                QJsonArray ccArr;
                for (const MidiControlChange& cc : d.midiControlChanges)
                {
                    QJsonObject co;
                    co[QStringLiteral("t")] = cc.timeSec;
                    co[QStringLiteral("ch")] = cc.channel;
                    co[QStringLiteral("c")] = cc.controller;
                    co[QStringLiteral("v")] = cc.value;
                    ccArr.append(co);
                }
                obj[QStringLiteral("midiControlChanges")] = ccArr;
            }
        }
        tracksArray.append(obj);
    }

    QJsonObject psObj;
    psObj[QStringLiteral("midiDeviceIndex")]     = m_projectSettings.midiDeviceIndex;
    psObj[QStringLiteral("midiInputPortName")]   = m_projectSettings.midiInputPortName;
    psObj[QStringLiteral("soundFontPath")]       = m_projectSettings.soundFontPath;
    psObj[QStringLiteral("midiVolumePercent")]   = m_projectSettings.midiVolumePercent;
    psObj[QStringLiteral("renderMidiToAudioForPlayback")] = m_projectSettings.renderMidiToAudioForPlayback;
    psObj[QStringLiteral("audioInputDeviceId")]  =
        QString::fromLatin1(m_projectSettings.audioInputDeviceId.toHex());
    psObj[QStringLiteral("audioOutputDeviceId")] =
        QString::fromLatin1(m_projectSettings.audioOutputDeviceId.toHex());
    psObj[QStringLiteral("sampleRate")]          = m_projectSettings.sampleRate;
    psObj[QStringLiteral("channelCount")]        = m_projectSettings.channelCount;
    psObj[QStringLiteral("useQtAudioInput")] = m_projectSettings.useQtAudioInput;
    psObj[QStringLiteral("portAudioInputDeviceIndex")] =
        m_projectSettings.portAudioInputDeviceIndex;
    psObj[QStringLiteral("monitorWhileRecording")] =
        m_projectSettings.monitorWhileRecording;
    psObj[QStringLiteral("mixEffectChain")] = m_projectSettings.mixEffectChain;
    psObj[QStringLiteral("auxEffectChain")] = m_projectSettings.auxEffectChain;
    QJsonArray tempoArr;
    for (const TempoMarker& m : m_projectSettings.tempoMarkers)
    {
        QJsonObject mo;
        mo[QStringLiteral("t")] = m.timeSec;
        mo[QStringLiteral("bpm")] = m.bpm;
        tempoArr.append(mo);
    }
    psObj[QStringLiteral("tempoMarkers")] = tempoArr;
    psObj[QStringLiteral("punchRecordingEnabled")] = m_projectSettings.punchRecordingEnabled;
    psObj[QStringLiteral("punchInSec")] = m_projectSettings.punchInSec;
    psObj[QStringLiteral("punchOutSec")] = m_projectSettings.punchOutSec;
    psObj[QStringLiteral("loopPlaybackEnabled")] = m_projectSettings.loopPlaybackEnabled;
    psObj[QStringLiteral("loopStartSec")] = m_projectSettings.loopStartSec;
    psObj[QStringLiteral("loopEndSec")] = m_projectSettings.loopEndSec;

    QJsonObject root;
    root[QStringLiteral("version")]         = 1;
    root[QStringLiteral("projectSettings")] = psObj;
    root[QStringLiteral("tracks")]          = tracksArray;

    const QString savePath = m_projectLocationEdit->text().trimmed()
                             + QDir::separator() + QStringLiteral("project.json");
    QFile f(savePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Save Failed"),
            tr("Could not write project file:\n%1").arg(savePath));
        return;
    }
    f.write(QJsonDocument(root).toJson());
    f.close();
    m_isDirty = false;
    QMessageBox::information(this, tr("Project Saved"),
        tr("Project saved to:\n%1").arg(savePath));
}

void MainWindow::onOpenProject()
{
    const QString startDir = m_projectLocationEdit->text().trimmed().isEmpty()
                             ? QDir::homePath()
                             : m_projectLocationEdit->text().trimmed();
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Open Project"), startDir,
        tr("Project files (*.json);;All files (*)"));
    if (path.isEmpty())
        return;
    loadProjectFromFile(path);
}

void MainWindow::loadProjectFromFile(const QString& path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Open Failed"),
            tr("Could not read project file:\n%1").arg(path));
        return;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    if (!doc.isObject())
    {
        QMessageBox::warning(this, tr("Open Failed"), tr("Invalid project file format."));
        return;
    }

    // Set the project directory to the folder containing the JSON
    const QString projectDir = QFileInfo(path).absolutePath();
    m_projectLocationEdit->setText(projectDir);
    AppSettings::instance().setProjectLocation(projectDir);
    AppSettings::instance().save();

    // Clear existing tracks
    for (auto* w : m_trackWidgets)
    {
        m_tracksLayout->removeWidget(w);
        delete w;
    }
    m_trackWidgets.clear();
    m_nextTrackId = 1;

    const QJsonArray tracks = doc.object()[QStringLiteral("tracks")].toArray();
    for (const QJsonValue& val : tracks)
    {
        if (!val.isObject())
            continue;
        const QJsonObject obj = val.toObject();

        TrackData d;
        d.id            = obj[QStringLiteral("id")].toInt();
        d.name          = obj[QStringLiteral("name")].toString();
        d.type          = (obj[QStringLiteral("type")].toString() == QStringLiteral("MIDI"))
                          ? TrackType::MIDI : TrackType::Audio;
        d.enabled       = obj[QStringLiteral("enabled")].toBool(true);
        d.armed         = obj[QStringLiteral("armed")].toBool(false);
        d.sampleRate    = obj[QStringLiteral("sampleRate")].toInt(44100);
        d.channelCount  = obj[QStringLiteral("channelCount")].toInt(2);
        d.lengthSeconds = obj[QStringLiteral("lengthSeconds")].toDouble(0.0);
        d.audioEffectChain = obj[QStringLiteral("audioEffectChain")].toArray();
        d.gainDb = static_cast<float>(obj[QStringLiteral("gainDb")].toDouble(0));
        d.pan = static_cast<float>(obj[QStringLiteral("pan")].toDouble(0));
        d.mute = obj[QStringLiteral("mute")].toBool(false);
        d.solo = obj[QStringLiteral("solo")].toBool(false);
        d.auxSend = static_cast<float>(obj[QStringLiteral("auxSend")].toDouble(0));
        d.trimStartSec = obj[QStringLiteral("trimStartSec")].toDouble(0);
        d.trimEndSec = obj[QStringLiteral("trimEndSec")].toDouble(0);

        if (d.type == TrackType::Audio)
        {
            const QString audioFile = obj[QStringLiteral("audioFile")].toString();
            if (!audioFile.isEmpty())
            {
                const QString audioPath = projectDir + QDir::separator() + audioFile;
                int fileSampleRate   = d.sampleRate;
                int fileChannelCount = d.channelCount;
                if (loadAudioFile(audioPath, d.audioData, fileSampleRate, fileChannelCount))
                {
                    d.sampleRate   = fileSampleRate;
                    d.channelCount = fileChannelCount;
                }
            }
        }
        else
        {
            // Prefer external .mid file if referenced; fall back to inline midiNotes.
            const QString midiFileName = obj[QStringLiteral("midiFile")].toString();
            bool loadedFromFile = false;
            if (!midiFileName.isEmpty())
            {
                const QString midiPath = projectDir + QDir::separator() + midiFileName;
                double fileLen = 0.0;
                QVector<MidiControlChange> ccRead;
                QVector<MidiNote> notes = MidiFile::read(midiPath, &fileLen, &ccRead);
                if (!notes.isEmpty() || !ccRead.isEmpty())
                {
                    d.midiNotes = notes;
                    d.midiControlChanges = ccRead;
                    if (fileLen > d.lengthSeconds)
                        d.lengthSeconds = fileLen;
                    loadedFromFile = true;
                }
                else
                {
                    qWarning() << "Failed to read MIDI file" << midiPath
                               << "- falling back to inline midiNotes if present";
                }
            }
            const QJsonArray notesArray = loadedFromFile
                ? QJsonArray()
                : obj[QStringLiteral("midiNotes")].toArray();
            for (const QJsonValue& nv : notesArray)
            {
                if (!nv.isObject())
                    continue;
                const QJsonObject no = nv.toObject();
                MidiNote n;
                n.note      = no[QStringLiteral("note")].toInt();
                n.velocity  = no[QStringLiteral("velocity")].toInt();
                // Channel is optional for backwards compatibility with
                // projects saved before per-note channel was tracked.
                // Default to 0 (= MIDI channel 1) when missing.
                n.channel   = no[QStringLiteral("channel")].toInt(0);
                n.startTime = no[QStringLiteral("startTime")].toDouble();
                n.duration  = no[QStringLiteral("duration")].toDouble();
                d.midiNotes.append(n);
            }
            if (!loadedFromFile)
            {
                const QJsonArray ccJson = obj[QStringLiteral("midiControlChanges")].toArray();
                for (const QJsonValue& cv : ccJson)
                {
                    if (!cv.isObject())
                        continue;
                    const QJsonObject co = cv.toObject();
                    MidiControlChange cc;
                    cc.timeSec = co[QStringLiteral("t")].toDouble();
                    cc.channel = co[QStringLiteral("ch")].toInt(0);
                    cc.controller = co[QStringLiteral("c")].toInt(0);
                    cc.value = co[QStringLiteral("v")].toInt(0);
                    d.midiControlChanges.append(cc);
                }
            }
        }

        if (d.id >= m_nextTrackId)
            m_nextTrackId = d.id + 1;

        auto* widget = new TrackWidget(d, this);
        wireTrackWidget(widget);
        m_trackWidgets.append(widget);
        m_tracksLayout->insertWidget(m_tracksLayout->count() - 1, widget);
    }

    // Load project-level settings (MIDI device, SoundFont, audio devices)
    const QJsonObject ps = doc.object()[QStringLiteral("projectSettings")].toObject();
    if (!ps.isEmpty())
    {
        m_projectSettings.midiDeviceIndex   = ps[QStringLiteral("midiDeviceIndex")].toInt(-1);
        m_projectSettings.midiInputPortName = ps[QStringLiteral("midiInputPortName")].toString();
        m_projectSettings.soundFontPath     = ps[QStringLiteral("soundFontPath")].toString();
        m_projectSettings.midiVolumePercent = ps[QStringLiteral("midiVolumePercent")].toInt(-1);
        m_projectSettings.renderMidiToAudioForPlayback =
            ps[QStringLiteral("renderMidiToAudioForPlayback")].toBool(true);
        m_projectSettings.audioInputDeviceId =
            QByteArray::fromHex(ps[QStringLiteral("audioInputDeviceId")].toString().toLatin1());
        m_projectSettings.audioOutputDeviceId =
            QByteArray::fromHex(ps[QStringLiteral("audioOutputDeviceId")].toString().toLatin1());
        m_projectSettings.sampleRate   = ps[QStringLiteral("sampleRate")].toInt(44100);
        m_projectSettings.channelCount = ps[QStringLiteral("channelCount")].toInt(2);
        if (ps.contains(QStringLiteral("useQtAudioInput")))
            m_projectSettings.useQtAudioInput = ps[QStringLiteral("useQtAudioInput")].toBool();
        if (ps.contains(QStringLiteral("portAudioInputDeviceIndex")))
            m_projectSettings.portAudioInputDeviceIndex =
                ps[QStringLiteral("portAudioInputDeviceIndex")].toInt(-1);
        if (ps.contains(QStringLiteral("monitorWhileRecording")))
            m_projectSettings.monitorWhileRecording =
                ps[QStringLiteral("monitorWhileRecording")].toBool();
        m_projectSettings.mixEffectChain = ps[QStringLiteral("mixEffectChain")].toArray();
        m_projectSettings.auxEffectChain = ps[QStringLiteral("auxEffectChain")].toArray();
        m_projectSettings.tempoMarkers.clear();
        const QJsonArray tempoArr = ps[QStringLiteral("tempoMarkers")].toArray();
        for (const QJsonValue& tv : tempoArr)
        {
            if (!tv.isObject())
                continue;
            const QJsonObject mo = tv.toObject();
            TempoMarker m;
            m.timeSec = mo[QStringLiteral("t")].toDouble();
            m.bpm = mo[QStringLiteral("bpm")].toDouble(120);
            if (m.bpm > 0)
                m_projectSettings.tempoMarkers.append(m);
        }
        m_projectSettings.punchRecordingEnabled =
            ps[QStringLiteral("punchRecordingEnabled")].toBool(false);
        m_projectSettings.punchInSec = ps[QStringLiteral("punchInSec")].toDouble(0);
        m_projectSettings.punchOutSec = ps[QStringLiteral("punchOutSec")].toDouble(0);
        m_projectSettings.loopPlaybackEnabled =
            ps[QStringLiteral("loopPlaybackEnabled")].toBool(false);
        m_projectSettings.loopStartSec = ps[QStringLiteral("loopStartSec")].toDouble(0);
        m_projectSettings.loopEndSec = ps[QStringLiteral("loopEndSec")].toDouble(0);
    }
    else
    {
        m_projectSettings = ProjectSettings{};
    }

    syncMonitorCheckboxFromSettings();

    // If the project has exactly one track and it hasn't been recorded yet,
    // arm it automatically so the user can start recording immediately.
    if (m_trackWidgets.size() == 1)
    {
        const TrackData& td = m_trackWidgets[0]->trackData();
        if (td.audioData.isEmpty() && td.midiNotes.isEmpty())
            m_trackWidgets[0]->setArmed(true);
    }

    m_isDirty = false;
    updatePlayRecordButton();
}

void MainWindow::markDirty()
{
    m_isDirty = true;
}

void MainWindow::onTrackNameChanged(TrackWidget* /*widget*/,
                                    const QString& oldName, const QString& newName)
{
    const QString projectDir = m_projectLocationEdit->text().trimmed();
    if (!projectDir.isEmpty())
    {
        const QString oldBase =
            projectDir + QDir::separator() + AudioUtils::sanitizedTrackFilesystemName(oldName);
        const QString newBase =
            projectDir + QDir::separator() + AudioUtils::sanitizedTrackFilesystemName(newName);
        for (const auto& fmt : AudioFormats::all())
        {
            const QString ext = QString::fromLatin1(fmt.extension);
            const QString oldPath = oldBase + ext;
            const QString newPath = newBase + ext;
            if (oldPath != newPath && QFile::exists(oldPath))
                QFile::rename(oldPath, newPath);
        }
    }
    markDirty();
}

void MainWindow::onTrackRemoveRequested(TrackWidget* widget)
{
    m_mixerUndoBaseline.remove(widget);
    m_trackWidgets.removeOne(widget);
    m_tracksLayout->removeWidget(widget);
    widget->deleteLater();
    markDirty();
    updatePlayRecordButton();
}

void MainWindow::onClearTracks()
{
    if (m_trackWidgets.isEmpty()) return;
    const auto result = QMessageBox::question(
        this, tr("Clear Tracks"),
        tr("Remove all %1 track(s) from the project?").arg(m_trackWidgets.size()),
        QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
    if (result != QMessageBox::Yes) return;
    m_mixerUndoBaseline.clear();
    for (auto* w : m_trackWidgets)
    {
        m_tracksLayout->removeWidget(w);
        delete w;
    }
    m_trackWidgets.clear();
    markDirty();
    updatePlayRecordButton();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (m_isDirty)
    {
        const auto result = QMessageBox::question(
            this, tr("Unsaved Changes"),
            tr("The project has unsaved changes. Save before closing?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
            QMessageBox::Yes);
        if (result == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
        if (result == QMessageBox::Yes)
        {
            onSaveProject();
            // If save was cancelled (directory chooser dismissed), abort close
            if (m_isDirty)
            {
                event->ignore();
                return;
            }
        }
    }
    event->accept();
}

void MainWindow::onSettingsConfiguration()
{
    SettingsDialog dlg(this);
    dlg.exec();
    // Refresh LED display color in case it changed
    if (m_timeDisplay)
        m_timeDisplay->setActiveColor(ledColorFromName(AppSettings::instance().ledColor()));
}

void MainWindow::onVirtualMidiKeyboard()
{
    // virtual_midi_keyboard is always in the same directory as musicians_canvas:
    // on Linux/macOS it sits alongside the executable (inside Contents/MacOS for
    // a macOS .app bundle); on Windows it has a .exe extension.
#ifdef _WIN32
    const QString exe = QStringLiteral("virtual_midi_keyboard.exe");
#else
    const QString exe = QStringLiteral("virtual_midi_keyboard");
#endif
    const QString path = QCoreApplication::applicationDirPath()
                         + QDir::separator() + exe;
    QProcess::startDetached(path, QStringList());
}

void MainWindow::onProjectSettings()
{
    const ProjectSettings preservedTimeline = m_projectSettings;
    ProjectSettingsDialog dlg(m_projectSettings, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        m_projectSettings = dlg.projectSettings();
        m_projectSettings.tempoMarkers = preservedTimeline.tempoMarkers;
        m_projectSettings.punchRecordingEnabled = preservedTimeline.punchRecordingEnabled;
        m_projectSettings.punchInSec = preservedTimeline.punchInSec;
        m_projectSettings.punchOutSec = preservedTimeline.punchOutSec;
        m_projectSettings.loopPlaybackEnabled = preservedTimeline.loopPlaybackEnabled;
        m_projectSettings.loopStartSec = preservedTimeline.loopStartSec;
        m_projectSettings.loopEndSec = preservedTimeline.loopEndSec;
        markDirty();
    }
}

void MainWindow::onExportStems()
{
    const QString projectPath = effectiveProjectPath();
    if (projectPath.isEmpty() || projectPath == QDir::tempPath())
    {
        QMessageBox::warning(this, tr("Export stems"),
                             tr("Please set a project directory before exporting stems."));
        return;
    }
    StemExportDialog dlg(projectPath, this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    const QString dir = dlg.outputDirectory();
    if (dir.isEmpty())
        return;
    const QString stemExt = dlg.outputFileExtension();
    QDir().mkpath(dir);
    QVector<TrackData> tracks;
    for (auto* w : m_trackWidgets)
        tracks.append(w->trackData());
    const int volPct =
        (m_projectSettings.midiVolumePercent >= 0)
            ? m_projectSettings.midiVolumePercent
            : AppSettings::instance().midiVolumePercent();
    const double midiGainMultiplier = std::clamp(volPct, 0, 200) / 100.0;
    QString sf = m_projectSettings.soundFontPath;
    if (sf.isEmpty())
        sf = AppSettings::instance().soundFontPath();
    if (AudioUtils::exportStemsToDirectory(tracks, dir, projectPath, m_projectSettings.sampleRate,
                                           sf, true, midiGainMultiplier, stemExt))
        QMessageBox::information(this, tr("Export stems"),
                                 tr("Stems exported to:\n%1").arg(dir));
    else
        QMessageBox::warning(this, tr("Export stems"),
                             tr("Could not export stems. Ensure tracks have audio or MIDI."));
}

void MainWindow::onRecordingOptions()
{
    RecordingOptionsDialog dlg(m_projectSettings, this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    const ProjectSettings s = dlg.settings();
    m_projectSettings.punchRecordingEnabled = s.punchRecordingEnabled;
    m_projectSettings.punchInSec = s.punchInSec;
    m_projectSettings.punchOutSec = s.punchOutSec;
    m_projectSettings.loopPlaybackEnabled = s.loopPlaybackEnabled;
    m_projectSettings.loopStartSec = s.loopStartSec;
    m_projectSettings.loopEndSec = s.loopEndSec;
    markDirty();
}

void MainWindow::onTempoMapEditor()
{
    TempoMapDialog dlg(m_projectSettings.tempoMarkers, this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    m_projectSettings.tempoMarkers = dlg.markers();
    markDirty();
}

void MainWindow::onQuantizeMidi()
{
    QuantizeMidiDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    const double bpm =
        tempoBpmAtTime(m_projectSettings.tempoMarkers, 0.0,
                       static_cast<double>(qBound(20, AppSettings::instance().metronomeBpm(), 300)));
    const double quarterSec = 60.0 / std::max(1.0, bpm);
    const double gridSec = quarterSec * dlg.gridAsQuarterNoteFraction();
    const double strength = dlg.strength();
    bool any = false;
    for (auto* w : m_trackWidgets)
    {
        if (w->trackData().type != TrackType::MIDI)
            continue;
        if (dlg.applyToArmedTrackOnly() && !w->isArmed())
            continue;
        TrackData d = w->trackData();
        quantizeMidiNotesInPlace(d.midiNotes, gridSec, strength);
        w->setTrackData(d);
        any = true;
    }
    if (any)
        markDirty();
    else
        QMessageBox::information(this, tr("Quantize MIDI"),
                                 tr("No MIDI tracks were changed."));
}

void MainWindow::onMetronomeSettings()
{
    MetronomeDialog dlg(this);
    dlg.exec();
}

void MainWindow::startMetronomeIfEnabled()
{
    if (!AppSettings::instance().metronomeEnabled())
        return;
    // The metronome is intentionally VISUAL ONLY — no audible click is ever
    // produced.  Any audible click (whether QApplication::beep(), a system
    // bell, or a generated tone played through QAudioSink) goes through the
    // user's speakers, where the recording microphone can pick it up and
    // bake it into the recorded audio.  Once that happens the metronome
    // tick is permanently embedded in the FLAC file and shows up in every
    // mix-to-file operation that includes that track.  We side-step the
    // entire problem by never producing any sound.
    //
    // The visual indicator briefly tints the LED time display on each beat
    // (handled by SegmentDisplay::flash()), giving the user a tempo cue
    // while keeping the audio path completely silent.
    if (!m_metronomeTimer)
        m_metronomeTimer = new QTimer(this);
    const int appBpm = AppSettings::instance().metronomeBpm();
    const double effBpm = tempoBpmAtTime(
        m_projectSettings.tempoMarkers,
        m_isActive ? (m_activeTimer.elapsed() / 1000.0) : 0.0,
        static_cast<double>(appBpm));
    const int intervalMs =
        static_cast<int>(60000.0 / qBound(20.0, effBpm, 300.0));
    connect(m_metronomeTimer, &QTimer::timeout, this, [this]()
    {
        if (m_timeDisplay)
            m_timeDisplay->flash();
    });
    m_metronomeTimer->start(intervalMs);
}

void MainWindow::onAbout()
{
#ifndef APP_VERSION
#define APP_VERSION "0.0.0"
#endif
    const QString text = tr(
        "<h3>Musician's Canvas %1</h3>"
        "<p>A multi-track music recording application for desktop PCs, "
        "supporting audio and MIDI tracks, an integrated FluidSynth "
        "synthesizer, overdub recording, a built-in metronome, and "
        "high-quality sample-rate conversion.</p>"
        "<p>Written in C++ with Qt6.</p>"
        "<p>Copyright &copy; Eric Oulashin</p>"
        "<p><a href=\"%2\">%2</a></p>"
    ).arg(QStringLiteral(APP_VERSION),
          QStringLiteral("https://github.com/EricOulashin/musicians_canvas"));
    QMessageBox::about(this, tr("About Musician's Canvas"), text);
}

void MainWindow::onManual()
{
    const QString pdfPath = resolveManualPdfPathForLanguage(AppSettings::instance().language());
    if (pdfPath.isEmpty() || !QFile::exists(pdfPath))
    {
        QMessageBox::warning(this, tr("Manual"),
                             tr("The user manual PDF could not be found."));
        return;
    }

    const QUrl url = QUrl::fromLocalFile(pdfPath);
    if (!QDesktopServices::openUrl(url))
    {
        QMessageBox::warning(this, tr("Manual"),
                             tr("Could not open the user manual:\n%1").arg(pdfPath));
    }
}

void MainWindow::onTimeDisplayTick()
{
    if (m_isActive && m_timeDisplay)
        m_timeDisplay->setTime(m_activeTimer.elapsed());
}

void MainWindow::onAddDemoData()
{
    if (m_trackWidgets.isEmpty())
    {
        QMessageBox::information(this, tr("No Tracks"), tr("Add a track first, then select it."));
        return;
    }
    TrackWidget* target = m_trackWidgets.last();
    TrackData data = target->trackData();
    if (data.type == TrackType::MIDI)
    {
        data.midiNotes = {{60, 100, 0.0, 0.5}, {64, 90, 0.0, 0.5}, {67, 95, 0.0, 0.5},
                          {60, 80, 1.0, 0.25}, {64, 80, 1.0, 0.25}, {67, 80, 1.0, 0.25},
                          {62, 100, 2.0, 0.5}, {65, 90, 2.0, 0.5}, {69, 95, 2.0, 0.5}};
        data.lengthSeconds = 3.0;
        target->setTrackData(data);
        QMessageBox::information(this, tr("Demo Data"),
            tr("Demo MIDI notes added. Try Mix → Mix tracks to file to export."));
    }
    else
    {
        QMessageBox::information(this, tr("Demo Data"),
            tr("Switch the track to MIDI type (Options) to add demo notes."));
    }
}

TrackData MainWindow::createNewTrack()
{
    TrackData data;
    data.id = m_nextTrackId++;
    data.name = tr("Track %1").arg(data.id);
    data.type = TrackType::Audio;
    data.enabled = true;
    return data;
}
