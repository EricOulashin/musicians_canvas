#include "mixdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

MixDialog::MixDialog(const QString& defaultDir, QWidget* parent)
    : QDialog(parent), m_defaultDir(defaultDir)
{
    setWindowTitle(tr("Mix Tracks to File"));
    setMinimumWidth(480);

    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(16, 16, 16, 16);

    // Output file row
    auto* fileRow = new QHBoxLayout();
    fileRow->addWidget(new QLabel(tr("Output file:")));
    m_filenameEdit = new QLineEdit();
    m_filenameEdit->setPlaceholderText(tr("Choose output filename..."));
    fileRow->addWidget(m_filenameEdit, 1);
    auto* browseBtn = new QPushButton(tr("Browse..."));
    connect(browseBtn, &QPushButton::clicked, this, &MixDialog::onBrowse);
    fileRow->addWidget(browseBtn);
    layout->addLayout(fileRow);

    // Buttons
    auto* buttons = new QDialogButtonBox(this);
    auto* acceptBtn = buttons->addButton(tr("Mix"), QDialogButtonBox::AcceptRole);
    auto* cancelBtn = buttons->addButton(QDialogButtonBox::Cancel);
    connect(acceptBtn, &QPushButton::clicked, this, &MixDialog::onAccept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    layout->addWidget(buttons);
}

QString MixDialog::outputFilename() const {
    return m_filenameEdit ? m_filenameEdit->text().trimmed() : QString();
}

void MixDialog::onBrowse() {
    const QString startDir = m_defaultDir.isEmpty() ? QDir::homePath() : m_defaultDir;
    QString path = QFileDialog::getSaveFileName(
        this, tr("Choose output file"), startDir,
        tr("WAV files (*.wav);;FLAC files (*.flac)"));
    if (path.isEmpty()) return;

    // Ensure a recognised extension is present
    if (!path.endsWith(".wav", Qt::CaseInsensitive) &&
        !path.endsWith(".flac", Qt::CaseInsensitive))
        path += ".wav";

    m_filenameEdit->setText(path);
}

void MixDialog::onAccept() {
    if (outputFilename().isEmpty()) {
        QMessageBox::warning(this, tr("No file chosen"), tr("Please choose an output file."));
        return;
    }
    accept();
}
