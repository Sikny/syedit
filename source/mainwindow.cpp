#include "mainwindow.h"

MainWindow::MainWindow(QStringList argv, QWidget * parent) : QMainWindow(parent){

  QDir::setCurrent(QFileInfo(argv.at(0)).path());

  Settings::Instance().initialize();

  lineCol = new QLabel(tr("Ready"), this);
  statusBar()->addPermanentWidget(lineCol);

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
  saveAction = new QAction(tr("Save"), nullptr);
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
  saveAction->setDisabled(true);

  // for opening a file with editor
  if(argv.count() > 1) {
      QFile file(argv.at(1));
      QString fileName = file.fileName();
      Editor* e = new Editor(editors);
      e->setFileName(QFileInfo(file).absoluteFilePath());
      e->openFile();
      editors->addTab(e, QFileInfo(file).fileName());
      setHighlighter(e, fileName);
      editors->setCurrentWidget(e);
      connect(e, SIGNAL(modificationChanged(bool)), this, SLOT(textEdited(bool)));
      saveAction->setEnabled(true);
   }
}

/**
 * @brief MainWindow::handleNew
 * Prompts new file name to user, then opens it in order to be saved
 * later @see MainWindow::handleSave()
 */
void MainWindow::handleNew(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"),
		QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
    qDebug(qPrintable(fileName));
	if(!fileName.isEmpty()){
		std::cout << fileName.toStdString() << std::endl;
        Editor* e = new Editor(editors);
        e->setFileName(fileName);
        editors->addTab(e, QFileInfo(fileName).fileName());
        setHighlighter(e, fileName);
        editors->setCurrentWidget(e);
        connect(e, SIGNAL(modificationChanged(bool)), this, SLOT(textEdited(bool)));
        connect(e, SIGNAL(cursorPositionChanged()), this, SLOT(updateStatusBar()));
        saveAction->setEnabled(true);
        updateStatusBar();
    }
}

/**
 * @brief MainWindow::handleOpen
 * Prompts user for one or several files to be opened in different tabs
 */
void MainWindow::handleOpen(){
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"),
        QStandardPaths::displayName(QStandardPaths::DocumentsLocation));
    if(!fileNames.isEmpty()){
        for(QString fileName : fileNames){
            Editor* e = new Editor(editors);
            e->setFileName(fileName);
            e->openFile();
            editors->addTab(e, QFileInfo(fileName).fileName());
            setHighlighter(e, fileName);
            editors->setCurrentWidget(e);
            connect(e, SIGNAL(modificationChanged(bool)), this, SLOT(textEdited(bool)));
            connect(e, SIGNAL(cursorPositionChanged()), this, SLOT(updateStatusBar()));
        }
        saveAction->setEnabled(true);
        updateStatusBar();
    }
}

/**
 * @brief MainWindow::handleSave
 * Saves the file at the current tab
 */
void MainWindow::handleSave(){
    static_cast<Editor*>(editors->currentWidget())->saveFile();
    editors->setTabText(editors->currentIndex(),
        static_cast<Editor*>(editors->currentWidget())->getFileNameWithoutPath());
    static_cast<Editor*>(editors->currentWidget())->document()->setModified(false);
}

/**
 * @brief MainWindow::setHighlighter
 * Initializes the highlighter for a given editor, basing on targeted language
 * @param editor
 * @param fileName
 */
void MainWindow::setHighlighter(Editor* editor, QString& fileName){
    QString extension((new QFileInfo(fileName))->suffix());
    if(extension == "c" || extension == "h" || extension == "hpp")
        extension = "cpp";
    highlighters.append(new Highlighter(editor->document(), extension));
}

/**
 * @brief MainWindow::closeTab
 * Closes the editor at the given index, and removes the associated highlighter.
 * Also disables the save action if needed
 * @param index
 */
void MainWindow::closeTab(int index){
    Editor* e = static_cast<Editor*>(editors->widget(index));
    if(editors->tabText(index).at(editors->tabText(index).size()-1) == "●"){
        QMessageBox messageBox(tr("Confirm close"), tr("There are unsaved changes. Close anyway ?"), QMessageBox::Icon(), QMessageBox::Save, QMessageBox::Discard, QMessageBox::Cancel, this);
        int ret = messageBox.exec();
        switch(ret){
            case QMessageBox::Save:
                handleSave();   // Save then close
                break;
            case QMessageBox::Discard:
                // Just go to the next (just close)
                break;
            case QMessageBox::Cancel:
                return;
            default:    // Should never be reached
                break;
        }
    }
    Highlighter* h = highlighters.at(index);
    highlighters.removeAt(index);
    editors->removeTab(index);
    if(editors->count() < 1)
        saveAction->setDisabled(true);
    delete h;
    delete e;
    if(editors->count() > 0)
        updateStatusBar();
}

/**
 * @brief MainWindow::loadTheme
 * Loads - or reloads a theme, by also rehighlighting the text
 */
void MainWindow::loadTheme(){
    QString bgColor = Settings::Instance().color("background").name();
    QString textColor = Settings::Instance().color("text").name();
    QString tabBordCol = Settings::Instance().color("tabborder").name();
    QString notSelTab = Settings::Instance().color("notselected").name();
    qApp->setStyleSheet(qApp->styleSheet()
+"MainWindow, MainWindow *, QTabWidget::pane, QTabBar::tab {"
    "background-color: " + bgColor + ";"
    "color: " + textColor + ";}"
"Editor{border: none;}"
"QMenuBar::item:selected {background-color: "+tabBordCol+";}"
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
"}"
"QStatusBar::item{border: none;}");
    QFont fontTmp = Settings::Instance().getFont();
    int tabSize = Settings::Instance().getTabSize();
    for(int i = 0; i < editors->count(); i++){
        Editor* e = static_cast<Editor*>(editors->widget(i));
        e->setFont(fontTmp);
        e->setTabStopWidth(tabSize * QFontMetrics(fontTmp).width(' '));
        e->highlightCurrentLine();
        highlighters.at(i)->rehighlight();
    }
}

/**
 * @brief MainWindow::textEdited
 * Signal for visual clue that the document is changed and unsaved
 * @param changed
 */
void MainWindow::textEdited(bool changed){
    if(changed)
        editors->setTabText(editors->currentIndex(),
            static_cast<Editor*>(editors->currentWidget())->getFileNameWithoutPath() + " ●");
}

/**
 * @brief MainWindow::closeEvent
 * When user clicks on the "x" button
 * @param event
 */
void MainWindow::closeEvent (QCloseEvent *event)
{
    bool someNotSaved = false;
    for(int i = 0; i < editors->count(); i++)
        if(editors->tabText(i).at(editors->tabText(i).size()-1) == "●")
            someNotSaved = true;
    if(someNotSaved){
        QMessageBox messageBox(tr("Confirm close"), tr("There are unsaved changes. Close anyway ?"), QMessageBox::Icon(), QMessageBox::SaveAll, QMessageBox::Discard, QMessageBox::Cancel, this);
        int ret = messageBox.exec();
        bool doSave = false;
        switch(ret){
            case QMessageBox::Save:
                doSave = true;
                break;
            case QMessageBox::Discard:
                doSave = false;
                break;
            case QMessageBox::Cancel:
                event->ignore();
                return;
            default:    // Should never be reached
                break;
        }
        while(editors->count() > 0){
            if(doSave)
                handleSave();
            int index = editors->currentIndex();
            Highlighter* h = highlighters.at(index);
            highlighters.removeAt(index);
            editors->removeTab(index);
            if(editors->count() < 1)
                saveAction->setDisabled(true);
            delete h;
        }   // no need to break there
    }
}

/**
 * @brief MainWindow::updateStatusBar
 * updates line and column numbers in status bar
 */
void MainWindow::updateStatusBar(){
    Editor* e = static_cast<Editor*>(editors->widget(editors->currentIndex()));
    int lineNumber = e->textCursor().block().firstLineNumber()+1;
    int colNumber = e->textCursor().columnNumber();
    lineCol->setText("Line: " + QString::number(lineNumber)
                     +", Col: " + QString::number(colNumber));
}
