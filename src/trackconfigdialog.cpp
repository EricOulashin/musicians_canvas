#include "trackconfigdialog.h"
#include "trackdata.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QFormLayout>

TrackConfigDialog::TrackConfigDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Track Configuration"));
    setMinimumSize(350, 200);
    resize(400, 220);

    auto* layout = new QVBoxLayout(this);

    auto* groupBox = new QGroupBox(tr("Track Type"));
    auto* groupLayout = new QVBoxLayout(groupBox);

    m_audioRadio = new QRadioButton(tr("Audio - Record from audio interface"));
    m_midiRadio = new QRadioButton(tr("MIDI - Record from MIDI device"));
    m_audioRadio->setChecked(true);

    m_typeGroup = new QButtonGroup(this);
    m_typeGroup->addButton(m_audioRadio);
    m_typeGroup->addButton(m_midiRadio);

    groupLayout->addWidget(m_audioRadio);
    groupLayout->addWidget(new QLabel(tr("Capture audio from microphone or line-in.")));
    groupLayout->addSpacing(12);
    groupLayout->addWidget(m_midiRadio);
    groupLayout->addWidget(new QLabel(tr("Capture MIDI notes from keyboard or controller.")));
    layout->addWidget(groupBox);

    m_trimGroup = new QGroupBox(tr("Clip trim (non-destructive)"));
    m_trimGroup->setToolTip(
        tr("Playback and mix ignore this much audio at the start and end of the clip. "
           "The original recording is not deleted."));
    auto* trimForm = new QFormLayout(m_trimGroup);
    m_trimStartSpin = new QDoubleSpinBox();
    m_trimStartSpin->setRange(0, 3600);
    m_trimStartSpin->setDecimals(3);
    m_trimStartSpin->setSuffix(tr(" s"));
    m_trimEndSpin = new QDoubleSpinBox();
    m_trimEndSpin->setRange(0, 3600);
    m_trimEndSpin->setDecimals(3);
    m_trimEndSpin->setSuffix(tr(" s"));
    trimForm->addRow(tr("Trim start:"), m_trimStartSpin);
    trimForm->addRow(tr("Trim end:"), m_trimEndSpin);
    layout->addWidget(m_trimGroup);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}

TrackType TrackConfigDialog::trackType() const
{
    return m_midiRadio->isChecked() ? TrackType::MIDI : TrackType::Audio;
}

void TrackConfigDialog::setTrackType(TrackType type)
{
    if (type == TrackType::MIDI)
    {
        m_midiRadio->setChecked(true);
    }
    else
    {
        m_audioRadio->setChecked(true);
    }
}

void TrackConfigDialog::setFromTrackData(const TrackData& d)
{
    setTrackType(d.type);
    if (m_trimStartSpin)
        m_trimStartSpin->setValue(d.trimStartSec);
    if (m_trimEndSpin)
        m_trimEndSpin->setValue(d.trimEndSec);
}

void TrackConfigDialog::applyTrimToTrackData(TrackData& d) const
{
    if (m_trimStartSpin)
        d.trimStartSec = m_trimStartSpin->value();
    if (m_trimEndSpin)
        d.trimEndSec = m_trimEndSpin->value();
}
