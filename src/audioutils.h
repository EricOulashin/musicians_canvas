#ifndef AUDIOUTILS_H
#define AUDIOUTILS_H

#include <QString>
#include <QVector>
#include "trackdata.h"

class AudioUtils
{
public:
    /// Filesystem-safe base name for a track (same rules as project audio/MIDI files).
    [[nodiscard]] static QString sanitizedTrackFilesystemName(const QString& trackName);

    // Legacy WAV-only mix (uses internal mixer)
    [[nodiscard]] static bool mixTracksToWav(const QVector<TrackData>& tracks,
                              const QString& outputPath,
                              int sampleRate = 44100,
                              const QString& soundFontPath = QString());

    // Mix to WAV or FLAC using audio_mixer_cpp.
    // When renderMidiToAudio is true, MIDI tracks are synthesized offline and mixed in.
    // When false, MIDI tracks are ignored (useful for real-time MIDI-out playback).
    [[nodiscard]] static bool mixTracksToFile(const QVector<TrackData>& tracks,
                               const QString& outputPath,
                               const QString& projectPath,
                               int sampleRate = 44100,
                               const QString& soundFontPath = QString(),
                               bool renderMidiToAudio = true,
                               double midiGainMultiplier = 1.0);

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

    // Resample Int16 PCM data from srcRate to dstRate using windowed sinc
    // interpolation (similar to libsoxr / Audacity).  The input contains
    // interleaved Int16 samples with the given channelCount.  Returns a new
    // QByteArray at the target rate.  If srcRate == dstRate, returns the
    // input unchanged.  Pitch and duration are preserved.
    [[nodiscard]] static QByteArray resampleInt16(const QByteArray& int16Data,
                                                   int srcRate,
                                                   int dstRate,
                                                   int channelCount);

    // Convert channel layout of Int16 PCM data.
    //   mono → stereo:  duplicates the single channel to both L and R
    //   stereo → mono:  averages L and R into a single channel
    // Returns a new QByteArray with the target channel count.
    // If srcChannels == dstChannels, returns the input unchanged.
    [[nodiscard]] static QByteArray convertChannels(const QByteArray& int16Data,
                                                      int srcChannels,
                                                      int dstChannels);

    /// Read 16-bit PCM from a RIFF/WAVE file (chunk scan; supports mono/stereo).
    [[nodiscard]] static bool readWavInt16Pcm(const QString& path, QByteArray& audioData,
                                               int& sampleRate, int& channelCount);

    /// Write 16-bit little-endian interleaved PCM to a standard WAV file (overwrites \p path).
    [[nodiscard]] static bool writeWavInt16Pcm(const QString& path, const QByteArray& int16Interleaved,
                                                int sampleRate, int channelCount);

    /// Apply mix-bus effect chain to an existing WAV or FLAC file (same formats as mix export).
    [[nodiscard]] static bool applyMixEffectChainToAudioFile(const QString& path,
                                                              const QJsonArray& chain);
};

#endif // AUDIOUTILS_H
