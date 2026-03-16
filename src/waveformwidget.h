#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QWidget>
#include <QByteArray>

class WaveformWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WaveformWidget(QWidget* parent = nullptr);
    void setAudioData(const QByteArray& data, int sampleRate = 44100, int channelCount = 2);
    void clear();

    // Live recording level (0.0–1.0).  Call with level >= 0 while recording,
    // then clearRecordingLevel() when done.
    void setRecordingLevel(float level);
    void clearRecordingLevel();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QByteArray m_audioData;
    int m_sampleRate = 44100;
    int m_channelCount = 2;
    QVector<float> m_peaks;  // Pre-computed peak values for display
    float m_recordingLevel = -1.0f;  // <0 = not recording
    void computePeaks();
};

#endif // WAVEFORMWIDGET_H
