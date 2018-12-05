#include "filemanager.h"

FileManager::FileManager(){
  openedFiles = new QList<QFileInfo*>();
}

void FileManager::openFile(std::string file, EditorManager* editor){
  std::ifstream is;
  std::string lineBuf;
  std::string fileContent;
  is.open(file);
  if(is.is_open()){
    while(getline(is, lineBuf)){
      fileContent += lineBuf;
      fileContent += "\n";
    }
    is.close();
  }
  else{
    std::cout << "Unable opening file " << file << std::endl;
  }
  QString title = addFile(file);
  editor->newTab(title, QString::fromStdString(fileContent));
}

QString FileManager::addFile(std::string file){
  QString s = QString::fromStdString(file);
  QFileInfo *fileInfo = new QFileInfo(s);
  if(fileInfo->exists()){
    openedFiles->append(fileInfo);
  }
  else
    std::cout << "file does not exists" << std::endl;
  return fileInfo->fileName();
}

void FileManager::newFile(std::string file, EditorManager* editor){
  if(QString::fromStdString(file).isEmpty())
    return;
  else{
    QFile theFile(QString::fromStdString(file));
    if(!theFile.open(QIODevice::WriteOnly)){
      std::cerr << "Error creating " << file << std::endl;
    }
    theFile.close();
  }
  QString title = addFile(file);
  editor->newTab(title, "");
}
