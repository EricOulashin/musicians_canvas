#include "mainwindow.h"
#include "themeutils.h"
#include "audiostartup.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Musician's Canvas");
    app.setOrganizationName("Eric Oulashin");

    ThemeUtils::applySavedTheme();

    // Initialise the platform audio subsystem for low-latency operation.
    // On Windows this verifies that at least one ASIO driver is installed;
    // on Linux it raises the process scheduling priority.
    QString audioError;
    if (!AudioStartup::initialize(audioError))
    {
        QMessageBox::critical(nullptr,
            QObject::tr("Audio System Error"),
            audioError);
        return 1;
    }

    MainWindow window;
    window.show();
    const int ret = app.exec();

    // Release audio resources and (on Windows) restore default audio device access.
    AudioStartup::shutdown();
    return ret;
}
