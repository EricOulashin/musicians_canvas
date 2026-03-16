#include "themeutils.h"
#include "appsettings.h"
#include <QApplication>
#include <QPalette>
#include <QStyle>
#include <QStyleFactory>

namespace ThemeUtils
{

static QString darkStylesheet()
{
    return R"(
        QMainWindow { background-color: #1e1e24; }
        QWidget { background-color: #252530; color: #e0e0e0; }
        QScrollArea { border: none; background-color: #252530; }
        QMenuBar { background-color: #252530; color: #e0e0e0; }
        QMenuBar::item:selected { background-color: #3a3a45; }
        QMenu { background-color: #2d2d34; color: #e0e0e0; }
        QMenu::item:selected { background-color: #4a4a55; }
        QFrame#trackFrame, TrackWidget {
            background-color: #2d2d34;
            border: 1px solid #3d3d45;
            border-radius: 6px;
        }
        TrackWidget:hover { border-color: #5a5a65; }
        QPushButton { background-color: #404050; color: #e0e0e0; border: none; border-radius: 4px; padding: 6px; }
        QPushButton:hover { background-color: #505060; }
        QPushButton:pressed { background-color: #606070; }
        QPushButton#addTrackBtn { background-color: #4a7c59; color: white; border-radius: 6px; padding: 0 20px; font-weight: bold; }
        QPushButton#addTrackBtn:hover { background-color: #5a8c69; }
        QPushButton#addTrackBtn:pressed { background-color: #3a6c49; }
        QPushButton#applyBtn { background-color: #4a7c59; color: white; border-radius: 4px; padding: 8px 20px; }
        QPushButton#applyBtn:hover { background-color: #5a8c69; }
        QCheckBox { color: #e0e0e0; }
        QGroupBox { color: #e0e0e0; }
        QLabel { color: #e0e0e0; }
        QLineEdit, QComboBox { background-color: #3d3d45; color: #e0e0e0; border: 1px solid #4d4d55; border-radius: 4px; }
        QTabWidget::pane { background-color: #2d2d34; border: 1px solid #3d3d45; }
    )";
}

static QString lightStylesheet()
{
    return R"(
        QMainWindow { background-color: #f0f0f0; }
        QWidget { background-color: #ffffff; color: #333333; }
        QScrollArea { border: none; background-color: #f5f5f5; }
        QMenuBar { background-color: #f5f5f5; color: #333333; }
        QMenuBar::item:selected { background-color: #e0e0e0; }
        QMenu { background-color: #ffffff; color: #333333; }
        QMenu::item:selected { background-color: #e8e8e8; }
        QFrame#trackFrame, TrackWidget {
            background-color: #ffffff;
            border: 1px solid #d0d0d0;
            border-radius: 6px;
        }
        TrackWidget:hover { border-color: #a0a0a0; }
        QPushButton { background-color: #e8e8e8; color: #333333; border: 1px solid #ccc; border-radius: 4px; padding: 6px; }
        QPushButton:hover { background-color: #d8d8d8; }
        QPushButton:pressed { background-color: #c8c8c8; }
        QPushButton#addTrackBtn { background-color: #4a7c59; color: white; border: none; border-radius: 6px; padding: 0 20px; font-weight: bold; }
        QPushButton#addTrackBtn:hover { background-color: #5a8c69; }
        QPushButton#addTrackBtn:pressed { background-color: #3a6c49; }
        QPushButton#applyBtn { background-color: #4a7c59; color: white; border: none; border-radius: 4px; padding: 8px 20px; }
        QPushButton#applyBtn:hover { background-color: #5a8c69; }
        QCheckBox { color: #333333; }
        QGroupBox { color: #333333; }
        QLabel { color: #333333; }
        QLineEdit, QComboBox { background-color: #ffffff; color: #333333; border: 1px solid #ccc; border-radius: 4px; }
        QTabWidget::pane { background-color: #ffffff; border: 1px solid #d0d0d0; }
    )";
}

void applyTheme(const QString& theme)
{
    QApplication* app = qobject_cast<QApplication*>(qApp);
    if (!app) return;

    app->setStyle(QStyleFactory::create("Fusion"));

    if (theme == "light")
    {
        app->setPalette(app->style()->standardPalette());
        app->setStyleSheet(lightStylesheet());
    }
    else
    {
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(30, 30, 36));
        darkPalette.setColor(QPalette::WindowText, QColor(224, 224, 224));
        darkPalette.setColor(QPalette::Base, QColor(45, 45, 52));
        darkPalette.setColor(QPalette::AlternateBase, QColor(50, 50, 58));
        darkPalette.setColor(QPalette::Text, QColor(224, 224, 224));
        darkPalette.setColor(QPalette::Button, QColor(64, 64, 80));
        darkPalette.setColor(QPalette::ButtonText, QColor(224, 224, 224));
        darkPalette.setColor(QPalette::Highlight, QColor(74, 124, 89));
        darkPalette.setColor(QPalette::HighlightedText, Qt::white);
        app->setPalette(darkPalette);
        app->setStyleSheet(darkStylesheet());
    }
}

void applySavedTheme()
{
    applyTheme(AppSettings::instance().theme());
}

} // namespace ThemeUtils
