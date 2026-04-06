#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QString>
#include <QByteArray>

class AppSettings
{
public:
    static AppSettings& instance();

    // MIDI settings
    int midiDeviceIndex() const
    {
        return m_midiDeviceIndex;
    }
    void setMidiDeviceIndex(int index)
    {
        m_midiDeviceIndex = index;
    }
    QString soundFontPath() const
    {
        return m_soundFontPath;
    }
    void setSoundFontPath(const QString& path)
    {
        m_soundFontPath = path;
    }

    // General settings
    QString theme() const
    {
        return m_theme;
    }
    void setTheme(const QString& theme)
    {
        m_theme = theme;
    }

    // Audio settings
    int audioInputDeviceIndex() const
    {
        return m_audioInputDeviceIndex;
    }
    void setAudioInputDeviceIndex(int index)
    {
        m_audioInputDeviceIndex = index;
    }
    QByteArray audioInputDeviceId() const
    {
        return m_audioInputDeviceId;
    }
    void setAudioInputDeviceId(const QByteArray& id)
    {
        m_audioInputDeviceId = id;
    }
    QByteArray audioOutputDeviceId() const
    {
        return m_audioOutputDeviceId;
    }
    void setAudioOutputDeviceId(const QByteArray& id)
    {
        m_audioOutputDeviceId = id;
    }

    // Debug log setting
    bool recordingDebugLog() const
    {
        return m_recordingDebugLog;
    }
    void setRecordingDebugLog(bool enabled)
    {
        m_recordingDebugLog = enabled;
    }

    // Language setting (empty = system default)
    QString language() const
    {
        return m_language;
    }
    void setLanguage(const QString& lang)
    {
        m_language = lang;
    }

    // Metronome settings
    bool metronomeEnabled() const { return m_metronomeEnabled; }
    void setMetronomeEnabled(bool enabled) { m_metronomeEnabled = enabled; }
    int metronomeBpm() const { return m_metronomeBpm; }
    void setMetronomeBpm(int bpm) { m_metronomeBpm = bpm; }

    // Display settings
    QString ledColor() const { return m_ledColor; }
    void setLedColor(const QString& color) { m_ledColor = color; }

    // Project settings
    QString projectLocation() const
    {
        return m_projectLocation;
    }
    void setProjectLocation(const QString& path)
    {
        m_projectLocation = path;
    }

    void load();
    void save();

private:
    AppSettings();
    AppSettings(const AppSettings&) = delete;
    AppSettings& operator=(const AppSettings&) = delete;

    bool m_recordingDebugLog = false;
    bool m_metronomeEnabled = false;
    int m_metronomeBpm = 120;
    QString m_ledColor = "light_green";
    QString m_language;  // empty = system default
    QString m_theme = "dark";
    int m_midiDeviceIndex = 0;
    QString m_soundFontPath;
    int m_audioInputDeviceIndex = 0;
    QByteArray m_audioInputDeviceId;
    QByteArray m_audioOutputDeviceId;
    QString m_projectLocation;
};

#endif // APPSETTINGS_H
