#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QList>
#include <QFileInfo>
#include <QString>
#include <QMessageBox>
#include <QIODevice>

#include <iostream>
#include <fstream>
#include <string>
#include "editormanager.h"

class FileManager{
public:
  FileManager();
  void openFile(std::string file, EditorManager* editor);
  QString addFile(std::string file);
  void newFile(std::string file, EditorManager* editor);
private:
  QList<QFileInfo*> *openedFiles;

  const QString projExt = ".syk";
};

#endif
