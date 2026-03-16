#include "vk_mainwindow.h"
#include <QApplication>

#ifdef HAVE_PIPEWIRE
#include <pipewire/pipewire.h>
#endif

int main(int argc, char* argv[])
{
    // Initialise PipeWire before any FluidSynth objects are created.
    // FluidSynth's native PipeWire audio driver calls pw_loop_new() internally,
    // which requires pw_init() to have been called first so that PipeWire can
    // locate its SPA plugins.  Without this call the driver fails with:
    //   "load lib: plugin directory undefined, set SPA_PLUGIN_DIR"
    //   "Failed to allocate PipeWire loop. Have you called pw_init() ?"
    // and FluidSynth falls back to PulseAudio or produces no audio at all.
#ifdef HAVE_PIPEWIRE
    pw_init(&argc, &argv);
#endif

    QApplication app(argc, argv);
    app.setApplicationName("Virtual MIDI Keyboard");

    VkMainWindow window;
    window.show();
    const int result = app.exec();

#ifdef HAVE_PIPEWIRE
    pw_deinit();
#endif
    return result;
}
