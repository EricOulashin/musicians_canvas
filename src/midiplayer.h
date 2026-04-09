#ifndef MIDIPLAYER_H
#define MIDIPLAYER_H

#include <QObject>
#include <QVector>
#include <QElapsedTimer>
#include <memory>
#include <vector>
#include "trackdata.h"

class QTimer;

/// Sends MIDI note events in real time to the configured MIDI output.
/// Intended for "play project" mode (not mixdown/export).
class MidiPlayer : public QObject
{
    Q_OBJECT
public:
    explicit MidiPlayer(QObject* parent = nullptr);
    ~MidiPlayer() override;

    /// midiDeviceIndex uses the same scheme as settings/project settings:
    ///   0 = built-in synth (NOT supported here for realtime MIDI out),
    ///   n>0 = RtMidiOut port (n-1).
    [[nodiscard]] bool start(int midiDeviceIndex,
                             const QVector<TrackData>& tracks,
                             int volumePercent = 100);
    void stop();
    [[nodiscard]] bool isRunning() const;

signals:
    void finished();

private slots:
    void onTick();
    void onFinishTimeout();

private:
    struct Event
    {
        double timeSec = 0.0;
        std::vector<unsigned char> msg;
    };

    void sendAllNotesOff();
    void sendInitialVolume(int volumePercent);

    std::shared_ptr<class RtMidiOut> m_midiOut;
    QVector<Event> m_events;
    int m_eventIndex = 0;
    bool m_running = false;
    QElapsedTimer m_clock;
    QTimer* m_timer = nullptr;
    QTimer* m_finishTimer = nullptr;
};

#endif // MIDIPLAYER_H

