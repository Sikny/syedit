#ifndef THEME_H
#define THEME_H

#include <QColor>
#include <QFont>
#include <QMap>
#include <QXmlStreamReader>
#include <QFile>
#include <iostream>

class Settings
{
public:
    static Settings& Instance(){
        return instance;
    }

    void readXmlTheme();
    QString toString();
    QColor color(QString key){
        return colors.value(key);
    }
    void setTheme(QString newTheme){
        themeName = newTheme;
        readXmlTheme();
    }
    QFont getFont(){
        return this->font;
    }

private:
    static Settings instance;
    Settings();

    QString themeName;
    QMap<QString, QColor> colors;
    QFont font;
};

#endif // THEME_H
