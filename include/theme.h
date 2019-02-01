#ifndef THEME_H
#define THEME_H

#include <QColor>
#include <QFont>
#include <QMap>
#include <QXmlStreamReader>
#include <QFile>
#include <iostream>

class Theme
{
public:
    static Theme& Instance(){
        return instance;
    }

    void readXmlTheme();
    QString toString();
    QColor color(QString key){
        return colors.value(key);
    }

private:
    static Theme instance;
    Theme();

    QString themeName;
    QMap<QString, QColor> colors;
    QFont font;
};

#endif // THEME_H
