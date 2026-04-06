#include "metronomedialog.h"
#include "appsettings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>

MetronomeDialog::MetronomeDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle(tr("Metronome Settings"));
    setMinimumWidth(320);

    auto* layout = new QVBoxLayout(this);

    auto* group = new QGroupBox(tr("Metronome"));
    auto* groupLayout = new QVBoxLayout(group);

    m_enabledCheck = new QCheckBox(tr("Enable metronome during recording"));
    groupLayout->addWidget(m_enabledCheck);

    auto* bpmRow = new QHBoxLayout();
    bpmRow->addWidget(new QLabel(tr("Beats per minute:")));
    m_bpmSpin = new QSpinBox();
    m_bpmSpin->setRange(20, 300);
    m_bpmSpin->setValue(120);
    m_bpmSpin->setSuffix(QStringLiteral(" BPM"));
    bpmRow->addWidget(m_bpmSpin);
    bpmRow->addStretch();
    groupLayout->addLayout(bpmRow);

    layout->addWidget(group);

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    auto* okBtn = new QPushButton(tr("OK"));
    connect(okBtn, &QPushButton::clicked, this, &MetronomeDialog::onApply);
    buttonLayout->addWidget(okBtn);
    auto* cancelBtn = new QPushButton(tr("Cancel"));
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(cancelBtn);
    layout->addLayout(buttonLayout);

    loadSettings();
}

void MetronomeDialog::loadSettings()
{
    const auto& s = AppSettings::instance();
    m_enabledCheck->setChecked(s.metronomeEnabled());
    m_bpmSpin->setValue(s.metronomeBpm());
}

void MetronomeDialog::saveSettings()
{
    auto& s = AppSettings::instance();
    s.setMetronomeEnabled(m_enabledCheck->isChecked());
    s.setMetronomeBpm(m_bpmSpin->value());
    s.save();
}

void MetronomeDialog::onApply()
{
    saveSettings();
    accept();
}
