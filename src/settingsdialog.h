#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QCheckBox;
class QTabWidget;
class QComboBox;
class QLineEdit;
class QPushButton;
class QSlider;
class QLabel;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);

private slots:
    void onBrowseSoundFont();
    void onApply();
    void refreshDevices();

private:
    void setupGeneralTab();
    void setupDisplayTab();
    void setupMidiTab();
    void setupAudioTab();
    void setupLanguageTab();
    void loadSettings();
    void saveSettings();

    QTabWidget* m_tabWidget = nullptr;
    QComboBox* m_themeCombo = nullptr;
    QCheckBox* m_debugLogCheck = nullptr;
    QComboBox* m_languageCombo = nullptr;
    QComboBox* m_midiDeviceCombo = nullptr;
    QLineEdit* m_soundFontEdit = nullptr;
    QSlider* m_midiVolumeSlider = nullptr;
    QLabel*  m_midiVolumeValue  = nullptr;
    QComboBox* m_ledColorCombo = nullptr;
    QComboBox* m_audioInputCombo = nullptr;
    QComboBox* m_audioOutputCombo = nullptr;
};

#endif // SETTINGSDIALOG_H
