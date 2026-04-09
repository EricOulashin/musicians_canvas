#ifndef MIDISYNTH_H
#define MIDISYNTH_H

#include <memory>
#include <QString>
#include <QVector>
#include "trackdata.h"

class MidiSynth
{
public:
    MidiSynth();
    ~MidiSynth();

    [[nodiscard]] bool loadSoundFont(const QString& path);
    [[nodiscard]] bool renderMidiToWav(const QVector<MidiNote>& notes, double lengthSeconds,
                         const QString& outputPath, int sampleRate = 44100,
                         const QString& soundFontPath = QString(),
                         double gainMultiplier = 1.0);
    [[nodiscard]] bool renderMidiFileToWav(const QString& midiPath, const QString& outputPath,
                            int sampleRate = 44100,
                            const QString& soundFontPath = QString());

private:
    class Impl;
    std::shared_ptr<Impl> m_impl;
};

#endif // MIDISYNTH_H
