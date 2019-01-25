QT += core gui widgets

INCLUDEPATH += include

CONFIG += c++11

OBJECTS_DIR = obj

MOC_DIR = moc

SOURCES += source/main.cpp\
	source/mainwindow.cpp\
	source/editor.cpp\
        source/highlighter.cpp \
        source/settingsloader.cpp \
        source/theme.cpp

HEADERS += include/mainwindow.h\
	include/editor.h\
        include/highlighter.h \
        include/settingsloader.h \
        include/theme.h
