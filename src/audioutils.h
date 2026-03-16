#ifndef AUDIOUTILS_H
#define AUDIOUTILS_H

#include <QString>
#include <QVector>
#include "trackdata.h"

class AudioUtils
{
public:
    // Legacy WAV-only mix (uses internal mixer)
    [[nodiscard]] static bool mixTracksToWav(const QVector<TrackData>& tracks,
                              const QString& outputPath,
                              int sampleRate = 44100,
                              const QString& soundFontPath = QString());

    // Mix to WAV or FLAC using audio_mixer_cpp; renders MIDI tracks to FLAC in projectPath
    [[nodiscard]] static bool mixTracksToFile(const QVector<TrackData>& tracks,
                               const QString& outputPath,
                               const QString& projectPath,
                               int sampleRate = 44100,
                               const QString& soundFontPath = QString());

    // Encode Int16 PCM audio data to a FLAC file via a temporary WAV intermediate.
    // Returns true on success.
    [[nodiscard]] static bool writeAudioDataToFlac(const QByteArray& int16Data,
                                                    int sampleRate,
                                                    int channelCount,
                                                    const QString& flacPath);

    // Decode a FLAC file into Int16 PCM and populate audioData, sampleRate,
    // and channelCount.  Returns true on success.
    [[nodiscard]] static bool readFlacAudioData(const QString& path,
                                                 QByteArray& audioData,
                                                 int& sampleRate,
                                                 int& channelCount);
};

#endif // AUDIOUTILS_H
