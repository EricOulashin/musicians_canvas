#ifndef AUDIOUTILS_H
#define AUDIOUTILS_H

#include <QString>
#include <QVector>
#include "trackdata.h"

class AudioUtils {
public:
    // Legacy WAV-only mix (uses internal mixer)
    [[nodiscard]] static bool mixTracksToWav(const QVector<TrackData>& tracks,
                              const QString& outputPath,
                              int sampleRate = 44100);

    // Mix to WAV or FLAC using audio_mixer_cpp; renders MIDI tracks to FLAC in projectPath
    [[nodiscard]] static bool mixTracksToFile(const QVector<TrackData>& tracks,
                               const QString& outputPath,
                               const QString& projectPath,
                               int sampleRate = 44100);
};

#endif // AUDIOUTILS_H
