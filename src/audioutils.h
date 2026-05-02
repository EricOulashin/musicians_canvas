#ifndef AUDIOUTILS_H
#define AUDIOUTILS_H

#include <QString>
#include <QVector>
#include "trackdata.h"

/// Lossy export options for formats where \c AudioFile::BitrateIsAdjustable() is true (e.g. MP3, Ogg).
struct MixExportEncodeSettings
{
    enum class Mp3BitrateKind
    {
        Constant,
        Average,
        Variable
    };
    Mp3BitrateKind mp3Kind = Mp3BitrateKind::Constant;
    quint32 mp3NominalKbps = 192;
    double mp3VbrCompressionZeroBestOneWorst = 0.35;

    enum class OggEncodeKind
    {
        QualityVbr,
        ApproxNominalBitrate
    };
    OggEncodeKind oggKind = OggEncodeKind::QualityVbr;
    double oggQualityBestOneWorstZero = 0.7;
    quint32 oggApproxNominalKbps = 192;
};

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

    // Mix to WAV, FLAC, MP3, Ogg Vorbis, or AIFF using audio_mixer_cpp (via temporary WAV + encoder).
    // When renderMidiToAudio is true, MIDI tracks are synthesized offline and mixed in.
    // When false, MIDI tracks are ignored (useful for real-time MIDI-out playback).
    [[nodiscard]] static bool mixTracksToFile(const QVector<TrackData>& tracks,
                               const QString& outputPath,
                               const QString& projectPath,
                               int sampleRate = 44100,
                               const QString& soundFontPath = QString(),
                               bool renderMidiToAudio = true,
                               double midiGainMultiplier = 1.0,
                               const QJsonArray& auxEffectChain = QJsonArray(),
                               const MixExportEncodeSettings* encodeSettings = nullptr);

    /// True when \p path selects a format that supports user-chosen encoding (MP3, Ogg Vorbis).
    [[nodiscard]] static bool exportDestinationSupportsAdjustableBitrate(const QString& path);

    /// Export each non-empty track as its own file (mixer gain/pan/trim; no aux/master FX).
    /// \p outputExtension must include the dot (e.g. ".wav", ".flac", ".mp3").
    [[nodiscard]] static bool exportStemsToDirectory(const QVector<TrackData>& tracks,
                                                      const QString& outputDirectory,
                                                      const QString& projectPath,
                                                      int sampleRate = 44100,
                                                      const QString& soundFontPath = QString(),
                                                      bool renderMidiToAudio = true,
                                                      double midiGainMultiplier = 1.0,
                                                      const QString& outputExtension = QStringLiteral(".wav"));

    // Encode Int16 PCM to a supported audio file (extension selects FLAC, MP3, Ogg, AIFF; WAV writes directly).
    [[nodiscard]] static bool writeInt16PcmToAudioFile(const QByteArray& int16Data,
                                                      int sampleRate,
                                                      int channelCount,
                                                      const QString& path,
                                                      const MixExportEncodeSettings* encodeSettings = nullptr);

    // Encode Int16 PCM audio data to a FLAC file via a temporary WAV intermediate.
    // Returns true on success.
    [[nodiscard]] static bool writeAudioDataToFlac(const QByteArray& int16Data,
                                                    int sampleRate,
                                                    int channelCount,
                                                    const QString& flacPath);

    /// Decode WAV (built-in reader) or any format handled by audio_mixer_cpp / libsndfile (FLAC, MP3, …).
    [[nodiscard]] static bool readEncodedAudioFile(const QString& path,
                                                    QByteArray& audioData,
                                                    int& sampleRate,
                                                    int& channelCount);

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

    /// Apply mix-bus effect chain to an existing audio file (same formats as mix export / import).
    [[nodiscard]] static bool applyMixEffectChainToAudioFile(const QString& path,
                                                              const QJsonArray& chain);
};

#endif // AUDIOUTILS_H
