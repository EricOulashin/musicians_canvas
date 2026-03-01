#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "trackdata.h"

class QScrollArea;
class QWidget;
class QVBoxLayout;
class QPushButton;
class TrackWidget;
class SettingsDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTrack();
    void onTrackConfigRequested(TrackWidget* widget);
    void onClose();
    void onMix();
    void onSettingsConfiguration();
    void onAddDemoData();
    void onVirtualMidiKeyboard();

private:
    void setupMenuBar();
    void setupUi();
    TrackData createNewTrack();

    QScrollArea* m_scrollArea = nullptr;
    QWidget* m_tracksContainer = nullptr;
    QVBoxLayout* m_tracksLayout = nullptr;
    QPushButton* m_addTrackButton = nullptr;
    QVector<TrackWidget*> m_trackWidgets;
    int m_nextTrackId = 1;
};

#endif // MAINWINDOW_H
