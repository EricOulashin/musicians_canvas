#ifndef AUDIOPCMLIMITS_H
#define AUDIOPCMLIMITS_H

#include <algorithm>
#include <cmath>

/// Helpers for normalized float samples in [-1, 1] before conversion to 16-bit PCM.
/// Used by real-time effect DSP and exposed via EffectWidget for consistency.
namespace AudioPcmLimits
{

/// Hard limit to ±1.0 (0 dBFS in normalized float space).
inline float hardLimitUnitFloat(float x)
{
    return std::max(-1.f, std::min(1.f, x));
}

/// Soft knee toward full scale to reduce harsh brick-wall clipping when converting to int16.
/// Peaks above \a kneeStart are compressed smoothly toward ±1.0.
inline float softLimitUnitFloat(float x, float kneeStart = 0.91f)
{
    const float ax = std::abs(x);
    if (ax <= kneeStart)
        return x;
    const float sign = (x < 0.f) ? -1.f : 1.f;
    const float room = 1.f - kneeStart;
    const float over = ax - kneeStart;
    const float shaped =
        kneeStart + room * std::tanh(over / std::max(room, 1e-6f));
    return sign * std::min(shaped, 1.f);
}

/// Recommended pipeline before int16: gentle limiting then hard clamp for safety.
inline float guardFloatSampleForInt16Pcm(float x)
{
    return hardLimitUnitFloat(softLimitUnitFloat(x));
}

}  // namespace AudioPcmLimits

#endif
