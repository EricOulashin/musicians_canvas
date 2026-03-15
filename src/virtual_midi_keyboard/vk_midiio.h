#ifndef VK_MIDIIO_H
#define VK_MIDIIO_H

#include <memory>
#include <QObject>
#include <QList>
#include <QString>

class VkMidiIo : public QObject {
    Q_OBJECT
public:
    struct SoundPreset {
        int bank;
        int program;
        QString name;
    };

    explicit VkMidiIo(QObject* parent = nullptr);
    ~VkMidiIo();

    void setOutputDevice(int index);
    void setInputDevice(int index);
    void setSoundFont(const QString& path);
    void setAudioOutputDevice(const QByteArray& deviceId);

    void sendNoteOn(int note, int velocity);
    void sendNoteOff(int note);
    void sendProgramChange(int program);
    void sendControlChange(int controller, int value);
    void selectPreset(int bank, int program);

    [[nodiscard]] bool isOutputOpen() const { return m_outputOpen; }
    [[nodiscard]] bool isUsingFluidSynth() const;
    [[nodiscard]] bool isAudioDriverRunning() const;
    [[nodiscard]] bool hasSoundFont() const;
    [[nodiscard]] QList<SoundPreset> getPresets() const;

public slots:
    void forwardMidiNoteOn(int note, int velocity);
    void forwardMidiNoteOff(int note);

signals:
    void midiNoteOn(int note, int velocity);
    void midiNoteOff(int note);
    void soundListChanged();

private:
    class Impl;
    std::shared_ptr<Impl> m_impl;
    bool m_outputOpen = false;
};

#endif // VK_MIDIIO_H
