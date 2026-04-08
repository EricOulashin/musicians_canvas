#ifndef SEGMENTDISPLAY_H
#define SEGMENTDISPLAY_H

#include <QWidget>
#include <QColor>
#include <QString>

class QTimer;

// A 7-segment LED numeric display widget that draws digits programmatically.
// Displays time in HH:MM:SS.hh format (hours, minutes, seconds, hundredths).
// Active segments use the configured color; inactive segments use a dim version.
class SegmentDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit SegmentDisplay(QWidget* parent = nullptr);

    void setTime(qint64 milliseconds);
    void reset();

    // Briefly tint the background to give a visual beat indicator (used by
    // the metronome).  The flash decays after a short fixed duration.
    void flash();

    void setActiveColor(const QColor& color);
    QColor activeColor() const { return m_activeColor; }

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawDigit(QPainter& painter, int digit, qreal x, qreal y,
                   qreal digitW, qreal digitH);
    void drawColon(QPainter& painter, qreal x, qreal y, qreal w, qreal h);
    void drawDot(QPainter& painter, qreal x, qreal y, qreal w, qreal h);

    // 7-segment lookup: bits 0-6 = segments a-g (top, top-right, bottom-right,
    // bottom, bottom-left, top-left, middle)
    static const quint8 s_segmentMap[10];

    QColor m_activeColor{80, 255, 80};    // default: bright/light green
    QString m_text;  // formatted time string
    qint64 m_millis = 0;
    bool   m_flashing = false;
    QTimer* m_flashTimer = nullptr;
};

#endif // SEGMENTDISPLAY_H
