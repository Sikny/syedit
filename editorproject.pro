QT += gui core widgets

MOC_DIR = mocs/

OBJECTS_DIR = objs/

SOURCES += source/main.cpp\
  source/mainwindow.cpp\
  source/editormanager.cpp\
  source/editor.cpp\
  source/filemanager.cpp\
  source/highlighter.cpp\

INCLUDEPATH += include/

HEADERS += include/mainwindow.h\
  include/editormanager.h\
  include/editor.h\
  include/filemanager.h\
  include/highlighter.h\
