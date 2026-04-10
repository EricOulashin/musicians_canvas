#ifndef AMPCABINETMODELWIDGET_H
#define AMPCABINETMODELWIDGET_H

#include "effectwidget.h"

class QComboBox;
class QSlider;
class QLabel;

/// Guitar amp + cabinet modeling: amp voicing, cabinet tone shaping, EQ, and wet/dry mix.
class AmpAndCabinetModelWidget final : public EffectWidget
{
    Q_OBJECT
public:
    explicit AmpAndCabinetModelWidget(QWidget* parent = nullptr);

    QString effectTypeId() const override
    {
        return QStringLiteral("amp_cabinet");
    }
    QString effectTitle() const override;
    QJsonObject parametersToJson() const override;
    void setParametersFromJson(const QJsonObject& o) override;

private slots:
    void onControlChanged();

private:
    void buildUi();
    void blockSignalsForLoad(bool block);

    QComboBox* m_ampCombo = nullptr;
    QComboBox* m_cabCombo = nullptr;
    QSlider*   m_gainPercent = nullptr;
    QSlider*   m_bassPercent = nullptr;
    QSlider*   m_midPercent = nullptr;
    QSlider*   m_treblePercent = nullptr;
    QSlider*   m_airPercent = nullptr;
    QSlider*   m_effectPercent = nullptr;

    QLabel* m_ampLabel = nullptr;
    QLabel* m_cabLabel = nullptr;
    QLabel* m_gainLabel = nullptr;
    QLabel* m_bassLabel = nullptr;
    QLabel* m_midLabel = nullptr;
    QLabel* m_trebleLabel = nullptr;
    QLabel* m_airLabel = nullptr;
    QLabel* m_effectLabel = nullptr;
};

#endif
