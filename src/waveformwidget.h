#ifndef WAVEFORMWIDGET_H
#define WAVEFORMWIDGET_H

#include <QWidget>
#include <QByteArray>

class WaveformWidget : public QWidget {
    Q_OBJECT
public:
    explicit WaveformWidget(QWidget* parent = nullptr);
    void setAudioData(const QByteArray& data, int sampleRate = 44100, int channelCount = 2);
    void clear();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QByteArray m_audioData;
    int m_sampleRate = 44100;
    int m_channelCount = 2;
    QVector<float> m_peaks;  // Pre-computed peak values for display
    void computePeaks();
};

#endif // WAVEFORMWIDGET_H
