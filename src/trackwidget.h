#ifndef TRACKWIDGET_H
#define TRACKWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QString>
#include "trackdata.h"

class QPushButton;
class QCheckBox;
class QLineEdit;
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

    bool isArmed() const;
    void setArmed(bool armed);
    void setArmEnabled(bool enabled);

signals:
    void configurationRequested(TrackWidget* widget);
    void armChanged(TrackWidget* widget, bool armed);
    void dataChanged(TrackWidget* widget);
    void nameChanged(TrackWidget* widget, const QString& oldName, const QString& newName);
    void removeRequested(TrackWidget* widget);

private slots:
    void onConfigClicked();
    void onEnabledToggled(bool checked);
    void onArmToggled(bool checked);

private:
    void setupUi();
    TrackData m_data;
    QString m_lastValidName;
    QPushButton* m_configButton = nullptr;
    QLineEdit* m_nameEdit = nullptr;
    QCheckBox* m_enabledCheck = nullptr;
    QCheckBox* m_armCheck = nullptr;
    QStackedWidget* m_stackedWidget = nullptr;
    WaveformWidget* m_waveformWidget = nullptr;
    MidiVisualizationWidget* m_midiWidget = nullptr;
};

#endif // TRACKWIDGET_H
