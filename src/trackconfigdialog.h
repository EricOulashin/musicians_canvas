#ifndef TRACKCONFIGDIALOG_H
#define TRACKCONFIGDIALOG_H

#include <QDialog>
#include <QButtonGroup>
#include "trackdata.h"

class QRadioButton;
class QDialogButtonBox;

class TrackConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TrackConfigDialog(QWidget* parent = nullptr);
    TrackType trackType() const;
    void setTrackType(TrackType type);

private:
    QRadioButton* m_audioRadio = nullptr;
    QRadioButton* m_midiRadio = nullptr;
    QButtonGroup* m_typeGroup = nullptr;
};

#endif // TRACKCONFIGDIALOG_H
