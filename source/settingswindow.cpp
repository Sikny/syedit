#include "include/settingswindow.h"

SettingsWindow::SettingsWindow(QMainWindow* mainWin) : QWidget()
{
    this->mainWindow = mainWin;
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle(tr("Settings"));

    QHBoxLayout* layout = new QHBoxLayout();
    QListWidget* navList = new QListWidget();
    navList->addItem(new QListWidgetItem(tr("Editor")));
    navList->addItem(new QListWidgetItem(tr("Theme")));
    layout->addWidget(navList, 1);

    tabs = new QTabWidget(this);

    QWidget* editorTab = new QWidget();
    QWidget* themeTab = new QWidget();
    tabs->tabBar()->hide();
    tabs->addTab(editorTab, tr("Editor"));
    tabs->addTab(themeTab, tr("Theme"));

    layout->addWidget(tabs, 5);
    this->setLayout(layout);
    this->setFixedSize(sizeHint());

    buildTabsWidgets();

    connect(navList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(handleNavigation(QListWidgetItem*)));
}

void SettingsWindow::buildTabsWidgets(){
    QVBoxLayout* layoutEditor = new QVBoxLayout();

    tabs->widget(0)->setLayout(layoutEditor);
    QVBoxLayout* layoutTheme = new QVBoxLayout();
    themeChoice = new QComboBox();
    // reading for theme files
    QDirIterator dirIter("resources/themes");
    while(dirIter.hasNext()){
        QString nextPath = dirIter.next();
        if(nextPath != "resources/themes/." && nextPath != "resources/themes/..")
            themeChoice->addItem(QFileInfo(nextPath).baseName());
    }
    layoutTheme->addWidget(themeChoice);
    QHBoxLayout* confirmButtons = new QHBoxLayout();
    QPushButton* applyB = new QPushButton(tr("Apply"));
    connect(applyB, SIGNAL(clicked()), this, SLOT(applySettings()));
    QPushButton* cancelB = new QPushButton(tr("Cancel"));
    connect(cancelB, SIGNAL(clicked()), this, SLOT(close()));
    confirmButtons->addWidget(applyB);
    confirmButtons->addWidget(cancelB);
    layoutTheme->addLayout(confirmButtons);
    tabs->widget(1)->setLayout(layoutTheme);
}

void SettingsWindow::applySettings(){
    Settings::Instance().setTheme(themeChoice->currentText());
    emit settingsModified();
}

void SettingsWindow::handleNavigation(QListWidgetItem* item){
    if(item->text() == "Editor"){
        tabs->setCurrentWidget(tabs->widget(0));
    } else if(item->text() == "Theme"){
        tabs->setCurrentWidget(tabs->widget(1));
    }
}
