#ifndef REVERBWIDGET_H
#define REVERBWIDGET_H

#include "effectwidget.h"

class QSlider;
class QLabel;

/// Stereo reverb for insert-effect recording (decay / pre-delay / mix / tone / modulation).
class ReverbWidget final : public EffectWidget
{
    Q_OBJECT
public:
    explicit ReverbWidget(QWidget* parent = nullptr);

    QString effectTypeId() const override
    {
        return QStringLiteral("reverb");
    }
    QString effectTitle() const override;
    QJsonObject parametersToJson() const override;
    void setParametersFromJson(const QJsonObject& o) override;

private slots:
    void onControlChanged();

private:
    void buildUi();
    void blockSignalsForLoad(bool block);

    QSlider* m_preDelayMs = nullptr;
    QSlider* m_decayTenths = nullptr;  // 0.1s resolution, 1–300 = 0.1–30s
    QSlider* m_mixPercent = nullptr;
    QSlider* m_tonePercent = nullptr;
    QSlider* m_modDepthPercent = nullptr;
    QSlider* m_modRateTenths = nullptr;  // 0.1 Hz resolution

    QLabel* m_preDelayLabel = nullptr;
    QLabel* m_decayLabel = nullptr;
    QLabel* m_mixLabel = nullptr;
    QLabel* m_toneLabel = nullptr;
    QLabel* m_modDepthLabel = nullptr;
    QLabel* m_modRateLabel = nullptr;
};

#endif
