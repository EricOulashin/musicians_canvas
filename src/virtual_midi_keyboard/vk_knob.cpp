#include "vk_knob.h"
#include <QPainter>
#include <QMouseEvent>
#include <cmath>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QValidator>
#include <algorithm>

VkKnob::VkKnob(QWidget* parent) : QWidget(parent)
{
    setMinimumSize(80, 100);
}

void VkKnob::setValue(int val)
{
    m_value = std::clamp(val, m_min, m_max);
    update();
}

void VkKnob::setRange(int minVal, int maxVal)
{
    m_min = minVal;
    m_max = maxVal;
    m_value = std::clamp(m_value, m_min, m_max);
}

void VkKnob::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const int knobSize = qMin(width(), 70);
    const int cx = width() / 2;
    const int cy = knobSize / 2;

    QLinearGradient grad(0, 0, knobSize, knobSize);
    grad.setColorAt(0, QColor(100, 100, 110));
    grad.setColorAt(0.3, QColor(70, 70, 80));
    grad.setColorAt(0.7, QColor(50, 50, 60));
    grad.setColorAt(1, QColor(90, 90, 100));

    p.setBrush(grad);
    p.setPen(QPen(QColor(40, 40, 50), 2));
    p.drawEllipse(cx - knobSize/2, 0, knobSize, knobSize);

    const double angle = -240.0 + (m_value - m_min) / double(m_max - m_min) * 270.0;
    const double rad = angle * 3.14159 / 180.0;
    const int r = knobSize / 2 - 8;
    const int tipX = cx + static_cast<int>(r * std::cos(rad));
    const int tipY = cy - static_cast<int>(r * std::sin(rad));
    p.setPen(QPen(QColor(200, 200, 200), 3));
    p.drawLine(cx, cy, tipX, tipY);
}

void VkKnob::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = true;
        m_dragStart = event->globalPosition().toPoint();
        m_dragStartValue = m_value;
    }
}

void VkKnob::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging)
    {
        const int dx = event->globalPosition().toPoint().x() - m_dragStart.x();
        const int range = m_max - m_min + 1;
        const int delta = (dx * range) / 150;
        const int newVal = std::clamp(m_dragStartValue + delta, m_min, m_max);
        if (newVal != m_value)
        {
            m_value = newVal;
            m_dragStart = event->globalPosition().toPoint();
            m_dragStartValue = m_value;
            emit valueChanged(m_value);
            update();
        }
    }
}

void VkKnob::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        m_dragging = false;
}
