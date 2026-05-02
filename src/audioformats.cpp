#include "audioformats.h"

#include <QCoreApplication>

namespace AudioFormats {

QString formatTitleForUi(const QString& extensionWithLeadingDot)
{
    const QString extLower =
        extensionWithLeadingDot.startsWith(QLatin1Char('.'))
            ? extensionWithLeadingDot.toLower()
            : QStringLiteral(".") + extensionWithLeadingDot.toLower();
    if (extLower == QLatin1String(".flac"))
        return QCoreApplication::translate("AudioFormats", "FLAC");
    if (extLower == QLatin1String(".wav"))
        return QCoreApplication::translate("AudioFormats", "WAV");
    if (extLower == QLatin1String(".mp3"))
        return QCoreApplication::translate("AudioFormats", "MP3");
    if (extLower == QLatin1String(".ogg"))
        return QCoreApplication::translate("AudioFormats", "Ogg Vorbis");
    if (extLower == QLatin1String(".aiff") || extLower == QLatin1String(".aif")
        || extLower == QLatin1String(".aifc"))
        return QCoreApplication::translate("AudioFormats", "AIFF");
    return extLower.mid(1).toUpper();
}

const QVector<Format>& all()
{
    static const QVector<Format> formats = {
        { ".flac" },
        { ".wav"  },
        { ".mp3"  },
        { ".ogg"  },
        { ".aiff" },
        { ".aif"  },
        { ".aifc" },
    };
    return formats;
}

namespace
{
// Builds "Label files (*.ext)" with the glob outside translated text so locales cannot drop %2
// (which previously produced empty parentheses in some translations).
void appendSingleExtensionFilter(QStringList& parts, const QString& extWithDot)
{
    const QString label = AudioFormats::formatTitleForUi(extWithDot);
    parts.append(QCoreApplication::translate("AudioFormats", "%1 files").arg(label)
                 + QStringLiteral(" (*.") + extWithDot.mid(1) + QLatin1Char(')'));
}
} // namespace

QString fileDialogFilter(bool asCombinedFilter)
{
    QStringList parts;

    if (asCombinedFilter)
    {
        QStringList allExts;
        for (const auto& f : all())
            allExts.append(QStringLiteral("*") + QString::fromLatin1(f.extension));
        parts.append(QCoreApplication::translate("AudioFormats", "Audio files (%1)")
                         .arg(allExts.join(QChar(' '))));
    }

    // One row per distinct format in the save dialog. AIFF uses three extensions (.aiff, .aif,
    // .aifc); list them in a single filter line so the menu does not show three identical labels.
    appendSingleExtensionFilter(parts, QStringLiteral(".flac"));
    appendSingleExtensionFilter(parts, QStringLiteral(".wav"));
    appendSingleExtensionFilter(parts, QStringLiteral(".mp3"));
    appendSingleExtensionFilter(parts, QStringLiteral(".ogg"));
    const QString aiffGlobs = QStringLiteral("*.aiff *.aif *.aifc");
    parts.append(QCoreApplication::translate("AudioFormats", "AIFF files (%1)").arg(aiffGlobs));

    parts.append(QCoreApplication::translate("AudioFormats", "All files (*)"));
    return parts.join(QStringLiteral(";;"));
}

} // namespace AudioFormats
