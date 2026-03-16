#include "projectsettingsdialog.h"
#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFileDialog>
#include <QDialogButtonBox>
#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QMediaDevices>
#include <QAudioDevice>
#endif
#include <RtMidi.h>
#include <string>

using std::string;

ProjectSettingsDialog::ProjectSettingsDialog(const ProjectSettings& settings,
                                             QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Project Settings"));
    setMinimumSize(450, 300);
    resize(500, 340);

    auto* layout = new QVBoxLayout(this);

    m_tabWidget = new QTabWidget();
    setupMidiTab();
    setupAudioTab();
    layout->addWidget(m_tabWidget);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok |
                                         QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);

    loadSettings(settings);
}

ProjectSettings ProjectSettingsDialog::projectSettings() const
{
    ProjectSettings s;
    s.midiDeviceIndex     = m_midiDeviceCombo->currentData().toInt();
    s.soundFontPath       = m_soundFontEdit->text().trimmed();
    s.audioInputDeviceId  = m_audioInputCombo->currentData().toByteArray();
    s.audioOutputDeviceId = m_audioOutputCombo->currentData().toByteArray();
    s.sampleRate          = m_sampleRateCombo->currentData().toInt();
    s.channelCount        = m_monoRadio->isChecked() ? 1 : 2;
    return s;
}

void ProjectSettingsDialog::setupMidiTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    auto* midiGroup = new QGroupBox(tr("MIDI Device"));
    auto* midiLayout = new QVBoxLayout(midiGroup);
    m_midiDeviceCombo = new QComboBox();
    m_midiDeviceCombo->setMinimumWidth(300);
    midiLayout->addWidget(m_midiDeviceCombo);

    auto* refreshBtn = new QPushButton(tr("Refresh"));
    connect(refreshBtn, &QPushButton::clicked,
            this, &ProjectSettingsDialog::refreshDevices);
    midiLayout->addWidget(refreshBtn);
    layout->addWidget(midiGroup);

    auto* sfGroup = new QGroupBox(tr("SoundFont (for MIDI synthesis)"));
    auto* sfLayout = new QHBoxLayout(sfGroup);
    m_soundFontEdit = new QLineEdit();
    m_soundFontEdit->setPlaceholderText(tr("Path to .sf2 SoundFont file (empty = app default)"));
    sfLayout->addWidget(m_soundFontEdit);

    auto* browseBtn = new QPushButton(tr("Browse..."));
    connect(browseBtn, &QPushButton::clicked,
            this, &ProjectSettingsDialog::onBrowseSoundFont);
    sfLayout->addWidget(browseBtn);
    layout->addWidget(sfGroup);

    layout->addStretch();
    m_tabWidget->addTab(widget, tr("MIDI"));
}

void ProjectSettingsDialog::setupAudioTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    // --- Device selection ---
    auto* inputGroup = new QGroupBox(tr("Audio Input Device"));
    auto* inputLayout = new QVBoxLayout(inputGroup);
    m_audioInputCombo = new QComboBox();
    m_audioInputCombo->setMinimumWidth(300);
    inputLayout->addWidget(m_audioInputCombo);
    layout->addWidget(inputGroup);

    auto* outputGroup = new QGroupBox(tr("Audio Output Device"));
    auto* outputLayout = new QVBoxLayout(outputGroup);
    m_audioOutputCombo = new QComboBox();
    m_audioOutputCombo->setMinimumWidth(300);
    outputLayout->addWidget(m_audioOutputCombo);
    layout->addWidget(outputGroup);

    // --- Audio format (applies to all audio tracks in this project) ---
    auto* fmtGroup = new QGroupBox(tr("Audio Format (applies to all audio tracks)"));
    auto* fmtLayout = new QVBoxLayout(fmtGroup);

    // Sample rate
    auto* srRow = new QHBoxLayout();
    srRow->addWidget(new QLabel(tr("Sample rate:")));
    m_sampleRateCombo = new QComboBox();
    m_sampleRateCombo->addItem(QStringLiteral("22050 Hz"),  22050);
    m_sampleRateCombo->addItem(QStringLiteral("44100 Hz"),  44100);
    m_sampleRateCombo->addItem(QStringLiteral("48000 Hz"),  48000);
    m_sampleRateCombo->addItem(QStringLiteral("96000 Hz"),  96000);
    m_sampleRateCombo->setCurrentIndex(1);  // default 44100
    srRow->addWidget(m_sampleRateCombo);
    srRow->addStretch();
    fmtLayout->addLayout(srRow);

    // Channels
    auto* chRow = new QHBoxLayout();
    chRow->addWidget(new QLabel(tr("Channels:")));
    m_monoRadio   = new QRadioButton(tr("Mono"));
    m_stereoRadio = new QRadioButton(tr("Stereo"));
    m_stereoRadio->setChecked(true);  // default stereo
    auto* chGroup = new QButtonGroup(this);
    chGroup->addButton(m_monoRadio,   1);
    chGroup->addButton(m_stereoRadio, 2);
    chRow->addWidget(m_monoRadio);
    chRow->addWidget(m_stereoRadio);
    chRow->addStretch();
    fmtLayout->addLayout(chRow);

    layout->addWidget(fmtGroup);
    layout->addStretch();
    m_tabWidget->addTab(widget, tr("Audio"));
}

void ProjectSettingsDialog::refreshDevices()
{
    m_midiDeviceCombo->clear();
    // Index -1 = use app default; index 0 = built-in FluidSynth; index >=1 = external port
    m_midiDeviceCombo->addItem(tr("(App default)"), -1);
    m_midiDeviceCombo->addItem(tr("Built-in FluidSynth synthesizer"), 0);
    try
    {
        RtMidiOut midiOut;
        const unsigned int portCount = midiOut.getPortCount();
        for (unsigned int i = 0; i < portCount; ++i)
        {
            const string name = midiOut.getPortName(i);
            // External ports are 1-based so index 0 stays reserved for FluidSynth
            m_midiDeviceCombo->addItem(QString::fromStdString(name), static_cast<int>(i + 1));
        }
    }
    catch (...)
    {
        // External MIDI output not available; FluidSynth entry above still works
    }

    m_audioInputCombo->clear();
    m_audioOutputCombo->clear();
#ifdef QT_MULTIMEDIA_AVAILABLE
    m_audioInputCombo->addItem(tr("(App default)"), QByteArray());
    const auto inputs = QMediaDevices::audioInputs();
    for (const auto& device : inputs)
        m_audioInputCombo->addItem(device.description(), device.id());

    m_audioOutputCombo->addItem(tr("(App default)"), QByteArray());
    const auto outputs = QMediaDevices::audioOutputs();
    for (const auto& device : outputs)
        m_audioOutputCombo->addItem(device.description(), device.id());
#else
    m_audioInputCombo->addItem(tr("(App default)"), QByteArray());
    m_audioOutputCombo->addItem(tr("(App default)"), QByteArray());
#endif
}

void ProjectSettingsDialog::loadSettings(const ProjectSettings& settings)
{
    refreshDevices();

    for (int i = 0; i < m_midiDeviceCombo->count(); ++i)
    {
        if (m_midiDeviceCombo->itemData(i).toInt() == settings.midiDeviceIndex)
        {
            m_midiDeviceCombo->setCurrentIndex(i);
            break;
        }
    }

    m_soundFontEdit->setText(settings.soundFontPath);

    for (int i = 0; i < m_audioInputCombo->count(); ++i)
    {
        if (m_audioInputCombo->itemData(i).toByteArray() == settings.audioInputDeviceId)
        {
            m_audioInputCombo->setCurrentIndex(i);
            break;
        }
    }

    for (int i = 0; i < m_audioOutputCombo->count(); ++i)
    {
        if (m_audioOutputCombo->itemData(i).toByteArray() == settings.audioOutputDeviceId)
        {
            m_audioOutputCombo->setCurrentIndex(i);
            break;
        }
    }

    // Sample rate
    int srIdx = m_sampleRateCombo->findData(settings.sampleRate);
    m_sampleRateCombo->setCurrentIndex(srIdx >= 0 ? srIdx : 1);  // fall back to 44100

    // Channels
    if (settings.channelCount == 1)
    {
        m_monoRadio->setChecked(true);
    }
    else
    {
        m_stereoRadio->setChecked(true);
    }
}

void ProjectSettingsDialog::onBrowseSoundFont()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Select SoundFont"),
        m_soundFontEdit->text(),
        tr("SoundFont files (*.sf2 *.SF2);;All files (*)"));
    if (!path.isEmpty())
        m_soundFontEdit->setText(path);
}
