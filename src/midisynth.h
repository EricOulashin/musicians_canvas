#ifndef MIDISYNTH_H
#define MIDISYNTH_H

#include <QString>
#include <QVector>
#include "trackdata.h"

class MidiSynth {
public:
    MidiSynth();
    ~MidiSynth();

    [[nodiscard]] bool loadSoundFont(const QString& path);
    [[nodiscard]] bool renderMidiToWav(const QVector<MidiNote>& notes, double lengthSeconds,
                         const QString& outputPath, int sampleRate = 44100);
    [[nodiscard]] bool renderMidiFileToWav(const QString& midiPath, const QString& outputPath,
                            int sampleRate = 44100);

private:
    class Impl;
    Impl* m_impl = nullptr;
};

#endif // MIDISYNTH_H
