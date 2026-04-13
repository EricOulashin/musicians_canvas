#ifndef TRACKMIXERUNDOCOMMAND_H
#define TRACKMIXERUNDOCOMMAND_H

#include <QUndoCommand>
#include "trackdata.h"

class TrackWidget;

/// Undo/redo for per-track mixer fields (gain, pan, mute, solo, aux, trim).
class TrackMixerUndoCommand final : public QUndoCommand
{
public:
    TrackMixerUndoCommand(TrackWidget* widget, const TrackData& before, const TrackData& after,
                          QUndoCommand* parent = nullptr);

    void undo() override;
    void redo() override;

private:
    static void copyMixerFields(const TrackData& from, TrackData& to);
    TrackWidget* m_widget = nullptr;
    TrackData m_before;
    TrackData m_after;
};

#endif
