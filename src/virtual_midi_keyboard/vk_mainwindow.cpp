#include "vk_mainwindow.h"
#include "vk_configdialog.h"
#include "vk_helpdialog.h"
#include "vk_midiio.h"
#include "vk_pianokeyboard.h"
#include "vk_knob.h"
#include "vk_settings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QScrollArea>
#include <QIntValidator>
#include <QApplication>
#include <QKeyEvent>

VkMainWindow::VkMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Virtual MIDI Keyboard"));
    setMinimumSize(400, 300);
    resize(1280, 430);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    m_midiIo = new VkMidiIo(this);
    connect(m_midiIo, &VkMidiIo::midiNoteOn, this, &VkMainWindow::onMidiNoteOn);
    connect(m_midiIo, &VkMidiIo::midiNoteOff, this, &VkMainWindow::onMidiNoteOff);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    setupMenuBar();
    setupToolbar(mainLayout);
    setupPiano(mainLayout);
    applyMidiOutput();
    m_piano->setFocus();
}

void VkMainWindow::setupMenuBar() {
    auto* menuBar = this->menuBar();

    auto* fileMenu = menuBar->addMenu(tr("&File"));
    auto* closeAction = fileMenu->addAction(tr("&Close"));
    closeAction->setShortcut(QKeySequence::Quit);
    connect(closeAction, &QAction::triggered, this, &VkMainWindow::onClose);

    auto* settingsMenu = menuBar->addMenu(tr("&Settings"));
    auto* configAction = settingsMenu->addAction(tr("&Configuration"));
    configAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Comma));
    connect(configAction, &QAction::triggered, this, &VkMainWindow::onConfiguration);

    auto* helpMenu = menuBar->addMenu(tr("&Help"));
    auto* helpAction = helpMenu->addAction(tr("&Using this application"));
    helpAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_U));
    connect(helpAction, &QAction::triggered, this, &VkMainWindow::onHelp);
}

void VkMainWindow::setupToolbar(QVBoxLayout* mainLayout) {
    auto* toolbar = new QWidget();
    auto* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setSpacing(2);
    toolbarLayout->setContentsMargins(2, 2, 2, 2);

    m_volumeSpin = new QSpinBox();
    m_volumeSpin->setRange(0, 127);
    m_volumeSpin->setValue(100);
    m_volumeSpin->setFixedWidth(50);
    m_volumeSpin->setFocusPolicy(Qt::ClickFocus);
    connect(m_volumeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &VkMainWindow::onVolumeChanged);

    auto* volumeCol = new QVBoxLayout();
    volumeCol->setSpacing(1);
    volumeCol->setContentsMargins(0, 0, 0, 0);
    volumeCol->addWidget(m_volumeSpin);
    volumeCol->addWidget(new QLabel(tr("Volume")));
    toolbarLayout->addLayout(volumeCol);

    auto* octaveDown = new QPushButton(tr("<"));
    octaveDown->setFixedSize(20, 20);
    octaveDown->setFocusPolicy(Qt::NoFocus);
    connect(octaveDown, &QPushButton::clicked, this, &VkMainWindow::onOctaveDown);
    m_octaveSpin = new QSpinBox();
    m_octaveSpin->setRange(-3, 5);
    m_octaveSpin->setValue(0);
    m_octaveSpin->setFixedWidth(40);
    m_octaveSpin->setFocusPolicy(Qt::ClickFocus);
    connect(m_octaveSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &VkMainWindow::onOctaveChanged);
    auto* octaveUp = new QPushButton(tr(">"));
    octaveUp->setFixedSize(20, 20);
    octaveUp->setFocusPolicy(Qt::NoFocus);
    connect(octaveUp, &QPushButton::clicked, this, &VkMainWindow::onOctaveUp);

    auto* octaveCol = new QVBoxLayout();
    octaveCol->setSpacing(1);
    octaveCol->setContentsMargins(0, 0, 0, 0);
    auto* octaveRow = new QHBoxLayout();
    octaveRow->setSpacing(1);
    octaveRow->addWidget(octaveDown);
    octaveRow->addWidget(m_octaveSpin);
    octaveRow->addWidget(octaveUp);
    octaveCol->addLayout(octaveRow);
    octaveCol->addWidget(new QLabel(tr("Octave")));
    toolbarLayout->addLayout(octaveCol);

    auto* knob = new VkKnob(this);
    knob->setFixedSize(48, 56);
    knob->setRange(1, 127);
    knob->setValue(64);
    knob->setFocusPolicy(Qt::NoFocus);
    connect(knob, &VkKnob::valueChanged, [this](int v) { m_chorusValue = v; if (m_chorusEdit) m_chorusEdit->setText(QString::number(v)); });
    m_chorusEdit = new QLineEdit();
    m_chorusEdit->setText("64");
    m_chorusEdit->setFixedWidth(34);
    m_chorusEdit->setValidator(new QIntValidator(1, 127, this));
    m_chorusEdit->setStyleSheet("background:#002200;color:#00ff00;font-family:monospace;");
    m_chorusEdit->setFocusPolicy(Qt::ClickFocus);
    connect(m_chorusEdit, &QLineEdit::editingFinished, [this, knob]() {
        const int v = m_chorusEdit->text().toInt();
        if (v >= 1 && v <= 127) { m_chorusValue = v; knob->setValue(v); }
    });

    auto* chorusCol = new QVBoxLayout();
    chorusCol->setSpacing(1);
    chorusCol->setContentsMargins(0, 0, 0, 0);
    auto* chorusTopRow = new QHBoxLayout();
    chorusTopRow->setSpacing(2);
    chorusTopRow->addWidget(knob);
    chorusTopRow->addWidget(m_chorusEdit);
    chorusCol->addLayout(chorusTopRow);
    chorusCol->addWidget(new QLabel(tr("Chorus")));
    auto* applyBtn = new QPushButton(tr("Apply"));
    applyBtn->setFocusPolicy(Qt::NoFocus);
    connect(applyBtn, &QPushButton::clicked, this, &VkMainWindow::onChorusApply);
    chorusCol->addWidget(applyBtn);
    toolbarLayout->addLayout(chorusCol);

    m_soundGroup = new QWidget();
    auto* soundLayout = new QVBoxLayout(m_soundGroup);
    soundLayout->setSpacing(1);
    soundLayout->setContentsMargins(4, 0, 4, 0);
    soundLayout->addWidget(new QLabel(tr("Virtual MIDI sound:")));
    m_soundCombo = new QComboBox();
    m_soundCombo->setMinimumWidth(200);
    m_soundCombo->setFocusPolicy(Qt::ClickFocus);
    soundLayout->addWidget(m_soundCombo);
    connect(m_soundCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &VkMainWindow::onSoundApply);
    m_soundGroup->setEnabled(false);
    toolbarLayout->addWidget(m_soundGroup);
    connect(m_midiIo, &VkMidiIo::soundListChanged, this, &VkMainWindow::onSoundListChanged);

    toolbarLayout->addStretch();
    mainLayout->addWidget(toolbar);
}

void VkMainWindow::setupPiano(QVBoxLayout* mainLayout) {
    m_piano = new VkPianoKeyboard(this);
    // Trying to make the piano window the width of the main window, but it's not working:
    QSize winSize = this->size();
    m_piano->resize(winSize.width(), m_piano->size().height());
    m_piano->setOctave(m_octave);
    connect(m_piano, &VkPianoKeyboard::noteOn, this, &VkMainWindow::onNoteOn);
    connect(m_piano, &VkPianoKeyboard::noteOff, this, &VkMainWindow::onNoteOff);
    mainLayout->addWidget(m_piano, 1);
}

void VkMainWindow::applyMidiOutput() {
    m_midiIo->setOutputDevice(VkSettings::instance().midiOutputIndex());
    m_midiIo->setInputDevice(VkSettings::instance().midiInputIndex());
}

void VkMainWindow::onClose() {
    QApplication::quit();
}

void VkMainWindow::onConfiguration() {
    VkConfigDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
        applyMidiOutput();
}

void VkMainWindow::onHelp() {
    VkHelpDialog dlg(this);
    dlg.exec();
}

void VkMainWindow::onOctaveChanged(int value) {
    m_octave = value;
    if (m_piano) m_piano->setOctave(m_octave);
}

void VkMainWindow::onOctaveUp() {
    m_octaveSpin->setValue(m_octaveSpin->value() + 1);
}

void VkMainWindow::onOctaveDown() {
    m_octaveSpin->setValue(m_octaveSpin->value() - 1);
}

void VkMainWindow::onChorusChanged(int value) {
    m_chorusValue = value;
}

void VkMainWindow::onChorusApply() {
    if (m_chorusEdit) {
        const int v = m_chorusEdit->text().toInt();
        if (v >= 1 && v <= 127) {
            m_chorusValue = v;
            m_midiIo->sendControlChange(93, v);
        }
    }
}

void VkMainWindow::onNoteOn(int note) {
    m_midiIo->sendNoteOn(note, 100);
}

void VkMainWindow::onNoteOff(int note) {
    m_midiIo->sendNoteOff(note);
}

void VkMainWindow::onMidiNoteOn(int note, int velocity) {
    m_midiIo->sendNoteOn(note, velocity);
}

void VkMainWindow::onMidiNoteOff(int note) {
    m_midiIo->sendNoteOff(note);
}

void VkMainWindow::onSoundListChanged() {
    const bool soft = m_midiIo->isUsingFluidSynth();
    m_soundGroup->setEnabled(soft);
    {
        QSignalBlocker blocker(m_soundCombo);
        m_soundCombo->clear();
        if (soft) {
            for (const auto& p : m_midiIo->getPresets()) {
                const QString label = QString("%1:%2 %3")
                    .arg(p.bank).arg(p.program, 3, 10, QChar('0')).arg(p.name);
                m_soundCombo->addItem(label, QPoint(p.bank, p.program));
            }
        }
    }
}

void VkMainWindow::onSoundApply() {
    if (!m_soundGroup->isEnabled() || m_soundCombo->currentIndex() < 0) return;
    const QPoint bp = m_soundCombo->currentData().toPoint();
    m_midiIo->selectPreset(bp.x(), bp.y());
}

void VkMainWindow::onVolumeChanged(int value) {
    m_midiIo->sendControlChange(7, value);
}

void VkMainWindow::keyPressEvent(QKeyEvent* event) {
    QApplication::sendEvent(m_piano, event);
}

void VkMainWindow::keyReleaseEvent(QKeyEvent* event) {
    QApplication::sendEvent(m_piano, event);
}
