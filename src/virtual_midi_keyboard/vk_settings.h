#ifndef VK_SETTINGS_H
#define VK_SETTINGS_H

#include <QString>
#include <QByteArray>

class VkSettings {
public:
    static VkSettings& instance();

    int midiOutputIndex() const { return m_midiOutputIndex; }
    void setMidiOutputIndex(int idx) { m_midiOutputIndex = idx; }

    int midiInputIndex() const { return m_midiInputIndex; }
    void setMidiInputIndex(int idx) { m_midiInputIndex = idx; }

    QString soundFontPath() const { return m_soundFontPath; }
    void setSoundFontPath(const QString& path) { m_soundFontPath = path; }

    int audioOutputIndex() const { return m_audioOutputIndex; }
    void setAudioOutputIndex(int idx) { m_audioOutputIndex = idx; }
    QByteArray audioOutputDeviceId() const { return m_audioOutputDeviceId; }
    void setAudioOutputDeviceId(const QByteArray& id) { m_audioOutputDeviceId = id; }

    void load();
    void save();

private:
    VkSettings();
    VkSettings(const VkSettings&) = delete;
    VkSettings& operator=(const VkSettings&) = delete;

    static QString configFilePath();

    int m_midiOutputIndex = 0;
    int m_midiInputIndex = -1;
    QString m_soundFontPath;
    int m_audioOutputIndex = 0;
    QByteArray m_audioOutputDeviceId;
};

#endif // VK_SETTINGS_H
