#include "settingsdialog.h"
#include "appsettings.h"
#include "themeutils.h"

// Defined in musicians_canvas_main.cpp — swaps the global QTranslator at runtime
extern void loadAppTranslation(const QString& lang);
#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <QIcon>
#include <QStyle>
#include <QFrame>
#include <QFileDialog>
#include <QDialogButtonBox>
#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QMediaDevices>
#include <QAudioDevice>
#endif
#include <RtMidi.h>
#include <string>

using std::string;

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Configuration"));
    setMinimumSize(450, 350);
    resize(500, 380);

    auto* layout = new QVBoxLayout(this);
    m_tabWidget = new QTabWidget();
    setupGeneralTab();
    setupDisplayTab();
    setupMidiTab();
    setupAudioTab();
    setupLanguageTab();
    layout->addWidget(m_tabWidget);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    auto* applyBtn = new QPushButton(tr("Apply"));
    applyBtn->setObjectName("applyBtn");
    connect(applyBtn, &QPushButton::clicked, this, &SettingsDialog::onApply);
    buttonLayout->addWidget(applyBtn);

    auto* closeBtn = new QPushButton(tr("Close"));
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(closeBtn);
    layout->addLayout(buttonLayout);

    loadSettings();
}

static QWidget* createTabHeader(const QIcon& icon, const QString& text)
{
    auto* header = new QFrame();
    header->setFrameShape(QFrame::NoFrame);
    auto* headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 12);
    auto* iconLabel = new QLabel();
    iconLabel->setPixmap(icon.pixmap(32, 32));
    iconLabel->setFixedSize(40, 40);
    iconLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(iconLabel);
    auto* textLabel = new QLabel(text);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    headerLayout->addWidget(textLabel, 1);
    return header;
}

void SettingsDialog::setupGeneralTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    QIcon gearIcon = QIcon::fromTheme("preferences-system",
        style()->standardIcon(QStyle::SP_FileDialogContentsView));
    layout->addWidget(createTabHeader(gearIcon,
        tr("Here, you can configure general application settings.")));

    auto* themeGroup = new QGroupBox(tr("Appearance"));
    auto* themeLayout = new QVBoxLayout(themeGroup);
    themeLayout->addWidget(new QLabel(tr("Theme:")));
    m_themeCombo = new QComboBox();
    m_themeCombo->addItem(tr("Dark"), QString("dark"));
    m_themeCombo->addItem(tr("Light"), QString("light"));
    m_themeCombo->setMinimumWidth(200);
    themeLayout->addWidget(m_themeCombo);
    layout->addWidget(themeGroup);

    auto* debugGroup = new QGroupBox(tr("Diagnostics"));
    auto* debugLayout = new QVBoxLayout(debugGroup);
    m_debugLogCheck = new QCheckBox(tr("Write recording debug log"));
    debugLayout->addWidget(m_debugLogCheck);
    layout->addWidget(debugGroup);

    layout->addStretch();
    m_tabWidget->addTab(widget, tr("General"));
}

void SettingsDialog::setupDisplayTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    QIcon displayIcon = QIcon::fromTheme("video-display",
        style()->standardIcon(QStyle::SP_DesktopIcon));
    layout->addWidget(createTabHeader(displayIcon,
        tr("Configure the appearance of display elements.")));

    auto* ledGroup = new QGroupBox(tr("Numeric Display"));
    auto* ledLayout = new QVBoxLayout(ledGroup);
    ledLayout->addWidget(new QLabel(tr("Numeric display LED color:")));
    m_ledColorCombo = new QComboBox();
    m_ledColorCombo->setMinimumWidth(200);
    m_ledColorCombo->addItem(tr("Light Red"), QStringLiteral("light_red"));
    m_ledColorCombo->addItem(tr("Dark Red"), QStringLiteral("dark_red"));
    m_ledColorCombo->addItem(tr("Light Green"), QStringLiteral("light_green"));
    m_ledColorCombo->addItem(tr("Dark Green"), QStringLiteral("dark_green"));
    m_ledColorCombo->addItem(tr("Light Blue"), QStringLiteral("light_blue"));
    m_ledColorCombo->addItem(tr("Dark Blue"), QStringLiteral("dark_blue"));
    m_ledColorCombo->addItem(tr("Yellow"), QStringLiteral("yellow"));
    m_ledColorCombo->addItem(tr("Orange"), QStringLiteral("orange"));
    m_ledColorCombo->addItem(tr("Light Cyan"), QStringLiteral("light_cyan"));
    m_ledColorCombo->addItem(tr("Dark Cyan"), QStringLiteral("dark_cyan"));
    ledLayout->addWidget(m_ledColorCombo);
    layout->addWidget(ledGroup);

    layout->addStretch();
    m_tabWidget->addTab(widget, tr("Display"));
}

void SettingsDialog::setupMidiTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    QIcon noteIcon = QIcon::fromTheme("audio-x-generic",
        style()->standardIcon(QStyle::SP_MediaPlay));
    layout->addWidget(createTabHeader(noteIcon,
        tr("Here, you can configure settings related to MIDI.")));

    auto* midiGroup = new QGroupBox(tr("MIDI Device"));
    auto* midiLayout = new QVBoxLayout(midiGroup);
    m_midiDeviceCombo = new QComboBox();
    m_midiDeviceCombo->setMinimumWidth(300);
    midiLayout->addWidget(m_midiDeviceCombo);

    auto* refreshMidiBtn = new QPushButton(tr("Refresh"));
    connect(refreshMidiBtn, &QPushButton::clicked, this, &SettingsDialog::refreshDevices);
    midiLayout->addWidget(refreshMidiBtn);

    layout->addWidget(midiGroup);

    auto* sfGroup = new QGroupBox(tr("SoundFont (for MIDI synthesis)"));
    auto* sfLayout = new QHBoxLayout(sfGroup);
    m_soundFontEdit = new QLineEdit();
    m_soundFontEdit->setPlaceholderText(tr("Path to .sf2 SoundFont file"));
    sfLayout->addWidget(m_soundFontEdit);

    auto* browseBtn = new QPushButton(tr("Browse..."));
    connect(browseBtn, &QPushButton::clicked, this, &SettingsDialog::onBrowseSoundFont);
    sfLayout->addWidget(browseBtn);
    layout->addWidget(sfGroup);

    layout->addStretch();
    m_tabWidget->addTab(widget, tr("MIDI"));
}

void SettingsDialog::setupAudioTab()
{
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    QIcon speakerIcon = QIcon::fromTheme("audio-volume-high",
        style()->standardIcon(QStyle::SP_MediaVolume));
    layout->addWidget(createTabHeader(speakerIcon,
        tr("Here, you can configure audio settings for the application.")));

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

    layout->addStretch();
    m_tabWidget->addTab(widget, tr("Audio"));
}

void SettingsDialog::setupLanguageTab()
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
    m_languageCombo->addItem(QStringLiteral("Dansk"), QStringLiteral("da"));
    m_languageCombo->addItem(QStringLiteral("Ελληνικά"), QStringLiteral("el"));
    m_languageCombo->addItem(QStringLiteral("Gaeilge"), QStringLiteral("ga"));
    m_languageCombo->addItem(QStringLiteral("Norsk"), QStringLiteral("nb"));
    m_languageCombo->addItem(QStringLiteral("Polski"), QStringLiteral("pl"));
    m_languageCombo->addItem(QStringLiteral("Русский"), QStringLiteral("ru"));
    m_languageCombo->addItem(QStringLiteral("Suomi"), QStringLiteral("fi"));
    m_languageCombo->addItem(QStringLiteral("Svenska"), QStringLiteral("sv"));
    m_languageCombo->addItem(QStringLiteral("Cymraeg"), QStringLiteral("cy"));
    m_languageCombo->addItem(QStringLiteral("Pirate"), QStringLiteral("pirate"));
    langLayout->addWidget(m_languageCombo);
    layout->addWidget(langGroup);

    layout->addStretch();
    m_tabWidget->addTab(widget, tr("Language"));
}

void SettingsDialog::refreshDevices()
{
    m_midiDeviceCombo->clear();
    // Index 0 is always the built-in FluidSynth synthesizer (matches VK convention
    // where setOutputDevice(<=0) uses the embedded FluidSynth).
    m_midiDeviceCombo->addItem(tr("Built-in FluidSynth synthesizer"), 0);
    try
    {
        RtMidiOut midiOut;
        const unsigned int portCount = midiOut.getPortCount();
        for (unsigned int i = 0; i < portCount; ++i)
        {
            const string name = midiOut.getPortName(i);
            // External ports are stored as 1-based so index 0 stays reserved for FluidSynth
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
    const auto inputs = QMediaDevices::audioInputs();
    m_audioInputCombo->addItem(tr("(Default)"), QByteArray());
    for (const auto& device : inputs)
    {
        m_audioInputCombo->addItem(device.description(), device.id());
    }
    const auto outputs = QMediaDevices::audioOutputs();
    m_audioOutputCombo->addItem(tr("(Default)"), QByteArray());
    for (const auto& device : outputs)
    {
        m_audioOutputCombo->addItem(device.description(), device.id());
    }
#else
    m_audioInputCombo->addItem(tr("(Default - install qt6-multimedia-dev for device list)"), QByteArray());
    m_audioOutputCombo->addItem(tr("(Default - install qt6-multimedia-dev for device list)"), QByteArray());
#endif
}

void SettingsDialog::loadSettings()
{
    refreshDevices();
    auto& settings = AppSettings::instance();
    if (m_themeCombo)
    {
        int idx = m_themeCombo->findData(settings.theme());
        if (idx >= 0) m_themeCombo->setCurrentIndex(idx);
    }
    if (m_debugLogCheck)
        m_debugLogCheck->setChecked(settings.recordingDebugLog());
    if (m_ledColorCombo)
    {
        int idx = m_ledColorCombo->findData(settings.ledColor());
        if (idx >= 0) m_ledColorCombo->setCurrentIndex(idx);
    }
    if (m_languageCombo)
    {
        int langIdx = m_languageCombo->findData(settings.language());
        if (langIdx >= 0) m_languageCombo->setCurrentIndex(langIdx);
    }
    m_soundFontEdit->setText(settings.soundFontPath());

    int midiIdx = settings.midiDeviceIndex();
    for (int i = 0; i < m_midiDeviceCombo->count(); ++i)
    {
        if (m_midiDeviceCombo->itemData(i).toInt() == midiIdx)
        {
            m_midiDeviceCombo->setCurrentIndex(i);
            break;
        }
    }

    QByteArray audioId = settings.audioInputDeviceId();
    for (int i = 0; i < m_audioInputCombo->count(); ++i)
    {
        if (m_audioInputCombo->itemData(i).toByteArray() == audioId)
        {
            m_audioInputCombo->setCurrentIndex(i);
            break;
        }
    }

    QByteArray outputId = settings.audioOutputDeviceId();
    for (int i = 0; i < m_audioOutputCombo->count(); ++i)
    {
        if (m_audioOutputCombo->itemData(i).toByteArray() == outputId)
        {
            m_audioOutputCombo->setCurrentIndex(i);
            break;
        }
    }
}

void SettingsDialog::saveSettings()
{
    auto& settings = AppSettings::instance();
    if (m_themeCombo)
    {
        settings.setTheme(m_themeCombo->currentData().toString());
    }
    if (m_debugLogCheck)
        settings.setRecordingDebugLog(m_debugLogCheck->isChecked());
    if (m_ledColorCombo)
        settings.setLedColor(m_ledColorCombo->currentData().toString());
    if (m_languageCombo)
        settings.setLanguage(m_languageCombo->currentData().toString());
    settings.setMidiDeviceIndex(m_midiDeviceCombo->currentData().toInt());
    settings.setSoundFontPath(m_soundFontEdit->text().trimmed());
    settings.setAudioInputDeviceId(m_audioInputCombo->currentData().toByteArray());
    for (int i = 0; i < m_audioInputCombo->count(); ++i)
    {
        if (m_audioInputCombo->itemData(i).toByteArray() == m_audioInputCombo->currentData().toByteArray())
        {
            settings.setAudioInputDeviceIndex(i);
            break;
        }
    }
    settings.setAudioOutputDeviceId(m_audioOutputCombo->currentData().toByteArray());
    settings.save();
}

void SettingsDialog::onBrowseSoundFont()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Select SoundFont"),
        m_soundFontEdit->text(),
        tr("SoundFont files (*.sf2 *.SF2);;All files (*)"));
    if (!path.isEmpty())
    {
        m_soundFontEdit->setText(path);
    }
}

void SettingsDialog::onApply()
{
    const QString oldLang = AppSettings::instance().language();
    saveSettings();
    ThemeUtils::applySavedTheme();

    // If the language changed, swap the translator so the UI updates immediately
    const QString newLang = AppSettings::instance().language();
    if (newLang != oldLang)
        loadAppTranslation(newLang);

    // Signal the parent (MainWindow) that display settings may have changed
    emit accepted();
    done(QDialog::Accepted);
}
