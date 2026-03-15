#include "mainwindow.h"
#include "trackwidget.h"
#include "trackconfigdialog.h"
#include "settingsdialog.h"
#include "audioutils.h"
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
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>

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
static QString sanitizedTrackFilename(const QString& name) {
    QString result = name.trimmed();
    static const QRegularExpression invalid(QStringLiteral("[/\\\\:*?\"<>|\\r\\n]"));
    result.replace(invalid, QStringLiteral("_"));
    return result;
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(tr("Musician's Canvas"));
    setMinimumSize(800, 500);
    resize(900, 600);

    m_playIcon   = QIcon(":/images/play_button.png");
    m_recordIcon = QIcon(":/images/record_button.png");
    m_stopIcon   = QIcon::fromTheme("media-playback-stop",
                       QIcon::fromTheme("process-stop"));

    setupMenuBar();
    setupUi();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupMenuBar() {
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

void MainWindow::setupUi() {
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
    connect(m_projectLocationEdit, &QLineEdit::editingFinished, this, [this]() {
        const QString oldDir = AppSettings::instance().projectLocation();
        const QString newDir = m_projectLocationEdit->text().trimmed();
        if (newDir != oldDir) {
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

void MainWindow::onAddTrack() {
    TrackData data = createNewTrack();
    auto* widget = new TrackWidget(data, this);
    connect(widget, &TrackWidget::configurationRequested, this, &MainWindow::onTrackConfigRequested);
    connect(widget, &TrackWidget::armChanged, this, &MainWindow::onArmChanged);
    connect(widget, &TrackWidget::dataChanged, this, [this](TrackWidget*) { markDirty(); });
    connect(widget, &TrackWidget::nameChanged, this, &MainWindow::onTrackNameChanged);
    connect(widget, &TrackWidget::removeRequested, this, &MainWindow::onTrackRemoveRequested);
    m_trackWidgets.append(widget);
    m_tracksLayout->insertWidget(m_tracksLayout->count() - 1, widget);
    markDirty();
}

void MainWindow::onArmChanged(TrackWidget* armed, bool isArmed) {
    if (isArmed) {
        // Uncheck Arm on all other tracks (keep them enabled so the user can switch)
        for (auto* w : m_trackWidgets) {
            if (w != armed)
                w->setArmed(false);
        }
    }
    updatePlayRecordButton();
}

void MainWindow::onTrackConfigRequested(TrackWidget* widget) {
    TrackConfigDialog dlg(this);
    dlg.setTrackType(widget->trackData().type);
    if (dlg.exec() == QDialog::Accepted) {
        TrackData data = widget->trackData();
        data.type = dlg.trackType();
        widget->setTrackData(data);
        markDirty();
    }
}

void MainWindow::onClose() {
    close();
}

void MainWindow::onMix() {
    QVector<TrackData> tracks;
    for (auto* w : m_trackWidgets)
        tracks.append(w->trackData());

    MixDialog dlg(effectiveProjectPath(), this);
    if (dlg.exec() != QDialog::Accepted) return;

    const QString outputPath = dlg.outputFilename();
    if (outputPath.isEmpty()) return;

    const QString projectPath = effectiveProjectPath();

    if (AudioUtils::mixTracksToFile(tracks, outputPath, projectPath)) {
        QMessageBox::information(this, tr("Mix Complete"),
            tr("Audio exported successfully to:\n%1").arg(outputPath));
    } else {
        QMessageBox::warning(this, tr("Mix Failed"),
            tr("Could not export audio. Please check that you have at least one "
               "enabled track with content, and a SoundFont configured for MIDI tracks."));
    }
}

void MainWindow::onBrowseProjectLocation() {
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

void MainWindow::moveProjectFiles(const QString& oldDir, const QString& newDir) {
    if (oldDir.isEmpty() || newDir.isEmpty() || oldDir == newDir) return;
    QDir src(oldDir);
    if (!src.exists()) return;
    QDir().mkpath(newDir);
    const QStringList filters = {QStringLiteral("*.wav"), QStringLiteral("*.flac")};
    const QStringList entries = src.entryList(filters, QDir::Files);
    for (const QString& filename : entries) {
        const QString srcPath = oldDir + QDir::separator() + filename;
        const QString dstPath = newDir + QDir::separator() + filename;
        QFile::rename(srcPath, dstPath);
    }
}

void MainWindow::onPlayRecord() {
    if (m_isActive) {
        stopPlaybackOrRecording();
    } else if (isAnyTrackArmed()) {
        startRecording();
    } else {
        startPlayback();
    }
}

void MainWindow::onPlaybackFinished() {
    stopPlaybackOrRecording();
}

bool MainWindow::isAnyTrackArmed() const {
    for (auto* w : m_trackWidgets)
        if (w->isArmed()) return true;
    return false;
}

TrackWidget* MainWindow::armedTrack() const {
    for (auto* w : m_trackWidgets)
        if (w->isArmed()) return w;
    return nullptr;
}

QString MainWindow::effectiveProjectPath() const {
    const QString loc = m_projectLocationEdit ? m_projectLocationEdit->text().trimmed() : QString();
    return loc.isEmpty() ? QDir::tempPath() : loc;
}

void MainWindow::updatePlayRecordButton() {
    if (!m_playRecordButton) return;
    if (m_isActive) {
        m_playRecordButton->setIcon(m_stopIcon);
        m_playRecordButton->setToolTip(tr("Stop"));
    } else if (isAnyTrackArmed()) {
        m_playRecordButton->setIcon(m_recordIcon);
        m_playRecordButton->setToolTip(tr("Record armed track"));
    } else {
        m_playRecordButton->setIcon(m_playIcon);
        m_playRecordButton->setToolTip(tr("Play all tracks"));
    }
}

void MainWindow::startPlayback() {
#ifdef QT_MULTIMEDIA_AVAILABLE
    QVector<TrackData> tracks;
    for (auto* w : m_trackWidgets)
        tracks.append(w->trackData());

    // Mix all enabled tracks to a temporary file and play it back
    QTemporaryFile* tmpFile = new QTemporaryFile(
        QDir::temp().filePath("mc_play_XXXXXX.wav"), this);
    tmpFile->setAutoRemove(false);
    if (!tmpFile->open()) {
        QMessageBox::warning(this, tr("Playback Error"), tr("Could not create temporary file."));
        return;
    }
    m_playbackTempFile = tmpFile->fileName();
    tmpFile->close();

    if (!AudioUtils::mixTracksToFile(tracks, m_playbackTempFile, effectiveProjectPath())) {
        QMessageBox::warning(this, tr("Playback Error"),
            tr("Could not mix tracks for playback. Make sure tracks have content."));
        QFile::remove(m_playbackTempFile);
        m_playbackTempFile.clear();
        return;
    }

    if (!m_player) {
        m_player = new QMediaPlayer(this);
        m_audioOutput = new QAudioOutput(this);
        m_player->setAudioOutput(m_audioOutput);
        connect(m_player, &QMediaPlayer::playbackStateChanged, this,
            [this](QMediaPlayer::PlaybackState state) {
                if (state == QMediaPlayer::StoppedState && m_isActive)
                    onPlaybackFinished();
            });
    }
    m_player->setSource(QUrl::fromLocalFile(m_playbackTempFile));
    m_player->play();

    m_isActive = true;
    updatePlayRecordButton();
#else
    QMessageBox::information(this, tr("Playback"),
        tr("Qt Multimedia is not available. Build with Qt6::Multimedia for playback support."));
#endif
}

void MainWindow::startRecording() {
#ifdef QT_MULTIMEDIA_AVAILABLE
    TrackWidget* armed = armedTrack();
    if (!armed) return;

    // Require an explicit project directory before recording
    if (m_projectLocationEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Project Directory Required"),
            tr("You must choose a project directory before recording.\n"
               "Please select a directory to store your recorded files."));
        onBrowseProjectLocation();
        if (m_projectLocationEdit->text().trimmed().isEmpty())
            return;  // User cancelled the directory chooser
    }

    const QString projectDir = effectiveProjectPath();
    QDir().mkpath(projectDir);

    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);

    const QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    if (inputDevice.isNull()) {
        QMessageBox::warning(this, tr("Recording Error"),
            tr("No audio input device found. Configure one in Settings."));
        return;
    }

    m_recordBuffer = new QBuffer(this);
    m_recordBuffer->open(QBuffer::WriteOnly);

    m_audioSource = new QAudioSource(inputDevice, format, this);
    // Use a small buffer to minimise recording latency.
    m_audioSource->setBufferSize(AudioStartup::recommendedBufferBytes());
    m_audioSource->start(m_recordBuffer);

    m_isActive = true;
    updatePlayRecordButton();
#else
    QMessageBox::information(this, tr("Recording"),
        tr("Qt Multimedia is not available. Build with Qt6::Multimedia for recording support."));
#endif
}

void MainWindow::stopPlaybackOrRecording() {
#ifdef QT_MULTIMEDIA_AVAILABLE
    if (m_player && m_player->playbackState() != QMediaPlayer::StoppedState)
        m_player->stop();

    if (m_audioSource) {
        m_audioSource->stop();

        // Save the recorded audio to the armed track
        if (m_recordBuffer) {
            const QByteArray& rawData = m_recordBuffer->data();
            if (!rawData.isEmpty()) {
                TrackWidget* armed = armedTrack();
                if (armed) {
                    TrackData data = armed->trackData();
                    data.type = TrackType::Audio;
                    data.audioData = rawData;
                    data.sampleRate = 44100;
                    data.channelCount = 2;
                    armed->setTrackData(data);
                    markDirty();

                    // Also persist to WAV in project directory
                    const QString wavPath = effectiveProjectPath() + QDir::separator() +
                                            sanitizedTrackFilename(data.name) + ".wav";
                    QFile wavFile(wavPath);
                    if (wavFile.open(QIODevice::WriteOnly)) {
                        // Write minimal WAV header
                        const quint32 sr = 44100;
                        const quint32 frames = rawData.size() / 4;
                        const quint32 byteRate = sr * 4;
                        const quint32 dataSize = rawData.size();
                        const quint32 fileSize = 36 + dataSize;
                        wavFile.write("RIFF", 4);
                        wavFile.write(reinterpret_cast<const char*>(&fileSize), 4);
                        wavFile.write("WAVE", 4);
                        wavFile.write("fmt ", 4);
                        quint32 fmtSize = 16; wavFile.write(reinterpret_cast<const char*>(&fmtSize), 4);
                        quint16 fmt = 1; wavFile.write(reinterpret_cast<const char*>(&fmt), 2);
                        quint16 ch = 2; wavFile.write(reinterpret_cast<const char*>(&ch), 2);
                        wavFile.write(reinterpret_cast<const char*>(&sr), 4);
                        wavFile.write(reinterpret_cast<const char*>(&byteRate), 4);
                        quint16 blk = 4; wavFile.write(reinterpret_cast<const char*>(&blk), 2);
                        quint16 bps = 16; wavFile.write(reinterpret_cast<const char*>(&bps), 2);
                        wavFile.write("data", 4);
                        wavFile.write(reinterpret_cast<const char*>(&dataSize), 4);
                        wavFile.write(rawData);
                        wavFile.close();
                    }
                }
            }
            delete m_recordBuffer;
            m_recordBuffer = nullptr;
        }
        delete m_audioSource;
        m_audioSource = nullptr;
    }

    if (!m_playbackTempFile.isEmpty()) {
        QFile::remove(m_playbackTempFile);
        m_playbackTempFile.clear();
    }
#endif
    m_isActive = false;
    updatePlayRecordButton();
}

void MainWindow::onSaveProject() {
    const QString projectDir = m_projectLocationEdit->text().trimmed();
    if (projectDir.isEmpty()) {
        QMessageBox::warning(this, tr("Save Project"),
            tr("Please choose a project directory before saving."));
        onBrowseProjectLocation();
        if (m_projectLocationEdit->text().trimmed().isEmpty()) return;
    }

    QJsonArray tracksArray;
    for (auto* w : m_trackWidgets) {
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

        if (d.type == TrackType::Audio) {
            // Reference the WAV file saved in the project directory
            obj[QStringLiteral("audioFile")] = sanitizedTrackFilename(d.name) + ".wav";
        } else {
            QJsonArray notesArray;
            for (const MidiNote& n : d.midiNotes) {
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

    QJsonObject root;
    root[QStringLiteral("version")] = 1;
    root[QStringLiteral("tracks")]  = tracksArray;

    const QString savePath = m_projectLocationEdit->text().trimmed()
                             + QDir::separator() + QStringLiteral("project.json");
    QFile f(savePath);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
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

void MainWindow::onOpenProject() {
    const QString startDir = m_projectLocationEdit->text().trimmed().isEmpty()
                             ? QDir::homePath()
                             : m_projectLocationEdit->text().trimmed();
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Open Project"), startDir,
        tr("Project files (*.json);;All files (*)"));
    if (path.isEmpty()) return;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Open Failed"),
            tr("Could not read project file:\n%1").arg(path));
        return;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    f.close();
    if (!doc.isObject()) {
        QMessageBox::warning(this, tr("Open Failed"), tr("Invalid project file format."));
        return;
    }

    // Set the project directory to the folder containing the JSON
    const QString projectDir = QFileInfo(path).absolutePath();
    const QString oldDir = AppSettings::instance().projectLocation();
    m_projectLocationEdit->setText(projectDir);
    AppSettings::instance().setProjectLocation(projectDir);
    AppSettings::instance().save();

    // Clear existing tracks
    for (auto* w : m_trackWidgets) {
        m_tracksLayout->removeWidget(w);
        delete w;
    }
    m_trackWidgets.clear();
    m_nextTrackId = 1;

    const QJsonArray tracks = doc.object()[QStringLiteral("tracks")].toArray();
    for (const QJsonValue& val : tracks) {
        if (!val.isObject()) continue;
        const QJsonObject obj = val.toObject();

        TrackData d;
        d.id           = obj[QStringLiteral("id")].toInt();
        d.name         = obj[QStringLiteral("name")].toString();
        d.type         = (obj[QStringLiteral("type")].toString() == QStringLiteral("MIDI"))
                         ? TrackType::MIDI : TrackType::Audio;
        d.enabled      = obj[QStringLiteral("enabled")].toBool(true);
        d.armed        = obj[QStringLiteral("armed")].toBool(false);
        d.sampleRate   = obj[QStringLiteral("sampleRate")].toInt(44100);
        d.channelCount = obj[QStringLiteral("channelCount")].toInt(2);
        d.lengthSeconds = obj[QStringLiteral("lengthSeconds")].toDouble(0.0);

        if (d.type == TrackType::Audio) {
            const QString audioFile = obj[QStringLiteral("audioFile")].toString();
            if (!audioFile.isEmpty()) {
                const QString wavPath = projectDir + QDir::separator() + audioFile;
                QFile wav(wavPath);
                if (wav.open(QIODevice::ReadOnly)) {
                    wav.seek(44);  // skip WAV header
                    d.audioData = wav.readAll();
                    wav.close();
                }
            }
        } else {
            const QJsonArray notesArray = obj[QStringLiteral("midiNotes")].toArray();
            for (const QJsonValue& nv : notesArray) {
                if (!nv.isObject()) continue;
                const QJsonObject no = nv.toObject();
                MidiNote n;
                n.note      = no[QStringLiteral("note")].toInt();
                n.velocity  = no[QStringLiteral("velocity")].toInt();
                n.startTime = no[QStringLiteral("startTime")].toDouble();
                n.duration  = no[QStringLiteral("duration")].toDouble();
                d.midiNotes.append(n);
            }
        }

        if (d.id >= m_nextTrackId) m_nextTrackId = d.id + 1;

        auto* widget = new TrackWidget(d, this);
        connect(widget, &TrackWidget::configurationRequested,
                this, &MainWindow::onTrackConfigRequested);
        connect(widget, &TrackWidget::armChanged, this, &MainWindow::onArmChanged);
        connect(widget, &TrackWidget::dataChanged, this, [this](TrackWidget*) { markDirty(); });
        connect(widget, &TrackWidget::nameChanged, this, &MainWindow::onTrackNameChanged);
        connect(widget, &TrackWidget::removeRequested, this, &MainWindow::onTrackRemoveRequested);
        m_trackWidgets.append(widget);
        m_tracksLayout->insertWidget(m_tracksLayout->count() - 1, widget);
    }

    m_isDirty = false;
    updatePlayRecordButton();
}

void MainWindow::markDirty() {
    m_isDirty = true;
}

void MainWindow::onTrackNameChanged(TrackWidget* /*widget*/,
                                    const QString& oldName, const QString& newName) {
    const QString projectDir = m_projectLocationEdit->text().trimmed();
    if (!projectDir.isEmpty()) {
        const QString oldPath = projectDir + QDir::separator() +
                                sanitizedTrackFilename(oldName) + ".wav";
        const QString newPath = projectDir + QDir::separator() +
                                sanitizedTrackFilename(newName) + ".wav";
        if (oldPath != newPath && QFile::exists(oldPath))
            QFile::rename(oldPath, newPath);
    }
    markDirty();
}

void MainWindow::onTrackRemoveRequested(TrackWidget* widget) {
    m_trackWidgets.removeOne(widget);
    m_tracksLayout->removeWidget(widget);
    widget->deleteLater();
    markDirty();
}

void MainWindow::onClearTracks() {
    if (m_trackWidgets.isEmpty()) return;
    const auto result = QMessageBox::question(
        this, tr("Clear Tracks"),
        tr("Remove all %1 track(s) from the project?").arg(m_trackWidgets.size()),
        QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel);
    if (result != QMessageBox::Yes) return;
    for (auto* w : m_trackWidgets) {
        m_tracksLayout->removeWidget(w);
        delete w;
    }
    m_trackWidgets.clear();
    markDirty();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (m_isDirty) {
        const auto result = QMessageBox::question(
            this, tr("Unsaved Changes"),
            tr("The project has unsaved changes. Save before closing?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
            QMessageBox::Yes);
        if (result == QMessageBox::Cancel) {
            event->ignore();
            return;
        }
        if (result == QMessageBox::Yes) {
            onSaveProject();
            // If save was cancelled (directory chooser dismissed), abort close
            if (m_isDirty) {
                event->ignore();
                return;
            }
        }
    }
    event->accept();
}

void MainWindow::onSettingsConfiguration() {
    SettingsDialog dlg(this);
    dlg.exec();
}

void MainWindow::onVirtualMidiKeyboard() {
#ifdef _WIN32
    const QString exe = QStringLiteral("virtual_midi_keyboard.exe");
#else
    const QString exe = QStringLiteral("virtual_midi_keyboard");
#endif
    const QString path = QCoreApplication::applicationDirPath() + QDir::separator() + exe;
    QProcess::startDetached(path, QStringList());
}

void MainWindow::onAddDemoData() {
    if (m_trackWidgets.isEmpty()) {
        QMessageBox::information(this, tr("No Tracks"), tr("Add a track first, then select it."));
        return;
    }
    TrackWidget* target = m_trackWidgets.last();
    TrackData data = target->trackData();
    if (data.type == TrackType::MIDI) {
        data.midiNotes = {{60, 100, 0.0, 0.5}, {64, 90, 0.0, 0.5}, {67, 95, 0.0, 0.5},
                          {60, 80, 1.0, 0.25}, {64, 80, 1.0, 0.25}, {67, 80, 1.0, 0.25},
                          {62, 100, 2.0, 0.5}, {65, 90, 2.0, 0.5}, {69, 95, 2.0, 0.5}};
        data.lengthSeconds = 3.0;
        target->setTrackData(data);
        QMessageBox::information(this, tr("Demo Data"), tr("Demo MIDI notes added. Try Mix → Mix tracks to file to export."));
    } else {
        QMessageBox::information(this, tr("Demo Data"), tr("Switch the track to MIDI type (Options) to add demo notes."));
    }
}

TrackData MainWindow::createNewTrack() {
    TrackData data;
    data.id = m_nextTrackId++;
    data.name = tr("Track %1").arg(data.id);
    data.type = TrackType::Audio;
    data.enabled = true;
    return data;
}
