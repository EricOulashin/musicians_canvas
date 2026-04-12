#include "drumtrackgenerator.h"
#include "audioutils.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace DrumTrackGenerator
{

namespace
{
constexpr int kDrumChannel = 9;  // MIDI channel 10 (0-based) — GM drums

inline float sampleToFloat(qint16 s)
{
    return s / 32768.f;
}
}  // namespace

int estimateBpmFromInt16Pcm(const QByteArray& pcm, int sampleRate, int channelCount)
{
    if (channelCount <= 0 || sampleRate < 4000 || pcm.size() < channelCount * 2 * sampleRate)
        return 0;

    const int totalSamples = pcm.size() / (channelCount * 2);
    std::vector<float> mono(static_cast<size_t>(totalSamples));
    const auto* src = reinterpret_cast<const qint16*>(pcm.constData());
    for (int i = 0; i < totalSamples; ++i)
    {
        float sum = 0.f;
        for (int c = 0; c < channelCount; ++c)
            sum += sampleToFloat(src[i * channelCount + c]);
        mono[static_cast<size_t>(i)] = sum / static_cast<float>(channelCount);
    }

    constexpr int kFrame = 2048;
    constexpr int kHop   = 1024;
    const int nFrames    = (totalSamples - kFrame) / kHop;
    if (nFrames < 48)
        return 0;

    std::vector<float> rms(static_cast<size_t>(nFrames));
    for (int f = 0; f < nFrames; ++f)
    {
        const int base = f * kHop;
        double e = 0.0;
        for (int k = 0; k < kFrame && base + k < totalSamples; ++k)
        {
            const float v = mono[static_cast<size_t>(base + k)];
            e += static_cast<double>(v) * static_cast<double>(v);
        }
        rms[static_cast<size_t>(f)] = static_cast<float>(std::sqrt(e / static_cast<double>(kFrame)));
    }

    std::vector<float> onset(static_cast<size_t>(nFrames), 0.f);
    for (int i = 1; i < nFrames; ++i)
        onset[static_cast<size_t>(i)] = std::max(0.f, rms[static_cast<size_t>(i)]
                                                      - rms[static_cast<size_t>(i - 1)]);

    const double hopSec = static_cast<double>(kHop) / static_cast<double>(sampleRate);
    // ~30 BPM (2 s beat) to ~200 BPM (0.3 s beat)
    int minLag = std::max(2, static_cast<int>(std::floor(0.28 / hopSec)));
    int maxLag = std::min(nFrames - 2, static_cast<int>(std::ceil(2.0 / hopSec)));
    if (maxLag <= minLag)
        return 0;

    double bestCorr = 0.0;
    int    bestLag  = minLag;
    for (int lag = minLag; lag <= maxLag; ++lag)
    {
        double c = 0.0;
        for (int i = 0; i + lag < nFrames; ++i)
            c += static_cast<double>(onset[static_cast<size_t>(i)])
                 * static_cast<double>(onset[static_cast<size_t>(i + lag)]);
        if (c > bestCorr)
        {
            bestCorr = c;
            bestLag  = lag;
        }
    }
    if (bestCorr < 1e-12)
        return 0;

    double bpm = 60.0 / (bestLag * hopSec);
    // Prefer musically plausible band; halve if we locked onto eighth-note pulsation
    while (bpm > 175.0 && bpm * 0.5 >= 55.0)
        bpm *= 0.5;
    while (bpm < 55.0 && bpm * 2.0 <= 190.0)
        bpm *= 2.0;

    return static_cast<int>(std::lround(std::clamp(bpm, 40.0, 220.0)));
}

QByteArray mixEnabledAudioTracksForAnalysis(const QVector<TrackData>& tracks, int targetSampleRate)
{
    if (targetSampleRate <= 0)
        return {};

    double maxLenSec = 0.0;
    int    nAudio    = 0;
    for (const TrackData& t : tracks)
    {
        if (t.type != TrackType::Audio || !t.enabled || t.audioData.isEmpty())
            continue;
        ++nAudio;
        maxLenSec = std::max(maxLenSec, t.length());
    }
    if (nAudio == 0 || maxLenSec <= 0.0)
        return {};

    const int totalFrames = static_cast<int>(std::ceil(maxLenSec * targetSampleRate));
    if (totalFrames <= 0)
        return {};

    std::vector<double> acc(static_cast<size_t>(totalFrames) * 2u, 0.0);
    for (const TrackData& t : tracks)
    {
        if (t.type != TrackType::Audio || !t.enabled || t.audioData.isEmpty())
            continue;

        QByteArray pcm = t.audioData;
        int        sr  = t.sampleRate > 0 ? t.sampleRate : 44100;
        int        ch  = t.channelCount > 0 ? t.channelCount : 2;
        if (sr != targetSampleRate)
            pcm = AudioUtils::resampleInt16(pcm, sr, targetSampleRate, ch);
        if (ch != 2)
            pcm = AudioUtils::convertChannels(pcm, ch, 2);

        const int frames = pcm.size() / 4;
        const auto* s    = reinterpret_cast<const qint16*>(pcm.constData());
        const int n      = std::min(frames, totalFrames);
        for (int i = 0; i < n; ++i)
        {
            acc[static_cast<size_t>(i * 2)]
                += sampleToFloat(s[i * 2]) / static_cast<double>(nAudio);
            acc[static_cast<size_t>(i * 2 + 1)]
                += sampleToFloat(s[i * 2 + 1]) / static_cast<double>(nAudio);
        }
    }

    QByteArray out(totalFrames * 4, 0);
    auto*      dst = reinterpret_cast<qint16*>(out.data());
    for (int i = 0; i < totalFrames * 2; ++i)
        dst[i] = static_cast<qint16>(
            std::clamp(acc[static_cast<size_t>(i)] * 32767.0, -32768.0, 32767.0));
    return out;
}

QVector<MidiNote> createGroovePattern(double bpm, int bars)
{
    QVector<MidiNote> notes;
    if (bpm < 1.0 || bars < 1)
        return notes;

    const double beatDur = 60.0 / bpm;
    const double eighth  = beatDur * 0.5;
    const double hitDur  = std::min(0.08, beatDur * 0.2);
    constexpr int velKick  = 110;
    constexpr int velSnare = 105;
    constexpr int velHat   = 85;

    // GM drum notes: kick 36, snare 38, closed hi-hat 42
    const int kick = 36;
    const int snare = 38;
    const int hat   = 42;

    auto addHit = [&](double t, int note, int vel)
    {
        MidiNote m;
        m.note      = note;
        m.velocity  = vel;
        m.startTime = t;
        m.duration  = hitDur;
        m.channel   = kDrumChannel;
        notes.push_back(m);
    };

    for (int bar = 0; bar < bars; ++bar)
    {
        const double barOff = bar * 4.0 * beatDur;
        for (int beat = 0; beat < 4; ++beat)
        {
            const double b = barOff + beat * beatDur;
            // Eighth-note hi-hats; kick on 1 and 3, snare on 2 and 4 (1-based beat index)
            addHit(b, hat, velHat);
            addHit(b + eighth, hat, velHat - 8);
            if (beat == 0 || beat == 2)
                addHit(b, kick, velKick);
            if (beat == 1 || beat == 3)
                addHit(b, snare, velSnare);
        }
    }

    return notes;
}

double patternLengthSeconds(double bpm, int bars)
{
    if (bpm < 1.0 || bars < 1)
        return 0.0;
    return bars * 4.0 * (60.0 / bpm);
}

}  // namespace DrumTrackGenerator
