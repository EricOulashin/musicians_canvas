#ifndef MIXDIALOG_H
#define MIXDIALOG_H

#include <QDialog>
#include <QString>

class QLineEdit;

class MixDialog : public QDialog
{
    Q_OBJECT
public:
    explicit MixDialog(const QString& defaultDir, QWidget* parent = nullptr);

    QString outputFilename() const;

private slots:
    void onBrowse();
    void onAccept();

private:
    QLineEdit* m_filenameEdit = nullptr;
    QString m_defaultDir;
};

#endif // MIXDIALOG_H
