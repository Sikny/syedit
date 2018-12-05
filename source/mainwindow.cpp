#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent){
  editorManager = new EditorManager(this);
  buildMenuBar();
  buildCentralWidget();
  buildStatusBar();
  buildDockWidgets();
  fileManager = new FileManager();
}

void MainWindow::buildMenuBar(){
  fileMenu = menuBar()->addMenu(tr("&File"));
  // New
  newFile = fileMenu->addAction(tr("New File..."));
  newFile->setShortcut(tr("ctrl+n"));
  connect(newFile, SIGNAL(triggered()), this, SLOT(addNewFile()));
  // Open
  openFile = fileMenu->addAction(tr("Open File..."));
  openFile->setShortcut(tr("ctrl+o"));
  connect(openFile, SIGNAL(triggered()), this, SLOT(openExistingFile()));
  // Quit
  quit = fileMenu->addAction(tr("&Quit"));
  connect(quit, SIGNAL(triggered()), this, SLOT(quitApp()));

  editMenu = menuBar()->addMenu(tr("&Edit"));
  closeTab = editMenu->addAction(tr("Close tab"));
  closeTab->setShortcut(tr("ctrl+w"));
  connect(closeTab, SIGNAL(triggered()), editorManager, SLOT(closeTab()));
  preferences = editMenu->addAction(tr("Preferences..."));

  runMenu = menuBar()->addMenu(tr("&Run"));
  helpMenu = menuBar()->addMenu(tr("&Help"));

}

void MainWindow::quitApp(){
    QMessageBox msgBox;
    msgBox.setText("Unsaved changes will be lost.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
        QApplication::instance()->quit();
}

void MainWindow::buildCentralWidget(){
  setCentralWidget(editorManager);
}

void MainWindow::buildStatusBar(){

}

void MainWindow::buildDockWidgets(){

}

void MainWindow::addNewFile(){

}

void MainWindow::openExistingFile(){
  QString file = QFileDialog::getOpenFileName(nullptr, tr("Select file"),
    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
  if(file != nullptr)
    fileManager->openFile(file.toStdString(), editorManager);
  // Input box for asking if user wants to add file to project
}
