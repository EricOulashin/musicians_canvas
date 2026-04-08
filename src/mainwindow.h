#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QIcon>
#include <QElapsedTimer>
#include <memory>
#include "trackdata.h"
#include "projectsettings.h"

#ifdef HAVE_PORTAUDIO
#include "portaudiorecorder.h"
#endif

class QScrollArea;
class QToolBar;
class QToolButton;
class QWidget;
class QVBoxLayout;
class QPushButton;
class QLineEdit;
class QLabel;
class QTimer;
class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;
class TrackWidget;
class SettingsDialog;
class SegmentDisplay;

#ifdef QT_MULTIMEDIA_AVAILABLE
class QMediaPlayer;
class QAudioOutput;
class QAudioSource;
class QBuffer;
#endif

class MainWindow : public QMainWindow
{
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
    void onProjectSettings();
    void onMetronomeSettings();
    void onTimeDisplayTick();
    void onAbout();

private:
    void setupMenuBar();
    void setupToolBar();
    void setupUi();
    void retranslateUi();
    void updateToolBarState();
    void changeEvent(QEvent* event) override;
    void loadProjectFromFile(const QString& path);
    TrackData createNewTrack();
    void updatePlayRecordButton();
    bool isAnyTrackArmed() const;
    TrackWidget* armedTrack() const;
    void startPlayback();
    void startRecording();
    void beginRecordingAfterCountdown();
    void stopPlaybackOrRecording();
    QString effectiveProjectPath() const;
    void moveProjectFiles(const QString& oldDir, const QString& newDir);
    void markDirty();
    void closeEvent(QCloseEvent* event) override;

    // Drag-and-drop: accept supported audio files dropped onto the window
    // when a project is open.  Each dropped file is copied into the project
    // directory (if not already there) and added as a new audio track named
    // after the file's base name.
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    bool addAudioFileAsTrack(const QString& sourcePath);

#ifdef QT_MULTIMEDIA_AVAILABLE
    void prepareOverdubPlayback();
    void startOverdubPlayback();
    void startRecordingLevelMeter();
    void finalizeRecordedAudio(const QByteArray& rawData, qint64 processedMicroseconds,
                               const char* captureBackendLabel);
#endif

    QToolBar* m_toolBar = nullptr;
    QToolButton* m_tbOpen = nullptr;
    QToolButton* m_tbSave = nullptr;
    QToolButton* m_tbProjectSettings = nullptr;
    QToolButton* m_tbConfig = nullptr;
    QToolButton* m_tbMetronome = nullptr;
    SegmentDisplay* m_timeDisplay = nullptr;
    QTimer* m_timeDisplayTimer = nullptr;
    QElapsedTimer m_activeTimer;  // tracks elapsed time during play/record
    QTimer* m_metronomeTimer = nullptr;
    QScrollArea* m_scrollArea = nullptr;
    QWidget* m_tracksContainer = nullptr;
    QVBoxLayout* m_tracksLayout = nullptr;
    QPushButton* m_addTrackButton = nullptr;
    QPushButton* m_playRecordButton = nullptr;
    QPushButton* m_browseButton = nullptr;
    QPushButton* m_clearTracksButton = nullptr;
    QLabel* m_projectLocationLabel = nullptr;
    QLineEdit* m_projectLocationEdit = nullptr;
    QVector<TrackWidget*> m_trackWidgets;
    int m_nextTrackId = 1;

    QIcon m_playIcon;
    QIcon m_recordIcon;
    QIcon m_stopIcon;
    bool m_isActive = false;  // true while playing or recording
    bool m_isDirty = false;   // true when there are unsaved changes

    ProjectSettings m_projectSettings;

    QTimer* m_recordingLevelTimer = nullptr;
    QTimer* m_countdownTimer = nullptr;
    int     m_countdownRemaining = 0;

#ifdef QT_MULTIMEDIA_AVAILABLE
    // Actual format the recording is using (may differ from project settings if unsupported)
    int  m_recordingSampleRate    = 44100;
    int  m_recordingChannelCount  = 2;
    int  m_recordingSampleFormat  = 2;   // QAudioFormat::SampleFormat enum value (2 = Int16)
    QElapsedTimer m_recordingTimer;      // validates reported sample rate
#endif

#ifdef QT_MULTIMEDIA_AVAILABLE
    QMediaPlayer* m_player = nullptr;
    QAudioOutput* m_audioOutput = nullptr;
    QAudioSource* m_audioSource = nullptr;
    QBuffer* m_recordBuffer = nullptr;
    QString m_playbackTempFile;
#endif

#ifdef HAVE_PORTAUDIO
    std::unique_ptr<PortAudioRecorder> m_portAudioRecorder;
    bool m_recordingUsesPortAudio = false;
#endif
};

#endif // MAINWINDOW_H
