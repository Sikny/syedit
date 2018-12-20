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

	connect(newAction, SIGNAL(triggered()), this, SLOT(handleNew()));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(handleSave()));
	connect(openAction, SIGNAL(triggered()), this, SLOT(handleOpen()));
}

void MainWindow::handleNew(){
  QString fileName = QFileDialog::getSaveFileName(this, tr("New File"),
		QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
	if(!fileName.isEmpty()){
		std::cout << fileName.toStdString() << std::endl;
		editor->setFileName(fileName);
	}
}

void MainWindow::handleOpen(){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
	if(!fileName.isEmpty()){
		editor->setFileName(fileName);
		editor->openFile();
	}
}

void MainWindow::handleSave(){
	editor->saveFile();
}
