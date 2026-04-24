#include "vk_mainwindow.h"
#include "vk_settings.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QDir>
#include <QByteArray>

#ifdef HAVE_PIPEWIRE
#include <pipewire/pipewire.h>
#endif

static QTranslator* s_vkTranslator = nullptr;
static QTranslator* s_vkQtTranslator = nullptr;

void loadVkTranslation(const QString& lang)
{
    auto* app = qApp;
    if (s_vkTranslator)
    {
        app->removeTranslator(s_vkTranslator);
        delete s_vkTranslator;
        s_vkTranslator = nullptr;
    }
    if (s_vkQtTranslator)
    {
        app->removeTranslator(s_vkQtTranslator);
        delete s_vkQtTranslator;
        s_vkQtTranslator = nullptr;
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
        s_vkQtTranslator = qtTr;
    }
    else
    {
        delete qtTr;
    }

    // Load app-specific translations
    auto* translator = new QTranslator(app);
    const QString baseName = QStringLiteral("virtual_midi_keyboard_") + effectiveLang;
    const QString trDir = QCoreApplication::applicationDirPath()
                          + QDir::separator() + QStringLiteral("translations");
    if (translator->load(baseName, trDir)
        || translator->load(baseName, QStringLiteral(":/translations"))
        || translator->load(QStringLiteral("virtual_midi_keyboard_") + effectiveLang.left(2), trDir)
        || translator->load(QStringLiteral("virtual_midi_keyboard_") + effectiveLang.left(2),
                            QStringLiteral(":/translations")))
    {
        app->installTranslator(translator);
        s_vkTranslator = translator;
    }
    else
    {
        delete translator;
    }
}

int main(int argc, char* argv[])
{
    // Initialise PipeWire before any FluidSynth objects are created.
#ifdef HAVE_PIPEWIRE
    pw_init(&argc, &argv);
#endif

    // QApplication, VkMainWindow, and all Qt/FluidSynth objects are scoped so
    // they are fully destroyed BEFORE pw_deinit() runs.  Calling pw_deinit()
    // while a QAudioSink (backed by PipeWire) still exists causes a segfault
    // on exit because the sink's internal thread tries to use an already-freed
    // PipeWire context.
    int result = 0;
    {
        QApplication app(argc, argv);
        app.setApplicationName("Virtual MIDI Keyboard");

        // Optional override (same as Musician's Canvas) for translators / screenshots.
        const QByteArray langEnv = qgetenv("MUSICIANS_CANVAS_LANG");
        if (!langEnv.isEmpty())
            loadVkTranslation(QString::fromLocal8Bit(langEnv));
        else
            loadVkTranslation(VkSettings::instance().language());

        VkMainWindow window;
        window.show();
        result = app.exec();
    } // window and app destroyed here — all PipeWire/Qt audio state released

#ifdef HAVE_PIPEWIRE
    pw_deinit();
#endif
    return result;
}
