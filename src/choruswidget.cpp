#include "choruswidget.h"

#include <algorithm>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QSlider>
#include <QVBoxLayout>

ChorusWidget::ChorusWidget(QWidget* parent) : EffectWidget(parent)
{
    buildUi();
}

QString ChorusWidget::effectTitle() const
{
    return tr("Chorus");
}

QJsonObject ChorusWidget::parametersToJson() const
{
    QJsonObject o;
    o.insert(QStringLiteral("effectLevel"),
             m_effectPercent ? double(m_effectPercent->value()) / 100.0 : 0.4);
    o.insert(QStringLiteral("rateHz"),
             m_rateTenths ? double(m_rateTenths->value()) / 10.0 : 0.45);
    o.insert(QStringLiteral("depthMs"),
             m_depthTenths ? double(m_depthTenths->value()) / 10.0 : 4.5);
    o.insert(QStringLiteral("eqLowDb"),
             m_eqLowTenths ? double(m_eqLowTenths->value()) / 10.0 : 0.0);
    o.insert(QStringLiteral("eqHighDb"),
             m_eqHighTenths ? double(m_eqHighTenths->value()) / 10.0 : 0.0);
    return o;
}

void ChorusWidget::setParametersFromJson(const QJsonObject& o)
{
    blockSignalsForLoad(true);
    if (m_effectPercent)
        m_effectPercent->setValue(int(std::lround(
            o.value(QStringLiteral("effectLevel")).toDouble(0.4) * 100.0)));
    if (m_rateTenths)
    {
        const double hz = o.value(QStringLiteral("rateHz")).toDouble(0.45);
        m_rateTenths->setValue(
            std::clamp(int(std::lround(hz * 10.0)), m_rateTenths->minimum(),
                       m_rateTenths->maximum()));
    }
    if (m_depthTenths)
    {
        const double ms = o.value(QStringLiteral("depthMs")).toDouble(4.5);
        m_depthTenths->setValue(
            std::clamp(int(std::lround(ms * 10.0)), m_depthTenths->minimum(),
                       m_depthTenths->maximum()));
    }
    if (m_eqLowTenths)
    {
        const double db = o.value(QStringLiteral("eqLowDb")).toDouble(0.0);
        m_eqLowTenths->setValue(
            std::clamp(int(std::lround(db * 10.0)), m_eqLowTenths->minimum(),
                       m_eqLowTenths->maximum()));
    }
    if (m_eqHighTenths)
    {
        const double db = o.value(QStringLiteral("eqHighDb")).toDouble(0.0);
        m_eqHighTenths->setValue(
            std::clamp(int(std::lround(db * 10.0)), m_eqHighTenths->minimum(),
                       m_eqHighTenths->maximum()));
    }
    blockSignalsForLoad(false);
    onControlChanged();
}

void ChorusWidget::onControlChanged()
{
    if (m_effectLabel && m_effectPercent)
        m_effectLabel->setText(tr("Effect level: %1%").arg(m_effectPercent->value()));
    if (m_rateLabel && m_rateTenths)
        m_rateLabel->setText(
            tr("Rate: %1 Hz").arg(QString::number(m_rateTenths->value() / 10.0, 'f', 2)));
    if (m_depthLabel && m_depthTenths)
        m_depthLabel->setText(
            tr("Depth: %1 ms").arg(QString::number(m_depthTenths->value() / 10.0, 'f', 1)));
    if (m_eqLowLabel && m_eqLowTenths)
        m_eqLowLabel->setText(
            tr("EQ low: %1 dB").arg(QString::number(m_eqLowTenths->value() / 10.0, 'f', 1)));
    if (m_eqHighLabel && m_eqHighTenths)
        m_eqHighLabel->setText(
            tr("EQ high: %1 dB").arg(QString::number(m_eqHighTenths->value() / 10.0, 'f', 1)));
    notifyParametersChanged();
}

void ChorusWidget::blockSignalsForLoad(bool block)
{
    const QList<QSlider*> sliders = {m_effectPercent, m_rateTenths, m_depthTenths,
                                     m_eqLowTenths, m_eqHighTenths};
    for (auto* s : sliders)
    {
        if (s)
            s->blockSignals(block);
    }
}

void ChorusWidget::buildUi()
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
        connect(slider, &QSlider::valueChanged, this, &ChorusWidget::onControlChanged);
    };

    addRow(0, m_effectLabel, m_effectPercent, 0, 100, 40,
           tr("Wet chorus level"));
    addRow(1, m_rateLabel, m_rateTenths, 1, 50, 5,
           tr("LFO rate (Hz ×10)"));
    addRow(2, m_depthLabel, m_depthTenths, 2, 150, 45,
           tr("Modulation depth (ms ×10)"));
    addRow(3, m_eqLowLabel, m_eqLowTenths, -120, 120, 0,
           tr("Low-frequency emphasis on wet signal (dB ×10)"));
    addRow(4, m_eqHighLabel, m_eqHighTenths, -120, 120, 0,
           tr("High-frequency emphasis on wet signal (dB ×10)"));

    root->addLayout(grid);
    onControlChanged();
}
