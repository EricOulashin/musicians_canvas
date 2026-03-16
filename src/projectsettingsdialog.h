#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include <QDialog>
#include "projectsettings.h"

class QTabWidget;
class QComboBox;
class QLineEdit;
class QRadioButton;

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

private:
    void setupMidiTab();
    void setupAudioTab();
    void loadSettings(const ProjectSettings& settings);

    QTabWidget*   m_tabWidget          = nullptr;
    QComboBox*    m_midiDeviceCombo    = nullptr;
    QLineEdit*    m_soundFontEdit      = nullptr;
    QComboBox*    m_audioInputCombo    = nullptr;
    QComboBox*    m_audioOutputCombo   = nullptr;
    QComboBox*    m_sampleRateCombo    = nullptr;
    QRadioButton* m_monoRadio          = nullptr;
    QRadioButton* m_stereoRadio        = nullptr;
};

#endif // PROJECTSETTINGSDIALOG_H
