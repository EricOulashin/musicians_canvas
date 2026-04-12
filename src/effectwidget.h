#ifndef EFFECTWIDGET_H
#define EFFECTWIDGET_H

#include <QFrame>
#include <QEvent>
#include <QJsonObject>
#include <QPoint>
#include <QString>

class QHBoxLayout;
class QLabel;
class QPushButton;
class QWidget;

/// Abstract base for effect configuration UIs. Subclasses expose parameters as JSON and are
/// ordered in EffectsDialog for an insert-effect recording chain.
class EffectWidget : public QFrame
{
    Q_OBJECT
public:
    explicit EffectWidget(QWidget* parent = nullptr);
    ~EffectWidget() override = default;

    [[nodiscard]] virtual QString effectTypeId() const = 0;
    [[nodiscard]] virtual QString effectTitle() const = 0;
    [[nodiscard]] virtual QJsonObject parametersToJson() const = 0;
    virtual void setParametersFromJson(const QJsonObject& o) = 0;

    /// Serialize one chain element: { "type": ..., "params": ... }
    [[nodiscard]] QJsonObject toChainEntry() const;

    static void applyCloseButtonStyle(QPushButton* btn);

    /// Normalized float (\p x in roughly ±1.0) soft-limited toward full scale before int16 conversion.
    [[nodiscard]] static float softLimitFloatSampleForInt16Pcm(float x);
    /// Soft limit plus hard clamp — use as the last step before scaling to int16 PCM.
    [[nodiscard]] static float guardFloatSampleForInt16Pcm(float x);

signals:
    void removeRequested(EffectWidget* self);
    void parametersChanged(EffectWidget* self);
    /// Emitted when the user drags from the title bar to reorder in EffectsDialog.
    void reorderDragStarted(EffectWidget* self);

protected:
    void setupHeader();
    void notifyParametersChanged();
    bool eventFilter(QObject* watched, QEvent* event) override;

    QHBoxLayout* m_headerLayout = nullptr;
    QLabel*      m_titleLabel = nullptr;
    QWidget*     m_dragHandle = nullptr;
    QPushButton* m_closeButton = nullptr;

private:
    QPoint m_dragPressPos;
    bool   m_dragMouseDown = false;
};

#endif
