#include "mainwindow.h"

MainWindow::MainWindow(QWidget * parent) : QMainWindow(parent), settingsInstance(Settings::Instance()){
  editors = new QTabWidget(this);
  editors->setTabsClosable(true);
  connect(editors, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

  menuBar = new QMenuBar(this);
  settingsWin = new SettingsWindow(this);
  connect(settingsWin, SIGNAL(settingsModified()), this, SLOT(loadTheme()));

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

  connect(settingsAction, SIGNAL(triggered()), settingsWin, SLOT(show()));
  loadTheme();
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
        editors->setCurrentWidget(e);
        connect(e, SIGNAL(modificationChanged(bool)), this, SLOT(textEdited(bool)));
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
        editors->setCurrentWidget(e);
        connect(e, SIGNAL(modificationChanged(bool)), this, SLOT(textEdited(bool)));
	}
}

void MainWindow::handleSave(){
    static_cast<Editor*>(editors->currentWidget())->saveFile();
    editors->setTabText(editors->currentIndex(),
        static_cast<Editor*>(editors->currentWidget())->getFileNameWithoutPath());
    static_cast<Editor*>(editors->currentWidget())->document()->setModified(false);
}

void MainWindow::setHighlighter(Editor* editor, QString& fileName){
    QString extension((new QFileInfo(fileName))->suffix());
    if(extension == "c" || extension == "h" || extension == "hpp")
        extension = "cpp";
    highlighters.append(new Highlighter(editor->document(), extension));
}

void MainWindow::closeTab(int index){
    Editor* e = static_cast<Editor*>(editors->widget(index));
    Highlighter* h = highlighters.at(index);
    highlighters.removeAt(index);
    editors->removeTab(index);
    delete e;
    delete h;
}

void MainWindow::loadTheme(){
    QString bgColor = Settings::Instance().color("background").name();
    QString textColor = Settings::Instance().color("text").name();
    QString tabBordCol = Settings::Instance().color("tabborder").name();
    QString notSelTab = Settings::Instance().color("notselected").name();
    setStyleSheet(
"MainWindow, MainWindow *, QTabWidget::pane, QTabBar::tab {"
    "background-color: " + bgColor + ";"
    "color: " + textColor + ";}"
"Editor{border: none;}"
"QTabWidget::pane {"
    "border-top: 1px solid" + tabBordCol + ";"
"}"
"QTabBar::tab{"
    "border-top: 1px solid " + tabBordCol + ";"
    "border-left: 1px solid " + tabBordCol + ";"
    "border-right: 1px solid " + tabBordCol + ";"
    "padding: 2px 5px 2px 5px;"
"}"
"QTabBar::tab:!selected{"
    "background-color: " + notSelTab + ";"
    "border-color: " + bgColor + ";"
    "color: " + bgColor + ";"
"}");
    for(int i = 0; i < editors->count(); i++){
        Editor* e = static_cast<Editor*>(editors->widget(i));
        e->setFont(Settings::Instance().getFont());
        e->highlightCurrentLine();
        highlighters.at(i)->rehighlight();
    }
}

void MainWindow::textEdited(bool changed){
    if(changed)
        editors->setTabText(editors->currentIndex(),
            static_cast<Editor*>(editors->currentWidget())->getFileNameWithoutPath() + " ●");
}
