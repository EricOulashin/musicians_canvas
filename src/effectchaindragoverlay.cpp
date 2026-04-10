#include "effectchaindragoverlay.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPen>

EffectChainDragOverlay::EffectChainDragOverlay(QWidget* parent) : QWidget(parent)
{
    setAutoFillBackground(false);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void EffectChainDragOverlay::setPlaceholderRect(const QRect& r)
{
    m_placeholder   = r;
    m_hasPlaceholder = true;
    update();
}

void EffectChainDragOverlay::clearPlaceholder()
{
    m_hasPlaceholder = false;
    update();
}

void EffectChainDragOverlay::setDropLine(int y, bool visible)
{
    m_lineY    = y;
    m_showLine = visible;
    update();
}

void EffectChainDragOverlay::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    if (m_hasPlaceholder)
    {
        QPen pen(QColor(80, 80, 80), 2, Qt::DashLine);
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(m_placeholder.adjusted(1, 1, -1, -1), 4, 4);
    }

    if (m_showLine && width() > 8)
    {
        const int y = qBound(0, m_lineY, height() - 1);
        QPen linePen(QColor(30, 100, 220), 3, Qt::SolidLine, Qt::FlatCap);
        p.setPen(linePen);
        p.drawLine(6, y, width() - 6, y);
    }
}
