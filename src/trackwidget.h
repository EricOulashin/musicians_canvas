#ifndef TRACKWIDGET_H
#define TRACKWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QJsonArray>
#include <QString>
#include "trackdata.h"

class QPushButton;
class QCheckBox;
class QRadioButton;
class QToolButton;
class QLineEdit;
class QStackedWidget;
class WaveformWidget;
class MidiVisualizationWidget;
class TrackConfigDialog;
class QSlider;
class QLabel;

class TrackWidget : public QFrame
{
    Q_OBJECT
public:
    explicit TrackWidget(const TrackData& data, QWidget* parent = nullptr);
    TrackData trackData() const
    {
        return m_data;
    }
    QJsonArray audioEffectChain() const
    {
        return m_data.audioEffectChain;
    }
    void setAudioEffectChain(const QJsonArray& chain);
    void setTrackData(const TrackData& data);
    void updateVisualization();

    bool isArmed() const;
    void setArmed(bool armed);
    void setArmEnabled(bool enabled);
    void setInteractiveControlsEnabled(bool enabled);
    void setRecordingLevel(float level);   // 0.0–1.0, shown in waveform area while recording
    void clearRecordingLevel();            // restore normal waveform display
    void setRecordingStatus(const QString& status);  // overlay text on waveform area
    void clearRecordingStatus();                     // hide overlay text
    void retranslateUi();

signals:
    void configurationRequested(TrackWidget* widget);
    void effectsRequested(TrackWidget* widget);
    void armChanged(TrackWidget* widget, bool armed);
    void dataChanged(TrackWidget* widget);
    void nameChanged(TrackWidget* widget, const QString& oldName, const QString& newName);
    void removeRequested(TrackWidget* widget);
    /// Emitted before a mixer slider drag changes values (for undo coalescing).
    void mixerInteractionStarted(TrackWidget* widget);
    /// Emitted after a mixer slider is released.
    void mixerInteractionEnded(TrackWidget* widget);

private slots:
    void onEffectsClicked();
    void onConfigClicked();
    void onEnabledToggled(bool checked);
    void onArmToggled(bool checked);
    void onGainPanAuxChanged();
    void onMuteToggled(bool checked);
    void onSoloToggled(bool checked);
    void onMixerSliderPressed();
    void onMixerSliderReleased();

private:
    void setupUi();
    void updateTypeIcon();
    void updateEffectsButtonVisibility();
    void syncMixerControlsFromData();
    void updateMixerValueLabels();
    TrackData m_data;
    QString m_lastValidName;
    QPushButton* m_configButton = nullptr;
    QPushButton* m_effectsButton = nullptr;
    QPushButton* m_removeBtn = nullptr;
    QToolButton* m_typeIconLabel = nullptr;
    QLineEdit* m_nameEdit = nullptr;
    QCheckBox* m_enabledCheck = nullptr;
    QRadioButton* m_armCheck = nullptr;
    QStackedWidget* m_stackedWidget = nullptr;
    WaveformWidget* m_waveformWidget = nullptr;
    MidiVisualizationWidget* m_midiWidget = nullptr;

    QSlider* m_gainSlider = nullptr;
    QSlider* m_panSlider = nullptr;
    QSlider* m_auxSlider = nullptr;
    QCheckBox* m_muteCheck = nullptr;
    QCheckBox* m_soloCheck = nullptr;
    QLabel* m_gainValueLabel = nullptr;
    QLabel* m_panValueLabel = nullptr;
    QLabel* m_auxValueLabel = nullptr;
};

#endif // TRACKWIDGET_H
