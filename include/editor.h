#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>

#include "highlighter.h"

class Editor : public QPlainTextEdit{
public:
  Editor(QWidget * parent = nullptr);
  QString getFileName(){
    return fileName;
  }
  void setFileName(QString name){
    fileName = name;
  }
  void saveFile();
  void openFile();
private:
  QString fileName;
};

#endif
