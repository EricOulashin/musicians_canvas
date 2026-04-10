#include "overdrivedistortionwidget.h"

#include <algorithm>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QSlider>
#include <QVBoxLayout>

OverdriveDistortionWidget::OverdriveDistortionWidget(QWidget* parent) : EffectWidget(parent)
{
    buildUi();
}

QString OverdriveDistortionWidget::effectTitle() const
{
    return tr("Overdrive / distortion");
}

QJsonObject OverdriveDistortionWidget::parametersToJson() const
{
    QJsonObject o;
    o.insert(QStringLiteral("drive"),
             m_drivePercent ? double(m_drivePercent->value()) / 100.0 : 0.45);
    o.insert(QStringLiteral("color"),
             m_colorPercent ? double(m_colorPercent->value()) / 100.0 : 0.5);
    o.insert(QStringLiteral("tone"),
             m_tonePercent ? double(m_tonePercent->value()) / 100.0 : 0.5);
    o.insert(QStringLiteral("effectLevel"),
             m_effectPercent ? double(m_effectPercent->value()) / 100.0 : 0.65);
    return o;
}

void OverdriveDistortionWidget::setParametersFromJson(const QJsonObject& o)
{
    blockSignalsForLoad(true);
    if (m_drivePercent)
        m_drivePercent->setValue(int(std::lround(
            o.value(QStringLiteral("drive")).toDouble(0.45) * 100.0)));
    if (m_colorPercent)
        m_colorPercent->setValue(int(std::lround(
            o.value(QStringLiteral("color")).toDouble(0.5) * 100.0)));
    if (m_tonePercent)
        m_tonePercent->setValue(int(std::lround(
            o.value(QStringLiteral("tone")).toDouble(0.5) * 100.0)));
    if (m_effectPercent)
        m_effectPercent->setValue(int(std::lround(
            o.value(QStringLiteral("effectLevel")).toDouble(0.65) * 100.0)));
    blockSignalsForLoad(false);
    onControlChanged();
}

void OverdriveDistortionWidget::onControlChanged()
{
    if (m_driveLabel && m_drivePercent)
        m_driveLabel->setText(tr("Drive: %1%").arg(m_drivePercent->value()));
    if (m_colorLabel && m_colorPercent)
        m_colorLabel->setText(
            m_colorPercent->value() < 42
                ? tr("Color: %1% (overdrive)").arg(m_colorPercent->value())
                : (m_colorPercent->value() > 58
                       ? tr("Color: %1% (distortion)").arg(m_colorPercent->value())
                       : tr("Color: %1% (blend)").arg(m_colorPercent->value())));
    if (m_toneLabel && m_tonePercent)
        m_toneLabel->setText(
            m_tonePercent->value() < 40
                ? tr("Tone: %1 (dark)").arg(m_tonePercent->value())
                : (m_tonePercent->value() > 65
                       ? tr("Tone: %1 (bright)").arg(m_tonePercent->value())
                       : tr("Tone: %1 (neutral)").arg(m_tonePercent->value())));
    if (m_effectLabel && m_effectPercent)
        m_effectLabel->setText(tr("Effect level: %1%").arg(m_effectPercent->value()));
    notifyParametersChanged();
}

void OverdriveDistortionWidget::blockSignalsForLoad(bool block)
{
    const QList<QSlider*> sliders = {m_drivePercent, m_colorPercent, m_tonePercent,
                                     m_effectPercent};
    for (auto* s : sliders)
    {
        if (s)
            s->blockSignals(block);
    }
}

void OverdriveDistortionWidget::buildUi()
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
        connect(slider, &QSlider::valueChanged, this, &OverdriveDistortionWidget::onControlChanged);
    };

    addRow(0, m_driveLabel, m_drivePercent, 0, 100, 45,
           tr("Input gain before clipping (similar to DRIVE on compact pedals)"));
    addRow(1, m_colorLabel, m_colorPercent, 0, 100, 50,
           tr("Blend between softer overdrive and harder distortion (similar to COLOR)"));
    addRow(2, m_toneLabel, m_tonePercent, 0, 100, 50,
           tr("Post-clipping brightness / darkness (similar to TONE)"));
    addRow(3, m_effectLabel, m_effectPercent, 0, 100, 65,
           tr("Wet/dry mix of the processed signal (similar to LEVEL)"));

    root->addLayout(grid);
    onControlChanged();
}
