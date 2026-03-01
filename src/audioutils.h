#ifndef AUDIOUTILS_H
#define AUDIOUTILS_H

#include <QString>
#include <QVector>
#include "trackdata.h"

class AudioUtils {
public:
    [[nodiscard]] static bool mixTracksToWav(const QVector<TrackData>& tracks,
                              const QString& outputPath,
                              int sampleRate = 44100);
};

#endif // AUDIOUTILS_H
