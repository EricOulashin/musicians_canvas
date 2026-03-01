#include "waveformwidget.h"
#include <QPainter>
#include <QPainterPath>
#include <cmath>

WaveformWidget::WaveformWidget(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(60);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setAutoFillBackground(true);
}

void WaveformWidget::setAudioData(const QByteArray& data, int sampleRate, int channelCount) {
    m_audioData = data;
    m_sampleRate = sampleRate;
    m_channelCount = channelCount;
    computePeaks();
    update();
}

void WaveformWidget::clear() {
    m_audioData.clear();
    m_peaks.clear();
    update();
}

void WaveformWidget::computePeaks() {
    m_peaks.clear();
    if (m_audioData.isEmpty()) return;

    const int peakCount = 512;  // Number of peaks to show
    const int bytesPerSample = 2;  // 16-bit
    const int frameSize = m_channelCount * bytesPerSample;
    const int totalFrames = m_audioData.size() / frameSize;
    const int framesPerPeak = qMax(1, totalFrames / peakCount);

    for (int i = 0; i < peakCount && (i * framesPerPeak) < totalFrames; ++i) {
        float maxVal = 0;
        int start = i * framesPerPeak * frameSize;
        int end = qMin(start + framesPerPeak * frameSize, m_audioData.size());
        for (int j = start; j + 1 < end; j += frameSize) {
            qint16 sample = *reinterpret_cast<const qint16*>(m_audioData.constData() + j);
            float val = std::abs(sample / 32768.0f);
            if (val > maxVal) maxVal = val;
        }
        m_peaks.append(maxVal);
    }
}

void WaveformWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QColor bgColor = palette().color(QPalette::Base);
    QColor waveColor = palette().color(QPalette::Highlight);
    if (waveColor.lightness() < 128) waveColor = QColor(100, 180, 255);
    QColor centerLineColor = palette().color(QPalette::Mid);
    painter.fillRect(rect(), bgColor);

    if (m_peaks.isEmpty()) {
        painter.setPen(centerLineColor);
        painter.drawLine(0, height() / 2, width(), height() / 2);
        painter.setPen(palette().color(QPalette::PlaceholderText));
        painter.drawText(rect(), Qt::AlignCenter, tr("No audio recorded"));
        return;
    }

    int centerY = height() / 2;
    int halfHeight = height() / 2 - 4;
    float step = width() / (float)qMax(1, m_peaks.size() - 1);

    QPainterPath path;
    path.moveTo(0, centerY);
    for (int i = 0; i < m_peaks.size(); ++i) {
        float x = i * step;
        float amplitude = m_peaks[i] * halfHeight;
        path.lineTo(x, centerY - amplitude);
    }
    for (int i = m_peaks.size() - 1; i >= 0; --i) {
        float x = i * step;
        float amplitude = m_peaks[i] * halfHeight;
        path.lineTo(x, centerY + amplitude);
    }
    path.closeSubpath();

    painter.setPen(Qt::NoPen);
    painter.fillPath(path, waveColor);
    painter.setPen(centerLineColor);
    painter.drawLine(0, centerY, width(), centerY);
}
