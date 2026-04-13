#include "vibratoeffect.h"

#include <algorithm>
#include <cmath>

#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QSlider>
#include <QVBoxLayout>

VibratoEffect::VibratoEffect(QWidget* parent) : EffectWidget(parent)
{
    buildUi();
}

QString VibratoEffect::effectTitle() const
{
    return tr("Vibrato (Tremolo)");
}

QJsonObject VibratoEffect::parametersToJson() const
{
    QJsonObject o;
    o.insert(QStringLiteral("speedHz"),
             m_speedTenthsHz ? double(m_speedTenthsHz->value()) / 10.0 : 3.5);
    o.insert(QStringLiteral("intensity"),
             m_intensityPercent ? double(m_intensityPercent->value()) / 100.0 : 0.35);
    return o;
}

void VibratoEffect::setParametersFromJson(const QJsonObject& o)
{
    blockSignalsForLoad(true);
    if (m_speedTenthsHz)
    {
        const double hz = o.value(QStringLiteral("speedHz")).toDouble(3.5);
        m_speedTenthsHz->setValue(
            std::clamp(int(std::lround(hz * 10.0)), m_speedTenthsHz->minimum(),
                       m_speedTenthsHz->maximum()));
    }
    if (m_intensityPercent)
    {
        const double d = o.value(QStringLiteral("intensity")).toDouble(0.35);
        m_intensityPercent->setValue(
            std::clamp(int(std::lround(d * 100.0)), m_intensityPercent->minimum(),
                       m_intensityPercent->maximum()));
    }
    blockSignalsForLoad(false);
    onControlChanged();
}

void VibratoEffect::onControlChanged()
{
    if (m_speedLabel && m_speedTenthsHz)
        m_speedLabel->setText(
            tr("Speed: %1 Hz").arg(QString::number(m_speedTenthsHz->value() / 10.0, 'f', 2)));
    if (m_intensityLabel && m_intensityPercent)
        m_intensityLabel->setText(tr("Intensity: %1%").arg(m_intensityPercent->value()));
    notifyParametersChanged();
}

void VibratoEffect::blockSignalsForLoad(bool block)
{
    const QList<QSlider*> sliders = {m_speedTenthsHz, m_intensityPercent};
    for (auto* s : sliders)
    {
        if (s)
            s->blockSignals(block);
    }
}

void VibratoEffect::buildUi()
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

    auto addRow = [&](int row, QLabel*& valueLbl, QSlider*& slider, int minV, int maxV, int initV,
                      const QString& tip)
    {
        valueLbl = new QLabel(this);
        slider = new QSlider(Qt::Horizontal, this);
        slider->setRange(minV, maxV);
        slider->setValue(initV);
        slider->setToolTip(tip);
        grid->addWidget(valueLbl, row, 0);
        grid->addWidget(slider, row, 1);
        connect(slider, &QSlider::valueChanged, this, &VibratoEffect::onControlChanged);
    };

    // Typical Fender ranges: ~0.5–10 Hz, intensity up to full cutout.
    addRow(0, m_speedLabel, m_speedTenthsHz, 5, 120, 35,
           tr("Modulation speed (Hz ×10, e.g. 35 = 3.5 Hz)"));
    addRow(1, m_intensityLabel, m_intensityPercent, 0, 100, 35,
           tr("Depth of volume modulation (0% subtle shimmer, 100% full chop)"));

    root->addLayout(grid);
    onControlChanged();
}

