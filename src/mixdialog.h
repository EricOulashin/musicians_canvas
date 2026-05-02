#ifndef MIXDIALOG_H
#define MIXDIALOG_H

#include <QDialog>
#include <optional>

#include "audioutils.h"

class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QSlider;
class QSpinBox;
class QWidget;

class MixDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MixDialog(const QString& defaultDir, QWidget* parent = nullptr);

    QString outputFilename() const;

    /// Encoding options when the output path uses MP3 or Ogg; absent for WAV/FLAC/AIFF.
    [[nodiscard]] std::optional<MixExportEncodeSettings> mixEncodeSettings() const;

private slots:
    void onBrowse();
    void onAccept();
    void refreshEncodingUi();
    void onMp3ModeChanged(int index);
    void onOggModeChanged(int index);
    void updateFitSize();

private:
    QLineEdit* m_filenameEdit = nullptr;
    QString m_defaultDir;

    QGroupBox* m_encodingGroup = nullptr;
    QWidget* m_mp3Widget = nullptr;
    QComboBox* m_mp3ModeCombo = nullptr;
    QSpinBox* m_mp3KbpsSpin = nullptr;
    QSlider* m_mp3VbrSlider = nullptr;
    QLabel* m_mp3VbrValueLabel = nullptr;

    QWidget* m_oggWidget = nullptr;
    QComboBox* m_oggModeCombo = nullptr;
    QSlider* m_oggQualitySlider = nullptr;
    QLabel* m_oggQualityValueLabel = nullptr;
    QSpinBox* m_oggApproxKbpsSpin = nullptr;
};

#endif // MIXDIALOG_H
