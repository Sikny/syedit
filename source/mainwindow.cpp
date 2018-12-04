#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent){
  editorManager = new EditorManager(this);
  buildMenuBar();
  buildCentralWidget();
  buildStatusBar();
  buildDockWidgets();
  fileManager = new FileManager(workspace);
  connect(workspace->getView(), SIGNAL(doubleClicked(QModelIndex)), this, SLOT(edit(QModelIndex)));
}

void MainWindow::buildMenuBar(){
  fileMenu = menuBar()->addMenu(tr("&File"));
  // New
  newSMenu = fileMenu->addMenu(tr("New"));
  newProject = newSMenu->addAction(tr("Project"));
  newProject->setShortcut(tr("ctrl+n"));
  connect(newProject, SIGNAL(triggered()), this, SLOT(createNewProject()));
  newFile = newSMenu->addAction(tr("File"));
  newFile->setShortcut(tr("ctrl+shift+n"));
  connect(newFile, SIGNAL(triggered()), this, SLOT(addNewFile()));
  // Open
  openSMenu = fileMenu->addMenu(tr("Open"));
  openProject = openSMenu->addAction(tr("Project"));
  openProject->setShortcut(tr("ctrl+o"));
  connect(openProject, SIGNAL(triggered()), this, SLOT(openExistingFolderProject()));
  openFile = openSMenu->addAction(tr("File"));
  openFile->setShortcut(tr("ctrl+shift+o"));
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
  workspace = new Workspace(tr("Workspace"), this);
  workspace->setAllowedAreas(Qt::LeftDockWidgetArea |
                            Qt::RightDockWidgetArea);
  workspace->setMinimumWidth(120);
  addDockWidget(Qt::LeftDockWidgetArea, workspace);
}

void MainWindow::addNewFile(){
  QString file = QFileDialog::getSaveFileName(nullptr, tr("Create file"),
    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
  fileManager->newFile(file.toStdString(), editorManager);
}

void MainWindow::openExistingFile(){
  QString file = QFileDialog::getOpenFileName(nullptr, tr("Select file"),
    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
  if(file != nullptr)
    fileManager->openFile(file.toStdString(), editorManager);
  // Input box for asking if user wants to add file to project
}

void MainWindow::createNewProject(){
  QString name;

  Assistant *projectCreation = new Assistant(fileManager);
  projectCreation->setWindowModality(Qt::ApplicationModal);
  projectCreation->show();

}

void MainWindow::openExistingFolderProject(){

}

void MainWindow::edit(QModelIndex modelIndex){
  QString name = workspace->getModel()->filePath(modelIndex);
  editorManager->
}
