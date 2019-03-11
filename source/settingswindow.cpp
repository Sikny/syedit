#include "settingswindow.h"

SettingsWindow::SettingsWindow(QMainWindow* mainWin) : QWidget()
{
    this->mainWindow = mainWin;
    this->setWindowModality(Qt::ApplicationModal);
    this->setWindowTitle(tr("Settings"));

    // building layouts
    QHBoxLayout* layout = new QHBoxLayout();
    QVBoxLayout* rightLayout = new QVBoxLayout();

    QListWidget* navList = new QListWidget(this);
    navList->addItem(new QListWidgetItem(tr("Editor")));
    navList->addItem(new QListWidgetItem(tr("Theme")));
    layout->addWidget(navList, 1);

    // styling navList
    qApp->setStyleSheet(qApp->styleSheet() + "QListWidget::item{"
                           "height: 25px;"
                           "}");

    tabs = new QTabWidget(this);

    QWidget* editorTab = new QWidget();
    QWidget* themeTab = new QWidget();
    tabs->tabBar()->hide();
    tabs->addTab(editorTab, tr("Editor"));
    tabs->addTab(themeTab, tr("Theme"));

    rightLayout->addWidget(tabs);
    this->setLayout(layout);

    buildTabsWidgets();

    QHBoxLayout* confirmButtons = new QHBoxLayout();
    QPushButton* applyB = new QPushButton(tr("Apply"));
    connect(applyB, SIGNAL(clicked()), this, SLOT(applySettings()));
    QPushButton* applyNCloseB = new QPushButton(tr("Apply and Close"));
    connect(applyNCloseB, SIGNAL(clicked()), this, SLOT(applyAndCloseSettings()));
    QPushButton* cancelB = new QPushButton(tr("Cancel"));
    connect(cancelB, SIGNAL(clicked()), this, SLOT(close()));
    confirmButtons->addWidget(applyB);
    confirmButtons->addWidget(applyNCloseB);
    confirmButtons->addWidget(cancelB);

    rightLayout->addLayout(confirmButtons, 5);
    layout->addLayout(rightLayout, 2);

    connect(navList, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(handleNavigation(QListWidgetItem*)));

    navList->setCurrentRow(0);
    this->setFixedSize(sizeHint());
}

/**
 * @brief SettingsWindow::buildTabsWidgets
 * Builds different tab widgets, each with a different layout
 */
void SettingsWindow::buildTabsWidgets(){
    QVBoxLayout* layoutEditor = new QVBoxLayout();
        QGroupBox* fontGroup = new QGroupBox(tr("Font"), this);
            QHBoxLayout* layoutFont = new QHBoxLayout();
                fontFamily = new QComboBox(this);
                fontFamily->setEditable(true);
                fontFamily->setInsertPolicy(QComboBox::NoInsert);
                QFontDatabase fontData;
                fontFamily->addItems(fontData.families());
                fontFamily->setCurrentText(Settings::Instance().getFont().family());
                layoutFont->addWidget(fontFamily);

                fontSize = new QSpinBox(this);
                fontSize->setValue(Settings::Instance().getFont().pointSize());
                layoutFont->addWidget(fontSize);
            fontGroup->setLayout(layoutFont);
        layoutEditor->addWidget(fontGroup);
        QGroupBox* tabGroup = new QGroupBox(tr("Tab"), this);
            QHBoxLayout* layoutTab = new QHBoxLayout();
                QLabel* tabSizeLabel = new QLabel(tr("Tab length"), this);
                tabSize = new QSpinBox(this);
                tabSize->setValue(Settings::Instance().getTabSize());
                layoutTab->addWidget(tabSizeLabel);
                layoutTab->addWidget(tabSize);
             tabGroup->setLayout(layoutTab);
        layoutEditor->addWidget(tabGroup);
    tabs->widget(0)->setLayout(layoutEditor);

    QVBoxLayout* layoutTheme = new QVBoxLayout();
        QGroupBox* themeGroup = new QGroupBox(tr("Theme"), this);
            QVBoxLayout* tGrpLayout = new QVBoxLayout();
                themeChoice = new QComboBox(this);
                // reading for theme files
                QDirIterator dirIter("resources/themes");
                while(dirIter.hasNext()){
                    QString nextPath = dirIter.next();
                    if(nextPath != "resources/themes/." && nextPath != "resources/themes/..")
                        themeChoice->addItem(QFileInfo(nextPath).baseName());
                }
                themeChoice->setCurrentText(Settings::Instance().getTheme());
                tGrpLayout->addWidget(themeChoice);
            themeGroup->setLayout(tGrpLayout);
        layoutTheme->addWidget(themeGroup);
    tabs->widget(1)->setLayout(layoutTheme);
}

/**
 * @brief SettingsWindow::applySettings
 * Applies the modified settings
 * @see Settings::saveSettings, MainWindow::loadTheme
 */
void SettingsWindow::applySettings(){
    Settings::Instance().setTheme(themeChoice->currentText());
    QFont newFont(fontFamily->currentText());
    newFont.setPointSize(fontSize->value());
    Settings::Instance().setFont(newFont);
    Settings::Instance().setTabSize(tabSize->value());
    Settings::Instance().saveSettings();
    emit settingsModified();
}

void SettingsWindow::applyAndCloseSettings(){
    applySettings();
    close();
}

/**
 * @brief SettingsWindow::handleNavigation
 * Changes tab widget when clicking on listview item
 * @param item
 */
void SettingsWindow::handleNavigation(QListWidgetItem* item){
    if(item->text() == "Editor"){
        tabs->setCurrentWidget(tabs->widget(0));
    } else if(item->text() == "Theme"){
        tabs->setCurrentWidget(tabs->widget(1));
    }
}
