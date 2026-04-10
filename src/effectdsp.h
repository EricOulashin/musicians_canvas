#ifndef EFFECTDSP_H
#define EFFECTDSP_H

#include <QByteArray>
#include <QJsonArray>

/// Apply an ordered chain of effects (JSON from TrackData::audioEffectChain) to
/// interleaved 16-bit little-endian PCM. Processes in-place on \p pcm.
///
/// Channel handling: \p sampleRate should match the PCM (typically the project rate after
/// RecordingPostProcess). Mono (\p channelCount == 1) is run through the stereo processors as
/// dual-mono and mixed back to one channel. Stereo uses independent L/R dry paths; reverb feeds
/// a mono sum into the tail while preserving stereo dry. Additional channels beyond the first
/// two are left unchanged.
void applyAudioEffectChain(QByteArray& pcm, int channelCount, int sampleRate,
                           const QJsonArray& chain);

#endif
