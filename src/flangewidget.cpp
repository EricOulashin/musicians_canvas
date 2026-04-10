#include "flangewidget.h"

#include <algorithm>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QSlider>
#include <QVBoxLayout>

FlangeWidget::FlangeWidget(QWidget* parent) : EffectWidget(parent)
{
    buildUi();
}

QString FlangeWidget::effectTitle() const
{
    return tr("Flanger");
}

QJsonObject FlangeWidget::parametersToJson() const
{
    QJsonObject o;
    o.insert(QStringLiteral("effectLevel"),
             m_effectPercent ? double(m_effectPercent->value()) / 100.0 : 0.45);
    o.insert(QStringLiteral("manualDelayMs"),
             m_manualTenths ? double(m_manualTenths->value()) / 10.0 : 2.0);
    o.insert(QStringLiteral("rateHz"),
             m_rateTenths ? double(m_rateTenths->value()) / 10.0 : 0.42);
    o.insert(QStringLiteral("depthMs"),
             m_depthTenths ? double(m_depthTenths->value()) / 10.0 : 2.0);
    o.insert(QStringLiteral("feedbackPercent"),
             m_feedbackPercent ? double(m_feedbackPercent->value()) : 35.0);
    return o;
}

void FlangeWidget::setParametersFromJson(const QJsonObject& o)
{
    blockSignalsForLoad(true);
    if (m_effectPercent)
        m_effectPercent->setValue(int(std::lround(
            o.value(QStringLiteral("effectLevel")).toDouble(0.45) * 100.0)));
    if (m_manualTenths)
    {
        const double ms = o.value(QStringLiteral("manualDelayMs")).toDouble(2.0);
        m_manualTenths->setValue(
            std::clamp(int(std::lround(ms * 10.0)), m_manualTenths->minimum(),
                       m_manualTenths->maximum()));
    }
    if (m_rateTenths)
    {
        const double hz = o.value(QStringLiteral("rateHz")).toDouble(0.42);
        m_rateTenths->setValue(
            std::clamp(int(std::lround(hz * 10.0)), m_rateTenths->minimum(),
                       m_rateTenths->maximum()));
    }
    if (m_depthTenths)
    {
        const double ms = o.value(QStringLiteral("depthMs")).toDouble(2.0);
        m_depthTenths->setValue(
            std::clamp(int(std::lround(ms * 10.0)), m_depthTenths->minimum(),
                       m_depthTenths->maximum()));
    }
    if (m_feedbackPercent)
    {
        const int fb = int(std::lround(o.value(QStringLiteral("feedbackPercent")).toDouble(35.0)));
        m_feedbackPercent->setValue(
            std::clamp(fb, m_feedbackPercent->minimum(), m_feedbackPercent->maximum()));
    }
    blockSignalsForLoad(false);
    onControlChanged();
}

void FlangeWidget::onControlChanged()
{
    if (m_effectLabel && m_effectPercent)
        m_effectLabel->setText(tr("Effect level: %1%").arg(m_effectPercent->value()));
    if (m_manualLabel && m_manualTenths)
        m_manualLabel->setText(
            tr("Manual (delay): %1 ms")
                .arg(QString::number(m_manualTenths->value() / 10.0, 'f', 1)));
    if (m_rateLabel && m_rateTenths)
        m_rateLabel->setText(
            tr("Rate: %1 Hz").arg(QString::number(m_rateTenths->value() / 10.0, 'f', 2)));
    if (m_depthLabel && m_depthTenths)
        m_depthLabel->setText(
            tr("Depth: %1 ms").arg(QString::number(m_depthTenths->value() / 10.0, 'f', 1)));
    if (m_feedbackLabel && m_feedbackPercent)
        m_feedbackLabel->setText(tr("Feedback: %1%").arg(m_feedbackPercent->value()));
    notifyParametersChanged();
}

void FlangeWidget::blockSignalsForLoad(bool block)
{
    const QList<QSlider*> sliders = {m_effectPercent, m_manualTenths, m_rateTenths,
                                     m_depthTenths, m_feedbackPercent};
    for (auto* s : sliders)
    {
        if (s)
            s->blockSignals(block);
    }
}

void FlangeWidget::buildUi()
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
        connect(slider, &QSlider::valueChanged, this, &FlangeWidget::onControlChanged);
    };

    addRow(0, m_effectLabel, m_effectPercent, 0, 100, 45,
           tr("Wet blend of the flanged signal (dry + modulated delay)"));
    addRow(1, m_manualLabel, m_manualTenths, 3, 120, 20,
           tr("Base delay / comb offset (ms ×10), like a stompbox Manual control"));
    addRow(2, m_rateLabel, m_rateTenths, 1, 50, 4,
           tr("LFO rate (Hz ×10)"));
    addRow(3, m_depthLabel, m_depthTenths, 1, 80, 20,
           tr("LFO sweep depth (ms ×10)"));
    addRow(4, m_feedbackLabel, m_feedbackPercent, -95, 95, 35,
           tr("Regeneration / resonance (−100…+100); negative inverts the feedback polarity"));

    root->addLayout(grid);
    onControlChanged();
}
