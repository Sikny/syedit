#include "mainwindow.h"

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent){
  editor = new Editor(this);
  menuBar = new QMenuBar(this);

  QMenu *menuFile;
  menuFile = new QMenu(tr("&File"));
  QAction *newAction = new QAction(tr("New"), nullptr);
  QAction *openAction = new QAction(tr("Open"), nullptr);
  QAction *saveAction = new QAction(tr("Save"), nullptr);
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
        setHighlighter(fileName);
	}
}

void MainWindow::handleOpen(){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
	if(!fileName.isEmpty()){
		editor->setFileName(fileName);
		editor->openFile();
        setHighlighter(fileName);
	}
}

void MainWindow::handleSave(){
	editor->saveFile();
}

void MainWindow::setHighlighter(QString& fileName){
    QString extension((new QFileInfo(fileName))->suffix());
    if(extension == "c" || extension == "h" || extension == "hpp")
        extension = "cpp";
    highlighter = new Highlighter(editor->document(), extension);
}
