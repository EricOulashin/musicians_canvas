#ifndef FLANGEWIDGET_H
#define FLANGEWIDGET_H

#include "effectwidget.h"

class QSlider;
class QLabel;

/// Stereo flanger (short modulated delay + feedback) for insert-effect recording.
class FlangeWidget final : public EffectWidget
{
    Q_OBJECT
public:
    explicit FlangeWidget(QWidget* parent = nullptr);

    QString effectTypeId() const override
    {
        return QStringLiteral("flanger");
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
    QSlider* m_manualTenths = nullptr;   // ms ×10 (0.3–12 ms)
    QSlider* m_rateTenths = nullptr;     // Hz ×10
    QSlider* m_depthTenths = nullptr;    // ms ×10
    QSlider* m_feedbackPercent = nullptr; // -95 … +95

    QLabel* m_effectLabel = nullptr;
    QLabel* m_manualLabel = nullptr;
    QLabel* m_rateLabel = nullptr;
    QLabel* m_depthLabel = nullptr;
    QLabel* m_feedbackLabel = nullptr;
};

#endif
