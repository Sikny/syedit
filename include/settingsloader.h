#ifndef SETTINGSLOADER_H
#define SETTINGSLOADER_H

#include <QFont>

class SettingsLoader
{
public:
    SettingsLoader();
    void readSettingsFile();

private:
    QFont* font;

};

#endif // SETTINGSLOADER_H
