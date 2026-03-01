#include "mainwindow.h"
#include "themeutils.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Musician's Canvas");
    app.setOrganizationName("Eric Oulashin");

    ThemeUtils::applySavedTheme();

    MainWindow window;
    window.show();
    return app.exec();
}
