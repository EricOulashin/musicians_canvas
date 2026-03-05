#include "vk_mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Virtual MIDI Keyboard");

    VkMainWindow window;
    window.show();
    return app.exec();
}
