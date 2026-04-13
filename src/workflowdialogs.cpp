#include "workflowdialogs.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QComboBox>
#include <QSlider>
#include <QRadioButton>
#include <QButtonGroup>
#include <QGroupBox>
#include <algorithm>
#include <cmath>

// --- RecordingOptionsDialog ---

RecordingOptionsDialog::RecordingOptionsDialog(const ProjectSettings& settings, QWidget* parent)
    : QDialog(parent), m_base(settings)
{
    setWindowTitle(tr("Recording options"));
    setMinimumWidth(400);

    auto* root = new QVBoxLayout(this);
    auto* punchGroup = new QGroupBox(tr("Punch-in recording (audio)"));
    auto* punchLay = new QFormLayout(punchGroup);
    m_punchEnable = new QCheckBox(tr("Replace only between punch times"));
    m_punchEnable->setChecked(settings.punchRecordingEnabled);
    m_punchIn = new QDoubleSpinBox();
    m_punchIn->setRange(0, 86400);
    m_punchIn->setDecimals(3);
    m_punchIn->setSuffix(tr(" s"));
    m_punchIn->setValue(settings.punchInSec);
    m_punchOut = new QDoubleSpinBox();
    m_punchOut->setRange(0, 86400);
    m_punchOut->setDecimals(3);
    m_punchOut->setSuffix(tr(" s"));
    m_punchOut->setValue(settings.punchOutSec);
    punchLay->addRow(m_punchEnable);
    punchLay->addRow(tr("Punch in:"), m_punchIn);
    punchLay->addRow(tr("Punch out:"), m_punchOut);
    root->addWidget(punchGroup);

    auto* loopGroup = new QGroupBox(tr("Loop playback"));
    auto* loopLay = new QFormLayout(loopGroup);
    m_loopEnable = new QCheckBox(tr("Loop playback region when playing all tracks"));
    m_loopEnable->setChecked(settings.loopPlaybackEnabled);
    m_loopStart = new QDoubleSpinBox();
    m_loopStart->setRange(0, 86400);
    m_loopStart->setDecimals(3);
    m_loopStart->setSuffix(tr(" s"));
    m_loopStart->setValue(settings.loopStartSec);
    m_loopEnd = new QDoubleSpinBox();
    m_loopEnd->setRange(0, 86400);
    m_loopEnd->setDecimals(3);
    m_loopEnd->setSuffix(tr(" s"));
    m_loopEnd->setValue(settings.loopEndSec);
    loopLay->addRow(m_loopEnable);
    loopLay->addRow(tr("Loop start:"), m_loopStart);
    loopLay->addRow(tr("Loop end:"), m_loopEnd);
    root->addWidget(loopGroup);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    root->addWidget(buttons);
}

ProjectSettings RecordingOptionsDialog::settings() const
{
    ProjectSettings s = m_base;
    s.punchRecordingEnabled = m_punchEnable && m_punchEnable->isChecked();
    s.punchInSec = m_punchIn ? m_punchIn->value() : 0;
    s.punchOutSec = m_punchOut ? m_punchOut->value() : 0;
    s.loopPlaybackEnabled = m_loopEnable && m_loopEnable->isChecked();
    s.loopStartSec = m_loopStart ? m_loopStart->value() : 0;
    s.loopEndSec = m_loopEnd ? m_loopEnd->value() : 0;
    return s;
}

// --- TempoMapDialog ---

TempoMapDialog::TempoMapDialog(const QVector<TempoMarker>& markers, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Tempo map"));
    setMinimumSize(420, 320);
    auto* root = new QVBoxLayout(this);
    root->addWidget(new QLabel(tr("Tempo changes (time in seconds from project start).")));

    m_list = new QListWidget();
    for (const TempoMarker& m : markers)
    {
        m_list->addItem(QStringLiteral("%1 s — %2 BPM")
                             .arg(QString::number(m.timeSec, 'f', 3),
 QString::number(m.bpm, 'f', 2)));
    }
    if (m_list->count() == 0)
        m_list->addItem(QStringLiteral("0 s — 120 BPM"));
    root->addWidget(m_list, 1);

    auto* editRow = new QHBoxLayout();
    m_timeSpin = new QDoubleSpinBox();
    m_timeSpin->setRange(0, 86400);
    m_timeSpin->setDecimals(3);
    m_timeSpin->setSuffix(tr(" s"));
    m_bpmSpin = new QDoubleSpinBox();
    m_bpmSpin->setRange(20, 400);
    m_bpmSpin->setDecimals(2);
    m_bpmSpin->setSuffix(tr(" BPM"));
    m_bpmSpin->setValue(120);
    editRow->addWidget(new QLabel(tr("New:")));
    editRow->addWidget(m_timeSpin);
    editRow->addWidget(m_bpmSpin);
    auto* addBtn = new QPushButton(tr("Add / update"));
    connect(addBtn, &QPushButton::clicked, this, &TempoMapDialog::onAdd);
    editRow->addWidget(addBtn);
    auto* remBtn = new QPushButton(tr("Remove"));
    connect(remBtn, &QPushButton::clicked, this, &TempoMapDialog::onRemove);
    editRow->addWidget(remBtn);
    root->addLayout(editRow);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    root->addWidget(buttons);
}

void TempoMapDialog::onAdd()
{
    const QString line = QStringLiteral("%1 s — %2 BPM")
                             .arg(QString::number(m_timeSpin->value(), 'f', 3),
                                   QString::number(m_bpmSpin->value(), 'f', 2));
    bool replaced = false;
    for (int i = 0; i < m_list->count(); ++i)
    {
        const QString t = m_list->item(i)->text();
        const int sep = t.indexOf(QLatin1String(" s"));
        if (sep > 0)
        {
            const double ts = t.left(sep).toDouble();
            if (std::abs(ts - m_timeSpin->value()) < 1e-6)
            {
                m_list->item(i)->setText(line);
                replaced = true;
                break;
            }
        }
    }
    if (!replaced)
        m_list->addItem(line);
}

void TempoMapDialog::onRemove()
{
    delete m_list->takeItem(m_list->currentRow());
    if (m_list->count() == 0)
        m_list->addItem(QStringLiteral("0 s — 120 BPM"));
}

QVector<TempoMarker> TempoMapDialog::markers() const
{
    QVector<TempoMarker> out;
    for (int i = 0; i < m_list->count(); ++i)
    {
        const QString t = m_list->item(i)->text();
        const int sep = t.indexOf(QLatin1String(" s"));
        const int dash = t.indexOf(QLatin1String("—"));
        if (sep <= 0 || dash < 0)
            continue;
        TempoMarker m;
        m.timeSec = t.left(sep).toDouble();
        const QString bpmPart = t.mid(dash + 1).trimmed();
        const int bpmEnd = bpmPart.indexOf(QLatin1Char(' '));
        m.bpm = bpmPart.left(bpmEnd < 0 ? bpmPart.size() : bpmEnd).toDouble();
        if (m.bpm > 0)
            out.append(m);
    }
    std::sort(out.begin(), out.end(),
 [](const TempoMarker& a, const TempoMarker& b) { return a.timeSec < b.timeSec; });
    return out;
}

// --- QuantizeMidiDialog ---

QuantizeMidiDialog::QuantizeMidiDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Quantize MIDI"));
    auto* root = new QVBoxLayout(this);
    m_divisionCombo = new QComboBox();
    m_divisionCombo->addItem(tr("1/4 (quarter note)"), 1.0);
    m_divisionCombo->addItem(tr("1/8"), 0.5);
    m_divisionCombo->addItem(tr("1/16"), 0.25);
    m_divisionCombo->addItem(tr("1/32"), 0.125);
    root->addWidget(new QLabel(tr("Grid:")));
    root->addWidget(m_divisionCombo);

    m_strengthSlider = new QSlider(Qt::Horizontal);
    m_strengthSlider->setRange(0, 100);
    m_strengthSlider->setValue(100);
    m_strengthLabel = new QLabel();
    connect(m_strengthSlider, &QSlider::valueChanged, this, [this](int v)
    {
        m_strengthLabel->setText(tr("Strength: %1%").arg(v));
    });
    m_strengthLabel->setText(tr("Strength: 100%"));
    root->addWidget(m_strengthLabel);
    root->addWidget(m_strengthSlider);

    m_allMidi = new QRadioButton(tr("All MIDI tracks"));
    m_armedOnly = new QRadioButton(tr("Armed MIDI track only"));
    m_allMidi->setChecked(true);
    auto* g = new QButtonGroup(this);
    g->addButton(m_allMidi);
    g->addButton(m_armedOnly);
    root->addWidget(m_allMidi);
    root->addWidget(m_armedOnly);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    root->addWidget(buttons);
}

double QuantizeMidiDialog::gridAsQuarterNoteFraction() const
{
    return m_divisionCombo ? m_divisionCombo->currentData().toDouble() : 0.25;
}

double QuantizeMidiDialog::strength() const
{
    return m_strengthSlider ? m_strengthSlider->value() / 100.0 : 1.0;
}

bool QuantizeMidiDialog::applyToArmedTrackOnly() const
{
    return m_armedOnly && m_armedOnly->isChecked();
}

void quantizeMidiNotesInPlace(QVector<MidiNote>& notes, double gridSeconds, double strength)
{
    if (gridSeconds <= 0 || notes.isEmpty())
        return;
    const double s = std::clamp(strength, 0.0, 1.0);
    for (MidiNote& n : notes)
    {
        const double snapped = std::llround(n.startTime / gridSeconds) * gridSeconds;
        n.startTime = n.startTime + (snapped - n.startTime) * s;
        if (n.startTime < 0)
            n.startTime = 0;
    }
}
