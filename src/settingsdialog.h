#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QTabWidget;
class QComboBox;
class QLineEdit;
class QPushButton;

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);

private slots:
    void onBrowseSoundFont();
    void onApply();
    void refreshDevices();

private:
    void setupGeneralTab();
    void setupMidiTab();
    void setupAudioTab();
    void loadSettings();
    void saveSettings();

    QTabWidget* m_tabWidget = nullptr;
    QComboBox* m_themeCombo = nullptr;
    QComboBox* m_midiDeviceCombo = nullptr;
    QLineEdit* m_soundFontEdit = nullptr;
    QComboBox* m_audioInputCombo = nullptr;
    QComboBox* m_audioOutputCombo = nullptr;
};

#endif // SETTINGSDIALOG_H
