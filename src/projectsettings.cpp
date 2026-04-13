#include "projectsettings.h"

#include <algorithm>

double tempoBpmAtTime(const QVector<TempoMarker>& markers, double tSec, double defaultBpm)
{
    if (markers.isEmpty())
        return defaultBpm;
    double bpm = markers.first().bpm;
    for (const TempoMarker& m : markers)
    {
        if (m.timeSec <= tSec)
            bpm = m.bpm;
        else
            break;
    }
    return std::max(1.0, bpm);
}
