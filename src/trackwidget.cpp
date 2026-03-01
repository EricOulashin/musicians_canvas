#include "trackwidget.h"
#include "waveformwidget.h"
#include "midivisualizationwidget.h"
#include "trackconfigdialog.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QStackedWidget>

TrackWidget::TrackWidget(const TrackData& data, QWidget* parent) : QFrame(parent), m_data(data) {
    setupUi();
}

void TrackWidget::setupUi() {
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setLineWidth(1);
    setObjectName("trackFrame");
    setMinimumHeight(80);

    auto* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(8, 6, 8, 6);
    mainLayout->setSpacing(12);

    auto* leftPanel = new QWidget();
    auto* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(6);

    m_configButton = new QPushButton(tr("Options"));
    m_configButton->setFixedWidth(80);
    connect(m_configButton, &QPushButton::clicked, this, [this]() { emit configurationRequested(this); });

    m_enabledCheck = new QCheckBox(tr("Enable"));
    m_enabledCheck->setChecked(m_data.enabled);
    connect(m_enabledCheck, &QCheckBox::toggled, this, &TrackWidget::onEnabledToggled);

    leftLayout->addWidget(m_configButton);
    leftLayout->addWidget(m_enabledCheck);
    leftLayout->addStretch();
    leftPanel->setFixedWidth(100);
    mainLayout->addWidget(leftPanel);

    m_stackedWidget = new QStackedWidget(this);
    m_waveformWidget = new WaveformWidget(this);
    m_midiWidget = new MidiVisualizationWidget(this);
    m_stackedWidget->addWidget(m_waveformWidget);
    m_stackedWidget->addWidget(m_midiWidget);
    m_stackedWidget->setCurrentWidget(m_data.type == TrackType::Audio ? static_cast<QWidget*>(m_waveformWidget) : static_cast<QWidget*>(m_midiWidget));
    mainLayout->addWidget(m_stackedWidget, 1);
    updateVisualization();
}

void TrackWidget::setTrackData(const TrackData& data) {
    m_data = data;
    m_enabledCheck->setChecked(m_data.enabled);
    if (m_stackedWidget) {
        m_stackedWidget->setCurrentWidget(m_data.type == TrackType::Audio ? static_cast<QWidget*>(m_waveformWidget) : static_cast<QWidget*>(m_midiWidget));
    }
    updateVisualization();
}

void TrackWidget::updateVisualization() {
    if (m_data.type == TrackType::Audio) {
        m_waveformWidget->setAudioData(m_data.audioData, m_data.sampleRate, m_data.channelCount);
        m_midiWidget->clear();
    } else {
        m_waveformWidget->clear();
        m_midiWidget->setMidiNotes(m_data.midiNotes, m_data.lengthSeconds);
    }
}

void TrackWidget::onConfigClicked() {
    emit configurationRequested(this);
}

void TrackWidget::onEnabledToggled(bool checked) {
    m_data.enabled = checked;
}
