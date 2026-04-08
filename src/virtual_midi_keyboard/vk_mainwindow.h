#ifndef VK_MAINWINDOW_H
#define VK_MAINWINDOW_H

#include <QMainWindow>

class QCheckBox;
class QEvent;
class QKeyEvent;
class QLabel;
class QPushButton;
class QVBoxLayout;
class VkMidiIo;
class VkPianoKeyboard;
class QSpinBox;
class QLineEdit;
class QComboBox;

class VkMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit VkMainWindow(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void changeEvent(QEvent* event) override;

private slots:
    void onClose();
    void onConfiguration();
    void onHelp();
    void onAbout();
    void onOctaveChanged(int value);
    void onOctaveUp();
    void onOctaveDown();
    void onChorusChanged(int value);
    void onChorusApply();
    void onMidiChannelChanged(int channel);  // 1-based UI value
    void onDrumsToggled(bool checked);
    void onNoteOn(int note);
    void onNoteOff(int note);
    void onMidiNoteOn(int note, int velocity);
    void onMidiNoteOff(int note);
    void onSoundListChanged();
    void onSoundApply();
    void onVolumeChanged(int value);

private:
    void setupMenuBar();
    void retranslateUi();
    void setupToolbar(QVBoxLayout* mainLayout);
    void setupPiano(QVBoxLayout* mainLayout);
    void applyMidiOutput();

    VkMidiIo* m_midiIo = nullptr;
    VkPianoKeyboard* m_piano = nullptr;
    QSpinBox* m_volumeSpin = nullptr;
    QSpinBox* m_octaveSpin = nullptr;
    QLineEdit* m_chorusEdit = nullptr;
    QWidget*   m_soundGroup = nullptr;
    QComboBox* m_soundCombo = nullptr;
    // Labels and buttons that need retranslation
    QLabel* m_volumeLabel = nullptr;
    QLabel* m_octaveLabel = nullptr;
    QLabel* m_chorusLabel = nullptr;
    QLabel* m_channelLabel = nullptr;
    QLabel* m_soundLabel = nullptr;
    QPushButton* m_chorusApplyBtn = nullptr;
    QSpinBox* m_channelSpin = nullptr;
    QCheckBox* m_drumsCheck = nullptr;
    QLabel* m_drumsLabel = nullptr;  // not used directly; checkbox carries the label
    int m_chorusValue = 64;
    int m_octave = 0;
    int m_channelBeforeDrums = 1;  // saved 1-based channel to restore when Drums is unticked
};

#endif // VK_MAINWINDOW_H
