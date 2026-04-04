#include "vk_configdialog.h"
#include "vk_settings.h"

// Defined in virtual_midi_keyboard_main.cpp — swaps the global QTranslator at runtime
extern void loadVkTranslation(const QString& lang);
#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QSlider>
#include <QFileDialog>
#include <QIcon>
#include <QStyle>
#include <QFrame>
#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QMediaDevices>
#include <QAudioDevice>
#endif
#include <RtMidi.h>

static QWidget* createTabHeader(const QIcon& icon, const QString& text)
{
    auto* header = new QFrame();
    header->setFrameShape(QFrame::NoFrame);
    auto* layout = new QHBoxLayout(header);
    layout->setContentsMargins(0, 0, 0, 12);
    auto* iconLabel = new QLabel();
    iconLabel->setPixmap(icon.pixmap(32, 32));
    iconLabel->setFixedSize(40, 40);
    layout->addWidget(iconLabel);
    auto* textLabel = new QLabel(text);
    textLabel->setWordWrap(true);
    layout->addWidget(textLabel, 1);
    return header;
}

VkConfigDialog::VkConfigDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Configuration"));
    setMinimumSize(500, 460);
    resize(540, 480);

    auto* layout = new QVBoxLayout(this);
    m_tabWidget = new QTabWidget();
    setupMidiTab();
    setupAudioTab();
    setupLanguageTab();
    layout->addWidget(m_tabWidget);

    auto* applyBtn = new QPushButton(tr("Apply"));
    connect(applyBtn, &QPushButton::clicked, this, &VkConfigDialog::onApply);
    layout->addWidget(applyBtn);

    loadSettings();
}

void VkConfigDialog::setupMidiTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    QIcon noteIcon = QIcon::fromTheme("audio-x-generic",
        style()->standardIcon(QStyle::SP_MediaPlay));
    layout->addWidget(createTabHeader(noteIcon,
        tr("Here, you can configure settings related to MIDI.")));

    auto* outGroup = new QGroupBox(tr("MIDI Output Device"));
    auto* outLayout = new QVBoxLayout(outGroup);
    m_midiOutputCombo = new QComboBox();
    m_midiOutputCombo->setMinimumWidth(300);
    connect(m_midiOutputCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &VkConfigDialog::onOutputDeviceChanged);
    outLayout->addWidget(m_midiOutputCombo);
    layout->addWidget(outGroup);

    auto* inGroup = new QGroupBox(tr("MIDI Input Device"));
    auto* inLayout = new QVBoxLayout(inGroup);
    m_midiInputCombo = new QComboBox();
    m_midiInputCombo->setMinimumWidth(300);
    inLayout->addWidget(m_midiInputCombo);
    layout->addWidget(inGroup);

    auto* sfGroup = new QGroupBox(tr("SoundFont (for software synthesizer)"));
    auto* sfLayout = new QHBoxLayout(sfGroup);
    m_soundFontEdit = new QLineEdit();
    m_soundFontEdit->setPlaceholderText(tr("Path to .sf2 SoundFont file"));
    sfLayout->addWidget(m_soundFontEdit);
    m_loadSoundFontBtn = new QPushButton(tr("Browse..."));
    connect(m_loadSoundFontBtn, &QPushButton::clicked, this, &VkConfigDialog::onBrowseSoundFont);
    sfLayout->addWidget(m_loadSoundFontBtn);
    layout->addWidget(sfGroup);

    auto* refreshBtn = new QPushButton(tr("Refresh"));
    connect(refreshBtn, &QPushButton::clicked, this, &VkConfigDialog::refreshDevices);
    layout->addWidget(refreshBtn);

    auto* gainGroup = new QGroupBox(tr("Synthesizer Volume (Master Gain)"));
    auto* gainLayout = new QVBoxLayout(gainGroup);
    auto* gainRow = new QHBoxLayout();
    gainLayout->addLayout(gainRow);

    gainRow->addWidget(new QLabel(tr("Quiet")));

    // Slider represents gain × 100 (range 10–200 → 0.10–2.00)
    m_gainSlider = new QSlider(Qt::Horizontal);
    m_gainSlider->setRange(10, 200);
    m_gainSlider->setTickInterval(10);
    m_gainSlider->setTickPosition(QSlider::TicksBelow);
    m_gainSlider->setFocusPolicy(Qt::ClickFocus);
    gainRow->addWidget(m_gainSlider, 1);

    gainRow->addWidget(new QLabel(tr("Loud")));

    m_gainLabel = new QLabel();
    m_gainLabel->setAlignment(Qt::AlignCenter);
    gainLayout->addWidget(m_gainLabel);

    connect(m_gainSlider, &QSlider::valueChanged, this, [this](int v)
    {
        m_gainLabel->setText(QString::number(v) + "%");
    });

    layout->addWidget(gainGroup);
    layout->addStretch();
    m_tabWidget->addTab(widget, tr("MIDI"));
}

void VkConfigDialog::setupAudioTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    QIcon speakerIcon = QIcon::fromTheme("audio-volume-high",
        style()->standardIcon(QStyle::SP_MediaVolume));
    layout->addWidget(createTabHeader(speakerIcon,
        tr("Here, you can configure audio settings for the application; specifically, "
           "for any virtual MIDI synthesizer selected.")));

    auto* audioGroup = new QGroupBox(tr("Audio Output Device"));
    auto* audioLayout = new QVBoxLayout(audioGroup);
    m_audioOutputCombo = new QComboBox();
    m_audioOutputCombo->setMinimumWidth(300);
    audioLayout->addWidget(m_audioOutputCombo);
    layout->addWidget(audioGroup);

    layout->addStretch();
    m_tabWidget->addTab(widget, tr("Audio"));
}

void VkConfigDialog::onOutputDeviceChanged(int /*index*/)
{
}

void VkConfigDialog::setupLanguageTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    QIcon langIcon = QIcon::fromTheme("preferences-desktop-locale",
        style()->standardIcon(QStyle::SP_FileDialogContentsView));
    layout->addWidget(createTabHeader(langIcon,
        tr("Choose the display language for the application.")));

    auto* langGroup = new QGroupBox(tr("Language"));
    auto* langLayout = new QVBoxLayout(langGroup);
    m_languageCombo = new QComboBox();
    m_languageCombo->setMinimumWidth(250);
    m_languageCombo->addItem(tr("System Default"), QString());
    m_languageCombo->addItem(QStringLiteral("English"), QStringLiteral("en"));
    m_languageCombo->addItem(QStringLiteral("Deutsch"), QStringLiteral("de"));
    m_languageCombo->addItem(QStringLiteral("Español"), QStringLiteral("es"));
    m_languageCombo->addItem(QStringLiteral("Français"), QStringLiteral("fr"));
    m_languageCombo->addItem(QStringLiteral("日本語"), QStringLiteral("ja"));
    m_languageCombo->addItem(QStringLiteral("Português (Brasil)"), QStringLiteral("pt_BR"));
    m_languageCombo->addItem(QStringLiteral("中文（繁體）"), QStringLiteral("zh_TW"));
    m_languageCombo->addItem(QStringLiteral("中文（简体）"), QStringLiteral("zh_CN"));
    m_languageCombo->addItem(QStringLiteral("Pirate"), QStringLiteral("pirate"));
    langLayout->addWidget(m_languageCombo);
    layout->addWidget(langGroup);

    layout->addStretch();
    m_tabWidget->addTab(widget, tr("Language"));
}

void VkConfigDialog::refreshDevices()
{
    // Enumerate MIDI output ports (places we can SEND notes to).
    // We use a scoped block so the temporary RtMidiOut is destroyed before the
    // RtMidiIn is created.  On Linux/ALSA, every RtMidi instance registers an
    // ALSA sequencer client; if both exist at the same time each one shows up
    // in the other's port list ("RtMidi Output Client" in the input list, etc.).
    // Sequential scoped enumeration prevents that cross-contamination.
    // Item data stores the port NAME (a stable identifier) rather than the
    // integer port index, which shifts as ALSA clients appear and disappear.
    m_midiOutputCombo->clear();
    m_midiOutputCombo->addItem(tr("Built-in Software Synthesizer"), QString());
    {
        try
        {
            RtMidiOut midiOut;
            for (unsigned i = 0; i < midiOut.getPortCount(); ++i)
            {
                const QString name = QString::fromStdString(midiOut.getPortName(i));
                m_midiOutputCombo->addItem(name, name);
            }
        }
        catch (...) {}
    }  // RtMidiOut destroyed here — removes its ALSA client before RtMidiIn is created

    // Enumerate MIDI input ports (sources we can RECEIVE notes from — external controllers).
    m_midiInputCombo->clear();
    m_midiInputCombo->addItem(tr("(None)"), QString());
    {
        try
        {
            RtMidiIn midiIn;
            for (unsigned i = 0; i < midiIn.getPortCount(); ++i)
            {
                const QString name = QString::fromStdString(midiIn.getPortName(i));
                m_midiInputCombo->addItem(name, name);
            }
        }
        catch (...) {}
    }  // RtMidiIn destroyed here

#ifdef QT_MULTIMEDIA_AVAILABLE
    m_audioOutputCombo->clear();
    m_audioOutputCombo->addItem(tr("(Default)"), QByteArray());
    for (const auto& d : QMediaDevices::audioOutputs())
        m_audioOutputCombo->addItem(d.description(), d.id());
#else
    m_audioOutputCombo->clear();
    m_audioOutputCombo->addItem(tr("(Default)"), QByteArray());
#endif
}

void VkConfigDialog::loadSettings()
{
    refreshDevices();
    auto& s = VkSettings::instance();
    m_soundFontEdit->setText(s.soundFontPath());

    if (m_languageCombo)
    {
        int langIdx = m_languageCombo->findData(s.language());
        if (langIdx >= 0) m_languageCombo->setCurrentIndex(langIdx);
    }

    // Match by port name (empty = built-in synth / none)
    const QString outName = s.midiOutputPortName();
    for (int i = 0; i < m_midiOutputCombo->count(); ++i)
    {
        if (m_midiOutputCombo->itemData(i).toString() == outName)
        {
            m_midiOutputCombo->setCurrentIndex(i);
            break;
        }
    }
    onOutputDeviceChanged(m_midiOutputCombo->currentIndex());

    const QString inName = s.midiInputPortName();
    for (int i = 0; i < m_midiInputCombo->count(); ++i)
    {
        if (m_midiInputCombo->itemData(i).toString() == inName)
        {
            m_midiInputCombo->setCurrentIndex(i);
            break;
        }
    }

    const QByteArray audioId = s.audioOutputDeviceId();
    for (int i = 0; i < m_audioOutputCombo->count(); ++i)
    {
        if (m_audioOutputCombo->itemData(i).toByteArray() == audioId)
        {
            m_audioOutputCombo->setCurrentIndex(i);
            break;
        }
    }

    // Gain slider: stored as 0.1–2.0, slider range 10–200
    const int gainSliderVal = qBound(10, qRound(s.synthGain() * 100.0f), 200);
    m_gainSlider->setValue(gainSliderVal);
}

void VkConfigDialog::saveSettings()
{
    auto& s = VkSettings::instance();
    if (m_languageCombo)
        s.setLanguage(m_languageCombo->currentData().toString());
    s.setMidiOutputPortName(m_midiOutputCombo->currentData().toString());
    s.setMidiInputPortName(m_midiInputCombo->currentData().toString());
    s.setSoundFontPath(m_soundFontEdit->text().trimmed());
    s.setAudioOutputDeviceId(m_audioOutputCombo->currentData().toByteArray());
    s.setSynthGain(m_gainSlider->value() / 100.0f);
    s.save();
}

void VkConfigDialog::onBrowseSoundFont()
{
    const QString path = QFileDialog::getOpenFileName(this, tr("Select SoundFont"),
        m_soundFontEdit->text(), tr("SoundFont (*.sf2 *.SF2);;All files (*)"));
    if (!path.isEmpty()) m_soundFontEdit->setText(path);
}

void VkConfigDialog::onApply()
{
    const QString oldLang = VkSettings::instance().language();
    saveSettings();

    // If the language changed, swap the translator so the UI updates immediately
    const QString newLang = VkSettings::instance().language();
    if (newLang != oldLang)
        loadVkTranslation(newLang);

    accept();
}
