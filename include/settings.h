#ifndef THEME_H
#define THEME_H

#include <QColor>
#include <QFont>
#include <QMap>
#include <QXmlStreamReader>
#include <QFile>
#include <QFileInfo>
#include <QSettings>

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
    QString getTheme(){
        return themeName;
    }
    void setFont(QFont font){
        this->font = font;
    }
    QFont getFont(){
        return this->font;
    }
    void saveSettings();

private:
    static Settings instance;
    Settings();

    QString themeName;
    QMap<QString, QColor> colors;
    QFont font;
};

#endif // THEME_H
