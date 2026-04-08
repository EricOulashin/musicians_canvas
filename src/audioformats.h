#ifndef AUDIOFORMATS_H
#define AUDIOFORMATS_H

#include <QString>
#include <QStringList>
#include <QFileInfo>

// Central registry of audio file formats supported by Musician's Canvas.
//
// This is the single source of truth for which audio file types Musician's
// Canvas can read or write.  All code that needs to know about supported
// audio formats — the file/save dialog filters, the drag-and-drop handler,
// the audio loader, the project-file loader, etc. — should consult this
// header instead of hard-coding extension lists.
//
// Adding a new format (e.g. .mp3, .ogg, .aiff): add a new entry to
// AudioFormats::all() below and implement the corresponding read/write
// function.  No other code needs to change.
namespace AudioFormats {

struct Format
{
    const char* extension;     // lowercase, including leading dot, e.g. ".flac"
    const char* description;   // human-readable description, e.g. "FLAC"
};

// All formats Musician's Canvas can currently read AND write.
// Update this list when adding support for new formats (e.g. mp3, ogg, aiff).
inline const QVector<Format>& all()
{
    static const QVector<Format> formats = {
        { ".flac", "FLAC" },
        { ".wav",  "WAV"  },
    };
    return formats;
}

// Returns just the extensions, e.g. {".flac", ".wav"}.
inline QStringList extensions()
{
    QStringList exts;
    for (const auto& f : all())
        exts.append(QString::fromLatin1(f.extension));
    return exts;
}

// Returns name filters suitable for QDir::entryList(), e.g. {"*.flac", "*.wav"}.
inline QStringList nameFilters()
{
    QStringList filters;
    for (const auto& f : all())
        filters.append(QStringLiteral("*") + QString::fromLatin1(f.extension));
    return filters;
}

// Returns true if the given filename has one of the supported audio extensions.
inline bool isSupported(const QString& filenameOrPath)
{
    const QString suffix = QStringLiteral(".") + QFileInfo(filenameOrPath).suffix().toLower();
    for (const auto& f : all())
    {
        if (suffix == QString::fromLatin1(f.extension))
            return true;
    }
    return false;
}

// Builds a Qt file-dialog filter string covering all supported audio formats.
//
// Two layouts are produced:
//
//   asCombinedFilter == true:
//     "Audio files (*.flac *.wav);;FLAC files (*.flac);;WAV files (*.wav);;All files (*)"
//   asCombinedFilter == false:
//     "FLAC files (*.flac);;WAV files (*.wav);;All files (*)"
//
// The combined-filter form is preferred for "open" dialogs where the user
// might want to see every supported audio file at once; the split form is
// fine for "save" dialogs where the user picks one specific output type.
inline QString fileDialogFilter(bool asCombinedFilter = false)
{
    QStringList parts;

    if (asCombinedFilter)
    {
        QStringList allExts;
        for (const auto& f : all())
            allExts.append(QStringLiteral("*") + QString::fromLatin1(f.extension));
        parts.append(QStringLiteral("Audio files (%1)").arg(allExts.join(QChar(' '))));
    }

    for (const auto& f : all())
    {
        parts.append(QStringLiteral("%1 files (*%2)")
                         .arg(QString::fromLatin1(f.description),
                              QString::fromLatin1(f.extension)));
    }
    parts.append(QStringLiteral("All files (*)"));
    return parts.join(QStringLiteral(";;"));
}

} // namespace AudioFormats

#endif // AUDIOFORMATS_H
