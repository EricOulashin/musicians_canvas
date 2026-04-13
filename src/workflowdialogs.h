#ifndef WORKFLOWDIALOGS_H
#define WORKFLOWDIALOGS_H

#include <QDialog>
#include "projectsettings.h"
#include "trackdata.h"

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QListWidget;
class QRadioButton;
class QSlider;

class RecordingOptionsDialog final : public QDialog
{
    Q_OBJECT
public:
    explicit RecordingOptionsDialog(const ProjectSettings& settings, QWidget* parent = nullptr);
    ProjectSettings settings() const;

private:
    ProjectSettings m_base;
    QCheckBox* m_punchEnable = nullptr;
    QDoubleSpinBox* m_punchIn = nullptr;
    QDoubleSpinBox* m_punchOut = nullptr;
    QCheckBox* m_loopEnable = nullptr;
    QDoubleSpinBox* m_loopStart = nullptr;
    QDoubleSpinBox* m_loopEnd = nullptr;
};

class TempoMapDialog final : public QDialog
{
    Q_OBJECT
public:
    explicit TempoMapDialog(const QVector<TempoMarker>& markers, QWidget* parent = nullptr);
    QVector<TempoMarker> markers() const;

private slots:
    void onAdd();
    void onRemove();

private:
    QListWidget* m_list = nullptr;
    QDoubleSpinBox* m_timeSpin = nullptr;
    QDoubleSpinBox* m_bpmSpin = nullptr;
};

class QuantizeMidiDialog final : public QDialog
{
    Q_OBJECT
public:
    explicit QuantizeMidiDialog(QWidget* parent = nullptr);
    /// Grid note length as fraction of a quarter note (e.g. 0.25 = sixteenth).
    double gridAsQuarterNoteFraction() const;
    /// 0 = no move, 1 = full snap.
    double strength() const;
    bool applyToArmedTrackOnly() const;

private:
    QComboBox* m_divisionCombo = nullptr;
    QSlider* m_strengthSlider = nullptr;
    QLabel* m_strengthLabel = nullptr;
    QRadioButton* m_allMidi = nullptr;
    QRadioButton* m_armedOnly = nullptr;
};

void quantizeMidiNotesInPlace(QVector<MidiNote>& notes, double gridSeconds, double strength);

#endif
