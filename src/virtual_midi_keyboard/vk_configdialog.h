#ifndef VK_CONFIGDIALOG_H
#define VK_CONFIGDIALOG_H

#include <QDialog>

class QTabWidget;
class QComboBox;
class QLineEdit;
class QPushButton;
class QSlider;
class QLabel;

class VkConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VkConfigDialog(QWidget* parent = nullptr);

private slots:
    void onBrowseSoundFont();
    void onApply();
    void onOutputDeviceChanged(int index);
    void refreshDevices();

private:
    void setupMidiTab();
    void setupAudioTab();
    void setupLanguageTab();
    void loadSettings();
    void saveSettings();

    QTabWidget* m_tabWidget = nullptr;
    QComboBox* m_languageCombo = nullptr;
    QComboBox* m_midiOutputCombo = nullptr;
    QComboBox* m_midiInputCombo = nullptr;
    QLineEdit* m_soundFontEdit = nullptr;
    QPushButton* m_loadSoundFontBtn = nullptr;
    QComboBox* m_audioOutputCombo = nullptr;
    QSlider* m_gainSlider = nullptr;
    QLabel* m_gainLabel = nullptr;
};

#endif // VK_CONFIGDIALOG_H
