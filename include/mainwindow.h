#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QTextEdit>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QModelIndex>

#include <iostream>

#include "editormanager.h"
#include "filemanager.h"

class MainWindow : public QMainWindow{
  Q_OBJECT
public:
  MainWindow(QWidget* parent = nullptr);
  void buildMenuBar();
  void buildCentralWidget();
  void buildStatusBar();
  void buildDockWidgets();

public slots:
  void quitApp();
  void addNewFile();
  void openExistingFile();
  void openExistingFolderProject();
  void createNewProject();
  void edit(QModelIndex);
private:
  EditorManager* editorManager;
  FileManager *fileManager;
  Workspace *workspace;
  QMenu *fileMenu, *newSMenu, *openSMenu, *editMenu, *runMenu, *helpMenu;
  QAction *quit, *newFile, *newProject, *openFile, *openProject, *saveProject,
    *preferences, *closeTab;
};

#endif  // MAINWIN_H
