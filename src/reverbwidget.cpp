#include "reverbwidget.h"

#include <algorithm>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>

ReverbWidget::ReverbWidget(QWidget* parent) : EffectWidget(parent)
{
    buildUi();
}

QString ReverbWidget::effectTitle() const
{
    return tr("Reverb");
}

QJsonObject ReverbWidget::parametersToJson() const
{
    QJsonObject o;
    o.insert(QStringLiteral("preDelayMs"),
             m_preDelayMs ? double(m_preDelayMs->value()) : 20.0);
    const double decaySec =
        m_decayTenths ? double(m_decayTenths->value()) / 10.0 : 2.5;
    o.insert(QStringLiteral("decayTimeSec"), decaySec);
    o.insert(QStringLiteral("mix"),
             m_mixPercent ? double(m_mixPercent->value()) / 100.0 : 0.35);
    o.insert(QStringLiteral("tone"),
             m_tonePercent ? double(m_tonePercent->value()) / 100.0 : 0.5);
    o.insert(QStringLiteral("modulationDepth"),
             m_modDepthPercent ? double(m_modDepthPercent->value()) / 100.0 : 0.25);
    o.insert(QStringLiteral("modulationRateHz"),
             m_modRateTenths ? double(m_modRateTenths->value()) / 10.0 : 0.35);
    return o;
}

void ReverbWidget::setParametersFromJson(const QJsonObject& o)
{
    blockSignalsForLoad(true);
    if (m_preDelayMs)
        m_preDelayMs->setValue(
            int(std::lround(o.value(QStringLiteral("preDelayMs")).toDouble(20.0))));
    if (m_decayTenths)
    {
        const double sec = o.value(QStringLiteral("decayTimeSec")).toDouble(2.5);
        m_decayTenths->setValue(
            std::clamp(int(std::lround(sec * 10.0)), m_decayTenths->minimum(),
                       m_decayTenths->maximum()));
    }
    if (m_mixPercent)
        m_mixPercent->setValue(int(std::lround(
            o.value(QStringLiteral("mix")).toDouble(0.35) * 100.0)));
    if (m_tonePercent)
        m_tonePercent->setValue(int(std::lround(
            o.value(QStringLiteral("tone")).toDouble(0.5) * 100.0)));
    if (m_modDepthPercent)
        m_modDepthPercent->setValue(int(std::lround(
            o.value(QStringLiteral("modulationDepth")).toDouble(0.25) * 100.0)));
    if (m_modRateTenths)
    {
        const double hz = o.value(QStringLiteral("modulationRateHz")).toDouble(0.35);
        m_modRateTenths->setValue(
            std::clamp(int(std::lround(hz * 10.0)), m_modRateTenths->minimum(),
                       m_modRateTenths->maximum()));
    }
    blockSignalsForLoad(false);
    onControlChanged();
}

void ReverbWidget::onControlChanged()
{
    if (m_preDelayLabel && m_preDelayMs)
        m_preDelayLabel->setText(tr("Pre-delay: %1 ms").arg(m_preDelayMs->value()));
    if (m_decayLabel && m_decayTenths)
        m_decayLabel->setText(
            tr("Decay: %1 s").arg(QString::number(m_decayTenths->value() / 10.0, 'f', 1)));
    if (m_mixLabel && m_mixPercent)
        m_mixLabel->setText(tr("Mix (wet): %1%").arg(m_mixPercent->value()));
    if (m_toneLabel && m_tonePercent)
        m_toneLabel->setText(
            tr("Tone: %1").arg(m_tonePercent->value() < 40
                               ? tr("dark")
                               : (m_tonePercent->value() > 65 ? tr("bright")
                                                               : tr("medium"))));
    if (m_modDepthLabel && m_modDepthPercent)
        m_modDepthLabel->setText(tr("Mod depth: %1%").arg(m_modDepthPercent->value()));
    if (m_modRateLabel && m_modRateTenths)
        m_modRateLabel->setText(
            tr("Mod rate: %1 Hz").arg(QString::number(m_modRateTenths->value() / 10.0, 'f', 2)));
    notifyParametersChanged();
}

void ReverbWidget::blockSignalsForLoad(bool block)
{
    const QList<QSlider*> sliders = {m_preDelayMs, m_decayTenths, m_mixPercent,
                                     m_tonePercent, m_modDepthPercent, m_modRateTenths};
    for (auto* s : sliders)
    {
        if (s)
            s->blockSignals(block);
    }
}

void ReverbWidget::buildUi()
{
    setupHeader();
    if (m_titleLabel)
        m_titleLabel->setText(effectTitle());

    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(6, 2, 6, 6);
    root->setSpacing(4);
    root->addLayout(m_headerLayout);

    auto* grid = new QGridLayout();
    grid->setHorizontalSpacing(8);
    grid->setVerticalSpacing(4);

    auto addRow = [&](int row, QLabel*& valueLbl, QSlider*& slider, int minV, int maxV,
                      int initV, const QString& tip)
    {
        valueLbl = new QLabel(this);
        slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(minV, maxV);
        slider->setValue(initV);
        slider->setToolTip(tip);
        grid->addWidget(valueLbl, row, 0);
        grid->addWidget(slider, row, 1);
        connect(slider, &QSlider::valueChanged, this, &ReverbWidget::onControlChanged);
    };

    addRow(0, m_preDelayLabel, m_preDelayMs, 0, 500, 20,
           tr("Delay before the reverb tail begins (ms)"));
    addRow(1, m_decayLabel, m_decayTenths, 1, 300, 25,
           tr("Reverb decay time (seconds ×10)"));
    addRow(2, m_mixLabel, m_mixPercent, 0, 100, 35, tr("Wet/dry mix: 0% = dry, 100% = wet"));
    addRow(3, m_toneLabel, m_tonePercent, 0, 100, 50,
           tr("High-frequency presence in the reverb tail"));
    addRow(4, m_modDepthLabel, m_modDepthPercent, 0, 100, 25,
           tr("Modulation depth (motion in the tail)"));
    addRow(5, m_modRateLabel, m_modRateTenths, 1, 50, 4,
           tr("Modulation speed (Hz ×10, e.g. 4 = 0.4 Hz)"));

    root->addLayout(grid);
    onControlChanged();
}
