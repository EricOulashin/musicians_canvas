#ifndef EFFECTSDIALOG_H
#define EFFECTSDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QJsonArray>
#include <QPoint>
#include <QVector>

class QPushButton;
class QScrollArea;
class QVBoxLayout;
class QWidget;
class TrackWidget;
class EffectWidget;
class QEvent;

class EffectsDialog final : public QDialog
{
    Q_OBJECT
public:
    explicit EffectsDialog(TrackWidget* trackWidget, QWidget* parent = nullptr);

protected:
    void reject() override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void onAddEffect();
    void onRemoveEffect(EffectWidget* w);
    void onReorderDrag(EffectWidget* source);
    void syncLive();

private:
    void rebuildFromChain();
    void relayoutChain();
    EffectWidget* createEffectWidget(const QString& type);
    EffectWidget* effectWidgetAtDropPos(const QPoint& localPos);
    int insertionIndexFromPos(const QPoint& localPos, EffectWidget* dragged);
    void handleReorderDrop(QDropEvent* event, const QPoint& posInChain);

    TrackWidget*      m_trackWidget = nullptr;
    QJsonArray        m_initialChain;
    QScrollArea*      m_scroll = nullptr;
    QWidget*          m_chainContainer = nullptr;
    QVBoxLayout*      m_chainLayout = nullptr;
    QPushButton*      m_addButton = nullptr;
    QVector<EffectWidget*> m_widgets;
    bool              m_suspendSync = false;
};

#endif
