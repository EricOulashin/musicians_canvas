#include "trackwidget.h"
#include "waveformwidget.h"
#include "midivisualizationwidget.h"
#include "trackconfigdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QToolButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QStackedWidget>
#include <QSlider>
#include <QLabel>
#include <cmath>

TrackWidget::TrackWidget(const TrackData& data, QWidget* parent)
    : QFrame(parent), m_data(data), m_lastValidName(data.name)
{
    setupUi();
}

void TrackWidget::setupUi()
{
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setLineWidth(1);
    setObjectName("trackFrame");
    setMinimumHeight(188);
    setMaximumHeight(220);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(8, 6, 8, 6);
    outerLayout->setSpacing(4);

    // Top row: Options button + track name edit + remove button
    auto* topRow = new QHBoxLayout();
    topRow->setSpacing(8);

    auto* leftBtns = new QVBoxLayout();
    leftBtns->setSpacing(4);
    leftBtns->setContentsMargins(0, 0, 0, 0);

    m_configButton = new QPushButton(tr("Options"));
    m_configButton->setFixedWidth(80);
    connect(m_configButton, &QPushButton::clicked, this,
            [this]()
            {
                emit configurationRequested(this);
            });
    leftBtns->addWidget(m_configButton);

    m_effectsButton = new QPushButton(tr("Effects"));
    m_effectsButton->setFixedWidth(80);
    m_effectsButton->setToolTip(
        tr("Configure insert effects for recordings on this audio track."));
    connect(m_effectsButton, &QPushButton::clicked, this, &TrackWidget::onEffectsClicked);
    leftBtns->addWidget(m_effectsButton);

    topRow->addLayout(leftBtns);

    m_typeIconLabel = new QToolButton();
    m_typeIconLabel->setFixedSize(26, 26);
    m_typeIconLabel->setAutoRaise(true);
    m_typeIconLabel->setCursor(Qt::PointingHandCursor);
    // Use a slightly larger font so the emoji is clear at the icon size
    QFont iconFont = m_typeIconLabel->font();
    iconFont.setPointSize(14);
    m_typeIconLabel->setFont(iconFont);
    connect(m_typeIconLabel, &QToolButton::clicked, this,
            [this]()
            {
                emit configurationRequested(this);
            });
    topRow->addWidget(m_typeIconLabel);

    m_nameEdit = new QLineEdit();
    m_nameEdit->setText(m_data.name);
    m_nameEdit->setPlaceholderText(tr("Track name"));
    connect(m_nameEdit, &QLineEdit::editingFinished, this, [this]()
    {
        const QString newName = m_nameEdit->text().trimmed();
        if (newName.isEmpty())
        {
            QMessageBox::warning(this, tr("Invalid Name"),
                                 tr("Track name cannot be empty."));
            QSignalBlocker blocker(m_nameEdit);
            m_nameEdit->setText(m_lastValidName);
            return;
        }
        if (newName != m_lastValidName)
        {
            const QString oldName = m_lastValidName;
            m_lastValidName = newName;
            m_data.name = newName;
            emit nameChanged(this, oldName, newName);
        }
    });
    topRow->addWidget(m_nameEdit, 1);

    m_removeBtn = new QPushButton(QStringLiteral("\u2715"));  // ✕
    m_removeBtn->setFixedSize(22, 22);
    m_removeBtn->setToolTip(tr("Remove track"));
    m_removeBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #cc2222;"
        "  color: white;"
        "  border-radius: 11px;"
        "  font-weight: bold;"
        "  font-size: 11px;"
        "  padding: 0px;"
        "}"
        "QPushButton:hover { background-color: #ee4444; }"
        "QPushButton:pressed { background-color: #aa1111; }");
    connect(m_removeBtn, &QPushButton::clicked, this,
            [this]()
            {
                emit removeRequested(this);
            });
    topRow->addWidget(m_removeBtn);

    outerLayout->addLayout(topRow);

    // Bottom row: Enable/Arm checkboxes + visualization
    auto* bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(12);

    auto* leftPanel = new QWidget();
    auto* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(4);

    m_enabledCheck = new QCheckBox(tr("Enable"));
    m_enabledCheck->setChecked(m_data.enabled);
    m_enabledCheck->setToolTip(tr("Include this track in playback and mix/export."));
    connect(m_enabledCheck, &QCheckBox::toggled, this, &TrackWidget::onEnabledToggled);

    m_armCheck = new QRadioButton(tr("Arm"));
    m_armCheck->setChecked(m_data.armed);
    m_armCheck->setToolTip(tr("Select this track as the target for recording."));
    connect(m_armCheck, &QRadioButton::toggled, this, &TrackWidget::onArmToggled);

    leftLayout->addWidget(m_enabledCheck);
    leftLayout->addWidget(m_armCheck);
    leftLayout->addStretch();
    leftPanel->setFixedWidth(100);
    bottomRow->addWidget(leftPanel);

    m_stackedWidget = new QStackedWidget(this);
    m_waveformWidget = new WaveformWidget(this);
    m_midiWidget = new MidiVisualizationWidget(this);
    m_stackedWidget->addWidget(m_waveformWidget);
    m_stackedWidget->addWidget(m_midiWidget);
    m_stackedWidget->setCurrentWidget(m_data.type == TrackType::Audio
                                      ? static_cast<QWidget*>(m_waveformWidget)
                                      : static_cast<QWidget*>(m_midiWidget));
    bottomRow->addWidget(m_stackedWidget, 1);
    outerLayout->addLayout(bottomRow);

    auto* mixRow = new QHBoxLayout();
    mixRow->setSpacing(6);
    m_gainValueLabel = new QLabel(this);
    m_gainValueLabel->setMinimumWidth(52);
    m_gainSlider = new QSlider(Qt::Horizontal, this);
    m_gainSlider->setRange(-600, 120);
    m_gainSlider->setToolTip(tr("Track gain (dB x10, e.g. 0 = 0 dB, -60 = -6.0 dB)"));
    m_gainSlider->setMaximumWidth(120);
    m_panValueLabel = new QLabel(this);
    m_panValueLabel->setMinimumWidth(40);
    m_panSlider = new QSlider(Qt::Horizontal, this);
    m_panSlider->setRange(-100, 100);
    m_panSlider->setToolTip(tr("Stereo pan (left -100 … right +100)"));
    m_panSlider->setMaximumWidth(100);
    m_auxValueLabel = new QLabel(this);
    m_auxValueLabel->setMinimumWidth(36);
    m_auxSlider = new QSlider(Qt::Horizontal, this);
    m_auxSlider->setRange(0, 100);
    m_auxSlider->setToolTip(tr("Aux send to the project aux effect bus (0–100%)"));
    m_auxSlider->setMaximumWidth(90);
    m_muteCheck = new QCheckBox(tr("Mute"), this);
    m_muteCheck->setToolTip(tr("Silence this track in the mix."));
    m_soloCheck = new QCheckBox(tr("Solo"), this);
    m_soloCheck->setToolTip(
        tr("When any track is soloed, only soloed tracks are heard (unless muted)."));
    connect(m_gainSlider, &QSlider::valueChanged, this, &TrackWidget::onGainPanAuxChanged);
    connect(m_panSlider, &QSlider::valueChanged, this, &TrackWidget::onGainPanAuxChanged);
    connect(m_auxSlider, &QSlider::valueChanged, this, &TrackWidget::onGainPanAuxChanged);
    connect(m_gainSlider, &QSlider::sliderPressed, this, &TrackWidget::onMixerSliderPressed);
    connect(m_gainSlider, &QSlider::sliderReleased, this, &TrackWidget::onMixerSliderReleased);
    connect(m_panSlider, &QSlider::sliderPressed, this, &TrackWidget::onMixerSliderPressed);
    connect(m_panSlider, &QSlider::sliderReleased, this, &TrackWidget::onMixerSliderReleased);
    connect(m_auxSlider, &QSlider::sliderPressed, this, &TrackWidget::onMixerSliderPressed);
    connect(m_auxSlider, &QSlider::sliderReleased, this, &TrackWidget::onMixerSliderReleased);
    connect(m_muteCheck, &QCheckBox::toggled, this, &TrackWidget::onMuteToggled);
    connect(m_soloCheck, &QCheckBox::toggled, this, &TrackWidget::onSoloToggled);
    mixRow->addWidget(new QLabel(tr("Gain"), this));
    mixRow->addWidget(m_gainValueLabel);
    mixRow->addWidget(m_gainSlider);
    mixRow->addWidget(new QLabel(tr("Pan"), this));
    mixRow->addWidget(m_panValueLabel);
    mixRow->addWidget(m_panSlider);
    mixRow->addWidget(new QLabel(tr("Aux"), this));
    mixRow->addWidget(m_auxValueLabel);
    mixRow->addWidget(m_auxSlider);
    mixRow->addWidget(m_muteCheck);
    mixRow->addWidget(m_soloCheck);
    mixRow->addStretch();
    outerLayout->addLayout(mixRow);

    updateVisualization();
    updateTypeIcon();
    updateEffectsButtonVisibility();
    syncMixerControlsFromData();
}

void TrackWidget::syncMixerControlsFromData()
{
    if (m_gainSlider)
    {
        QSignalBlocker b1(m_gainSlider);
        QSignalBlocker b2(m_panSlider);
        QSignalBlocker b3(m_auxSlider);
        QSignalBlocker b4(m_muteCheck);
        QSignalBlocker b5(m_soloCheck);
        const int g = static_cast<int>(std::lround(std::clamp(m_data.gainDb, -60.f, 12.f) * 10.f));
        m_gainSlider->setValue(std::clamp(g, m_gainSlider->minimum(), m_gainSlider->maximum()));
        const int p = static_cast<int>(std::lround(std::clamp(m_data.pan, -1.f, 1.f) * 100.f));
        m_panSlider->setValue(std::clamp(p, m_panSlider->minimum(), m_panSlider->maximum()));
        const int a = static_cast<int>(std::lround(std::clamp(m_data.auxSend, 0.f, 1.f) * 100.f));
        m_auxSlider->setValue(std::clamp(a, m_auxSlider->minimum(), m_auxSlider->maximum()));
        m_muteCheck->setChecked(m_data.mute);
        m_soloCheck->setChecked(m_data.solo);
    }
    updateMixerValueLabels();
}

void TrackWidget::updateMixerValueLabels()
{
    if (m_gainValueLabel && m_gainSlider)
        m_gainValueLabel->setText(QStringLiteral("%1 dB")
 .arg(QString::number(m_gainSlider->value() / 10.0, 'f', 1)));
    if (m_panValueLabel && m_panSlider)
        m_panValueLabel->setText(QString::number(m_panSlider->value()));
    if (m_auxValueLabel && m_auxSlider)
        m_auxValueLabel->setText(QStringLiteral("%1%").arg(m_auxSlider->value()));
}

void TrackWidget::onGainPanAuxChanged()
{
    if (m_gainSlider)
        m_data.gainDb = m_gainSlider->value() / 10.f;
    if (m_panSlider)
        m_data.pan = m_panSlider->value() / 100.f;
    if (m_auxSlider)
        m_data.auxSend = m_auxSlider->value() / 100.f;
    updateMixerValueLabels();
    emit dataChanged(this);
}

void TrackWidget::onMixerSliderPressed()
{
    emit mixerInteractionStarted(this);
}

void TrackWidget::onMixerSliderReleased()
{
    emit mixerInteractionEnded(this);
}

void TrackWidget::onMuteToggled(bool checked)
{
    emit mixerInteractionStarted(this);
    m_data.mute = checked;
    emit dataChanged(this);
    emit mixerInteractionEnded(this);
}

void TrackWidget::onSoloToggled(bool checked)
{
    emit mixerInteractionStarted(this);
    m_data.solo = checked;
    emit dataChanged(this);
    emit mixerInteractionEnded(this);
}

void TrackWidget::setAudioEffectChain(const QJsonArray& chain)
{
    m_data.audioEffectChain = chain;
    emit dataChanged(this);
}

void TrackWidget::onEffectsClicked()
{
    if (m_data.type == TrackType::Audio)
        emit effectsRequested(this);
}

void TrackWidget::updateEffectsButtonVisibility()
{
    const bool audio = (m_data.type == TrackType::Audio);
    if (m_effectsButton)
    {
        m_effectsButton->setVisible(audio);
        m_effectsButton->setEnabled(audio);
    }
}

bool TrackWidget::isArmed() const
{
    return m_armCheck ? m_armCheck->isChecked() : false;
}

void TrackWidget::setArmed(bool armed)
{
    m_data.armed = armed;
    if (m_armCheck)
    {
        QSignalBlocker blocker(m_armCheck);
        m_armCheck->setChecked(armed);
    }
}

void TrackWidget::setArmEnabled(bool enabled)
{
    if (m_armCheck)
    {
        if (!enabled && m_armCheck->isChecked())
        {
            QSignalBlocker blocker(m_armCheck);
            m_armCheck->setChecked(false);
            m_data.armed = false;
            // Notify the rest of the app that arm was cleared
            emit armChanged(this, false);
        }
        m_armCheck->setEnabled(enabled);
    }
}

void TrackWidget::setInteractiveControlsEnabled(bool enabled)
{
    if (m_enabledCheck)   m_enabledCheck->setEnabled(enabled);
    if (m_armCheck)       m_armCheck->setEnabled(enabled);
    if (m_nameEdit)       m_nameEdit->setEnabled(enabled);
    if (m_removeBtn)      m_removeBtn->setEnabled(enabled);
    if (m_typeIconLabel)  m_typeIconLabel->setEnabled(enabled);
    if (m_effectsButton)  m_effectsButton->setEnabled(enabled && m_data.type == TrackType::Audio);
    if (m_gainSlider)     m_gainSlider->setEnabled(enabled);
    if (m_panSlider) m_panSlider->setEnabled(enabled);
    if (m_auxSlider)      m_auxSlider->setEnabled(enabled);
    if (m_muteCheck)      m_muteCheck->setEnabled(enabled);
    if (m_soloCheck)      m_soloCheck->setEnabled(enabled);
}

void TrackWidget::setTrackData(const TrackData& data)
{
    m_data = data;
    m_lastValidName = data.name;
    if (m_nameEdit)
    {
        QSignalBlocker blocker(m_nameEdit);
        m_nameEdit->setText(m_data.name);
    }
    m_enabledCheck->setChecked(m_data.enabled);
    if (m_stackedWidget)
    {
        m_stackedWidget->setCurrentWidget(m_data.type == TrackType::Audio
                                          ? static_cast<QWidget*>(m_waveformWidget)
                                          : static_cast<QWidget*>(m_midiWidget));
    }
    updateVisualization();
    updateTypeIcon();
    updateEffectsButtonVisibility();
    syncMixerControlsFromData();
}

void TrackWidget::setRecordingLevel(float level)
{
    if (m_waveformWidget) m_waveformWidget->setRecordingLevel(level);
}

void TrackWidget::clearRecordingLevel()
{
    if (m_waveformWidget) m_waveformWidget->clearRecordingLevel();
}

void TrackWidget::setRecordingStatus(const QString& status)
{
    // Write the status to whichever visualization widget is currently shown.
    // Audio tracks display the waveform; MIDI tracks display the piano roll.
    // Without this branch, the countdown text was being set on the hidden
    // waveform widget for MIDI tracks and never appeared on screen.
    const QColor color(220, 160, 30);
    if (m_data.type == TrackType::MIDI)
    {
        if (m_midiWidget) m_midiWidget->setStatusText(status, color);
    }
    else
    {
        if (m_waveformWidget) m_waveformWidget->setStatusText(status, color);
    }
}

void TrackWidget::clearRecordingStatus()
{
    if (m_waveformWidget) m_waveformWidget->clearStatusText();
    if (m_midiWidget)     m_midiWidget->clearStatusText();
}

void TrackWidget::retranslateUi()
{
    if (m_configButton) m_configButton->setText(tr("Options"));
    if (m_effectsButton) m_effectsButton->setText(tr("Effects"));
    if (m_removeBtn) m_removeBtn->setToolTip(tr("Remove track"));
    if (m_enabledCheck) m_enabledCheck->setText(tr("Enable"));
    if (m_armCheck) m_armCheck->setText(tr("Arm"));
    updateTypeIcon();
}

void TrackWidget::updateTypeIcon()
{
    if (!m_typeIconLabel) return;
    if (m_data.type == TrackType::MIDI)
    {
        m_typeIconLabel->setText(QStringLiteral("\U0001F3B9"));  // 🎹 piano keyboard
        m_typeIconLabel->setToolTip(tr("MIDI track — click to change input type"));
    }
    else
    {
        m_typeIconLabel->setText(QStringLiteral("\U0001F50A"));  // 🔊 speaker
        m_typeIconLabel->setToolTip(tr("Audio track — click to change input type"));
    }
}

void TrackWidget::updateVisualization()
{
    if (m_data.type == TrackType::Audio)
    {
        m_waveformWidget->setAudioData(m_data.audioData, m_data.sampleRate, m_data.channelCount);
        m_midiWidget->clear();
    }
    else
    {
        m_waveformWidget->clear();
        m_midiWidget->setMidiNotes(m_data.midiNotes, m_data.lengthSeconds);
    }
}

void TrackWidget::onConfigClicked()
{
    emit configurationRequested(this);
}

void TrackWidget::onEnabledToggled(bool checked)
{
    m_data.enabled = checked;
    if (!checked && m_armCheck && m_armCheck->isChecked())
    {
        QSignalBlocker blocker(m_armCheck);
        m_armCheck->setChecked(false);
        m_data.armed = false;
        emit armChanged(this, false);
    }
    emit dataChanged(this);
}

void TrackWidget::onArmToggled(bool checked)
{
    m_data.armed = checked;
    emit armChanged(this, checked);
}
