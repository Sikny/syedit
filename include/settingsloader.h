#ifndef SETTINGSLOADER_H
#define SETTINGSLOADER_H

#include <QFont>

#include "theme.h"

class SettingsLoader
{
public:
    SettingsLoader();
    void readSettingsFile();
    void loadTheme();
    const Theme& getTheme();

private:
    //Theme& theme;
    QFont* font;

};

#endif // SETTINGSLOADER_H
