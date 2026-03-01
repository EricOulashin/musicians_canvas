#include "mainwindow.h"
#include "trackwidget.h"
#include "trackconfigdialog.h"
#include "settingsdialog.h"
#include "audioutils.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
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

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(tr("Musician's Canvas"));
    setMinimumSize(800, 500);
    resize(900, 600);
    setupMenuBar();
    setupUi();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupMenuBar() {
    auto* menuBar = this->menuBar();
    auto* fileMenu = menuBar->addMenu(tr("&File"));
    auto* closeAction = fileMenu->addAction(tr("&Close"));
    closeAction->setShortcut(QKeySequence::Quit);
    connect(closeAction, &QAction::triggered, this, &MainWindow::onClose);

    auto* projectMenu = menuBar->addMenu(tr("&Project"));
    auto* mixAction = projectMenu->addAction(tr("&Mix"));
    mixAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    connect(mixAction, &QAction::triggered, this, &MainWindow::onMix);
    auto* demoAction = projectMenu->addAction(tr("Add Demo &Data to Selected Track"));
    connect(demoAction, &QAction::triggered, this, &MainWindow::onAddDemoData);

    auto* settingsMenu = menuBar->addMenu(tr("&Settings"));
    auto* configAction = settingsMenu->addAction(tr("&Configuration"));
    configAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Comma));
    connect(configAction, &QAction::triggered, this, &MainWindow::onSettingsConfiguration);

    auto* toolsMenu = menuBar->addMenu(tr("&Tools"));
    toolsMenu->menuAction()->setShortcut(QKeySequence(Qt::ALT | Qt::Key_T));
    auto* vkAction = toolsMenu->addAction(tr("&Virtual MIDI Keyboard"));
    connect(vkAction, &QAction::triggered, this, &MainWindow::onVirtualMidiKeyboard);
}

void MainWindow::setupUi() {
    auto* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    auto* toolbarLayout = new QHBoxLayout();
    m_addTrackButton = new QPushButton(tr("+ Add Track"));
    m_addTrackButton->setObjectName("addTrackBtn");
    m_addTrackButton->setFixedHeight(36);
    connect(m_addTrackButton, &QPushButton::clicked, this, &MainWindow::onAddTrack);
    toolbarLayout->addWidget(m_addTrackButton);
    toolbarLayout->addStretch();
    mainLayout->addLayout(toolbarLayout);

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
    m_trackWidgets.append(widget);
    m_tracksLayout->insertWidget(m_tracksLayout->count() - 1, widget);
}

void MainWindow::onTrackConfigRequested(TrackWidget* widget) {
    TrackConfigDialog dlg(this);
    dlg.setTrackType(widget->trackData().type);
    if (dlg.exec() == QDialog::Accepted) {
        TrackData data = widget->trackData();
        data.type = dlg.trackType();
        widget->setTrackData(data);
    }
}

void MainWindow::onClose() {
    QApplication::quit();
}

void MainWindow::onMix() {
    QVector<TrackData> tracks;
    for (auto* w : m_trackWidgets) {
        tracks.append(w->trackData());
    }

    QString path = QFileDialog::getSaveFileName(this, tr("Save Mixed Audio"),
        QDir::homePath(),
        tr("WAV files (*.wav);;All files (*)"));
    if (path.isEmpty()) return;

    if (!path.endsWith(".wav", Qt::CaseInsensitive)) {
        path += ".wav";
    }

    if (AudioUtils::mixTracksToWav(tracks, path, 44100)) {
        QMessageBox::information(this, tr("Mix Complete"),
            tr("Audio exported successfully to:\n%1").arg(path));
    } else {
        QMessageBox::warning(this, tr("Mix Failed"),
            tr("Could not export audio. Please check that you have at least one "
               "enabled track with content, and a SoundFont configured for MIDI tracks."));
    }
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
        QMessageBox::information(this, tr("Demo Data"), tr("Demo MIDI notes added. Try Project → Mix to export."));
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
