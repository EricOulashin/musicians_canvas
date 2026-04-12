#include "effectchaineditorwidget.h"

#include "ampandcabinetmodelwidget.h"
#include "choruswidget.h"
#include "effectchaindragoverlay.h"
#include "effectwidget.h"
#include "flangewidget.h"
#include "overdrivedistortionwidget.h"
#include "reverbwidget.h"

#include <QComboBox>
#include <QCursor>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QMessageBox>
#include <QMimeData>
#include <QPointer>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

namespace
{
constexpr char kMimeEffectPtr[] = "application/x-mc-effect-ptr";

QPoint dragPointInChain(QObject* watched, QEvent* event, QWidget* chain)
{
    auto* w = qobject_cast<QWidget*>(watched);
    if (!chain)
        return {};
    if (!w)
        return chain->mapFromGlobal(QCursor::pos());

    switch (event->type())
    {
        case QEvent::DragMove:
            return chain->mapFrom(w, static_cast<QDragMoveEvent*>(event)->position().toPoint());
        case QEvent::DragEnter:
            return chain->mapFrom(w, static_cast<QDragEnterEvent*>(event)->position().toPoint());
        default:
            break;
    }
    return chain->mapFromGlobal(QCursor::pos());
}
}  // namespace

EffectChainEditorWidget::EffectChainEditorWidget(QWidget* parent) : QWidget(parent)
{
    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(8);

    auto* topRow = new QHBoxLayout();
    m_addButton = new QPushButton(tr("Add effect…"), this);
    connect(m_addButton, &QPushButton::clicked, this, &EffectChainEditorWidget::onAddEffect);
    topRow->addWidget(m_addButton);
    topRow->addStretch();
    outer->addLayout(topRow);

    m_scroll = new QScrollArea(this);
    m_scroll->setWidgetResizable(true);
    m_scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_chainContainer = new QWidget(m_scroll);
    m_chainLayout = new QVBoxLayout(m_chainContainer);
    m_chainLayout->setContentsMargins(0, 0, 0, 0);
    m_chainLayout->setSpacing(8);

    m_scroll->setWidget(m_chainContainer);
    outer->addWidget(m_scroll, 1);

    m_chainContainer->setAcceptDrops(true);
    m_chainContainer->installEventFilter(this);
    m_scroll->installEventFilter(this);
    if (QWidget* vp = m_scroll->viewport())
        vp->installEventFilter(this);
    installEventFilter(this);
}

void EffectChainEditorWidget::setEmitChanges(bool on)
{
    m_emitChanges = on;
}

void EffectChainEditorWidget::setChain(const QJsonArray& chain)
{
    m_storedChain = chain;
    rebuildFromStoredChain();
}

QJsonArray EffectChainEditorWidget::chain() const
{
    QJsonArray a;
    for (auto* w : std::as_const(m_widgets))
        a.append(w->toChainEntry());
    return a;
}

void EffectChainEditorWidget::emitChainIfNeeded()
{
    if (m_emitChanges)
        emit chainChanged();
}

EffectWidget* EffectChainEditorWidget::createEffectWidget(const QString& type)
{
    EffectWidget* w = nullptr;
    if (type == QStringLiteral("reverb"))
        w = new ReverbWidget(m_chainContainer);
    else if (type == QStringLiteral("chorus"))
        w = new ChorusWidget(m_chainContainer);
    else if (type == QStringLiteral("flanger"))
        w = new FlangeWidget(m_chainContainer);
    else if (type == QStringLiteral("overdrive_distortion"))
        w = new OverdriveDistortionWidget(m_chainContainer);
    else if (type == QStringLiteral("amp_cabinet"))
        w = new AmpAndCabinetModelWidget(m_chainContainer);

    if (!w)
        return nullptr;

    w->setAcceptDrops(true);
    w->installEventFilter(this);

    connect(w, &EffectWidget::removeRequested, this, &EffectChainEditorWidget::onRemoveEffect);
    connect(w, &EffectWidget::parametersChanged, this, &EffectChainEditorWidget::emitChainIfNeeded);
    connect(w, &EffectWidget::reorderDragStarted, this, &EffectChainEditorWidget::onReorderDrag);
    return w;
}

void EffectChainEditorWidget::rebuildFromStoredChain()
{
    const bool prevEmit = m_emitChanges;
    m_emitChanges       = false;
    for (auto* w : std::as_const(m_widgets))
    {
        w->removeEventFilter(this);
        w->setParent(nullptr);
        w->deleteLater();
    }
    m_widgets.clear();

    for (const QJsonValue& v : std::as_const(m_storedChain))
    {
        if (!v.isObject())
            continue;
        const QJsonObject o      = v.toObject();
        const QString     type   = o.value(QStringLiteral("type")).toString();
        const QJsonObject params = o.value(QStringLiteral("params")).toObject();
        EffectWidget* w = createEffectWidget(type);
        if (!w)
            continue;
        w->setParametersFromJson(params);
        m_widgets.append(w);
    }
    relayoutChain();
    m_emitChanges = prevEmit;
}

void EffectChainEditorWidget::relayoutChain()
{
    while (m_chainLayout->count() > 0)
    {
        QLayoutItem* it = m_chainLayout->takeAt(0);
        delete it;
    }
    for (auto* w : std::as_const(m_widgets))
        m_chainLayout->addWidget(w);
    m_chainLayout->addStretch(1);
}

void EffectChainEditorWidget::onAddEffect()
{
    QWidget* host = window() ? window() : this;
    QDialog chooser(host);
    chooser.setWindowTitle(tr("Add effect"));
    auto* vl = new QVBoxLayout(&chooser);
    vl->addWidget(new QLabel(tr("Choose an effect:"), &chooser));
    auto* combo = new QComboBox(&chooser);
    combo->addItem(tr("Reverb"), QStringLiteral("reverb"));
    combo->addItem(tr("Chorus"), QStringLiteral("chorus"));
    combo->addItem(tr("Flanger"), QStringLiteral("flanger"));
    combo->addItem(tr("Overdrive / distortion"), QStringLiteral("overdrive_distortion"));
    combo->addItem(tr("Amp & cabinet"), QStringLiteral("amp_cabinet"));
    vl->addWidget(combo);
    auto* bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &chooser);
    vl->addWidget(bb);
    connect(bb, &QDialogButtonBox::accepted, &chooser, &QDialog::accept);
    connect(bb, &QDialogButtonBox::rejected, &chooser, &QDialog::reject);

    if (chooser.exec() != QDialog::Accepted)
        return;

    const QString type = combo->currentData().toString();
    EffectWidget* w = createEffectWidget(type);
    if (!w)
    {
        QMessageBox::warning(host, tr("Effects"), tr("Unknown effect type."));
        return;
    }
    m_widgets.append(w);
    relayoutChain();
    emitChainIfNeeded();
}

void EffectChainEditorWidget::onRemoveEffect(EffectWidget* w)
{
    if (!w)
        return;
    const int idx = m_widgets.indexOf(w);
    if (idx < 0)
        return;

    w->removeEventFilter(this);
    m_widgets.removeAt(idx);
    w->setParent(nullptr);
    w->deleteLater();
    relayoutChain();
    emitChainIfNeeded();
}

void EffectChainEditorWidget::onReorderDrag(EffectWidget* source)
{
    if (!source)
        return;

    beginReorderVisuals(source);

    auto* mime = new QMimeData;
    mime->setData(QString::fromLatin1(kMimeEffectPtr),
                  QByteArray::number(reinterpret_cast<quintptr>(source)));

    QDrag drag(this);
    drag.setMimeData(mime);
    drag.exec(Qt::MoveAction);

    endReorderVisuals();
}

void EffectChainEditorWidget::beginReorderVisuals(EffectWidget* source)
{
    m_dragSource        = source;
    m_reorderDragActive = true;
    if (m_dragSource)
    {
        auto* fx = new QGraphicsOpacityEffect(m_dragSource);
        fx->setOpacity(0.35);
        m_dragSource->setGraphicsEffect(fx);
    }

    if (!m_dropOverlay)
        m_dropOverlay = new EffectChainDragOverlay(m_chainContainer);
    m_dropOverlay->setGeometry(m_chainContainer->rect());
    m_dropOverlay->raise();
    m_dropOverlay->show();
    updateReorderOverlay(m_chainContainer->mapFromGlobal(QCursor::pos()));
}

void EffectChainEditorWidget::endReorderVisuals()
{
    m_reorderDragActive = false;
    if (m_dragSource)
    {
        m_dragSource->setGraphicsEffect(nullptr);
        m_dragSource = nullptr;
    }
    if (m_dropOverlay)
    {
        m_dropOverlay->hide();
        m_dropOverlay->clearPlaceholder();
        m_dropOverlay->setDropLine(0, false);
    }
}

void EffectChainEditorWidget::updateReorderOverlay(const QPoint& localInChain)
{
    QPointer<EffectWidget> srcGuard(m_dragSource);
    if (!m_dropOverlay || !srcGuard)
        return;

    m_dropOverlay->setGeometry(m_chainContainer->rect());
    m_dropOverlay->raise();

    const QRect cr = m_chainContainer->rect();
    m_dropOverlay->setPlaceholderRect(srcGuard->geometry());

    if (!cr.adjusted(-1, -1, 1, 1).contains(localInChain))
    {
        m_dropOverlay->setDropLine(0, false);
        m_dropOverlay->update();
        return;
    }

    const int idx   = dropIndexFromPos(localInChain);
    const int lineY = dropLineYForIndex(idx);
    m_dropOverlay->setDropLine(lineY, true);
}

EffectWidget* EffectChainEditorWidget::effectWidgetAtDropPos(const QPoint& localPos) const
{
    for (EffectWidget* w : std::as_const(m_widgets))
    {
        if (w && w->geometry().contains(localPos))
            return w;
    }
    return nullptr;
}

int EffectChainEditorWidget::dropIndexFromPos(const QPoint& localPos) const
{
    if (m_widgets.isEmpty())
        return 0;

    EffectWidget* const under = effectWidgetAtDropPos(localPos);
    if (under != nullptr)
    {
        int insertIndex = m_widgets.indexOf(under);
        if (insertIndex < 0)
            return 0;
        const QRect gr = under->geometry();
        if (localPos.y() >= gr.center().y())
            insertIndex += 1;
        return insertIndex;
    }

    for (int i = 0; i < m_widgets.size(); ++i)
    {
        EffectWidget* row = m_widgets[i];
        if (!row)
            continue;
        const QRect gr = row->geometry();
        const int   midY = gr.center().y();
        if (localPos.y() < midY)
            return i;
    }
    return int(m_widgets.size());
}

int EffectChainEditorWidget::dropLineYForIndex(int insertIndex) const
{
    const int n = int(m_widgets.size());
    if (n <= 0)
        return 4;
    insertIndex = qBound(0, insertIndex, n);
    const int spacing = m_chainLayout ? m_chainLayout->spacing() : 8;
    const int half    = qMax(2, spacing / 2);

    if (insertIndex == 0)
        return m_widgets[0]->geometry().top() - half;
    if (insertIndex >= n)
        return m_widgets[n - 1]->geometry().bottom() + half;
    const int top = m_widgets[insertIndex - 1]->geometry().bottom();
    const int bot = m_widgets[insertIndex]->geometry().top();
    return (top + bot) / 2;
}

void EffectChainEditorWidget::handleReorderDrop(QDropEvent* event, const QPoint& localInChain)
{
    if (!event->mimeData()->hasFormat(QString::fromLatin1(kMimeEffectPtr)))
    {
        event->ignore();
        return;
    }

    auto* source = reinterpret_cast<EffectWidget*>(
        event->mimeData()->data(QString::fromLatin1(kMimeEffectPtr)).toULongLong(nullptr, 10));
    if (source == nullptr || !m_widgets.contains(source))
    {
        event->ignore();
        return;
    }

    int insertIndex = dropIndexFromPos(localInChain);

    int from = m_widgets.indexOf(source);
    if (from < 0)
    {
        event->ignore();
        return;
    }

    m_widgets.removeAt(from);
    if (insertIndex > from)
        insertIndex -= 1;
    insertIndex = qBound(0, insertIndex, m_widgets.size());
    m_widgets.insert(insertIndex, source);

    relayoutChain();
    emitChainIfNeeded();
    event->acceptProposedAction();
}

bool EffectChainEditorWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (m_reorderDragActive
        && (event->type() == QEvent::DragMove || event->type() == QEvent::DragEnter))
        updateReorderOverlay(dragPointInChain(watched, event, m_chainContainer));

    if (event->type() == QEvent::Resize && watched == m_chainContainer && m_dropOverlay
        && m_dropOverlay->isVisible())
        m_dropOverlay->setGeometry(m_chainContainer->rect());

    QWidget* scrollVp = m_scroll ? m_scroll->viewport() : nullptr;
    const bool dropWatcher = watched == m_chainContainer || watched == m_scroll || watched == scrollVp
                             || watched == this
                             || m_widgets.contains(qobject_cast<EffectWidget*>(watched));
    if (dropWatcher)
    {
        const auto mimeOk = [](const QMimeData* m)
        {
            return m && m->hasFormat(QString::fromLatin1(kMimeEffectPtr));
        };

        switch (event->type())
        {
            case QEvent::DragEnter: {
                auto* e = static_cast<QDragEnterEvent*>(event);
                if (mimeOk(e->mimeData()))
                {
                    e->acceptProposedAction();
                    return true;
                }
                break;
            }
            case QEvent::DragMove: {
                auto* e = static_cast<QDragMoveEvent*>(event);
                if (mimeOk(e->mimeData()))
                {
                    e->acceptProposedAction();
                    return true;
                }
                break;
            }
            case QEvent::Drop: {
                auto* de = static_cast<QDropEvent*>(event);
                if (!mimeOk(de->mimeData()))
                    break;
                auto* w = qobject_cast<QWidget*>(watched);
                if (!w)
                    break;
                const QPoint inChain = m_chainContainer->mapFrom(w, de->position().toPoint());
                handleReorderDrop(de, inChain);
                return true;
            }
            default:
                break;
        }
    }
    return QWidget::eventFilter(watched, event);
}
