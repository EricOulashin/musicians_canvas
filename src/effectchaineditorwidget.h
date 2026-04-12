#ifndef EFFECTCHAINEDITORWIDGET_H
#define EFFECTCHAINEDITORWIDGET_H

#include <QJsonArray>
#include <QPoint>
#include <QVector>
#include <QWidget>

class QPushButton;
class QScrollArea;
class QVBoxLayout;
class EffectWidget;
class EffectChainDragOverlay;
class QEvent;
class QDropEvent;

/// Scrollable add / remove / reorder UI for an audio effect chain (same widgets as track effects).
class EffectChainEditorWidget final : public QWidget
{
    Q_OBJECT
public:
    explicit EffectChainEditorWidget(QWidget* parent = nullptr);

    void setChain(const QJsonArray& chain);
    [[nodiscard]] QJsonArray chain() const;

    /// When false, parameter and structure updates do not emit chainChanged (loading).
    void setEmitChanges(bool on);

signals:
    void chainChanged();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void onAddEffect();
    void onRemoveEffect(EffectWidget* w);
    void onReorderDrag(EffectWidget* source);
    void emitChainIfNeeded();

private:
    void rebuildFromStoredChain();
    void relayoutChain();
    EffectWidget* createEffectWidget(const QString& type);
    EffectWidget* effectWidgetAtDropPos(const QPoint& localPos) const;
    int dropIndexFromPos(const QPoint& localPos) const;
    int dropLineYForIndex(int insertIndex) const;
    void beginReorderVisuals(EffectWidget* source);
    void endReorderVisuals();
    void updateReorderOverlay(const QPoint& localInChain);
    void handleReorderDrop(QDropEvent* event, const QPoint& posInChain);

    QJsonArray        m_storedChain;
    QPushButton*      m_addButton = nullptr;
    QScrollArea*      m_scroll = nullptr;
    QWidget*          m_chainContainer = nullptr;
    QVBoxLayout*      m_chainLayout = nullptr;
    QVector<EffectWidget*> m_widgets;
    bool              m_emitChanges = true;

    EffectWidget*            m_dragSource = nullptr;
    EffectChainDragOverlay*  m_dropOverlay = nullptr;
    bool                     m_reorderDragActive = false;
};

#endif
