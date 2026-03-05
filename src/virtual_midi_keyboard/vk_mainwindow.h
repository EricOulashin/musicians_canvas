#ifndef VK_MAINWINDOW_H
#define VK_MAINWINDOW_H

#include <QMainWindow>

class QKeyEvent;
class QVBoxLayout;
class VkMidiIo;
class VkPianoKeyboard;
class QSpinBox;
class QLineEdit;
class QComboBox;

class VkMainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit VkMainWindow(QWidget* parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    void onClose();
    void onConfiguration();
    void onHelp();
    void onOctaveChanged(int value);
    void onOctaveUp();
    void onOctaveDown();
    void onChorusChanged(int value);
    void onChorusApply();
    void onNoteOn(int note);
    void onNoteOff(int note);
    void onMidiNoteOn(int note, int velocity);
    void onMidiNoteOff(int note);
    void onSoundListChanged();
    void onSoundApply();
    void onVolumeChanged(int value);

private:
    void setupMenuBar();
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
    int m_chorusValue = 64;
    int m_octave = 0;
};

#endif // VK_MAINWINDOW_H
