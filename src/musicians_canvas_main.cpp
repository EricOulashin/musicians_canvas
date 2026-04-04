#include "mainwindow.h"
#include "themeutils.h"
#include "audiostartup.h"
#include "appsettings.h"
#include "portaudiorecorder.h"
#include <QApplication>
#include <QMessageBox>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDir>

static QTranslator* s_translator = nullptr;
static QTranslator* s_qtTranslator = nullptr;

void loadAppTranslation(const QString& lang)
{
    auto* app = qApp;
    if (s_translator)
    {
        app->removeTranslator(s_translator);
        delete s_translator;
        s_translator = nullptr;
    }
    if (s_qtTranslator)
    {
        app->removeTranslator(s_qtTranslator);
        delete s_qtTranslator;
        s_qtTranslator = nullptr;
    }

    QString effectiveLang = lang;
    if (effectiveLang.isEmpty())
        effectiveLang = QLocale::system().name();

    // Load Qt's built-in translations (Yes/No/Cancel, etc.)
    auto* qtTr = new QTranslator(app);
    if (qtTr->load(QStringLiteral("qtbase_") + effectiveLang,
                   QLibraryInfo::path(QLibraryInfo::TranslationsPath))
        || qtTr->load(QStringLiteral("qtbase_") + effectiveLang.left(2),
                      QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
    {
        app->installTranslator(qtTr);
        s_qtTranslator = qtTr;
    }
    else
    {
        delete qtTr;
    }

    // Load app-specific translations
    auto* translator = new QTranslator(app);
    const QString baseName = QStringLiteral("musicians_canvas_") + effectiveLang;
    const QString trDir = QCoreApplication::applicationDirPath()
                          + QDir::separator() + QStringLiteral("translations");
    if (translator->load(baseName, trDir)
        || translator->load(baseName, QStringLiteral(":/translations"))
        || translator->load(QStringLiteral("musicians_canvas_") + effectiveLang.left(2), trDir)
        || translator->load(QStringLiteral("musicians_canvas_") + effectiveLang.left(2),
                            QStringLiteral(":/translations")))
    {
        app->installTranslator(translator);
        s_translator = translator;
    }
    else
    {
        delete translator;
    }
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Musician's Canvas");
    app.setOrganizationName("Eric Oulashin");

    // Load translation based on saved language setting or system locale
    loadAppTranslation(AppSettings::instance().language());

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

#if defined(HAVE_PORTAUDIO)
      {
          QString paInitErr;
          if (!PortAudioRecorder::initializeLibrary(&paInitErr))
          {
              QMessageBox::warning(
                  nullptr,
                  QObject::tr("PortAudio"),
                  QObject::tr("PortAudio could not be initialized. Recording will use Qt Multimedia "
                              "only.\n%1")
                      .arg(paInitErr));
          }
      }
#endif

    MainWindow window;
    window.show();
    const int ret = app.exec();

#if defined(HAVE_PORTAUDIO)
    PortAudioRecorder::terminateLibrary();
#endif

    // Release audio resources and (on Windows) restore default audio device access.
    AudioStartup::shutdown();
    return ret;
}
