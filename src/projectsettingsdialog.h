#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include <QDialog>
#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QAudioDevice>
#endif
#include "projectsettings.h"

class QTabWidget;
class QComboBox;
class QLineEdit;
class QRadioButton;
class QSlider;
class QLabel;
class QCheckBox;
class EffectChainEditorWidget;

class ProjectSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProjectSettingsDialog(const ProjectSettings& settings,
                                   QWidget* parent = nullptr);

    // Returns the settings as edited by the user (valid only if accepted).
    ProjectSettings projectSettings() const;

private slots:
    void onBrowseSoundFont();
    void refreshDevices();
    void updateAudioFormatOptions();
    void refreshSampleRateList();

private:
    void setupMidiTab();
    void setupAudioTab();
    void setupMixEffectsTab();
    void loadSettings(const ProjectSettings& settings);
#ifdef QT_MULTIMEDIA_AVAILABLE
    QAudioDevice selectedInputDevice() const;
    void probeDeviceRates();
#endif
#if defined(HAVE_PORTAUDIO)
    void refreshPortAudioInputDevices();
    void onRecordingBackendChanged();
#endif

    QTabWidget*   m_tabWidget          = nullptr;
    QComboBox*    m_midiDeviceCombo    = nullptr;
    QComboBox*    m_midiInputCombo     = nullptr;
    QLineEdit*    m_soundFontEdit      = nullptr;
    QCheckBox*    m_renderMidiToAudioForPlayback = nullptr;
    QCheckBox*    m_midiVolumeUseAppDefault = nullptr;
    QSlider*      m_midiVolumeSlider   = nullptr;
    QLabel*       m_midiVolumeValue    = nullptr;
    QComboBox*    m_audioInputCombo    = nullptr;
    QComboBox*    m_audioOutputCombo   = nullptr;
    QComboBox*    m_sampleRateCombo    = nullptr;
    QRadioButton* m_monoRadio          = nullptr;
    QRadioButton* m_stereoRadio        = nullptr;
#if defined(HAVE_PORTAUDIO)
    QRadioButton* m_recordingUsePortAudioRadio = nullptr;
    QRadioButton* m_recordingUseQtRadio        = nullptr;
    QComboBox*    m_portAudioDeviceCombo        = nullptr;
#endif

    // Probed actual max sample rates for the selected input device.
    // These may differ from the device's reported rates when the audio
    // backend delivers data at a lower rate than advertised.
    int m_probedMonoMaxRate   = 0;
    int m_probedStereoMaxRate = 0;

    EffectChainEditorWidget* m_mixEffectsEditor = nullptr;
};

#endif // PROJECTSETTINGSDIALOG_H
