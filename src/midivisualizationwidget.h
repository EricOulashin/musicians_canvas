#ifndef MIDIVISUALIZATIONWIDGET_H
#define MIDIVISUALIZATIONWIDGET_H

#include <QWidget>
#include <QColor>
#include "trackdata.h"

class MidiVisualizationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MidiVisualizationWidget(QWidget* parent = nullptr);
    void setMidiNotes(const QVector<MidiNote>& notes, double lengthSeconds = 0);
    void clear();

    void setStatusText(const QString& text, const QColor& color = QColor());
    void clearStatusText();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QVector<MidiNote> m_notes;
    double m_lengthSeconds = 0;
    QString m_statusText;
    QColor  m_statusColor;
};

#endif // MIDIVISUALIZATIONWIDGET_H
