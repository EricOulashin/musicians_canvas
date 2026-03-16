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

    QString m_theme = "dark";
    int m_midiDeviceIndex = 0;
    QString m_soundFontPath;
    int m_audioInputDeviceIndex = 0;
    QByteArray m_audioInputDeviceId;
    QByteArray m_audioOutputDeviceId;
    QString m_projectLocation;
};

#endif // APPSETTINGS_H
