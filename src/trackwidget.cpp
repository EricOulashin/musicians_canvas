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
    setFixedHeight(120);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto* outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(8, 6, 8, 6);
    outerLayout->setSpacing(4);

    // Top row: Options button + track name edit + remove button
    auto* topRow = new QHBoxLayout();
    topRow->setSpacing(8);

    m_configButton = new QPushButton(tr("Options"));
    m_configButton->setFixedWidth(80);
    connect(m_configButton, &QPushButton::clicked, this,
            [this]()
            {
                emit configurationRequested(this);
            });
    topRow->addWidget(m_configButton);

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

    updateVisualization();
    updateTypeIcon();
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
