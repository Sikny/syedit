#include "editor.h"

Editor::Editor(QWidget * parent) : QPlainTextEdit(parent){}

void Editor::saveFile(){
  QFile file(fileName);
  if(file.open(QFile::WriteOnly)){
    QTextStream out(&file);
	  out << this->toPlainText().toStdString().c_str();
  }
}

void Editor::openFile(){
  QFile file(fileName);
  if(file.open(QFile::ReadOnly)){
    QTextStream in(&file);
    QString line;
    QString content("");
    while(in.readLineInto(&line)){
      content.append(line);
    }
    this->setPlainText(content);
  }
}
