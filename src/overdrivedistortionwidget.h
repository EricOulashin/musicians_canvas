#ifndef OVERDRIVEDISTORTIONWIDGET_H
#define OVERDRIVEDISTORTIONWIDGET_H

#include "effectwidget.h"

class QSlider;
class QLabel;

/// Overdrive / distortion blend inspired by dual-stage pedals (e.g. OD/DS color blend, drive, tone, level).
class OverdriveDistortionWidget final : public EffectWidget
{
    Q_OBJECT
public:
    explicit OverdriveDistortionWidget(QWidget* parent = nullptr);

    QString effectTypeId() const override
    {
        return QStringLiteral("overdrive_distortion");
    }
    QString effectTitle() const override;
    QJsonObject parametersToJson() const override;
    void setParametersFromJson(const QJsonObject& o) override;

private slots:
    void onControlChanged();

private:
    void buildUi();
    void blockSignalsForLoad(bool block);

    QSlider* m_drivePercent = nullptr;
    QSlider* m_colorPercent = nullptr;
    QSlider* m_tonePercent = nullptr;
    QSlider* m_effectPercent = nullptr;

    QLabel* m_driveLabel = nullptr;
    QLabel* m_colorLabel = nullptr;
    QLabel* m_toneLabel = nullptr;
    QLabel* m_effectLabel = nullptr;
};

#endif
