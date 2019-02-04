#include "settings.h"

Settings Settings::instance = Settings();

Settings::Settings() {
    //initializing settings
    QSettings settings("resources/config.ini", QSettings::IniFormat);
    settings.beginGroup("Font");
    font.setFamily(settings.value("family").toString());
    font.setPointSize(settings.value("size").toInt());
    settings.endGroup();

    settings.beginGroup("Theme");
    themeName = settings.value("name").toString();
    settings.endGroup();

    readXmlTheme();
}

void Settings::readXmlTheme(){
    QXmlStreamReader xml;
    QFile *themeFile = new QFile("resources/themes/"+themeName+".xml");
    if(themeFile->open(QIODevice::ReadOnly))
        xml.setDevice(themeFile);
    else {
        qDebug(qPrintable("Error loading theme '" + themeFile->fileName() + "'"));
        return;
    }
    while(!xml.atEnd()){
        xml.readNextStartElement();
        if(xml.name() == "color" && !xml.isEndElement()){
            QString key = xml.attributes().value("type").toString();

            xml.readNextStartElement();
            int r = xml.readElementText().toInt();

            xml.readNextStartElement();
            int g = xml.readElementText().toInt();

            xml.readNextStartElement();
            int b = xml.readElementText().toInt();

            colors.insert(key, QColor(r, g, b));

            xml.skipCurrentElement();
        }
    }
    std::cout << "Theme '" + themeName.toStdString() + "' loaded" << std::endl;
}

QString Settings::toString(){
    QString str = "";
    str += "Colors : \n";
    for(QString key : colors.keys()){
        str += key + " : " + colors.value(key).name() + "\n";
    }
    return str;
}

void Settings::saveSettings(){
    QSettings settings("resources/config.ini", QSettings::IniFormat);
    settings.beginGroup("Font");
    settings.setValue("family", font.family());
    settings.setValue("size", font.pointSize());
    settings.endGroup();

    settings.beginGroup("Theme");
    settings.setValue("name", themeName);
    settings.endGroup();
}
