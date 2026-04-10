#ifndef CHORUSWIDGET_H
#define CHORUSWIDGET_H

#include "effectwidget.h"

class QSlider;
class QLabel;

/// Stereo chorus / doubler for insert-effect recording (level, rate, depth, EQ).
class ChorusWidget final : public EffectWidget
{
    Q_OBJECT
public:
    explicit ChorusWidget(QWidget* parent = nullptr);

    QString effectTypeId() const override
    {
        return QStringLiteral("chorus");
    }
    QString effectTitle() const override;
    QJsonObject parametersToJson() const override;
    void setParametersFromJson(const QJsonObject& o) override;

private slots:
    void onControlChanged();

private:
    void buildUi();
    void blockSignalsForLoad(bool block);

    QSlider* m_effectPercent = nullptr;
    QSlider* m_rateTenths = nullptr;
    QSlider* m_depthTenths = nullptr;  // 0.1 ms steps
    QSlider* m_eqLowTenths = nullptr;  // dB ×10, -120..+120 = -12..+12
    QSlider* m_eqHighTenths = nullptr;

    QLabel* m_effectLabel = nullptr;
    QLabel* m_rateLabel = nullptr;
    QLabel* m_depthLabel = nullptr;
    QLabel* m_eqLowLabel = nullptr;
    QLabel* m_eqHighLabel = nullptr;
};

#endif
