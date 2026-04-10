#include "effectsdialog.h"

#include "choruswidget.h"
#include "effectwidget.h"
#include "flangewidget.h"
#include "reverbwidget.h"
#include "trackwidget.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QMimeData>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

namespace
{
constexpr char kMimeEffectPtr[] = "application/x-mc-effect-ptr";
}

EffectsDialog::EffectsDialog(TrackWidget* trackWidget, QWidget* parent)
    : QDialog(parent)
    , m_trackWidget(trackWidget)
{
    setWindowTitle(tr("Track effects"));
    setModal(true);
    resize(520, 420);
    if (m_trackWidget)
        m_initialChain = m_trackWidget->audioEffectChain();

    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(10, 10, 10, 10);
    outer->setSpacing(8);

    auto* topRow = new QHBoxLayout();
    m_addButton = new QPushButton(tr("Add effect…"), this);
    connect(m_addButton, &QPushButton::clicked, this, &EffectsDialog::onAddEffect);
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

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    outer->addWidget(buttons);

    m_chainContainer->setAcceptDrops(true);
    m_chainContainer->installEventFilter(this);

    rebuildFromChain();
}

void EffectsDialog::reject()
{
    if (m_trackWidget)
        m_trackWidget->setAudioEffectChain(m_initialChain);
    QDialog::reject();
}

EffectWidget* EffectsDialog::createEffectWidget(const QString& type)
{
    EffectWidget* w = nullptr;
    if (type == QStringLiteral("reverb"))
        w = new ReverbWidget(m_chainContainer);
    else if (type == QStringLiteral("chorus"))
        w = new ChorusWidget(m_chainContainer);
    else if (type == QStringLiteral("flanger"))
        w = new FlangeWidget(m_chainContainer);

    if (!w)
        return nullptr;

    w->setAcceptDrops(true);
    w->installEventFilter(this);

    connect(w, &EffectWidget::removeRequested, this, &EffectsDialog::onRemoveEffect);
    connect(w, &EffectWidget::parametersChanged, this, &EffectsDialog::syncLive);
    connect(w, &EffectWidget::reorderDragStarted, this, &EffectsDialog::onReorderDrag);
    return w;
}

void EffectsDialog::rebuildFromChain()
{
    m_suspendSync = true;
    for (auto* w : std::as_const(m_widgets))
    {
        w->removeEventFilter(this);
        w->setParent(nullptr);
        w->deleteLater();
    }
    m_widgets.clear();

    QJsonArray chain;
    if (m_trackWidget)
        chain = m_trackWidget->audioEffectChain();

    for (const QJsonValue& v : chain)
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
    m_suspendSync = false;
}

void EffectsDialog::relayoutChain()
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

void EffectsDialog::syncLive()
{
    if (m_suspendSync || !m_trackWidget)
        return;
    QJsonArray a;
    for (auto* w : std::as_const(m_widgets))
        a.append(w->toChainEntry());
    m_trackWidget->setAudioEffectChain(a);
}

void EffectsDialog::onAddEffect()
{
    QDialog chooser(this);
    chooser.setWindowTitle(tr("Add effect"));
    auto* vl = new QVBoxLayout(&chooser);
    vl->addWidget(new QLabel(tr("Choose an effect:"), &chooser));
    auto* combo = new QComboBox(&chooser);
    combo->addItem(tr("Reverb"), QStringLiteral("reverb"));
    combo->addItem(tr("Chorus"), QStringLiteral("chorus"));
    combo->addItem(tr("Flanger"), QStringLiteral("flanger"));
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
        QMessageBox::warning(this, tr("Effects"), tr("Unknown effect type."));
        return;
    }
    m_widgets.append(w);
    relayoutChain();
    syncLive();
}

void EffectsDialog::onRemoveEffect(EffectWidget* w)
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
    syncLive();
}

void EffectsDialog::onReorderDrag(EffectWidget* source)
{
    if (!source)
        return;

    auto* mime = new QMimeData;
    mime->setData(QString::fromLatin1(kMimeEffectPtr),
                  QByteArray::number(reinterpret_cast<quintptr>(source)));

    QDrag drag(this);
    drag.setMimeData(mime);
    drag.exec(Qt::MoveAction);
}

EffectWidget* EffectsDialog::effectWidgetAtDropPos(const QPoint& localPos)
{
    QWidget* at = m_chainContainer->childAt(localPos);
    while (at != nullptr)
    {
        if (auto* ew = qobject_cast<EffectWidget*>(at))
            return ew;
        at = at->parentWidget();
    }
    return nullptr;
}

int EffectsDialog::insertionIndexFromPos(const QPoint& localPos, EffectWidget* dragged)
{
    Q_UNUSED(dragged);
    if (m_widgets.isEmpty())
        return 0;

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
    return m_widgets.size();
}

void EffectsDialog::handleReorderDrop(QDropEvent* event, const QPoint& localInChain)
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

    EffectWidget* under = effectWidgetAtDropPos(localInChain);
    int insertIndex = insertionIndexFromPos(localInChain, source);

    if (under != nullptr)
    {
        insertIndex = m_widgets.indexOf(under);
        const QRect gr = under->geometry();
        if (localInChain.y() >= gr.center().y())
            insertIndex += 1;
    }

    int from = m_widgets.indexOf(source);
    if (from < 0)
    {
        event->ignore();
        return;
    }

    m_widgets.removeAt(from);
    if (insertIndex > from)
        insertIndex -= 1;
    insertIndex      = qBound(0, insertIndex, m_widgets.size());
    m_widgets.insert(insertIndex, source);

    relayoutChain();
    syncLive();
    event->acceptProposedAction();
}

bool EffectsDialog::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_chainContainer || m_widgets.contains(qobject_cast<EffectWidget*>(watched)))
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
    return QDialog::eventFilter(watched, event);
}
