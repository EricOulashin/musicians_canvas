#include "audioutils.h"
#include "midisynth.h"
#include <QFile>
#include <QDir>
#include <QTemporaryFile>
#include <algorithm>

static void writeWavHeader(QFile& file, quint32 sampleRate, quint32 numSamples) {
    quint32 byteRate = sampleRate * 4;
    quint32 dataSize = numSamples * 4;
    quint32 fileSize = 36 + dataSize;

    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&fileSize), 4);
    file.write("WAVE", 4);
    file.write("fmt ", 4);
    quint32 fmtSize = 16;
    file.write(reinterpret_cast<const char*>(&fmtSize), 4);
    quint16 audioFormat = 1;
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    quint16 numChannels = 2;
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    quint16 blockAlign = 4;
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    quint16 bitsPerSample = 16;
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);
    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);
}

bool AudioUtils::mixTracksToWav(const QVector<TrackData>& tracks,
                                const QString& outputPath,
                                int sampleRate) {
    double maxLength = 0;
    for (const auto& t : tracks) {
        if (!t.enabled) continue;
        double len = t.length();
        if (len > maxLength) maxLength = len;
    }
    if (maxLength <= 0) maxLength = 1.0;

    const int totalFrames = static_cast<int>(maxLength * sampleRate);
    const int totalSamples = totalFrames * 2;
    QVector<float> mixLeft(totalFrames, 0.0f);
    QVector<float> mixRight(totalFrames, 0.0f);

    for (const auto& track : tracks) {
        if (!track.enabled) continue;

        if (track.type == TrackType::Audio) {
            int trackFrames = track.audioData.size() / (track.channelCount * 2);
            const qint16* src = reinterpret_cast<const qint16*>(track.audioData.constData());
            float scale = track.sampleRate == sampleRate ? 1.0f : 1.0f;
            for (int i = 0; i < qMin(trackFrames, totalFrames); ++i) {
                float l = src[i * track.channelCount] / 32768.0f;
                float r = track.channelCount > 1 ? src[i * track.channelCount + 1] / 32768.0f : l;
                mixLeft[i] += l * scale;
                mixRight[i] += r * scale;
            }
        } else if (track.type == TrackType::MIDI && !track.midiNotes.isEmpty()) {
            QTemporaryFile tmpWav(QDir::temp().filePath("mc_mix_XXXXXX.wav"));
            tmpWav.setAutoRemove(true);
            if (!tmpWav.open()) continue;
            const QString tmpPath = tmpWav.fileName();
            tmpWav.close();

            MidiSynth synth;
            if (synth.renderMidiToWav(track.midiNotes, track.lengthSeconds, tmpPath, sampleRate)) {
                if (QFile f(tmpPath); f.open(QIODevice::ReadOnly)) {
                    f.seek(44);
                    const QByteArray data = f.readAll();
                    const qint16* src = reinterpret_cast<const qint16*>(data.constData());
                    const int frames = data.size() / 4;
                    for (int i = 0; i < qMin(frames, totalFrames); ++i) {
                        mixLeft[i] += src[i * 2] / 32768.0f;
                        mixRight[i] += src[i * 2 + 1] / 32768.0f;
                    }
                }
            }
        }
    }

    float maxVal = 0;
    for (int i = 0; i < totalFrames; ++i) {
        float m = qMax(qAbs(mixLeft[i]), qAbs(mixRight[i]));
        if (m > maxVal) maxVal = m;
    }
    float gain = (maxVal > 0.9f) ? (0.9f / maxVal) : 1.0f;

    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly)) return false;
    writeWavHeader(file, sampleRate, totalSamples / 2);
    for (int i = 0; i < totalFrames; ++i) {
        constexpr float scale = 32767.0f;
        constexpr float minVal = -32768.0f;
        qint16 l = static_cast<qint16>(std::clamp(mixLeft[i] * gain * scale, minVal, scale));
        qint16 r = static_cast<qint16>(std::clamp(mixRight[i] * gain * scale, minVal, scale));
        file.write(reinterpret_cast<const char*>(&l), 2);
        file.write(reinterpret_cast<const char*>(&r), 2);
    }
    file.close();
    return true;
}
