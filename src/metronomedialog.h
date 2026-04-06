#ifndef METRONOMEDIALOG_H
#define METRONOMEDIALOG_H

#include <QDialog>

class QCheckBox;
class QSpinBox;

class MetronomeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MetronomeDialog(QWidget* parent = nullptr);

private slots:
    void onApply();

private:
    void loadSettings();
    void saveSettings();

    QCheckBox* m_enabledCheck = nullptr;
    QSpinBox*  m_bpmSpin = nullptr;
};

#endif // METRONOMEDIALOG_H
