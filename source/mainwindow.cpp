#include "mainwindow.h"

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent){
  Settings::Instance().setTheme("default");
  Settings::Instance().readXmlTheme();

  editors = new QTabWidget(this);
  editors->setTabsClosable(true);
  connect(editors, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

  menuBar = new QMenuBar(this);
  settings = new SettingsWindow(this);
  connect(settings, SIGNAL(settingsModified()), this, SLOT(loadTheme()));

  QMenu *menuFile;
  menuFile = new QMenu(tr("&File"));
  QAction *newAction = new QAction(tr("New..."), nullptr);
  newAction->setShortcut(QKeySequence("Ctrl+N"));
  QAction *openAction = new QAction(tr("Open..."), nullptr);
  openAction->setShortcut(QKeySequence("Ctrl+O"));
  QAction *saveAction = new QAction(tr("Save"), nullptr);
  saveAction->setShortcut(QKeySequence("Ctrl+S"));
  menuFile->addAction(newAction);
  menuFile->addAction(openAction);
  menuFile->addAction(saveAction);

  menuBar->addMenu(menuFile);

  QMenu *menuEdit = new QMenu(tr("&Edit"));
  QAction *settingsAction = new QAction(tr("&Options..."));
  menuEdit->addAction(settingsAction);

  menuBar->addMenu(menuEdit);

  setCentralWidget(editors);
  setMenuBar(menuBar);

  connect(newAction, SIGNAL(triggered()), this, SLOT(handleNew()));
  connect(saveAction, SIGNAL(triggered()), this, SLOT(handleSave()));
  connect(openAction, SIGNAL(triggered()), this, SLOT(handleOpen()));

  connect(settingsAction, SIGNAL(triggered()), settings, SLOT(show()));
}

void MainWindow::handleNew(){
  QString fileName = QFileDialog::getSaveFileName(this, tr("New File"),
		QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
	if(!fileName.isEmpty()){
		std::cout << fileName.toStdString() << std::endl;
        Editor* e = new Editor(editors);
        e->setFileName(fileName);
        editors->addTab(e, QFileInfo(fileName).fileName());
        setHighlighter(e, fileName);
        loadTheme();
	}
}

void MainWindow::handleOpen(){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
	if(!fileName.isEmpty()){
        Editor* e = new Editor(editors);
        e->setFileName(fileName);
        e->openFile();
        editors->addTab(e, QFileInfo(fileName).fileName());
        setHighlighter(e, fileName);
        loadTheme();
	}
}

void MainWindow::handleSave(){
    static_cast<Editor*>(editors->currentWidget())->saveFile();
}

void MainWindow::setHighlighter(Editor* editor, QString& fileName){
    QString extension((new QFileInfo(fileName))->suffix());
    if(extension == "c" || extension == "h" || extension == "hpp")
        extension = "cpp";
    highlighters.append(new Highlighter(editor->document(), extension));
}

void MainWindow::closeTab(int index){
    Editor* e = static_cast<Editor*>(editors->widget(index));
    highlighters.removeAt(index);
    editors->removeTab(index);
    delete e;
}
