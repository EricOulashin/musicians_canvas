#ifndef VK_SETTINGS_H
#define VK_SETTINGS_H

#include <QString>
#include <QByteArray>

class VkSettings
{
public:
    static VkSettings& instance();

    // MIDI output: empty string = built-in software synthesizer,
    //              non-empty    = name of the external RtMidiOut port to open.
    QString midiOutputPortName() const  { return m_midiOutputPortName; }
    void setMidiOutputPortName(const QString& name) { m_midiOutputPortName = name; }

    // MIDI input: empty string = none (no external controller),
    //             non-empty    = name of the RtMidiIn port to open.
    QString midiInputPortName() const   { return m_midiInputPortName; }
    void setMidiInputPortName(const QString& name)  { m_midiInputPortName = name; }

    QString soundFontPath() const       { return m_soundFontPath; }
    void setSoundFontPath(const QString& path)      { m_soundFontPath = path; }

    QByteArray audioOutputDeviceId() const          { return m_audioOutputDeviceId; }
    void setAudioOutputDeviceId(const QByteArray& id) { m_audioOutputDeviceId = id; }

    // Master gain for the FluidSynth synthesiser (0.1 – 2.0, default 0.8).
    // FluidSynth's built-in default of 0.2 is intentionally conservative;
    // 0.8 gives a much more usable output level without risking clipping.
    float synthGain() const              { return m_synthGain; }
    void setSynthGain(float gain)        { m_synthGain = gain; }

    void load();
    void save();

private:
    VkSettings();
    VkSettings(const VkSettings&) = delete;
    VkSettings& operator=(const VkSettings&) = delete;

    static QString configFilePath();

    QString    m_midiOutputPortName;   // "" = built-in synth
    QString    m_midiInputPortName;    // "" = none
    QString    m_soundFontPath;
    QByteArray m_audioOutputDeviceId;
    float      m_synthGain = 0.8f;
};

#endif // VK_SETTINGS_H
