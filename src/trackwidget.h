#ifndef TRACKWIDGET_H
#define TRACKWIDGET_H

#include <QWidget>
#include <QFrame>
#include "trackdata.h"

class QPushButton;
class QCheckBox;
class QStackedWidget;
class WaveformWidget;
class MidiVisualizationWidget;
class TrackConfigDialog;

class TrackWidget : public QFrame {
    Q_OBJECT
public:
    explicit TrackWidget(const TrackData& data, QWidget* parent = nullptr);
    TrackData trackData() const { return m_data; }
    void setTrackData(const TrackData& data);
    void updateVisualization();

signals:
    void configurationRequested(TrackWidget* widget);

private slots:
    void onConfigClicked();
    void onEnabledToggled(bool checked);

private:
    void setupUi();
    TrackData m_data;
    QPushButton* m_configButton = nullptr;
    QCheckBox* m_enabledCheck = nullptr;
    QStackedWidget* m_stackedWidget = nullptr;
    WaveformWidget* m_waveformWidget = nullptr;
    MidiVisualizationWidget* m_midiWidget = nullptr;
};

#endif // TRACKWIDGET_H
