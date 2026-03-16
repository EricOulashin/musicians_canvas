#ifndef VK_MIDIIO_H
#define VK_MIDIIO_H

#include <memory>
#include <QObject>
#include <QList>
#include <QString>

class VkMidiIo : public QObject
{
    Q_OBJECT
public:
    struct SoundPreset
    {
        int bank;
        int program;
        QString name;
    };

    explicit VkMidiIo(QObject* parent = nullptr);
    ~VkMidiIo();

    // portName: empty string = built-in software synthesizer (FluidSynth via Qt audio)
    //           non-empty   = open the RtMidiOut port whose name matches portName
    void setOutputDevice(const QString& portName);

    // portName: empty string = no MIDI input (external controller disabled)
    //           non-empty   = open the RtMidiIn port whose name matches portName
    void setInputDevice(const QString& portName);
    void setSoundFont(const QString& path);
    void setAudioOutputDevice(const QByteArray& deviceId);
    // Adjust the FluidSynth master gain (0.1 – 2.0). Takes effect immediately.
    void setSynthGain(float gain);

    void sendNoteOn(int note, int velocity);
    void sendNoteOff(int note);
    void sendProgramChange(int program);
    void sendControlChange(int controller, int value);
    void selectPreset(int bank, int program);

    [[nodiscard]] bool isOutputOpen() const
    {
        return m_outputOpen;
    }
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
