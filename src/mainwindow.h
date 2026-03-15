#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QIcon>
#include "trackdata.h"

class QScrollArea;
class QWidget;
class QVBoxLayout;
class QPushButton;
class QLineEdit;
class TrackWidget;
class SettingsDialog;

#ifdef QT_MULTIMEDIA_AVAILABLE
class QMediaPlayer;
class QAudioOutput;
class QAudioSource;
class QBuffer;
#endif

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTrack();
    void onTrackConfigRequested(TrackWidget* widget);
    void onArmChanged(TrackWidget* widget, bool armed);
    void onClose();
    void onMix();
    void onSaveProject();
    void onOpenProject();
    void onSettingsConfiguration();
    void onAddDemoData();
    void onVirtualMidiKeyboard();
    void onPlayRecord();
    void onBrowseProjectLocation();
    void onPlaybackFinished();
    void onTrackNameChanged(TrackWidget* widget, const QString& oldName, const QString& newName);
    void onTrackRemoveRequested(TrackWidget* widget);
    void onClearTracks();

private:
    void setupMenuBar();
    void setupUi();
    TrackData createNewTrack();
    void updatePlayRecordButton();
    bool isAnyTrackArmed() const;
    TrackWidget* armedTrack() const;
    void startPlayback();
    void startRecording();
    void stopPlaybackOrRecording();
    QString effectiveProjectPath() const;
    void moveProjectFiles(const QString& oldDir, const QString& newDir);
    void markDirty();
    void closeEvent(QCloseEvent* event) override;

    QScrollArea* m_scrollArea = nullptr;
    QWidget* m_tracksContainer = nullptr;
    QVBoxLayout* m_tracksLayout = nullptr;
    QPushButton* m_addTrackButton = nullptr;
    QPushButton* m_playRecordButton = nullptr;
    QLineEdit* m_projectLocationEdit = nullptr;
    QVector<TrackWidget*> m_trackWidgets;
    int m_nextTrackId = 1;

    QIcon m_playIcon;
    QIcon m_recordIcon;
    QIcon m_stopIcon;
    bool m_isActive = false;  // true while playing or recording
    bool m_isDirty = false;   // true when there are unsaved changes

#ifdef QT_MULTIMEDIA_AVAILABLE
    QMediaPlayer* m_player = nullptr;
    QAudioOutput* m_audioOutput = nullptr;
    QAudioSource* m_audioSource = nullptr;
    QBuffer* m_recordBuffer = nullptr;
    QString m_playbackTempFile;
#endif
};

#endif // MAINWINDOW_H
