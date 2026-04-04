#include "mainwindow.h"
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
#include "appsettings.h"
#include "audiostartup.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>
#include <QCoreApplication>
#include <QFrame>
#include <QProcess>
#include <QDir>
#include <QIcon>
#include <QPixmap>
#include <QTemporaryFile>
#include <QCloseEvent>
#include <QRegularExpression>
#include <QTimer>
#include <cmath>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QTextStream>

#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QAudioSource>
#include <QAudioFormat>
#include <QMediaDevices>
#include <QBuffer>
#include <QUrl>
#endif

// Returns a filesystem-safe version of a track name (strips chars invalid on all platforms)
static QString sanitizedTrackFilename(const QString& name)
{
    QString result = name.trimmed();
    static const QRegularExpression invalid(QStringLiteral("[/\\\\:*?\"<>|\\r\\n]"));
    result.replace(invalid, QStringLiteral("_"));
    return result;
}

// Parses a WAV file's RIFF/WAVE header by scanning chunks, populates audioData,
// sampleRate, and channelCount from the file's own header (more robust than seek(44)).
// Returns true on success.
static bool readWavAudioData(const QString& path, QByteArray& audioData,
                             int& sampleRate, int& channelCount)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    // RIFF descriptor (12 bytes): "RIFF" + file-size + "WAVE"
    const QByteArray riff = f.read(12);
    if (riff.size() < 12
        || riff.left(4)  != QByteArray("RIFF", 4)
        || riff.mid(8, 4) != QByteArray("WAVE", 4))
    {
        return false;
    }

    bool foundFmt  = false;
    bool foundData = false;

    while (!f.atEnd())
    {
        const QByteArray chunkHdr = f.read(8);
        if (chunkHdr.size() < 8)
            break;

        const QByteArray chunkId = chunkHdr.left(4);
        const quint32 chunkSize =
            static_cast<quint32>(static_cast<unsigned char>(chunkHdr[4])) |
            (static_cast<quint32>(static_cast<unsigned char>(chunkHdr[5])) << 8)  |
            (static_cast<quint32>(static_cast<unsigned char>(chunkHdr[6])) << 16) |
            (static_cast<quint32>(static_cast<unsigned char>(chunkHdr[7])) << 24);

        if (chunkId == QByteArray("fmt ", 4))
        {
            if (chunkSize < 16)
                return false;
            const QByteArray fmt = f.read(chunkSize);
            if (static_cast<quint32>(fmt.size()) < chunkSize)
                return false;
            // bytes 2-3: num channels, bytes 4-7: sample rate
            channelCount =
                static_cast<int>(static_cast<unsigned char>(fmt[2])) |
                (static_cast<int>(static_cast<unsigned char>(fmt[3])) << 8);
            sampleRate =
                static_cast<int>(static_cast<unsigned char>(fmt[4])) |
                (static_cast<int>(static_cast<unsigned char>(fmt[5])) << 8) |
                (static_cast<int>(static_cast<unsigned char>(fmt[6])) << 16) |
                (static_cast<int>(static_cast<unsigned char>(fmt[7])) << 24);
            foundFmt = true;
        }
        else if (chunkId == QByteArray("data", 4))
        {
            audioData = f.read(chunkSize);
            foundData = true;
            break;
        }
        else
        {
            // Unknown chunk — skip it (RIFF chunks are word-aligned)
            qint64 skipBytes = static_cast<qint64>(chunkSize);
            if (skipBytes % 2 != 0)
                skipBytes++;
            f.seek(f.pos() + skipBytes);
        }
    }

    return foundFmt && foundData;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle(tr("Musician's Canvas"));
    setMinimumSize(800, 500);
    resize(900, 600);

    m_playIcon   = QIcon(":/images/play_button.png");
    m_recordIcon = QIcon(":/images/record_button.png");
    m_stopIcon   = QIcon(":/images/stop_button.png");

    setupMenuBar();
    setupUi();

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

void MainWindow::setupMenuBar()
{
    auto* menuBar = this->menuBar();

    auto* fileMenu = menuBar->addMenu(tr("&File"));
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
    auto* vkAction = toolsMenu->addAction(tr("&Virtual MIDI Keyboard"));
    connect(vkAction, &QAction::triggered, this, &MainWindow::onVirtualMidiKeyboard);
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
    projectRow->addWidget(new QLabel(tr("Project location:")));
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
        }
    });
    projectRow->addWidget(m_projectLocationEdit, 1);
    auto* browseBtn = new QPushButton(tr("Browse..."));
    browseBtn->setFixedHeight(28);
    connect(browseBtn, &QPushButton::clicked, this, &MainWindow::onBrowseProjectLocation);
    projectRow->addWidget(browseBtn);
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

    m_addTrackButton = new QPushButton(tr("+ Add Track"));
    m_addTrackButton->setObjectName("addTrackBtn");
    m_addTrackButton->setFixedHeight(36);
    connect(m_addTrackButton, &QPushButton::clicked, this, &MainWindow::onAddTrack);
    toolbarLayout->addWidget(m_addTrackButton);

    auto* clearTracksBtn = new QPushButton(tr("Clear Tracks"));
    clearTracksBtn->setFixedHeight(36);
    clearTracksBtn->setStyleSheet(
        "QPushButton { background-color: #e8521e; color: white; font-weight: bold; }"
        "QPushButton:hover { background-color: #f07040; }"
        "QPushButton:pressed { background-color: #c03010; }");
    connect(clearTracksBtn, &QPushButton::clicked, this, &MainWindow::onClearTracks);
    toolbarLayout->addWidget(clearTracksBtn);

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

void MainWindow::onAddTrack()
{
    TrackData data = createNewTrack();
    auto* widget = new TrackWidget(data, this);
    connect(widget, &TrackWidget::configurationRequested, this, &MainWindow::onTrackConfigRequested);
    connect(widget, &TrackWidget::armChanged, this, &MainWindow::onArmChanged);
    connect(widget, &TrackWidget::dataChanged, this, [this](TrackWidget*)
    {
        markDirty();
        updatePlayRecordButton();
    });
    connect(widget, &TrackWidget::nameChanged, this, &MainWindow::onTrackNameChanged);
    connect(widget, &TrackWidget::removeRequested, this, &MainWindow::onTrackRemoveRequested);
    m_trackWidgets.append(widget);
    m_tracksLayout->insertWidget(m_tracksLayout->count() - 1, widget);

    // If this is the only track and it hasn't been recorded yet, arm it
    // automatically so the user can start recording immediately.
    if (m_trackWidgets.size() == 1 && data.audioData.isEmpty() && data.midiNotes.isEmpty())
        widget->setArmed(true);

    markDirty();
    updatePlayRecordButton();
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
    dlg.setTrackType(widget->trackData().type);
    if (dlg.exec() == QDialog::Accepted)
    {
        TrackData data = widget->trackData();
        data.type = dlg.trackType();
        widget->setTrackData(data);
        markDirty();
    }
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

    const QString projectPath = effectiveProjectPath();

    if (AudioUtils::mixTracksToFile(tracks, outputPath, projectPath,
                                    m_projectSettings.sampleRate,
                                    m_projectSettings.soundFontPath))
    {
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
}

void MainWindow::moveProjectFiles(const QString& oldDir, const QString& newDir)
{
    if (oldDir.isEmpty() || newDir.isEmpty() || oldDir == newDir) return;
    QDir src(oldDir);
    if (!src.exists()) return;
    QDir().mkpath(newDir);
    const QStringList filters = {QStringLiteral("*.wav"), QStringLiteral("*.flac")};
    const QStringList entries = src.entryList(filters, QDir::Files);
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

    // Mix all enabled tracks to a temporary file and play it back
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

    if (!AudioUtils::mixTracksToFile(tracks, m_playbackTempFile, effectiveProjectPath(),
                                     m_projectSettings.sampleRate,
                                     m_projectSettings.soundFontPath))
    {
        QMessageBox::warning(this, tr("Playback Error"),
            tr("Could not mix tracks for playback. Make sure tracks have content."));
        QFile::remove(m_playbackTempFile);
        m_playbackTempFile.clear();
        return;
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
                    onPlaybackFinished();
            });
    }
    m_player->setSource(QUrl::fromLocalFile(m_playbackTempFile));
    m_player->play();

    m_isActive = true;
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

    // Use the device selected in project settings, or system default
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

    // Verify the input device can handle the project's configured sample rate.
    // Disable controls and show countdown on the armed track
    m_isActive = true;
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
    const bool mixed = AudioUtils::mixTracksToFile(
        overdubTracks, m_playbackTempFile,
        effectiveProjectPath(),
        m_projectSettings.sampleRate,
        m_projectSettings.soundFontPath);
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
        if (m_recordBuffer == nullptr) return;
        const QByteArray& buf = m_recordBuffer->data();
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

    const RecordingPostProcessResult rec = RecordingPostProcess::process(rp);

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
                             sanitizedTrackFilename(data.name) + ".flac";
    static_cast<void>(
        AudioUtils::writeAudioDataToFlac(rec.int16Pcm,
                                         rec.sampleRate,
                                         rec.channelCount,
                                         flacPath));
}
#endif  // QT_MULTIMEDIA_AVAILABLE

void MainWindow::beginRecordingAfterCountdown()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
    const QString projectDir = effectiveProjectPath();
    QDir().mkpath(projectDir);

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

            const int logDev =
                (paDev < 0) ? PortAudioRecorder::defaultInputDeviceIndex() : paDev;
            {
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
            startOverdubPlayback();
            startRecordingLevelMeter();
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

    m_recordBuffer = new QBuffer(this);
    m_recordBuffer->open(QBuffer::WriteOnly);

    m_audioSource = new QAudioSource(inputDevice, format, this);
    // Use the platform default buffer size.  Overriding with a small value
    // (e.g. 2048) causes massive data loss on devices that use 32-bit
    // sample formats (Int32/Float32), because the tiny buffer cannot hold
    // enough frames and the audio callback drops ~75% of the data.

    m_audioSource->start(m_recordBuffer);
    m_recordingTimer.start();

    startOverdubPlayback();
    startRecordingLevelMeter();
#endif
}

void MainWindow::stopPlaybackOrRecording()
{
#ifdef QT_MULTIMEDIA_AVAILABLE
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

        if (m_recordBuffer != nullptr)
        {
            const QByteArray rawData = m_recordBuffer->data();
            if (!rawData.isEmpty())
                finalizeRecordedAudio(rawData, processedMicroseconds, "Qt Multimedia");
            delete m_recordBuffer;
            m_recordBuffer = nullptr;
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

        if (d.type == TrackType::Audio)
        {
            // Reference whichever audio file exists (FLAC preferred for new recordings,
            // WAV retained for projects recorded before the FLAC switch).
            const QString baseName = sanitizedTrackFilename(d.name);
            QString audioExt = QStringLiteral(".flac");
            if (!QFile::exists(projectDir + QDir::separator() + baseName + QStringLiteral(".flac"))
                && QFile::exists(projectDir + QDir::separator() + baseName + QStringLiteral(".wav")))
            {
                audioExt = QStringLiteral(".wav");
            }
            obj[QStringLiteral("audioFile")] = baseName + audioExt;
        }
        else
        {
            QJsonArray notesArray;
            for (const MidiNote& n : d.midiNotes)
            {
                QJsonObject no;
                no[QStringLiteral("note")]      = n.note;
                no[QStringLiteral("velocity")]  = n.velocity;
                no[QStringLiteral("startTime")] = n.startTime;
                no[QStringLiteral("duration")]  = n.duration;
                notesArray.append(no);
            }
            obj[QStringLiteral("midiNotes")] = notesArray;
        }
        tracksArray.append(obj);
    }

    QJsonObject psObj;
    psObj[QStringLiteral("midiDeviceIndex")]     = m_projectSettings.midiDeviceIndex;
    psObj[QStringLiteral("soundFontPath")]       = m_projectSettings.soundFontPath;
    psObj[QStringLiteral("audioInputDeviceId")]  =
        QString::fromLatin1(m_projectSettings.audioInputDeviceId.toHex());
    psObj[QStringLiteral("audioOutputDeviceId")] =
        QString::fromLatin1(m_projectSettings.audioOutputDeviceId.toHex());
    psObj[QStringLiteral("sampleRate")]          = m_projectSettings.sampleRate;
    psObj[QStringLiteral("channelCount")]        = m_projectSettings.channelCount;
    psObj[QStringLiteral("useQtAudioInput")] = m_projectSettings.useQtAudioInput;
    psObj[QStringLiteral("portAudioInputDeviceIndex")] =
        m_projectSettings.portAudioInputDeviceIndex;

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

        if (d.type == TrackType::Audio)
        {
            const QString audioFile = obj[QStringLiteral("audioFile")].toString();
            if (!audioFile.isEmpty())
            {
                const QString audioPath = projectDir + QDir::separator() + audioFile;
                int fileSampleRate   = d.sampleRate;
                int fileChannelCount = d.channelCount;
                bool loaded = false;
                if (audioFile.endsWith(QStringLiteral(".flac"), Qt::CaseInsensitive))
                    loaded = AudioUtils::readFlacAudioData(audioPath, d.audioData, fileSampleRate, fileChannelCount);
                else
                    loaded = readWavAudioData(audioPath, d.audioData, fileSampleRate, fileChannelCount);
                if (loaded)
                {
                    d.sampleRate   = fileSampleRate;
                    d.channelCount = fileChannelCount;
                }
            }
        }
        else
        {
            const QJsonArray notesArray = obj[QStringLiteral("midiNotes")].toArray();
            for (const QJsonValue& nv : notesArray)
            {
                if (!nv.isObject())
                    continue;
                const QJsonObject no = nv.toObject();
                MidiNote n;
                n.note      = no[QStringLiteral("note")].toInt();
                n.velocity  = no[QStringLiteral("velocity")].toInt();
                n.startTime = no[QStringLiteral("startTime")].toDouble();
                n.duration  = no[QStringLiteral("duration")].toDouble();
                d.midiNotes.append(n);
            }
        }

        if (d.id >= m_nextTrackId)
            m_nextTrackId = d.id + 1;

        auto* widget = new TrackWidget(d, this);
        connect(widget, &TrackWidget::configurationRequested,
                this, &MainWindow::onTrackConfigRequested);
        connect(widget, &TrackWidget::armChanged, this, &MainWindow::onArmChanged);
        connect(widget, &TrackWidget::dataChanged, this, [this](TrackWidget*)
        {
            markDirty();
            updatePlayRecordButton();
        });
        connect(widget, &TrackWidget::nameChanged, this, &MainWindow::onTrackNameChanged);
        connect(widget, &TrackWidget::removeRequested, this, &MainWindow::onTrackRemoveRequested);
        m_trackWidgets.append(widget);
        m_tracksLayout->insertWidget(m_tracksLayout->count() - 1, widget);
    }

    // Load project-level settings (MIDI device, SoundFont, audio devices)
    const QJsonObject ps = doc.object()[QStringLiteral("projectSettings")].toObject();
    if (!ps.isEmpty())
    {
        m_projectSettings.midiDeviceIndex = ps[QStringLiteral("midiDeviceIndex")].toInt(-1);
        m_projectSettings.soundFontPath   = ps[QStringLiteral("soundFontPath")].toString();
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
    }
    else
    {
        m_projectSettings = ProjectSettings{};
    }

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
        const QString oldBase = projectDir + QDir::separator() + sanitizedTrackFilename(oldName);
        const QString newBase = projectDir + QDir::separator() + sanitizedTrackFilename(newName);
        for (const QString& ext : {QStringLiteral(".flac"), QStringLiteral(".wav")})
        {
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
}

void MainWindow::onVirtualMidiKeyboard()
{
#ifdef _WIN32
    const QString exe = QStringLiteral("virtual_midi_keyboard.exe");
#else
    const QString exe = QStringLiteral("virtual_midi_keyboard");
#endif
    const QString path = QCoreApplication::applicationDirPath() + QDir::separator() + exe;
    QProcess::startDetached(path, QStringList());
}

void MainWindow::onProjectSettings()
{
    ProjectSettingsDialog dlg(m_projectSettings, this);
    if (dlg.exec() == QDialog::Accepted)
    {
        m_projectSettings = dlg.projectSettings();
        markDirty();
    }
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
