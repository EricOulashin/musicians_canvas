#include "settingsdialog.h"
#include "appsettings.h"
#include "themeutils.h"
#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
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

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle(tr("Configuration"));
    setMinimumSize(450, 350);
    resize(500, 380);

    auto* layout = new QVBoxLayout(this);
    m_tabWidget = new QTabWidget();
    setupGeneralTab();
    setupMidiTab();
    setupAudioTab();
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

static QWidget* createTabHeader(const QIcon& icon, const QString& text) {
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

void SettingsDialog::setupGeneralTab() {
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
    layout->addStretch();
    m_tabWidget->addTab(widget, tr("General"));
}

void SettingsDialog::setupMidiTab() {
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

void SettingsDialog::setupAudioTab() {
    auto* widget = new QWidget();
    auto* layout = new QVBoxLayout(widget);

    QIcon speakerIcon = QIcon::fromTheme("audio-volume-high",
        style()->standardIcon(QStyle::SP_MediaVolume));
    layout->addWidget(createTabHeader(speakerIcon,
        tr("Here, you can configure audio settings for the application.")));

    auto* audioGroup = new QGroupBox(tr("Audio Input Device"));
    auto* audioLayout = new QVBoxLayout(audioGroup);
    m_audioInputCombo = new QComboBox();
    m_audioInputCombo->setMinimumWidth(300);
    audioLayout->addWidget(m_audioInputCombo);
    layout->addWidget(audioGroup);
    layout->addStretch();
    m_tabWidget->addTab(widget, tr("Audio"));
}

void SettingsDialog::refreshDevices() {
    m_midiDeviceCombo->clear();
    try {
        RtMidiIn midiIn;
        unsigned int portCount = midiIn.getPortCount();
        m_midiDeviceCombo->addItem(tr("(No MIDI device)"), -1);
        for (unsigned int i = 0; i < portCount; ++i) {
            std::string name = midiIn.getPortName(i);
            m_midiDeviceCombo->addItem(QString::fromStdString(name), (int)i);
        }
    } catch (...) {
        m_midiDeviceCombo->addItem(tr("(MIDI not available)"), -1);
    }

    m_audioInputCombo->clear();
#ifdef QT_MULTIMEDIA_AVAILABLE
    const auto inputs = QMediaDevices::audioInputs();
    m_audioInputCombo->addItem(tr("(Default)"), QByteArray());
    for (const auto& device : inputs) {
        m_audioInputCombo->addItem(device.description(), device.id());
    }
#else
    m_audioInputCombo->addItem(tr("(Default - install qt6-multimedia-dev for device list)"), QByteArray());
#endif
}

void SettingsDialog::loadSettings() {
    refreshDevices();
    auto& settings = AppSettings::instance();
    if (m_themeCombo) {
        int idx = m_themeCombo->findData(settings.theme());
        if (idx >= 0) m_themeCombo->setCurrentIndex(idx);
    }
    m_soundFontEdit->setText(settings.soundFontPath());

    int midiIdx = settings.midiDeviceIndex();
    for (int i = 0; i < m_midiDeviceCombo->count(); ++i) {
        if (m_midiDeviceCombo->itemData(i).toInt() == midiIdx) {
            m_midiDeviceCombo->setCurrentIndex(i);
            break;
        }
    }

    QByteArray audioId = settings.audioInputDeviceId();
    for (int i = 0; i < m_audioInputCombo->count(); ++i) {
        if (m_audioInputCombo->itemData(i).toByteArray() == audioId) {
            m_audioInputCombo->setCurrentIndex(i);
            break;
        }
    }
}

void SettingsDialog::saveSettings() {
    auto& settings = AppSettings::instance();
    if (m_themeCombo) {
        settings.setTheme(m_themeCombo->currentData().toString());
    }
    settings.setMidiDeviceIndex(m_midiDeviceCombo->currentData().toInt());
    settings.setSoundFontPath(m_soundFontEdit->text().trimmed());
    settings.setAudioInputDeviceId(m_audioInputCombo->currentData().toByteArray());
    for (int i = 0; i < m_audioInputCombo->count(); ++i) {
        if (m_audioInputCombo->itemData(i).toByteArray() == m_audioInputCombo->currentData().toByteArray()) {
            settings.setAudioInputDeviceIndex(i);
            break;
        }
    }
    settings.save();
}

void SettingsDialog::onBrowseSoundFont() {
    QString path = QFileDialog::getOpenFileName(this, tr("Select SoundFont"),
        m_soundFontEdit->text(),
        tr("SoundFont files (*.sf2 *.SF2);;All files (*)"));
    if (!path.isEmpty()) {
        m_soundFontEdit->setText(path);
    }
}

void SettingsDialog::onApply() {
    saveSettings();
    ThemeUtils::applySavedTheme();
    accept();
}
