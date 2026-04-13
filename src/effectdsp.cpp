#include "effectdsp.h"
#include "audiopcmlimits.h"

#include <QJsonObject>
#include <QJsonValue>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

namespace
{

inline float clampf(float x, float lo, float hi)
{
    return std::max(lo, std::min(hi, x));
}

inline float dbToLin(float db)
{
    return std::pow(10.f, db / 20.f);
}

/// Clamp to rates the rest of the pipeline supports; delay lines scale with sample rate in configure().
inline int normalizedSampleRate(int sampleRateIn)
{
    return std::clamp(sampleRateIn, 8000, 192000);
}

// ── Schroeder-style reverb (parallel combs + series allpass), stereo decorrelation ────────

class ReverbDsp
{
public:
    void configure(const QJsonObject& p, int sampleRateIn)
    {
        m_sr = normalizedSampleRate(sampleRateIn);
        m_preDelayMs     = float(p.value(QStringLiteral("preDelayMs")).toDouble(20.0));
        m_decaySec       = float(p.value(QStringLiteral("decayTimeSec")).toDouble(2.5));
        m_wetMix         = float(p.value(QStringLiteral("mix")).toDouble(0.35));
        m_tone           = float(p.value(QStringLiteral("tone")).toDouble(0.5));
        m_modDepthNorm   = float(p.value(QStringLiteral("modulationDepth")).toDouble(0.25));
        m_modRateHz      = float(p.value(QStringLiteral("modulationRateHz")).toDouble(0.35));

        m_wetMix       = clampf(m_wetMix, 0.f, 1.f);
        m_tone         = clampf(m_tone, 0.f, 1.f);
        m_modDepthNorm = clampf(m_modDepthNorm, 0.f, 1.f);
        m_modRateHz    = clampf(m_modRateHz, 0.05f, 5.f);
        m_decaySec     = clampf(m_decaySec, 0.1f, 30.f);
        m_preDelayMs   = clampf(m_preDelayMs, 0.f, 500.f);

        rebuild();
    }

    void reset()
    {
        m_lfoPhase = 0.f;
        for (auto& c : m_combs)
            std::fill(c.line.begin(), c.line.end(), 0.f);
        for (auto& a : m_allpass)
            std::fill(a.line.begin(), a.line.end(), 0.f);
        std::fill(m_pre.begin(), m_pre.end(), 0.f);
        m_write = 0;
    }

    void processFrame(float& l, float& r)
    {
        const float dryL = l;
        const float dryR = r;
        const float mono = (dryL + dryR) * 0.5f;

        const float delayedMono = preDelayReadWrite(mono);

        const float lfo = std::sin(m_lfoPhase);
        m_lfoPhase += 2.f * float(M_PI) * m_modRateHz / float(m_sr);
        if (m_lfoPhase > float(M_PI) * 2.f)
            m_lfoPhase -= float(M_PI) * 2.f;
        const float modSamples = lfo * m_modDepthSamples;

        float combSum = 0.f;
        for (std::size_t i = 0; i < m_combs.size(); ++i)
            combSum += m_combs[i].tick(delayedMono, modSamples * (0.35f + 0.15f * float(i)));

        float ap = combSum;
        for (auto& a : m_allpass)
            ap = a.tick(ap);

        m_toneLp += (ap - m_toneLp) * m_toneCoeff;
        const float wet = m_toneLp;

        // Light stereo width from comb output imbalance
        const float wetL = wet * (1.f + 0.08f * lfo);
        const float wetR = wet * (1.f - 0.08f * lfo);

        const float dryGain = 1.f - m_wetMix;
        const float wetGain = m_wetMix;
        l = dryGain * dryL + wetGain * wetL;
        r = dryGain * dryR + wetGain * wetR;
    }

private:
    struct Comb
    {
        std::vector<float> line;
        int                w = 0;
        int                baseDelay = 1000;
        float              g = 0.82f;

        float tick(float x, float modOff)
        {
            if (line.empty())
                return x;
            const int n = static_cast<int>(line.size());

            const float readPos =
                float(w) - float(baseDelay) - modOff;
            int i0 = int(std::floor(readPos));
            float frac = readPos - float(i0);
            while (i0 < 0)
                i0 += n;
            i0 %= n;
            int i1 = (i0 + 1) % n;

            const float yDel = line[i0] * (1.f - frac) + line[i1] * frac;

            line[w] = x + g * yDel;
            const float y = line[w];
            w = (w + 1) % n;
            return y;
        }
    };

    struct Allpass
    {
        std::vector<float> line;
        int                w = 0;
        int                d = 100;
        float              coeff = 0.5f;

        float tick(float x)
        {
            if (line.empty())
                return x;
            const int n = static_cast<int>(line.size());
            const int r = (w - d + n * 2) % n;
            const float bufOut = line[r];
            const float temp = x + coeff * bufOut;
            line[w] = temp;
            const float y = -coeff * temp + bufOut;
            w = (w + 1) % n;
            return y;
        }
    };

    void rebuild()
    {
        static const int kComb441[] = {1116, 1188, 1277, 1356};
        static const int kAp441[]   = {556, 441};

        const float scale = float(m_sr) / 44100.f;

        m_combs.resize(4);
        for (int i = 0; i < 4; ++i)
        {
            int d = std::max(32, int(std::lround(float(kComb441[i]) * scale)));
            m_combs[static_cast<std::size_t>(i)].baseDelay = d;
            m_combs[static_cast<std::size_t>(i)].line.assign(size_t(d + 8), 0.f);
            const float loopTimeSec =
                float(d) / float(m_sr);
            // Map user decay to feedback gain (stable, perceptual)
            const float t = std::max(0.05f, m_decaySec);
            float g = std::exp(-3.f * loopTimeSec / t);
            g = clampf(g, 0.1f, 0.93f);
            m_combs[static_cast<std::size_t>(i)].g = g;
        }

        m_allpass.resize(2);
        for (int i = 0; i < 2; ++i)
        {
            int d = std::max(16, int(std::lround(float(kAp441[i]) * scale)));
            m_allpass[static_cast<std::size_t>(i)].d = d;
            m_allpass[static_cast<std::size_t>(i)].coeff = 0.5f;
            m_allpass[static_cast<std::size_t>(i)].line.assign(size_t(d + 2), 0.f);
        }

        const int preSamples =
            std::clamp(int(std::lround(m_preDelayMs * float(m_sr) / 1000.f)), 0, m_sr * 2);
        m_pre.assign(size_t(preSamples + 2), 0.f);
        m_write = 0;

        // Tone: brighter knob increases smoothing follower bandwidth (more "air")
        const float fc =
            400.f + m_tone * 14000.f;  // Hz, lowpass of diffuse sum before wet mix
        m_toneCoeff =
            clampf(1.f - std::exp(-2.f * float(M_PI) * fc / float(m_sr)), 0.001f, 0.98f);
        m_toneLp = 0.f;

        m_modDepthSamples = m_modDepthNorm * float(6 + m_sr / 8000);  // ~ sub-sample to a few ms at depth=1

        reset();
    }

    float preDelayReadWrite(float x)
    {
        if (m_pre.empty())
            return x;
        const int n = static_cast<int>(m_pre.size());
        const int d = std::max(0, n - 2);
        const int r = (m_write - d + n * 8) % n;
        const float y = m_pre[r];
        m_pre[m_write] = x;
        m_write = (m_write + 1) % n;
        return y;
    }

    int           m_sr = 44100;
    float         m_preDelayMs = 20.f;
    float         m_decaySec = 2.5f;
    float         m_wetMix = 0.35f;
    float         m_tone = 0.5f;
    float         m_modDepthNorm = 0.25f;
    float         m_modRateHz = 0.35f;
    float         m_modDepthSamples = 1.f;
    float         m_lfoPhase = 0.f;
    std::vector<float> m_pre;
    int           m_write = 0;
    std::vector<Comb>  m_combs;
    std::vector<Allpass> m_allpass;
    float         m_toneLp = 0.f;
    float         m_toneCoeff = 0.1f;
};

// ── Stereo chorus (modulated delay) + simple wet EQ split ──────────────────────────────────

class ChorusDsp
{
public:
    void configure(const QJsonObject& p, int sampleRateIn)
    {
        m_sr = normalizedSampleRate(sampleRateIn);
        m_effectLevel = float(p.value(QStringLiteral("effectLevel")).toDouble(0.4));
        m_rateHz      = float(p.value(QStringLiteral("rateHz")).toDouble(0.45));
        m_depthMs     = float(p.value(QStringLiteral("depthMs")).toDouble(4.5));
        m_eqLowDb     = float(p.value(QStringLiteral("eqLowDb")).toDouble(0.0));
        m_eqHighDb    = float(p.value(QStringLiteral("eqHighDb")).toDouble(0.0));

        m_effectLevel = clampf(m_effectLevel, 0.f, 1.f);
        m_rateHz      = clampf(m_rateHz, 0.05f, 5.f);
        m_depthMs     = clampf(m_depthMs, 0.2f, 15.f);
        m_eqLowDb     = clampf(m_eqLowDb, -12.f, 12.f);
        m_eqHighDb    = clampf(m_eqHighDb, -12.f, 12.f);

        const int maxDelaySamples =
            std::clamp(int(std::lround(0.05f * float(m_sr))), m_sr / 200, m_sr * 2);
        m_bufL.assign(size_t(maxDelaySamples + 32), 0.f);
        m_bufR.assign(size_t(maxDelaySamples + 32), 0.f);
        m_write = 0;
        m_cap = static_cast<int>(m_bufL.size());

        m_baseDelaySamples =
            std::clamp(int(std::lround(0.018f * float(m_sr))), 64, m_cap - 16);
        m_depthSamples = m_depthMs * float(m_sr) / 1000.f;

        m_phaseL = 0.f;
        m_phaseR = float(M_PI) * 0.5f;

        m_lpWetL = m_lpWetR = 0.f;
    }

    void reset()
    {
        std::fill(m_bufL.begin(), m_bufL.end(), 0.f);
        std::fill(m_bufR.begin(), m_bufR.end(), 0.f);
        m_write = 0;
        m_phaseL = 0.f;
        m_phaseR = float(M_PI) * 0.5f;
        m_lpWetL = m_lpWetR = 0.f;
    }

    void processFrame(float& l, float& r)
    {
        const float dryL = l;
        const float dryR = r;

        m_bufL[static_cast<std::size_t>(m_write)] = dryL;
        m_bufR[static_cast<std::size_t>(m_write)] = dryR;

        m_phaseL += 2.f * float(M_PI) * m_rateHz / float(m_sr);
        m_phaseR += 2.f * float(M_PI) * m_rateHz / float(m_sr);
        if (m_phaseL > float(M_PI) * 2.f) m_phaseL -= float(M_PI) * 2.f;
        if (m_phaseR > float(M_PI) * 2.f) m_phaseR -= float(M_PI) * 2.f;

        const float dl = float(m_baseDelaySamples) + m_depthSamples * std::sin(m_phaseL);
        const float dr = float(m_baseDelaySamples) + m_depthSamples * std::sin(m_phaseR);

        float wetL = readFrac(m_bufL, dl);
        float wetR = readFrac(m_bufR, dr);

        // Wet EQ: split low vs high with one pole, apply separate gains (Boss-style tilt)
        const float lpCut = 1200.f;
        const float a = clampf(1.f - std::exp(-2.f * float(M_PI) * lpCut / float(m_sr)), 0.01f, 0.99f);
        m_lpWetL += (wetL - m_lpWetL) * a;
        m_lpWetR += (wetR - m_lpWetR) * a;
        const float hiL = wetL - m_lpWetL;
        const float hiR = wetR - m_lpWetR;
        wetL = m_lpWetL * dbToLin(m_eqLowDb) + hiL * dbToLin(m_eqHighDb);
        wetR = m_lpWetR * dbToLin(m_eqLowDb) + hiR * dbToLin(m_eqHighDb);

        const float dryW = 1.f - m_effectLevel;
        const float wetW = m_effectLevel;
        l = dryW * dryL + wetW * wetL;
        r = dryW * dryR + wetW * wetR;

        m_write = (m_write + 1) % m_cap;
    }

private:
    float readFrac(std::vector<float>& buf, float delaySamples) const
    {
        if (delaySamples < 1.f)
            delaySamples = 1.f;
        if (delaySamples > float(m_cap - 2))
            delaySamples = float(m_cap - 2);
        const float readPos = float(m_write) - delaySamples;
        int i0 = int(std::floor(readPos));
        float frac = readPos - float(i0);
        while (i0 < 0)
            i0 += m_cap;
        i0 %= m_cap;
        const int i1 = (i0 + 1) % m_cap;
        return buf[static_cast<std::size_t>(i0)] * (1.f - frac)
            + buf[static_cast<std::size_t>(i1)] * frac;
    }

    int                 m_sr = 44100;
    float               m_effectLevel = 0.4f;
    float               m_rateHz = 0.45f;
    float               m_depthMs = 4.5f;
    float               m_eqLowDb = 0.f;
    float               m_eqHighDb = 0.f;
    std::vector<float> m_bufL;
    std::vector<float> m_bufR;
    int                 m_cap = 0;
    int                 m_write = 0;
    int                 m_baseDelaySamples = 800;
    float               m_depthSamples = 80.f;
    float               m_phaseL = 0.f;
    float               m_phaseR = 0.f;
    float               m_lpWetL = 0.f;
    float               m_lpWetR = 0.f;
};

// ── Stereo flanger (modulated delay + feedback) ─────────────────────────────────────────────

class FlangeDsp
{
public:
    void configure(const QJsonObject& p, int sampleRateIn)
    {
        m_sr = normalizedSampleRate(sampleRateIn);
        m_effectLevel = float(p.value(QStringLiteral("effectLevel")).toDouble(0.45));
        m_manualDelayMs = float(p.value(QStringLiteral("manualDelayMs")).toDouble(2.0));
        m_rateHz        = float(p.value(QStringLiteral("rateHz")).toDouble(0.42));
        m_depthMs       = float(p.value(QStringLiteral("depthMs")).toDouble(2.0));
        m_feedback      = float(p.value(QStringLiteral("feedbackPercent")).toDouble(35.0)) / 100.f;

        m_effectLevel   = clampf(m_effectLevel, 0.f, 1.f);
        m_manualDelayMs = clampf(m_manualDelayMs, 0.3f, 12.f);
        m_rateHz        = clampf(m_rateHz, 0.02f, 5.f);
        m_depthMs       = clampf(m_depthMs, 0.1f, 8.f);
        m_feedback      = clampf(m_feedback, -0.95f, 0.95f);

        const int maxD =
            std::clamp(int(std::lround(0.022f * float(m_sr))), m_sr / 128, m_sr * 2);
        m_bufL.assign(size_t(maxD + 32), 0.f);
        m_bufR.assign(size_t(maxD + 32), 0.f);
        m_cap   = static_cast<int>(m_bufL.size());
        m_write = 0;

        m_manualSamples = m_manualDelayMs * float(m_sr) / 1000.f;
        m_depthSamples  = m_depthMs * float(m_sr) / 1000.f;
        m_phaseL = 0.f;
        m_phaseR = float(M_PI) * 0.5f;
        reset();
    }

    void reset()
    {
        std::fill(m_bufL.begin(), m_bufL.end(), 0.f);
        std::fill(m_bufR.begin(), m_bufR.end(), 0.f);
        m_write = 0;
        m_phaseL = 0.f;
        m_phaseR = float(M_PI) * 0.5f;
    }

    void processFrame(float& l, float& r)
    {
        const float dryL = l;
        const float dryR = r;

        m_phaseL += 2.f * float(M_PI) * m_rateHz / float(m_sr);
        m_phaseR += 2.f * float(M_PI) * m_rateHz / float(m_sr);
        if (m_phaseL > float(M_PI) * 2.f)
            m_phaseL -= float(M_PI) * 2.f;
        if (m_phaseR > float(M_PI) * 2.f)
            m_phaseR -= float(M_PI) * 2.f;

        float dl = m_manualSamples + m_depthSamples * std::sin(m_phaseL);
        float dr = m_manualSamples + m_depthSamples * std::sin(m_phaseR);
        dl       = std::max(1.f, dl);
        dr       = std::max(1.f, dr);

        const float delL = readFrac(m_bufL, dl);
        const float delR = readFrac(m_bufR, dr);

        const float flangeL = dryL + delL;
        const float flangeR = dryR + delR;

        m_bufL[static_cast<std::size_t>(m_write)] = dryL + m_feedback * delL;
        m_bufR[static_cast<std::size_t>(m_write)] = dryR + m_feedback * delR;
        m_write                                   = (m_write + 1) % m_cap;

        const float dryW = 1.f - m_effectLevel;
        const float wetW = m_effectLevel;
        l                = dryW * dryL + wetW * flangeL;
        r                = dryW * dryR + wetW * flangeR;
    }

private:
    float readFrac(std::vector<float>& buf, float delaySamples) const
    {
        if (delaySamples < 1.f)
            delaySamples = 1.f;
        if (delaySamples > float(m_cap - 2))
            delaySamples = float(m_cap - 2);
        const float readPos = float(m_write) - delaySamples;
        int         i0      = int(std::floor(readPos));
        float       frac    = readPos - float(i0);
        while (i0 < 0)
            i0 += m_cap;
        i0 %= m_cap;
        const int i1 = (i0 + 1) % m_cap;
        return buf[static_cast<std::size_t>(i0)] * (1.f - frac)
            + buf[static_cast<std::size_t>(i1)] * frac;
    }

    int                 m_sr = 44100;
    float               m_effectLevel = 0.45f;
    float               m_manualDelayMs = 2.f;
    float               m_rateHz = 0.42f;
    float               m_depthMs = 2.f;
    float               m_feedback = 0.35f;
    std::vector<float> m_bufL;
    std::vector<float> m_bufR;
    int                 m_cap = 0;
    int                 m_write = 0;
    float               m_manualSamples = 80.f;
    float               m_depthSamples = 80.f;
    float               m_phaseL = 0.f;
    float               m_phaseR = 0.f;
};

// ── Overdrive / distortion (soft vs hard clip blend + tone) ───────────────────────────────

class OverdriveDistortionDsp
{
public:
    void configure(const QJsonObject& p, int sampleRateIn)
    {
        m_sr = normalizedSampleRate(sampleRateIn);
        m_drive = float(p.value(QStringLiteral("drive")).toDouble(0.45));
        m_color = float(p.value(QStringLiteral("color")).toDouble(0.5));
        m_tone  = float(p.value(QStringLiteral("tone")).toDouble(0.5));
        m_mix   = float(p.value(QStringLiteral("effectLevel")).toDouble(0.65));

        m_drive = clampf(m_drive, 0.02f, 1.f);
        m_color = clampf(m_color, 0.f, 1.f);
        m_tone  = clampf(m_tone, 0.f, 1.f);
        m_mix   = clampf(m_mix, 0.f, 1.f);

        const float fc = 350.f + m_tone * 9500.f;
        m_toneLpA =
            clampf(1.f - std::exp(-2.f * float(M_PI) * fc / float(m_sr)), 0.002f, 0.99f);
        reset();
    }

    void reset()
    {
        m_lpPostL = m_lpPostR = 0.f;
    }

    void processFrame(float& l, float& r)
    {
        const float dryL = l;
        const float dryR = r;

        auto wetChannel = [&](float x, float& lpPost)
        {
            const float in = x * (0.18f + m_drive * 22.f);
            const float od = std::tanh(in * 1.32f);
            float ds       = in * 3.8f;
            ds             = std::tanh(std::clamp(ds, -4.f, 4.f));
            float y        = (1.f - m_color) * od + m_color * ds;
            lpPost += (y - lpPost) * m_toneLpA;
            const float hi = y - lpPost;
            const float lowGain  = 0.22f + 0.78f * (1.f - m_tone);
            const float highGain = 0.12f + 0.88f * m_tone;
            return lpPost * lowGain + hi * highGain;
        };

        const float wetL = wetChannel(dryL, m_lpPostL);
        const float wetR = wetChannel(dryR, m_lpPostR);

        const float dryW = 1.f - m_mix;
        const float wetW = m_mix;
        l                = dryW * dryL + wetW * wetL;
        r                = dryW * dryR + wetW * wetR;
    }

private:
    int   m_sr = 44100;
    float m_drive = 0.45f;
    float m_color = 0.5f;
    float m_tone  = 0.5f;
    float m_mix   = 0.65f;
    float m_toneLpA = 0.08f;
    float m_lpPostL = 0.f;
    float m_lpPostR = 0.f;
};

// ── Amp + cabinet modeling (voicing curve, EQ, cab rolloff, air) ─────────────────────────

class AmpCabinetModelDsp
{
public:
    void configure(const QJsonObject& p, int sampleRateIn)
    {
        m_sr = normalizedSampleRate(sampleRateIn);
        m_ampModel = std::clamp(p.value(QStringLiteral("ampModel")).toInt(2), 0, 5);
        m_cabModel = std::clamp(p.value(QStringLiteral("cabinetModel")).toInt(1), 0, 5);
        m_gain     = float(p.value(QStringLiteral("gain")).toDouble(0.45));
        m_bass     = float(p.value(QStringLiteral("bass")).toDouble(0.5));
        m_mid      = float(p.value(QStringLiteral("mid")).toDouble(0.5));
        m_treble   = float(p.value(QStringLiteral("treble")).toDouble(0.5));
        m_air      = float(p.value(QStringLiteral("air")).toDouble(0.35));
        m_mix      = float(p.value(QStringLiteral("effectLevel")).toDouble(0.75));

        m_gain   = clampf(m_gain, 0.01f, 1.f);
        m_bass   = clampf(m_bass, 0.f, 1.f);
        m_mid    = clampf(m_mid, 0.f, 1.f);
        m_treble = clampf(m_treble, 0.f, 1.f);
        m_air    = clampf(m_air, 0.f, 1.f);
        m_mix    = clampf(m_mix, 0.f, 1.f);

        static const float kShape[6] = {0.95f, 1.15f, 1.38f, 1.58f, 1.92f, 1.48f};
        m_shape                        = kShape[static_cast<std::size_t>(m_ampModel)];

        static const float kCabFc[6] = {9000.f, 7800.f, 6500.f, 5200.f, 4800.f, 14000.f};
        const float cabFc            = kCabFc[static_cast<std::size_t>(m_cabModel)];
        m_cabLpA =
            clampf(1.f - std::exp(-2.f * float(M_PI) * cabFc / float(m_sr)), 0.01f, 0.99f);

        const float bassFc  = 120.f + m_bass * 280.f;
        const float midFc   = 400.f + m_mid * 2200.f;
        const float trebFc  = 2000.f + m_treble * 8000.f;
        m_bassLpA  = clampf(1.f - std::exp(-2.f * float(M_PI) * bassFc / float(m_sr)), 0.01f, 0.95f);
        m_midLpA   = clampf(1.f - std::exp(-2.f * float(M_PI) * midFc / float(m_sr)), 0.01f, 0.95f);
        m_trebLpA  = clampf(1.f - std::exp(-2.f * float(M_PI) * trebFc / float(m_sr)), 0.01f, 0.95f);
        m_airLpA   = clampf(1.f - std::exp(-2.f * float(M_PI) * 9000.f / float(m_sr)), 0.02f, 0.95f);
        m_airAmt   = m_air * 0.55f;

        m_bassDb   = (m_bass - 0.5f) * 14.f;
        m_midDb    = (m_mid - 0.5f) * 12.f;
        m_trebleDb = (m_treble - 0.5f) * 14.f;

        reset();
    }

    void reset()
    {
        m_hpL = m_hpR = 0.f;
        m_lpBL = m_lpBR = m_lpML = m_lpMR = m_lpTL = m_lpTR = 0.f;
        m_cLpL = m_cLpR = 0.f;
        m_airLpL = m_airLpR = 0.f;
    }

    void processFrame(float& l, float& r)
    {
        const float dryL = l;
        const float dryR = r;

        auto process = [&](float x, float& hp, float& lpB, float& lpM, float& lpT, float& cLp,
                           float& airLp) -> float
        {
            const float pre = x * (0.12f + m_gain * 14.f);
            hp += (pre - hp) * 0.12f;
            float w = pre - hp * (0.35f + float(m_ampModel) * 0.04f);

            lpB += (w - lpB) * m_bassLpA;
            lpM += (w - lpM) * m_midLpA;
            lpT += (w - lpT) * m_trebLpA;
            const float low  = lpB;
            const float midb = lpM - lpB * 0.55f;
            const float high = w - lpT;

            w = low * dbToLin(m_bassDb) * 0.45f + midb * dbToLin(m_midDb) * 0.9f
                + high * dbToLin(m_trebleDb) * 0.55f + w * 0.25f;

            w = std::tanh(w * m_shape);

            cLp += (w - cLp) * m_cabLpA;
            w = cLp;

            airLp += (w - airLp) * m_airLpA;
            const float airHi = w - airLp;
            w += airHi * m_airAmt;
            return w;
        };

        const float wetL = process(l, m_hpL, m_lpBL, m_lpML, m_lpTL, m_cLpL, m_airLpL);
        const float wetR = process(r, m_hpR, m_lpBR, m_lpMR, m_lpTR, m_cLpR, m_airLpR);

        const float dryW = 1.f - m_mix;
        const float wetW = m_mix;
        l                = dryW * dryL + wetW * wetL;
        r                = dryW * dryR + wetW * wetR;
    }

private:
    int   m_sr = 44100;
    int   m_ampModel = 2;
    int   m_cabModel = 1;
    float m_gain = 0.45f;
    float m_bass = 0.5f;
    float m_mid = 0.5f;
    float m_treble = 0.5f;
    float m_air = 0.35f;
    float m_mix = 0.75f;
    float m_shape = 1.4f;
    float m_cabLpA = 0.2f;
    float m_bassLpA = 0.1f;
    float m_midLpA = 0.1f;
    float m_trebLpA = 0.1f;
    float m_airLpA = 0.2f;
    float m_airAmt = 0.2f;
    float m_bassDb = 0.f;
    float m_midDb = 0.f;
    float m_trebleDb = 0.f;

    float m_hpL = 0.f;
    float m_hpR = 0.f;
    float m_lpBL = 0.f;
    float m_lpBR = 0.f;
    float m_lpML = 0.f;
    float m_lpMR = 0.f;
    float m_lpTL = 0.f;
    float m_lpTR = 0.f;
    float m_cLpL = 0.f;
    float m_cLpR = 0.f;
    float m_airLpL = 0.f;
    float m_airLpR = 0.f;
};

// ── Fender-style "Vibrato" (tremolo): amplitude modulation ────────────────────────────────

class VibratoTremoloDsp
{
public:
    void configure(const QJsonObject& p, int sampleRateIn)
    {
        m_sr = normalizedSampleRate(sampleRateIn);
        m_speedHz   = float(p.value(QStringLiteral("speedHz")).toDouble(3.5));
        m_intensity = float(p.value(QStringLiteral("intensity")).toDouble(0.35));

        m_speedHz   = clampf(m_speedHz, 0.05f, 12.f);
        m_intensity = clampf(m_intensity, 0.f, 1.f);
        reset();
    }

    void reset()
    {
        m_phase = 0.f;
    }

    void processFrame(float& l, float& r)
    {
        // Mod factor maps intensity to 1..(1-intensity) at LFO trough, and 1 at peak.
        const float s = std::sin(m_phase);
        m_phase += 2.f * float(M_PI) * m_speedHz / float(m_sr);
        if (m_phase > float(M_PI) * 2.f)
            m_phase -= float(M_PI) * 2.f;

        const float u = 0.5f * (1.f + s);                // 0..1
        const float g = (1.f - m_intensity) + m_intensity * u; // (1-intensity)..1
        l *= g;
        r *= g;
    }

private:
    int   m_sr = 44100;
    float m_speedHz = 3.5f;
    float m_intensity = 0.35f;
    float m_phase = 0.f;
};

// ── Chain runner ──────────────────────────────────────────────────────────────────────────

void applyChainImpl(QByteArray& pcm, int channelCount, int sampleRate, const QJsonArray& chain)
{
    if (pcm.isEmpty() || channelCount <= 0 || sampleRate <= 0 || chain.isEmpty())
        return;

    const int bytesPerFrame = channelCount * static_cast<int>(sizeof(qint16));
    if (pcm.size() % bytesPerFrame != 0)
        return;

    std::vector<ReverbDsp> reverbs;
    std::vector<ChorusDsp> choruses;
    std::vector<FlangeDsp> flangers;
    std::vector<OverdriveDistortionDsp> odDists;
    std::vector<AmpCabinetModelDsp> ampCabs;
    std::vector<VibratoTremoloDsp> vibratos;
    reverbs.reserve(size_t(chain.size()));
    choruses.reserve(size_t(chain.size()));
    flangers.reserve(size_t(chain.size()));
    odDists.reserve(size_t(chain.size()));
    ampCabs.reserve(size_t(chain.size()));
    vibratos.reserve(size_t(chain.size()));
    enum class Kind { Reverb, Chorus, Flanger, OverdriveDistortion, AmpCabinet, Vibrato };
    std::vector<Kind> kinds;
    kinds.reserve(size_t(chain.size()));

    for (const QJsonValue& v : chain)
    {
        if (!v.isObject())
            continue;
        const QJsonObject o = v.toObject();
        const QString type = o.value(QStringLiteral("type")).toString();
        const QJsonObject params = o.value(QStringLiteral("params")).toObject();
        if (type == QStringLiteral("reverb"))
        {
            ReverbDsp rv;
            rv.configure(params, sampleRate);
            reverbs.push_back(std::move(rv));
            kinds.push_back(Kind::Reverb);
        }
        else if (type == QStringLiteral("chorus"))
        {
            ChorusDsp ch;
            ch.configure(params, sampleRate);
            choruses.push_back(std::move(ch));
            kinds.push_back(Kind::Chorus);
        }
        else if (type == QStringLiteral("flanger"))
        {
            FlangeDsp fg;
            fg.configure(params, sampleRate);
            flangers.push_back(std::move(fg));
            kinds.push_back(Kind::Flanger);
        }
        else if (type == QStringLiteral("overdrive_distortion"))
        {
            OverdriveDistortionDsp od;
            od.configure(params, sampleRate);
            odDists.push_back(std::move(od));
            kinds.push_back(Kind::OverdriveDistortion);
        }
        else if (type == QStringLiteral("amp_cabinet"))
        {
            AmpCabinetModelDsp ac;
            ac.configure(params, sampleRate);
            ampCabs.push_back(std::move(ac));
            kinds.push_back(Kind::AmpCabinet);
        }
        else if (type == QStringLiteral("vibrato"))
        {
            VibratoTremoloDsp vb;
            vb.configure(params, sampleRate);
            vibratos.push_back(std::move(vb));
            kinds.push_back(Kind::Vibrato);
        }
    }

    if (kinds.empty())
        return;

    // Reset all processors before a fresh pass
    size_t ir = 0;
    size_t ic = 0;
    size_t iz = 0;
    size_t io = 0;
    size_t ia = 0;
    size_t iv = 0;
    for (Kind k : kinds)
    {
        if (k == Kind::Reverb)
            reverbs[ir++].reset();
        else if (k == Kind::Chorus)
            choruses[ic++].reset();
        else if (k == Kind::Flanger)
            flangers[iz++].reset();
        else if (k == Kind::OverdriveDistortion)
            odDists[io++].reset();
        else if (k == Kind::AmpCabinet)
            ampCabs[ia++].reset();
        else
            vibratos[iv++].reset();
    }

    ir = 0;
    ic = 0;
    iz = 0;
    io = 0;
    ia = 0;
    iv = 0;

    auto* s = reinterpret_cast<qint16*>(pcm.data());
    const int frameCount = pcm.size() / bytesPerFrame;

    if (channelCount == 1)
    {
        for (int f = 0; f < frameCount; ++f)
        {
            float x = s[f] / 32768.f;
            float l = x;
            float r = x;
            ir = 0;
            ic = 0;
            iz = 0;
            io = 0;
            ia = 0;
            iv = 0;
            for (Kind k : kinds)
            {
                if (k == Kind::Reverb)
                    reverbs[ir++].processFrame(l, r);
                else if (k == Kind::Chorus)
                    choruses[ic++].processFrame(l, r);
                else if (k == Kind::Flanger)
                    flangers[iz++].processFrame(l, r);
                else if (k == Kind::OverdriveDistortion)
                    odDists[io++].processFrame(l, r);
                else if (k == Kind::AmpCabinet)
                    ampCabs[ia++].processFrame(l, r);
                else
                    vibratos[iv++].processFrame(l, r);
            }
            x = (l + r) * 0.5f;
            x = AudioPcmLimits::guardFloatSampleForInt16Pcm(x);
            s[f] = static_cast<qint16>(std::lround(x * 32767.f));
        }
    }
    else
    {
        for (int f = 0; f < frameCount; ++f)
        {
            const int b = f * channelCount;
            float l = s[b] / 32768.f;
            float r = s[b + 1] / 32768.f;
            ir = 0;
            ic = 0;
            iz = 0;
            io = 0;
            ia = 0;
            iv = 0;
            for (Kind k : kinds)
            {
                if (k == Kind::Reverb)
                    reverbs[ir++].processFrame(l, r);
                else if (k == Kind::Chorus)
                    choruses[ic++].processFrame(l, r);
                else if (k == Kind::Flanger)
                    flangers[iz++].processFrame(l, r);
                else if (k == Kind::OverdriveDistortion)
                    odDists[io++].processFrame(l, r);
                else if (k == Kind::AmpCabinet)
                    ampCabs[ia++].processFrame(l, r);
                else
                    vibratos[iv++].processFrame(l, r);
            }
            l = AudioPcmLimits::guardFloatSampleForInt16Pcm(l);
            r = AudioPcmLimits::guardFloatSampleForInt16Pcm(r);
            s[b]     = static_cast<qint16>(std::lround(l * 32767.f));
            s[b + 1] = static_cast<qint16>(std::lround(r * 32767.f));
            // Additional channels, if any, are left unchanged (this app is typically mono/stereo).
        }
    }
}

}  // namespace

void applyAudioEffectChain(QByteArray& pcm, int channelCount, int sampleRate, const QJsonArray& chain)
{
    applyChainImpl(pcm, channelCount, sampleRate, chain);
}
