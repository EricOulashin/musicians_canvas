#include "effectsdialog.h"

#include "effectchaineditorwidget.h"
#include "trackwidget.h"

#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

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

    m_editor = new EffectChainEditorWidget(this);
    if (m_trackWidget)
        m_editor->setChain(m_trackWidget->audioEffectChain());
    outer->addWidget(m_editor, 1);

    connect(m_editor, &EffectChainEditorWidget::chainChanged, this,
            [this]()
            {
                if (m_trackWidget)
                    m_trackWidget->setAudioEffectChain(m_editor->chain());
            });

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    outer->addWidget(buttons);
}

void EffectsDialog::reject()
{
    if (m_trackWidget)
        m_trackWidget->setAudioEffectChain(m_initialChain);
    QDialog::reject();
}
