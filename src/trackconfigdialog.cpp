#include "trackconfigdialog.h"
#include <QRadioButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLabel>

TrackConfigDialog::TrackConfigDialog(QWidget* parent) : QDialog(parent) {
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

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttonBox);
}

TrackType TrackConfigDialog::trackType() const {
    return m_midiRadio->isChecked() ? TrackType::MIDI : TrackType::Audio;
}

void TrackConfigDialog::setTrackType(TrackType type) {
    if (type == TrackType::MIDI) {
        m_midiRadio->setChecked(true);
    } else {
        m_audioRadio->setChecked(true);
    }
}
