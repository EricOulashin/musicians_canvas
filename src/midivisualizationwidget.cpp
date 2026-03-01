#include "midivisualizationwidget.h"
#include <QPainter>
#include <algorithm>

MidiVisualizationWidget::MidiVisualizationWidget(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(60);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setAutoFillBackground(true);
}

void MidiVisualizationWidget::setMidiNotes(const QVector<MidiNote>& notes, double lengthSeconds) {
    m_notes = notes;
    m_lengthSeconds = lengthSeconds;
    for (const auto& n : notes) {
        double end = n.startTime + n.duration;
        if (end > m_lengthSeconds) m_lengthSeconds = end;
    }
    update();
}

void MidiVisualizationWidget::clear() {
    m_notes.clear();
    m_lengthSeconds = 0;
    update();
}

void MidiVisualizationWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QColor bgColor = palette().color(QPalette::Base);
    QColor noteColor = palette().color(QPalette::Highlight);
    if (noteColor.lightness() < 128) noteColor = QColor(180, 140, 255);
    QColor gridColor = palette().color(QPalette::Mid);
    QColor emptyColor = palette().color(QPalette::PlaceholderText);
    painter.fillRect(rect(), bgColor);

    if (m_notes.isEmpty() && m_lengthSeconds <= 0) {
        painter.setPen(emptyColor);
        painter.drawText(rect(), Qt::AlignCenter, tr("No MIDI data recorded"));
        return;
    }

    double maxTime = m_lengthSeconds;
    for (const auto& n : m_notes) {
        double end = n.startTime + n.duration;
        if (end > maxTime) maxTime = end;
    }
    if (maxTime < 0.1) maxTime = 1.0;

    const int minNote = 21;   // A0
    const int maxNote = 108;  // C8
    const int noteRange = maxNote - minNote;
    const double pixelsPerSecond = width() / maxTime;
    const double noteHeight = (height() - 4) / (double)noteRange;

    for (int i = 0; i <= 10; ++i) {
        int y = height() * i / 10;
        painter.setPen(gridColor);
        painter.drawLine(0, y, width(), y);
    }
    for (int i = 0; i <= 10; ++i) {
        int x = width() * i / 10;
        painter.setPen(gridColor);
        painter.drawLine(x, 0, x, height());
    }

    for (const auto& note : m_notes) {
        if (note.note < minNote || note.note > maxNote) continue;
        int y = height() - 2 - (note.note - minNote) * noteHeight;
        int x = note.startTime * pixelsPerSecond;
        int w = qMax(2, (int)(note.duration * pixelsPerSecond));
        QColor col = noteColor;
        col.setAlpha(100 + (note.velocity * 155 / 127));
        painter.fillRect(x, y, w, qMax(2, (int)noteHeight), col);
        painter.setPen(noteColor);
        painter.drawRect(x, y, w, qMax(2, (int)noteHeight));
    }
}
