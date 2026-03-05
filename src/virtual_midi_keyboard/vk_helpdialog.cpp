#include "vk_helpdialog.h"
#include <QVBoxLayout>
#include <QLabel>

VkHelpDialog::VkHelpDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle(tr("Using this application"));
    setMinimumSize(450, 300);
    resize(500, 350);

    auto* layout = new QVBoxLayout(this);

    auto* pianoLabel = new QLabel(tr(
        "<h3>Playing the piano keyboard</h3>"
        "<p>You can play notes using either the mouse or the computer keyboard.</p>"
        "<p><b>Mouse:</b> Click and hold on a piano key to play a note. The note will "
        "continue playing while the left mouse button is held down. Release the mouse "
        "button to stop the note.</p>"
        "<p><b>Keyboard:</b> Press and release the computer keyboard keys mapped to "
        "piano keys. The keys are mapped similarly to a piano - for example, the "
        "A through L keys play the white keys (C major scale), and the W, E, T, Y, U "
        "keys play the black keys (sharps/flats). The octave can be adjusted using "
        "the Octave control in the toolbar.</p>"));
    pianoLabel->setWordWrap(true);
    layout->addWidget(pianoLabel);

    auto* configLabel = new QLabel(tr(
        "<h3>Configuration</h3>"
        "<p>Use Settings → Configuration to configure the application:</p>"
        "<ul>"
        "<li><b>MIDI tab:</b> Select the MIDI output device (physical port or built-in "
        "software synthesizer), MIDI input device for forwarding external MIDI, and "
        "SoundFont file for the software synthesizer.</li>"
        "<li><b>Audio tab:</b> Select the audio output device used when the built-in "
        "software synthesizer is selected.</li>"
        "</ul>"));
    configLabel->setWordWrap(true);
    layout->addWidget(configLabel);

    layout->addStretch();
}
