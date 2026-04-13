#include "trackdata.h"

bool trackShouldMix(const TrackData& t, const QVector<TrackData>& allTracks)
{
    if (!t.enabled || t.mute)
        return false;
    bool anySolo = false;
    for (const TrackData& x : allTracks)
    {
        if (x.enabled && !x.mute && x.solo)
        {
            anySolo = true;
            break;
        }
    }
    if (anySolo && !t.solo)
        return false;
    return true;
}
