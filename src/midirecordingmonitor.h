#ifndef MIDIRECORDINGMONITOR_H
#define MIDIRECORDINGMONITOR_H

#include <QByteArray>
#include <QObject>
#include <QMutex>
#include <QString>
#include <vector>

#include <fluidsynth.h>

#ifdef QT_MULTIMEDIA_AVAILABLE
#include <QAudioDevice>
#endif

class QTimer;

#ifdef QT_MULTIMEDIA_AVAILABLE
class QAudioSink;
class QIODevice;
#endif

/// Software MIDI monitor: plays incoming MIDI during record via FluidSynth + QAudioSink.
class MidiRecordingMonitor : public QObject
{
    Q_OBJECT
public:
    explicit MidiRecordingMonitor(QObject* parent = nullptr);
    ~MidiRecordingMonitor() override;

#ifdef QT_MULTIMEDIA_AVAILABLE
    bool start(const QString& soundFontPath, int sampleRate, const QAudioDevice& outputDevice,
               double gainMultiplier);
#endif
    void stop();

    void postMidiBytes(const std::vector<unsigned char>& msg);

private slots:
    void tick();

private:
    void processMidiBytes(const unsigned char* data, int len);
    void teardownSynth();

    QMutex              m_mutex;
    std::vector<QByteArray> m_messageQueue;

    fluid_settings_t*   m_settings = nullptr;
    fluid_synth_t*      m_synth = nullptr;

    QTimer*             m_timer = nullptr;
#ifdef QT_MULTIMEDIA_AVAILABLE
    QAudioSink*         m_sink = nullptr;
    QIODevice*          m_sinkDevice = nullptr;
#endif
    int                 m_sampleRate = 44100;
};

#endif
