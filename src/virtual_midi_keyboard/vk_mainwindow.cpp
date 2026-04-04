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
#include <QEvent>

void VkMainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
        retranslateUi();
    QMainWindow::changeEvent(event);
}

void VkMainWindow::retranslateUi()
{
    setWindowTitle(tr("Virtual MIDI Keyboard"));
    menuBar()->clear();
    setupMenuBar();
    if (m_volumeLabel) m_volumeLabel->setText(tr("Volume"));
    if (m_octaveLabel) m_octaveLabel->setText(tr("Octave"));
    if (m_chorusLabel) m_chorusLabel->setText(tr("Chorus"));
    if (m_chorusApplyBtn) m_chorusApplyBtn->setText(tr("Apply"));
    if (m_soundLabel) m_soundLabel->setText(tr("Virtual MIDI sound:"));
}

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

void VkMainWindow::setupMenuBar()
{
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

void VkMainWindow::setupToolbar(QVBoxLayout* mainLayout)
{
    auto* toolbar = new QWidget();
    auto* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setSpacing(2);
    toolbarLayout->setContentsMargins(2, 2, 2, 2);

    m_volumeSpin = new QSpinBox();
    m_volumeSpin->setRange(0, 127);
    m_volumeSpin->setValue(127);
    m_volumeSpin->setFixedWidth(50);
    m_volumeSpin->setFocusPolicy(Qt::ClickFocus);
    connect(m_volumeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &VkMainWindow::onVolumeChanged);

    auto* volumeCol = new QVBoxLayout();
    volumeCol->setSpacing(1);
    volumeCol->setContentsMargins(0, 0, 0, 0);
    volumeCol->addWidget(m_volumeSpin);
    m_volumeLabel = new QLabel(tr("Volume"));
    volumeCol->addWidget(m_volumeLabel);
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
    m_octaveLabel = new QLabel(tr("Octave"));
    octaveCol->addWidget(m_octaveLabel);
    toolbarLayout->addLayout(octaveCol);

    auto* knob = new VkKnob(this);
    knob->setFixedSize(48, 56);
    knob->setRange(1, 127);
    knob->setValue(64);
    knob->setFocusPolicy(Qt::NoFocus);
    connect(knob, &VkKnob::valueChanged, [this](int v)
    {
        m_chorusValue = v;
        if (m_chorusEdit)
            m_chorusEdit->setText(QString::number(v));
    });
    m_chorusEdit = new QLineEdit();
    m_chorusEdit->setText("64");
    m_chorusEdit->setFixedWidth(34);
    m_chorusEdit->setValidator(new QIntValidator(1, 127, this));
    m_chorusEdit->setStyleSheet("background:#002200;color:#00ff00;font-family:monospace;");
    m_chorusEdit->setFocusPolicy(Qt::ClickFocus);
    connect(m_chorusEdit, &QLineEdit::editingFinished, [this, knob]()
    {
        const int v = m_chorusEdit->text().toInt();
        if (v >= 1 && v <= 127)
        {
            m_chorusValue = v;
            knob->setValue(v);
        }
    });

    auto* chorusCol = new QVBoxLayout();
    chorusCol->setSpacing(1);
    chorusCol->setContentsMargins(0, 0, 0, 0);
    auto* chorusTopRow = new QHBoxLayout();
    chorusTopRow->setSpacing(2);
    chorusTopRow->addWidget(knob);
    chorusTopRow->addWidget(m_chorusEdit);
    chorusCol->addLayout(chorusTopRow);
    m_chorusLabel = new QLabel(tr("Chorus"));
    chorusCol->addWidget(m_chorusLabel);
    m_chorusApplyBtn = new QPushButton(tr("Apply"));
    m_chorusApplyBtn->setFocusPolicy(Qt::NoFocus);
    connect(m_chorusApplyBtn, &QPushButton::clicked, this, &VkMainWindow::onChorusApply);
    chorusCol->addWidget(m_chorusApplyBtn);
    toolbarLayout->addLayout(chorusCol);

    m_soundGroup = new QWidget();
    auto* soundLayout = new QVBoxLayout(m_soundGroup);
    soundLayout->setSpacing(1);
    soundLayout->setContentsMargins(4, 0, 4, 0);
    m_soundLabel = new QLabel(tr("Virtual MIDI sound:"));
    soundLayout->addWidget(m_soundLabel);
    m_soundCombo = new QComboBox();
    m_soundCombo->setMinimumWidth(200);
    m_soundCombo->setFocusPolicy(Qt::ClickFocus);
    soundLayout->addWidget(m_soundCombo);
    connect(m_soundCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &VkMainWindow::onSoundApply);
    toolbarLayout->addWidget(m_soundGroup);
    connect(m_midiIo, &VkMidiIo::soundListChanged, this, &VkMainWindow::onSoundListChanged);

    toolbarLayout->addStretch();
    mainLayout->addWidget(toolbar);
}

void VkMainWindow::setupPiano(QVBoxLayout* mainLayout)
{
    m_piano = new VkPianoKeyboard(this);
    // Trying to make the piano window the width of the main window, but it's not working:
    QSize winSize = this->size();
    m_piano->resize(winSize.width(), m_piano->size().height());
    m_piano->setOctave(m_octave);
    connect(m_piano, &VkPianoKeyboard::noteOn, this, &VkMainWindow::onNoteOn);
    connect(m_piano, &VkPianoKeyboard::noteOff, this, &VkMainWindow::onNoteOff);
    mainLayout->addWidget(m_piano, 1);
}

void VkMainWindow::applyMidiOutput()
{
    m_midiIo->setOutputDevice(VkSettings::instance().midiOutputPortName());
    m_midiIo->setInputDevice(VkSettings::instance().midiInputPortName());
}

void VkMainWindow::onClose()
{
    QApplication::quit();
}

void VkMainWindow::onConfiguration()
{
    VkConfigDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        applyMidiOutput();
        // Apply the new gain immediately to the live synth without requiring restart
        m_midiIo->setSynthGain(VkSettings::instance().synthGain());
    }
}

void VkMainWindow::onHelp()
{
    VkHelpDialog dlg(this);
    dlg.exec();
}

void VkMainWindow::onOctaveChanged(int value)
{
    m_octave = value;
    if (m_piano) m_piano->setOctave(m_octave);
}

void VkMainWindow::onOctaveUp()
{
    m_octaveSpin->setValue(m_octaveSpin->value() + 1);
}

void VkMainWindow::onOctaveDown()
{
    m_octaveSpin->setValue(m_octaveSpin->value() - 1);
}

void VkMainWindow::onChorusChanged(int value)
{
    m_chorusValue = value;
}

void VkMainWindow::onChorusApply()
{
    if (m_chorusEdit)
    {
        const int v = m_chorusEdit->text().toInt();
        if (v >= 1 && v <= 127)
        {
            m_chorusValue = v;
            m_midiIo->sendControlChange(93, v);
        }
    }
}

void VkMainWindow::onNoteOn(int note)
{
    m_midiIo->sendNoteOn(note, 100);
}

void VkMainWindow::onNoteOff(int note)
{
    m_midiIo->sendNoteOff(note);
}

void VkMainWindow::onMidiNoteOn(int note, int velocity)
{
    m_midiIo->sendNoteOn(note, velocity);
}

void VkMainWindow::onMidiNoteOff(int note)
{
    m_midiIo->sendNoteOff(note);
}

// Standard General MIDI Level 1 program names (128 instruments)
static const char* const s_gmNames[128] = {
    // Piano
    "Acoustic Grand Piano","Bright Acoustic Piano","Electric Grand Piano","Honky-tonk Piano",
    "Electric Piano 1","Electric Piano 2","Harpsichord","Clavinet",
    // Chromatic Perc
    "Celesta","Glockenspiel","Music Box","Vibraphone","Marimba","Xylophone","Tubular Bells","Dulcimer",
    // Organ
    "Drawbar Organ","Percussive Organ","Rock Organ","Church Organ","Reed Organ","Accordion","Harmonica","Tango Accordion",
    // Guitar
    "Nylon String Guitar","Steel String Guitar","Jazz Guitar","Clean Electric Guitar",
    "Muted Electric Guitar","Overdriven Guitar","Distortion Guitar","Guitar Harmonics",
    // Bass
    "Acoustic Bass","Fingered Bass","Picked Bass","Fretless Bass",
    "Slap Bass 1","Slap Bass 2","Synth Bass 1","Synth Bass 2",
    // Strings
    "Violin","Viola","Cello","Contrabass","Tremolo Strings","Pizzicato Strings","Orchestral Harp","Timpani",
    // Ensemble
    "String Ensemble 1","String Ensemble 2","Synth Strings 1","Synth Strings 2",
    "Choir Aahs","Voice Oohs","Synth Voice","Orchestra Hit",
    // Brass
    "Trumpet","Trombone","Tuba","Muted Trumpet","French Horn","Brass Section","Synth Brass 1","Synth Brass 2",
    // Reed
    "Soprano Sax","Alto Sax","Tenor Sax","Baritone Sax","Oboe","English Horn","Bassoon","Clarinet",
    // Pipe
    "Piccolo","Flute","Recorder","Pan Flute","Blown Bottle","Shakuhachi","Whistle","Ocarina",
    // Synth Lead
    "Square Lead","Sawtooth Lead","Calliope Lead","Chiff Lead","Charang Lead","Voice Lead","Fifths Lead","Bass+Lead",
    // Synth Pad
    "New Age Pad","Warm Pad","Polysynth Pad","Choir Pad","Bowed Pad","Metallic Pad","Halo Pad","Sweep Pad",
    // Synth FX
    "Rain FX","Soundtrack FX","Crystal FX","Atmosphere FX","Brightness FX","Goblins FX","Echoes FX","Sci-fi FX",
    // Ethnic
    "Sitar","Banjo","Shamisen","Koto","Kalimba","Bagpipe","Fiddle","Shanai",
    // Percussive
    "Tinkle Bell","Agogo","Steel Drums","Woodblock","Taiko Drum","Melodic Tom","Synth Drum","Reverse Cymbal",
    // Sound FX
    "Guitar Fret Noise","Breath Noise","Seashore","Bird Tweet","Telephone Ring","Helicopter","Applause","Gunshot"
};

void VkMainWindow::onSoundListChanged()
{
    const bool usingFluid = m_midiIo->isUsingFluidSynth();
    {
        QSignalBlocker blocker(m_soundCombo);
        m_soundCombo->clear();
        if (usingFluid)
        {
            const auto presets = m_midiIo->getPresets();
            if (presets.isEmpty())
            {
                // FluidSynth loaded but no SoundFont — show placeholder
                m_soundCombo->addItem(tr("(No SoundFont — configure one in Settings)"), QPoint(0, 0));
            }
            else
            {
                for (const auto& p : presets)
                {
                    const QString label = QString("%1:%2 %3")
                        .arg(p.bank).arg(p.program, 3, 10, QChar('0')).arg(p.name);
                    m_soundCombo->addItem(label, QPoint(p.bank, p.program));
                }
            }
        }
        else if (m_midiIo->isOutputOpen())
        {
            // External MIDI device: offer standard GM program names
            for (int i = 0; i < 128; ++i)
            {
                const QString label = QString("%1 %2").arg(i + 1, 3).arg(s_gmNames[i]);
                m_soundCombo->addItem(label, QPoint(0, i));
            }
        }
        // else: no output open, leave combo empty
    }
    // Apply the first sound immediately so audio is ready on startup
    if (m_soundCombo->count() > 0)
        onSoundApply();

    // Warn the user if audio driver failed for the built-in synth
    if (usingFluid && !m_midiIo->isAudioDriverRunning())
    {
        m_soundCombo->setToolTip(tr("Audio driver failed to initialise. "
            "Check that PulseAudio or PipeWire is running."));
    }
    else
    {
        m_soundCombo->setToolTip(QString());
    }
}

void VkMainWindow::onSoundApply()
{
    if (m_soundCombo->currentIndex() < 0) return;
    const QPoint bp = m_soundCombo->currentData().toPoint();
    m_midiIo->selectPreset(bp.x(), bp.y());
}

void VkMainWindow::onVolumeChanged(int value)
{
    m_midiIo->sendControlChange(7, value);
}

void VkMainWindow::keyPressEvent(QKeyEvent* event)
{
    QApplication::sendEvent(m_piano, event);
}

void VkMainWindow::keyReleaseEvent(QKeyEvent* event)
{
    QApplication::sendEvent(m_piano, event);
}
