#include "mixdialog.h"
#include "audioformats.h"
#include <QFileInfo>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QTimer>
#include <QCoreApplication>
#include <QEventLoop>

namespace
{
constexpr int kMinMixDialogWidth = 520;
}

MixDialog::MixDialog(const QString& defaultDir, QWidget* parent)
    : QDialog(parent), m_defaultDir(defaultDir)
{
    setWindowTitle(tr("Mix Tracks to File"));

    auto* layout = new QVBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(16, 16, 16, 16);

    auto* fileRow = new QHBoxLayout();
    fileRow->addWidget(new QLabel(tr("Output file:")));
    m_filenameEdit = new QLineEdit();
    m_filenameEdit->setPlaceholderText(tr("Choose output filename..."));
    connect(m_filenameEdit, &QLineEdit::textChanged, this, &MixDialog::refreshEncodingUi);
    fileRow->addWidget(m_filenameEdit, 1);
    auto* browseBtn = new QPushButton(tr("Browse..."));
    connect(browseBtn, &QPushButton::clicked, this, &MixDialog::onBrowse);
    fileRow->addWidget(browseBtn);
    layout->addLayout(fileRow);

    m_encodingGroup = new QGroupBox(tr("Encoding"));
    m_encodingGroup->setVisible(false);
    auto* encOuter = new QVBoxLayout(m_encodingGroup);

    m_mp3Widget = new QWidget();
    auto* mp3Form = new QFormLayout(m_mp3Widget);
    mp3Form->setContentsMargins(0, 0, 0, 0);
    m_mp3ModeCombo = new QComboBox();
    m_mp3ModeCombo->addItem(tr("Constant bitrate (CBR)"));
    m_mp3ModeCombo->addItem(tr("Average bitrate (ABR)"));
    m_mp3ModeCombo->addItem(tr("Variable bitrate (VBR)"));
    connect(m_mp3ModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MixDialog::onMp3ModeChanged);
    mp3Form->addRow(tr("Bitrate mode:"), m_mp3ModeCombo);

    m_mp3KbpsSpin = new QSpinBox();
    m_mp3KbpsSpin->setRange(32, 320);
    m_mp3KbpsSpin->setSingleStep(16);
    m_mp3KbpsSpin->setSuffix(QStringLiteral(" kbps"));
    m_mp3KbpsSpin->setValue(192);
    mp3Form->addRow(tr("Bitrate:"), m_mp3KbpsSpin);

    auto* vbrRow = new QHBoxLayout();
    m_mp3VbrSlider = new QSlider(Qt::Horizontal);
    m_mp3VbrSlider->setRange(0, 100);
    m_mp3VbrSlider->setValue(35);
    connect(m_mp3VbrSlider, &QSlider::valueChanged, this,
            [this](int v)
            {
                m_mp3VbrValueLabel->setText(
                    tr("%1% — lower is higher quality, larger files").arg(v));
            });
    m_mp3VbrValueLabel = new QLabel();
    vbrRow->addWidget(m_mp3VbrSlider, 1);
    vbrRow->addWidget(m_mp3VbrValueLabel);
    mp3Form->addRow(tr("VBR setting:"), vbrRow);

    m_mp3Widget->setVisible(false);
    encOuter->addWidget(m_mp3Widget);

    m_oggWidget = new QWidget();
    auto* oggForm = new QFormLayout(m_oggWidget);
    oggForm->setContentsMargins(0, 0, 0, 0);
    m_oggModeCombo = new QComboBox();
    m_oggModeCombo->addItem(tr("Quality-based VBR"));
    m_oggModeCombo->addItem(tr("Approximate nominal bitrate"));
    connect(m_oggModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MixDialog::onOggModeChanged);
    oggForm->addRow(tr("Encoding:"), m_oggModeCombo);

    auto* qualRow = new QHBoxLayout();
    m_oggQualitySlider = new QSlider(Qt::Horizontal);
    m_oggQualitySlider->setRange(0, 100);
    m_oggQualitySlider->setValue(70);
    connect(m_oggQualitySlider, &QSlider::valueChanged, this,
            [this](int v)
            {
                m_oggQualityValueLabel->setText(
                    tr("%1% — higher is better quality").arg(v));
            });
    m_oggQualityValueLabel = new QLabel();
    qualRow->addWidget(m_oggQualitySlider, 1);
    qualRow->addWidget(m_oggQualityValueLabel);
    oggForm->addRow(tr("Quality:"), qualRow);

    m_oggApproxKbpsSpin = new QSpinBox();
    m_oggApproxKbpsSpin->setRange(48, 320);
    m_oggApproxKbpsSpin->setSingleStep(16);
    m_oggApproxKbpsSpin->setSuffix(QStringLiteral(" kbps"));
    m_oggApproxKbpsSpin->setValue(192);
    oggForm->addRow(tr("Nominal bitrate:"), m_oggApproxKbpsSpin);

    m_oggWidget->setVisible(false);
    encOuter->addWidget(m_oggWidget);

    layout->addWidget(m_encodingGroup);

    auto* buttons = new QDialogButtonBox(this);
    auto* acceptBtn = buttons->addButton(tr("Mix"), QDialogButtonBox::AcceptRole);
    auto* cancelBtn = buttons->addButton(QDialogButtonBox::Cancel);
    connect(acceptBtn, &QPushButton::clicked, this, &MixDialog::onAccept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    layout->addWidget(buttons);

    onMp3ModeChanged(0);
    onOggModeChanged(0);
    refreshEncodingUi();
}

QString MixDialog::outputFilename() const
{
    return m_filenameEdit ? m_filenameEdit->text().trimmed() : QString();
}

std::optional<MixExportEncodeSettings> MixDialog::mixEncodeSettings() const
{
    const QString path = outputFilename();
    if (path.isEmpty() || !AudioUtils::exportDestinationSupportsAdjustableBitrate(path))
        return std::nullopt;

    MixExportEncodeSettings s;
    const QString ext = QFileInfo(path).suffix().toLower();
    if (ext == QLatin1String("mp3"))
    {
        switch (m_mp3ModeCombo->currentIndex())
        {
        case 0:
            s.mp3Kind = MixExportEncodeSettings::Mp3BitrateKind::Constant;
            break;
        case 1:
            s.mp3Kind = MixExportEncodeSettings::Mp3BitrateKind::Average;
            break;
        default:
            s.mp3Kind = MixExportEncodeSettings::Mp3BitrateKind::Variable;
            break;
        }
        s.mp3NominalKbps = static_cast<quint32>(m_mp3KbpsSpin->value());
        s.mp3VbrCompressionZeroBestOneWorst =
            static_cast<double>(m_mp3VbrSlider->value()) / 100.0;
        return s;
    }
    if (ext == QLatin1String("ogg") || ext == QLatin1String("oga"))
    {
        s.oggKind = (m_oggModeCombo->currentIndex() == 0)
                        ? MixExportEncodeSettings::OggEncodeKind::QualityVbr
                        : MixExportEncodeSettings::OggEncodeKind::ApproxNominalBitrate;
        s.oggQualityBestOneWorstZero = static_cast<double>(m_oggQualitySlider->value()) / 100.0;
        s.oggApproxNominalKbps = static_cast<quint32>(m_oggApproxKbpsSpin->value());
        return s;
    }
    return std::nullopt;
}

void MixDialog::refreshEncodingUi()
{
    const QString path = outputFilename();
    const bool adjustable =
        !path.isEmpty() && AudioUtils::exportDestinationSupportsAdjustableBitrate(path);
    m_encodingGroup->setVisible(adjustable);
    if (!adjustable)
    {
        updateFitSize();
        return;
    }

    const QString ext = QFileInfo(path).suffix().toLower();
    const bool mp3 = ext == QLatin1String("mp3");
    const bool ogg = ext == QLatin1String("ogg") || ext == QLatin1String("oga");
    m_mp3Widget->setVisible(mp3);
    m_oggWidget->setVisible(ogg);
    updateFitSize();
}

void MixDialog::onMp3ModeChanged(int index)
{
    const bool nominal = (index == 0 || index == 1);
    m_mp3KbpsSpin->setVisible(nominal);
    m_mp3VbrSlider->setVisible(!nominal);
    m_mp3VbrValueLabel->setVisible(!nominal);
    if (!nominal && m_mp3VbrSlider)
        m_mp3VbrValueLabel->setText(
            tr("%1% — lower is higher quality, larger files").arg(m_mp3VbrSlider->value()));
    updateFitSize();
}

void MixDialog::onOggModeChanged(int index)
{
    const bool quality = (index == 0);
    m_oggQualitySlider->setVisible(quality);
    m_oggQualityValueLabel->setVisible(quality);
    m_oggApproxKbpsSpin->setVisible(!quality);
    if (quality && m_oggQualitySlider)
        m_oggQualityValueLabel->setText(
            tr("%1% — higher is better quality").arg(m_oggQualitySlider->value()));
    updateFitSize();
}

void MixDialog::updateFitSize()
{
    if (layout())
    {
        layout()->invalidate();
        layout()->activate();
    }
    if (m_encodingGroup)
        m_encodingGroup->updateGeometry();

    // After Browse (or toggling Encoding visibility), sizeHint() can run before child
    // layouts settle; process pending layout/geometry so heights include the Encoding block.
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    updateGeometry();

    QSize combined = sizeHint();
    if (layout())
    {
        const QSize layMin = layout()->minimumSize();
        combined.setWidth(qMax(combined.width(), layMin.width()));
        combined.setHeight(qMax(combined.height(), layMin.height()));
    }
    const QSize minHint = minimumSizeHint();
    combined.setWidth(qMax(combined.width(), minHint.width()));
    combined.setHeight(qMax(combined.height(), minHint.height()));

    const int w = qMax(kMinMixDialogWidth, combined.width());
    const int h = combined.height();
    setMinimumSize(w, h);
    resize(w, h);
}

void MixDialog::onBrowse()
{
    const QString startDir = m_defaultDir.isEmpty() ? QDir::homePath() : m_defaultDir;
    QString path = QFileDialog::getSaveFileName(
        this, tr("Choose output file"), startDir,
        AudioFormats::fileDialogFilter());
    if (path.isEmpty()) return;

    if (!AudioFormats::isSupported(path))
        path += QString::fromLatin1(AudioFormats::all().first().extension);

    m_filenameEdit->setText(path);
    // Native file dialog teardown defers layout; run two deferred passes so the Encoding
    // section is fully measured before we set the dialog height.
    QTimer::singleShot(
        0, this,
        [this]()
        {
            updateFitSize();
            QTimer::singleShot(
                0, this,
                [this]()
                {
                    updateFitSize();
                });
        });
}

void MixDialog::onAccept()
{
    if (outputFilename().isEmpty())
    {
        QMessageBox::warning(this, tr("No file chosen"), tr("Please choose an output file."));
        return;
    }
    accept();
}
