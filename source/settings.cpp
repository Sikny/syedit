#include "settings.h"

Settings Settings::instance = Settings();

Settings::Settings() {
    themeName = "default";
}

void Settings::readXmlTheme(){
    QXmlStreamReader xml;
    QFile *themeFile = new QFile("resources/themes/"+themeName+".xml");
    if(themeFile->open(QIODevice::ReadOnly))
        xml.setDevice(themeFile);
    if(xml.hasError()){
        std::cout << "Error loading theme" << std::endl;
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
    std::cout << "Theme '" + themeName.toStdString() + " 'loaded" << std::endl;
    std::cout << toString().toStdString() << std::endl;
}

QString Settings::toString(){
    QString str = "";
    str += "Colors : \n";
    for(QString key : colors.keys()){
        str += key + " : " + colors.value(key).name() + "\n";
    }
    return str;
}
