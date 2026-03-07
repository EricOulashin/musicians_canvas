#include "vk_configdialog.h"
#include "vk_settings.h"
#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QFileDialog>
#include <QIcon>
#include <QStyle>
#include <QFrame>
#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QMediaDevices>
#include <QAudioDevice>
#endif
#include <RtMidi.h>

static QWidget* createTabHeader(const QIcon& icon, const QString& text) {
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

VkConfigDialog::VkConfigDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle(tr("Configuration"));
    setMinimumSize(450, 350);
    resize(500, 380);

    auto* layout = new QVBoxLayout(this);
    m_tabWidget = new QTabWidget();
    setupMidiTab();
    setupAudioTab();
    layout->addWidget(m_tabWidget);

    auto* applyBtn = new QPushButton(tr("Apply"));
    connect(applyBtn, &QPushButton::clicked, this, &VkConfigDialog::onApply);
    layout->addWidget(applyBtn);

    loadSettings();
}

void VkConfigDialog::setupMidiTab() {
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

    layout->addStretch();
    m_tabWidget->addTab(widget, tr("MIDI"));
}

void VkConfigDialog::setupAudioTab() {
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

void VkConfigDialog::onOutputDeviceChanged(int /*index*/) {
}

void VkConfigDialog::refreshDevices() {
    m_midiOutputCombo->clear();
    m_midiOutputCombo->addItem(tr("Built-in Software Synthesizer"), 0);
    try {
        RtMidiOut midiOut;
        for (unsigned i = 0; i < midiOut.getPortCount(); ++i)
            m_midiOutputCombo->addItem(QString::fromStdString(midiOut.getPortName(i)), static_cast<int>(i + 1));
    } catch (...) {}

    m_midiInputCombo->clear();
    m_midiInputCombo->addItem(tr("(None)"), -1);
    try {
        RtMidiIn midiIn;
        for (unsigned i = 0; i < midiIn.getPortCount(); ++i)
            m_midiInputCombo->addItem(QString::fromStdString(midiIn.getPortName(i)), static_cast<int>(i));
    } catch (...) {}

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

void VkConfigDialog::loadSettings() {
    refreshDevices();
    auto& s = VkSettings::instance();
    m_soundFontEdit->setText(s.soundFontPath());

    for (int i = 0; i < m_midiOutputCombo->count(); ++i) {
        if (m_midiOutputCombo->itemData(i).toInt() == s.midiOutputIndex()) {
            m_midiOutputCombo->setCurrentIndex(i);
            break;
        }
    }
    onOutputDeviceChanged(m_midiOutputCombo->currentIndex());

    for (int i = 0; i < m_midiInputCombo->count(); ++i) {
        if (m_midiInputCombo->itemData(i).toInt() == s.midiInputIndex()) {
            m_midiInputCombo->setCurrentIndex(i);
            break;
        }
    }

    const QByteArray audioId = s.audioOutputDeviceId();
    for (int i = 0; i < m_audioOutputCombo->count(); ++i) {
        if (m_audioOutputCombo->itemData(i).toByteArray() == audioId) {
            m_audioOutputCombo->setCurrentIndex(i);
            break;
        }
    }
}

void VkConfigDialog::saveSettings() {
    auto& s = VkSettings::instance();
    s.setMidiOutputIndex(m_midiOutputCombo->currentData().toInt());
    s.setMidiInputIndex(m_midiInputCombo->currentData().toInt());
    s.setSoundFontPath(m_soundFontEdit->text().trimmed());
    s.setAudioOutputDeviceId(m_audioOutputCombo->currentData().toByteArray());
    s.save();
}

void VkConfigDialog::onBrowseSoundFont() {
    const QString path = QFileDialog::getOpenFileName(this, tr("Select SoundFont"),
        m_soundFontEdit->text(), tr("SoundFont (*.sf2 *.SF2);;All files (*)"));
    if (!path.isEmpty()) m_soundFontEdit->setText(path);
}

void VkConfigDialog::onApply() {
    saveSettings();
    accept();
}
