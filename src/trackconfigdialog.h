#ifndef TRACKCONFIGDIALOG_H
#define TRACKCONFIGDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include "trackdata.h"

class QRadioButton;
class QDialogButtonBox;
class QDoubleSpinBox;
class QGroupBox;

class TrackConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TrackConfigDialog(QWidget* parent = nullptr);
    TrackType trackType() const;
    void setTrackType(TrackType type);
    void setFromTrackData(const TrackData& d);
    void applyTrimToTrackData(TrackData& d) const;

private:
    QRadioButton* m_audioRadio = nullptr;
    QRadioButton* m_midiRadio = nullptr;
    QButtonGroup* m_typeGroup = nullptr;
    QGroupBox* m_trimGroup = nullptr;
    QDoubleSpinBox* m_trimStartSpin = nullptr;
    QDoubleSpinBox* m_trimEndSpin = nullptr;
};

#endif // TRACKCONFIGDIALOG_H
