#ifndef VK_KNOB_H
#define VK_KNOB_H

#include <QWidget>

class VkKnob : public QWidget {
    Q_OBJECT
public:
    explicit VkKnob(QWidget* parent = nullptr);

    [[nodiscard]] int value() const { return m_value; }
    void setValue(int val);
    void setRange(int minVal, int maxVal);

signals:
    void valueChanged(int value);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    int m_value = 64;
    int m_min = 0;
    int m_max = 127;
    bool m_dragging = false;
    QPoint m_dragStart;
    int m_dragStartValue;
};

#endif // VK_KNOB_H
