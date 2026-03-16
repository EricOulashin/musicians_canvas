#ifndef THEMEUTILS_H
#define THEMEUTILS_H

#include <QString>

class QApplication;

namespace ThemeUtils
{
    void applyTheme(const QString& theme);
    void applySavedTheme();
}

#endif // THEMEUTILS_H
