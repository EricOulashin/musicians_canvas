#include "trackmixerundocommand.h"
#include "trackwidget.h"

TrackMixerUndoCommand::TrackMixerUndoCommand(TrackWidget* widget, const TrackData& before,
                                             const TrackData& after, QUndoCommand* parent)
    : QUndoCommand(parent), m_widget(widget), m_before(before), m_after(after)
{
}

void TrackMixerUndoCommand::copyMixerFields(const TrackData& from, TrackData& to)
{
    to.gainDb = from.gainDb;
    to.pan = from.pan;
    to.mute = from.mute;
    to.solo = from.solo;
    to.auxSend = from.auxSend;
    to.trimStartSec = from.trimStartSec;
    to.trimEndSec = from.trimEndSec;
}

void TrackMixerUndoCommand::undo()
{
    if (!m_widget)
        return;
    TrackData d = m_widget->trackData();
    copyMixerFields(m_before, d);
    m_widget->setTrackData(d);
}

void TrackMixerUndoCommand::redo()
{
    if (!m_widget)
        return;
    TrackData d = m_widget->trackData();
    copyMixerFields(m_after, d);
    m_widget->setTrackData(d);
}
