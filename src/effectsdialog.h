#ifndef EFFECTSDIALOG_H
#define EFFECTSDIALOG_H

#include <QDialog>
#include <QEvent>
#include <QJsonArray>

class TrackWidget;
class EffectChainEditorWidget;

class EffectsDialog final : public QDialog
{
    Q_OBJECT
public:
    explicit EffectsDialog(TrackWidget* trackWidget, QWidget* parent = nullptr);

protected:
    void reject() override;

private:
    TrackWidget*              m_trackWidget = nullptr;
    QJsonArray                m_initialChain;
    EffectChainEditorWidget*  m_editor = nullptr;
};

#endif
