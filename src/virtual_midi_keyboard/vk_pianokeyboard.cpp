#include "vk_pianokeyboard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QHash>

static bool isBlackKey(int note) {
    const int k = note % 12;
    return (k == 1 || k == 3 || k == 6 || k == 8 || k == 10);
}

static int whiteKeyIndex(int note) {
    const int k = note % 12;
    int w = 0;
    if (k >= 1) w++;
    if (k >= 3) w++;
    if (k >= 5) w++;
    if (k >= 6) w++;
    if (k >= 8) w++;
    if (k >= 10) w++;
    return (note / 12) * 7 + w;
}

VkPianoKeyboard::VkPianoKeyboard(QWidget* parent) : QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    updateKeyCount();
}

void VkPianoKeyboard::setOctave(int octave) {
    m_octave = octave;
    m_baseNote = 36 + m_octave * 12;
    updateKeyCount();
}

void VkPianoKeyboard::setVisibleKeyCount(int count) {
    m_keyCount = std::clamp(count, 12, 88);
    update();
}

void VkPianoKeyboard::updateKeyCount() {
    if (width() <= 0) return;
    const int maxWhite = static_cast<int>(width() / m_keyWidth);
    int count = 0;
    int whiteCount = 0;
    for (int n = m_baseNote; n <= 127 && whiteCount < maxWhite; ++n) {
        ++count;
        if (!isBlackKey(n)) ++whiteCount;
    }
    m_keyCount = std::clamp(count, 12, 88);
    update();
}

void VkPianoKeyboard::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateKeyCount();
}

int VkPianoKeyboard::noteAt(const QPoint& pos) const {
    const int blackHeight = height() * 3 / 5;
    const bool inBlackZone = pos.y() < blackHeight;

    for (int n = m_baseNote; n < m_baseNote + m_keyCount; ++n) {
        if (isBlackKey(n)) {
            const int wi = whiteKeyIndex(n) - whiteKeyIndex(m_baseNote);
            if (wi < 0) continue;
            const int x = static_cast<int>(wi * m_keyWidth - m_keyWidth * 0.4);
            const int w = static_cast<int>(m_keyWidth * 0.65);
            if (inBlackZone && pos.x() >= x && pos.x() < x + w)
                return n;
        }
    }
    int count = 0;
    for (int n = m_baseNote; n < m_baseNote + m_keyCount; ++n) {
        if (!isBlackKey(n)) {
            const int x = static_cast<int>(count * m_keyWidth);
            const int w = static_cast<int>(m_keyWidth) + 1;
            if (pos.x() >= x && pos.x() < x + w)
                return n;
            count++;
        }
    }
    return -1;
}

void VkPianoKeyboard::pressNote(int note) {
    if (note < 0 || note > 127 || m_pressedNotes.contains(note)) return;
    m_pressedNotes.append(note);
    update();
    emit noteOn(note);
}

void VkPianoKeyboard::releaseNote(int note) {
    if (m_pressedNotes.removeOne(note)) {
        update();
        emit noteOff(note);
    }
}

void VkPianoKeyboard::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const float ww = m_keyWidth;

    int idx = 0;
    for (int n = m_baseNote; n < m_baseNote + m_keyCount; ++n) {
        if (!isBlackKey(n)) {
            const int x = static_cast<int>(idx * ww);
            const int w = static_cast<int>(ww) + 1;
            const bool pressed = m_pressedNotes.contains(n);
            p.fillRect(x, 0, w, height(),
                pressed ? QColor(180, 200, 255) : QColor(250, 250, 250));
            p.setPen(QColor(180, 180, 180));
            p.drawLine(x + w - 1, 0, x + w - 1, height());
            ++idx;
        }
    }

    for (int n = m_baseNote; n < m_baseNote + m_keyCount; ++n) {
        if (isBlackKey(n)) {
            const int wi = whiteKeyIndex(n) - whiteKeyIndex(m_baseNote);
            if (wi < 0) continue;
            const int x = static_cast<int>(wi * ww - ww * 0.4);
            const int w = static_cast<int>(ww * 0.65);
            const int h = height() * 3 / 5;
            const bool pressed = m_pressedNotes.contains(n);
            p.fillRect(x, 0, w, h, pressed ? QColor(80, 80, 120) : QColor(50, 50, 55));
            p.setPen(QColor(30, 30, 35));
            p.drawRect(x, 0, w, h);
        }
    }
}

void VkPianoKeyboard::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        m_mousePressedNote = noteAt(event->position().toPoint());
        pressNote(m_mousePressedNote);
    }
}

void VkPianoKeyboard::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        releaseNote(m_mousePressedNote);
        m_mousePressedNote = -1;
    }
}

void VkPianoKeyboard::keyPressEvent(QKeyEvent* event) {
    static const QHash<int, int> keyMap = {
        { Qt::Key_Z, 0 }, { Qt::Key_S, 1 }, { Qt::Key_X, 2 }, { Qt::Key_D, 3 },
        { Qt::Key_C, 4 }, { Qt::Key_V, 5 }, { Qt::Key_G, 6 }, { Qt::Key_B, 7 },
        { Qt::Key_H, 8 }, { Qt::Key_N, 9 }, { Qt::Key_J, 10 }, { Qt::Key_M, 11 },
        { Qt::Key_Q, 12 }, { Qt::Key_2, 13 }, { Qt::Key_W, 14 }, { Qt::Key_3, 15 },
        { Qt::Key_E, 16 }, { Qt::Key_R, 17 }, { Qt::Key_5, 18 }, { Qt::Key_T, 19 },
        { Qt::Key_6, 20 }, { Qt::Key_Y, 21 }, { Qt::Key_7, 22 }, { Qt::Key_U, 23 },
        { Qt::Key_I, 24 }, { Qt::Key_9, 25 }, { Qt::Key_O, 26 }, { Qt::Key_0, 27 },
        { Qt::Key_P, 28 },
    };
    if (event->isAutoRepeat()) return;
    const auto it = keyMap.find(event->key());
    if (it != keyMap.end()) {
        const int note = m_baseNote + it.value();
        pressNote(note);
        event->accept();
    }
}

void VkPianoKeyboard::keyReleaseEvent(QKeyEvent* event) {
    static const QHash<int, int> keyMap = {
        { Qt::Key_Z, 0 }, { Qt::Key_S, 1 }, { Qt::Key_X, 2 }, { Qt::Key_D, 3 },
        { Qt::Key_C, 4 }, { Qt::Key_V, 5 }, { Qt::Key_G, 6 }, { Qt::Key_B, 7 },
        { Qt::Key_H, 8 }, { Qt::Key_N, 9 }, { Qt::Key_J, 10 }, { Qt::Key_M, 11 },
        { Qt::Key_Q, 12 }, { Qt::Key_2, 13 }, { Qt::Key_W, 14 }, { Qt::Key_3, 15 },
        { Qt::Key_E, 16 }, { Qt::Key_R, 17 }, { Qt::Key_5, 18 }, { Qt::Key_T, 19 },
        { Qt::Key_6, 20 }, { Qt::Key_Y, 21 }, { Qt::Key_7, 22 }, { Qt::Key_U, 23 },
        { Qt::Key_I, 24 }, { Qt::Key_9, 25 }, { Qt::Key_O, 26 }, { Qt::Key_0, 27 },
        { Qt::Key_P, 28 },
    };
    if (event->isAutoRepeat()) return;
    const auto it = keyMap.find(event->key());
    if (it != keyMap.end()) {
        const int note = m_baseNote + it.value();
        releaseNote(note);
        event->accept();
    }
}

void VkPianoKeyboard::focusOutEvent(QFocusEvent*) {
    for (int n : m_pressedNotes)
        emit noteOff(n);
    m_pressedNotes.clear();
    update();
}
