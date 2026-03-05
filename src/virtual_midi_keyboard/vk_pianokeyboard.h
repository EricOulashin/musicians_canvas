#ifndef VK_PIANOKEYBOARD_H
#define VK_PIANOKEYBOARD_H

#include <QWidget>
#include <QVector>

class VkPianoKeyboard : public QWidget {
    Q_OBJECT
public:
    explicit VkPianoKeyboard(QWidget* parent = nullptr);

    void setOctave(int octave);
    void setVisibleKeyCount(int count);
    [[nodiscard]] int baseNote() const { return 36 + m_octave * 12; }
    [[nodiscard]] int keyCount() const { return m_keyCount; }

signals:
    void noteOn(int note);
    void noteOff(int note);

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void focusOutEvent(QFocusEvent*) override;

private:
    int noteAt(const QPoint& pos) const;
    void pressNote(int note);
    void releaseNote(int note);
    void updateKeyCount();

    int m_octave = 0;
    int m_keyCount = 61;
    int m_baseNote = 36;
    float m_keyWidth = 20.0f;
    QVector<int> m_pressedNotes;
    int m_mousePressedNote = -1;
};

#endif // VK_PIANOKEYBOARD_H
