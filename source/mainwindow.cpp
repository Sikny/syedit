#include "mainwindow.h"

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent){
  editor = new Editor(this);
  menuBar = new QMenuBar(this);

  QMenu *menuFile = new QMenu(tr("&File"));
  QAction *newAction = new QAction(tr("New"));
  QAction *openAction = new QAction(tr("Open"));
  QAction *saveAction = new QAction(tr("Save"));
  menuFile->addAction(newAction);
  menuFile->addAction(openAction);
  menuFile->addAction(saveAction);

  menuBar->addMenu(menuFile);

  setCentralWidget(editor);
  setMenuBar(menuBar);
}

void MainWindow::handleNew(){
  QString fileName = QFileDialog::getSaveFileName();
}

void MainWindow::handleOpen(){

}

void MainWindow::handleSave(){

}
