#include "effectwidget.h"
#include "audiopcmlimits.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QStyle>

EffectWidget::EffectWidget(QWidget* parent) : QFrame(parent)
{
    setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    setLineWidth(1);
}

QJsonObject EffectWidget::toChainEntry() const
{
    QJsonObject o;
    o.insert(QStringLiteral("type"), effectTypeId());
    o.insert(QStringLiteral("params"), parametersToJson());
    return o;
}

float EffectWidget::softLimitFloatSampleForInt16Pcm(float x)
{
    return AudioPcmLimits::softLimitUnitFloat(x);
}

float EffectWidget::guardFloatSampleForInt16Pcm(float x)
{
    return AudioPcmLimits::guardFloatSampleForInt16Pcm(x);
}

void EffectWidget::applyCloseButtonStyle(QPushButton* btn)
{
    if (!btn) return;
    btn->setFixedSize(22, 22);
    btn->setStyleSheet(
        "QPushButton {"
        "  background-color: #cc2222;"
        "  color: white;"
        "  border-radius: 11px;"
        "  font-weight: bold;"
        "  font-size: 11px;"
        "  padding: 0px;"
        "}"
        "QPushButton:hover { background-color: #ee4444; }"
        "QPushButton:pressed { background-color: #aa1111; }");
}

void EffectWidget::setupHeader()
{
    m_headerLayout = new QHBoxLayout();
    m_headerLayout->setContentsMargins(4, 4, 4, 4);
    m_headerLayout->setSpacing(6);

    m_dragHandle = new QWidget(this);
    m_dragHandle->setFixedWidth(16);
    m_dragHandle->setCursor(Qt::OpenHandCursor);
    m_dragHandle->setToolTip(tr("Drag to reorder effects"));
    auto* grip = new QLabel(QStringLiteral("\u2261"), m_dragHandle);  // ≡
    auto* gh = new QHBoxLayout(m_dragHandle);
    gh->setContentsMargins(0, 0, 0, 0);
    gh->addWidget(grip);
    QFont gf = grip->font();
    gf.setPointSize(12);
    gf.setBold(true);
    grip->setFont(gf);
    m_dragHandle->installEventFilter(this);

    m_titleLabel = new QLabel(this);
    QFont tf = m_titleLabel->font();
    tf.setBold(true);
    m_titleLabel->setFont(tf);

    m_closeButton = new QPushButton(QStringLiteral("\u2715"), this);  // ✕
    m_closeButton->setToolTip(tr("Remove effect"));
    applyCloseButtonStyle(m_closeButton);
    connect(m_closeButton, &QPushButton::clicked, this,
            [this]()
            {
                emit removeRequested(this);
            });

    m_headerLayout->addWidget(m_dragHandle);
    m_headerLayout->addWidget(m_titleLabel, 1);
    m_headerLayout->addWidget(m_closeButton);
}

void EffectWidget::notifyParametersChanged()
{
    emit parametersChanged(this);
}

bool EffectWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (watched != m_dragHandle)
        return false;

    switch (event->type())
    {
        case QEvent::MouseButtonPress: {
            auto* me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton)
            {
                m_dragMouseDown = true;
                m_dragPressPos  = me->pos();
            }
            break;
        }
        case QEvent::MouseMove: {
            auto* me = static_cast<QMouseEvent*>(event);
            if (m_dragMouseDown && (me->buttons() & Qt::LeftButton))
            {
                if ((me->pos() - m_dragPressPos).manhattanLength()
                    >= QApplication::startDragDistance())
                {
                    m_dragMouseDown = false;
                    emit reorderDragStarted(this);
                }
            }
            break;
        }
        case QEvent::MouseButtonRelease:
            m_dragMouseDown = false;
            break;
        default:
            break;
    }
    return false;
}
