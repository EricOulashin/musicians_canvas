#include "segmentdisplay.h"
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <cmath>

// Segment map for digits 0-9.
// Bits: 0=top, 1=top-right, 2=bottom-right, 3=bottom,
//       4=bottom-left, 5=top-left, 6=middle
const quint8 SegmentDisplay::s_segmentMap[10] = {
    0x3F, // 0: all except middle
    0x06, // 1: top-right + bottom-right
    0x5B, // 2: top, top-right, middle, bottom-left, bottom
    0x4F, // 3: top, top-right, middle, bottom-right, bottom
    0x66, // 4: top-left, top-right, middle, bottom-right
    0x6D, // 5: top, top-left, middle, bottom-right, bottom
    0x7D, // 6: top, top-left, middle, bottom-right, bottom-left, bottom
    0x07, // 7: top, top-right, bottom-right
    0x7F, // 8: all
    0x6F, // 9: top, top-right, top-left, middle, bottom-right, bottom
};

SegmentDisplay::SegmentDisplay(QWidget* parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

// Compute the exact width needed for the content at a given height
static qreal contentWidthForHeight(qreal h)
{
    const qreal padding = 4.0;
    const qreal digitH = h - 2 * padding;
    const qreal digitW = digitH / 2.0;
    const qreal colonW = digitW * 0.4;
    const qreal dotW   = digitW * 0.3;
    const qreal digitGap = digitW * 0.1;
    // 8 digits + 2 colons + 1 dot + 10 gaps + padding on both sides
    return 8 * digitW + 2 * colonW + dotW + 10 * digitGap + 2 * padding;
}

void SegmentDisplay::setTime(qint64 milliseconds)
{
    m_millis = milliseconds;
    update();
}

void SegmentDisplay::reset()
{
    m_millis = 0;
    update();
}

void SegmentDisplay::flash()
{
    m_flashing = true;
    update();
    if (!m_flashTimer)
    {
        m_flashTimer = new QTimer(this);
        m_flashTimer->setSingleShot(true);
        connect(m_flashTimer, &QTimer::timeout, this, [this]()
        {
            m_flashing = false;
            update();
        });
    }
    m_flashTimer->start(80);  // 80 ms flash, short enough to not blur into the next beat
}

void SegmentDisplay::setActiveColor(const QColor& color)
{
    m_activeColor = color;
    update();
}

QSize SegmentDisplay::sizeHint() const
{
    const int h = 44;
    return QSize(qRound(contentWidthForHeight(h)), h);
}

QSize SegmentDisplay::minimumSizeHint() const
{
    const int h = 30;
    return QSize(qRound(contentWidthForHeight(h)), h);
}

static void drawSegment(QPainter& painter, const QColor& color,
                         qreal x, qreal y, qreal w, qreal h, bool horizontal)
{
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    if (horizontal)
    {
        // Horizontal segment: pointed ends (hexagonal)
        qreal indent = h * 0.4;
        path.moveTo(x + indent, y);
        path.lineTo(x + w - indent, y);
        path.lineTo(x + w, y + h / 2);
        path.lineTo(x + w - indent, y + h);
        path.lineTo(x + indent, y + h);
        path.lineTo(x, y + h / 2);
        path.closeSubpath();
    }
    else
    {
        // Vertical segment: pointed ends (hexagonal)
        qreal indent = w * 0.4;
        path.moveTo(x + w / 2, y);
        path.lineTo(x + w, y + indent);
        path.lineTo(x + w, y + h - indent);
        path.lineTo(x + w / 2, y + h);
        path.lineTo(x, y + h - indent);
        path.lineTo(x, y + indent);
        path.closeSubpath();
    }
    painter.drawPath(path);
}

void SegmentDisplay::drawDigit(QPainter& painter, int digit, qreal x, qreal y,
                                qreal digitW, qreal digitH)
{
    const quint8 segs = (digit >= 0 && digit <= 9) ? s_segmentMap[digit] : 0;
    const qreal segThick = digitW * 0.18;
    const qreal gap = segThick * 0.2;
    // Middle of the digit — splits it into two equal halves
    const qreal midY = y + digitH / 2.0;
    // Vertical segment length for each half (from horizontal segment edge to edge)
    const qreal vSegLen = digitH / 2.0 - segThick - 2 * gap;

    QColor dim = m_activeColor;
    dim.setAlpha(30);

    // Segment a: top horizontal
    drawSegment(painter, (segs & 0x01) ? m_activeColor : dim,
                x + segThick + gap, y, digitW - 2 * segThick - 2 * gap, segThick, true);
    // Segment b: top-right vertical
    drawSegment(painter, (segs & 0x02) ? m_activeColor : dim,
                x + digitW - segThick, y + segThick + gap, segThick, vSegLen, false);
    // Segment c: bottom-right vertical
    drawSegment(painter, (segs & 0x04) ? m_activeColor : dim,
                x + digitW - segThick, midY + segThick / 2.0 + gap, segThick, vSegLen, false);
    // Segment d: bottom horizontal
    drawSegment(painter, (segs & 0x08) ? m_activeColor : dim,
                x + segThick + gap, y + digitH - segThick, digitW - 2 * segThick - 2 * gap, segThick, true);
    // Segment e: bottom-left vertical
    drawSegment(painter, (segs & 0x10) ? m_activeColor : dim,
                x, midY + segThick / 2.0 + gap, segThick, vSegLen, false);
    // Segment f: top-left vertical
    drawSegment(painter, (segs & 0x20) ? m_activeColor : dim,
                x, y + segThick + gap, segThick, vSegLen, false);
    // Segment g: middle horizontal
    drawSegment(painter, (segs & 0x40) ? m_activeColor : dim,
                x + segThick + gap, midY - segThick / 2.0, digitW - 2 * segThick - 2 * gap, segThick, true);
}

void SegmentDisplay::drawColon(QPainter& painter, qreal x, qreal y, qreal w, qreal h)
{
    painter.setBrush(m_activeColor);
    painter.setPen(Qt::NoPen);
    qreal dotSize = h * 0.1;
    qreal cx = x + w / 2 - dotSize / 2;
    painter.drawEllipse(QRectF(cx, y + h * 0.3 - dotSize / 2, dotSize, dotSize));
    painter.drawEllipse(QRectF(cx, y + h * 0.7 - dotSize / 2, dotSize, dotSize));
}

void SegmentDisplay::drawDot(QPainter& painter, qreal x, qreal y, qreal w, qreal h)
{
    painter.setBrush(m_activeColor);
    painter.setPen(Qt::NoPen);
    qreal dotSize = h * 0.1;
    painter.drawEllipse(QRectF(x + w / 2 - dotSize / 2, y + h - dotSize * 1.5, dotSize, dotSize));
}

void SegmentDisplay::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Dark background
    // Background — slightly tinted while a metronome flash is active.
    if (m_flashing)
    {
        // Tint the background with a dim version of the LED color so the
        // flash is unmistakable but doesn't drown out the digits.
        QColor bg = m_activeColor;
        bg.setAlpha(70);
        painter.fillRect(rect(), QColor(20, 20, 20));
        painter.fillRect(rect(), bg);
    }
    else
    {
        painter.fillRect(rect(), QColor(20, 20, 20));
    }

    // Parse time
    const qint64 totalHundredths = m_millis / 10;
    const int hundredths = static_cast<int>(totalHundredths % 100);
    const qint64 totalSec = m_millis / 1000;
    const int secs = static_cast<int>(totalSec % 60);
    const int mins = static_cast<int>((totalSec / 60) % 60);
    const int hrs  = static_cast<int>(totalSec / 3600);

    // Each digit is an upright rectangle: width = height / 2, so the middle
    // horizontal segment divides it into two approximately square halves.
    const qreal padding = 4.0;
    const qreal digitH = height() - 2 * padding;
    const qreal digitW = digitH / 2.0;
    const qreal colonW = digitW * 0.4;
    const qreal dotW   = digitW * 0.3;
    const qreal digitGap = digitW * 0.1;  // small gap between digits

    // Total content width: 8 digits + 2 colons + 1 dot + gaps
    const qreal contentW = 8 * digitW + 2 * colonW + dotW + 10 * digitGap;
    // Center horizontally
    qreal cx = padding + (width() - 2 * padding - contentW) / 2.0;
    if (cx < padding) cx = padding;
    const qreal cy = padding;

    // HH
    drawDigit(painter, hrs / 10, cx, cy, digitW, digitH); cx += digitW + digitGap;
    drawDigit(painter, hrs % 10, cx, cy, digitW, digitH); cx += digitW + digitGap;
    // :
    drawColon(painter, cx, cy, colonW, digitH); cx += colonW + digitGap;
    // MM
    drawDigit(painter, mins / 10, cx, cy, digitW, digitH); cx += digitW + digitGap;
    drawDigit(painter, mins % 10, cx, cy, digitW, digitH); cx += digitW + digitGap;
    // :
    drawColon(painter, cx, cy, colonW, digitH); cx += colonW + digitGap;
    // SS
    drawDigit(painter, secs / 10, cx, cy, digitW, digitH); cx += digitW + digitGap;
    drawDigit(painter, secs % 10, cx, cy, digitW, digitH); cx += digitW;
    // .
    drawDot(painter, cx, cy, dotW, digitH); cx += dotW;
    // hh (hundredths)
    drawDigit(painter, hundredths / 10, cx, cy, digitW, digitH); cx += digitW + digitGap;
    drawDigit(painter, hundredths % 10, cx, cy, digitW, digitH);
}
