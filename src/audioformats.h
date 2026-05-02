#ifndef AUDIOFORMATS_H
#define AUDIOFORMATS_H

#include <QString>
#include <QStringList>
#include <QFileInfo>
#include <QVector>

// Central registry of audio file formats supported by Musician's Canvas.
//
// This is the single source of truth for which audio file types Musician's
// Canvas can read or write.  All code that needs to know about supported
// audio formats — the file/save dialog filters, the drag-and-drop handler,
// the audio loader, the project-file loader, etc. — should consult this
// header instead of hard-coding extension lists.
//
// Adding a new format: add a new entry to AudioFormats::all() in
// audioformats.cpp and ensure AudioUtils can read/write it (via
// audio_mixer_cpp).  Update translations for format labels in fileDialogFilter().
namespace AudioFormats {

struct Format
{
    const char* extension; // lowercase, including leading dot, e.g. ".flac"
};

const QVector<Format>& all();

// Returns just the extensions, e.g. {".flac", ".wav", ".mp3", ...}.
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

// Human-readable format name for UI (translated, context "AudioFormats").
QString formatTitleForUi(const QString& extensionWithLeadingDot);

// Builds a Qt file-dialog filter string covering all supported audio formats.
// Strings are translated (context "AudioFormats"; see audioformats.cpp).
QString fileDialogFilter(bool asCombinedFilter = false);

} // namespace AudioFormats

#endif // AUDIOFORMATS_H
