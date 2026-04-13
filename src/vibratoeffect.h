#ifndef VIBRATOEFFECT_H
#define VIBRATOEFFECT_H

#include "effectwidget.h"

class QLabel;
class QSlider;

/// Fender-style "Vibrato" (tremolo): periodic volume modulation over time.
class VibratoEffect final : public EffectWidget
{
    Q_OBJECT
public:
    explicit VibratoEffect(QWidget* parent = nullptr);

    QString effectTypeId() const override
    {
        return QStringLiteral("vibrato");
    }
    QString effectTitle() const override;
    QJsonObject parametersToJson() const override;
    void setParametersFromJson(const QJsonObject& o) override;

private slots:
    void onControlChanged();

private:
    void buildUi();
    void blockSignalsForLoad(bool block);

    // Fender amp labels: Speed + Intensity (Depth)
    QSlider* m_speedTenthsHz = nullptr;     // 0.1 Hz units
    QSlider* m_intensityPercent = nullptr;  // 0..100%

    QLabel* m_speedLabel = nullptr;
    QLabel* m_intensityLabel = nullptr;
};

#endif

