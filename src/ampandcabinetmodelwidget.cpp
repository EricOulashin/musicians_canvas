#include "ampandcabinetmodelwidget.h"

#include <algorithm>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QSlider>
#include <QVBoxLayout>

AmpAndCabinetModelWidget::AmpAndCabinetModelWidget(QWidget* parent) : EffectWidget(parent)
{
    buildUi();
}

QString AmpAndCabinetModelWidget::effectTitle() const
{
    return tr("Amp & cabinet");
}

QJsonObject AmpAndCabinetModelWidget::parametersToJson() const
{
    QJsonObject o;
    o.insert(QStringLiteral("ampModel"),
             m_ampCombo ? m_ampCombo->currentIndex() : 2);
    o.insert(QStringLiteral("cabinetModel"),
             m_cabCombo ? m_cabCombo->currentIndex() : 1);
    o.insert(QStringLiteral("gain"),
             m_gainPercent ? double(m_gainPercent->value()) / 100.0 : 0.45);
    o.insert(QStringLiteral("bass"),
             m_bassPercent ? double(m_bassPercent->value()) / 100.0 : 0.5);
    o.insert(QStringLiteral("mid"),
             m_midPercent ? double(m_midPercent->value()) / 100.0 : 0.5);
    o.insert(QStringLiteral("treble"),
             m_treblePercent ? double(m_treblePercent->value()) / 100.0 : 0.5);
    o.insert(QStringLiteral("air"),
             m_airPercent ? double(m_airPercent->value()) / 100.0 : 0.35);
    o.insert(QStringLiteral("effectLevel"),
             m_effectPercent ? double(m_effectPercent->value()) / 100.0 : 0.75);
    return o;
}

void AmpAndCabinetModelWidget::setParametersFromJson(const QJsonObject& o)
{
    blockSignalsForLoad(true);
    if (m_ampCombo)
        m_ampCombo->setCurrentIndex(
            std::clamp(o.value(QStringLiteral("ampModel")).toInt(2), 0,
                       m_ampCombo->count() - 1));
    if (m_cabCombo)
        m_cabCombo->setCurrentIndex(
            std::clamp(o.value(QStringLiteral("cabinetModel")).toInt(1), 0,
                       m_cabCombo->count() - 1));
    if (m_gainPercent)
        m_gainPercent->setValue(int(std::lround(
            o.value(QStringLiteral("gain")).toDouble(0.45) * 100.0)));
    if (m_bassPercent)
        m_bassPercent->setValue(int(std::lround(
            o.value(QStringLiteral("bass")).toDouble(0.5) * 100.0)));
    if (m_midPercent)
        m_midPercent->setValue(int(std::lround(
            o.value(QStringLiteral("mid")).toDouble(0.5) * 100.0)));
    if (m_treblePercent)
        m_treblePercent->setValue(int(std::lround(
            o.value(QStringLiteral("treble")).toDouble(0.5) * 100.0)));
    if (m_airPercent)
        m_airPercent->setValue(int(std::lround(
            o.value(QStringLiteral("air")).toDouble(0.35) * 100.0)));
    if (m_effectPercent)
        m_effectPercent->setValue(int(std::lround(
            o.value(QStringLiteral("effectLevel")).toDouble(0.75) * 100.0)));
    blockSignalsForLoad(false);
    onControlChanged();
}

void AmpAndCabinetModelWidget::onControlChanged()
{
    if (m_gainLabel && m_gainPercent)
        m_gainLabel->setText(tr("Gain: %1%").arg(m_gainPercent->value()));
    if (m_bassLabel && m_bassPercent)
        m_bassLabel->setText(tr("Bass: %1%").arg(m_bassPercent->value()));
    if (m_midLabel && m_midPercent)
        m_midLabel->setText(tr("Mid: %1%").arg(m_midPercent->value()));
    if (m_trebleLabel && m_treblePercent)
        m_trebleLabel->setText(tr("Treble: %1%").arg(m_treblePercent->value()));
    if (m_airLabel && m_airPercent)
        m_airLabel->setText(tr("Air / presence: %1%").arg(m_airPercent->value()));
    if (m_effectLabel && m_effectPercent)
        m_effectLabel->setText(tr("Effect level: %1%").arg(m_effectPercent->value()));
    notifyParametersChanged();
}

void AmpAndCabinetModelWidget::blockSignalsForLoad(bool block)
{
    const QList<QSlider*> sliders = {m_gainPercent, m_bassPercent, m_midPercent,
                                     m_treblePercent, m_airPercent, m_effectPercent};
    for (auto* s : sliders)
    {
        if (s)
            s->blockSignals(block);
    }
    if (m_ampCombo)
        m_ampCombo->blockSignals(block);
    if (m_cabCombo)
        m_cabCombo->blockSignals(block);
}

void AmpAndCabinetModelWidget::buildUi()
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

    int row = 0;
    m_ampLabel = new QLabel(tr("Amp type:"), this);
    m_ampCombo = new QComboBox(this);
    m_ampCombo->addItem(tr("Clean / warm"), 0);
    m_ampCombo->addItem(tr("Tweed / vintage"), 1);
    m_ampCombo->addItem(tr("Classic crunch"), 2);
    m_ampCombo->addItem(tr("British lead"), 3);
    m_ampCombo->addItem(tr("Modern high-gain"), 4);
    m_ampCombo->addItem(tr("Brown / pushed"), 5);
    m_ampCombo->setToolTip(tr("Amp voicing curve (inspired by multi-model amp pedals)"));
    grid->addWidget(m_ampLabel, row, 0);
    grid->addWidget(m_ampCombo, row, 1);
    connect(m_ampCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &AmpAndCabinetModelWidget::onControlChanged);
    ++row;

    m_cabLabel = new QLabel(tr("Cabinet:"), this);
    m_cabCombo = new QComboBox(this);
    m_cabCombo->addItem(tr("Small combo"), 0);
    m_cabCombo->addItem(tr("1×12 open"), 1);
    m_cabCombo->addItem(tr("2×12 closed"), 2);
    m_cabCombo->addItem(tr("4×12 slant"), 3);
    m_cabCombo->addItem(tr("4×12 straight"), 4);
    m_cabCombo->addItem(tr("Direct / minimal"), 5);
    m_cabCombo->setToolTip(tr("Speaker/cab rolloff and resonance (IR-style approximation)"));
    grid->addWidget(m_cabLabel, row, 0);
    grid->addWidget(m_cabCombo, row, 1);
    connect(m_cabCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &AmpAndCabinetModelWidget::onControlChanged);
    ++row;

    auto addSlider = [&](QLabel*& lbl, QSlider*& s, int init, const QString& tip)
    {
        lbl = new QLabel(this);
        s = new QSlider(Qt::Horizontal, this);
        s->setRange(0, 100);
        s->setValue(init);
        s->setToolTip(tip);
        grid->addWidget(lbl, row, 0);
        grid->addWidget(s, row, 1);
        connect(s, &QSlider::valueChanged, this, &AmpAndCabinetModelWidget::onControlChanged);
        ++row;
    };

    addSlider(m_gainLabel, m_gainPercent, 45,
              tr("Preamp drive before the amp voicing stage"));
    addSlider(m_bassLabel, m_bassPercent, 50, tr("Low-frequency response"));
    addSlider(m_midLabel, m_midPercent, 50, tr("Midrange response"));
    addSlider(m_trebleLabel, m_treblePercent, 50, tr("High-frequency response"));
    addSlider(m_airLabel, m_airPercent, 35,
              tr("Very high-frequency openness (similar to AIR / presence on amp sims)"));
    addSlider(m_effectLabel, m_effectPercent, 75,
              tr("Wet/dry mix of the modeled amp and cabinet"));

    root->addLayout(grid);
    m_ampCombo->setCurrentIndex(2);
    m_cabCombo->setCurrentIndex(1);
    onControlChanged();
}
