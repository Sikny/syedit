#include "editormanager.h"

EditorManager::EditorManager(QWidget* parent) : QTabWidget(parent){
  setTabsClosable(true);
  connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
  tabSize = 15;
  defaultFont = QFont("Courier New");
  defaultFont.setPointSize(10);
  syntaxHighlighters = new QList<Highlighter*>();
}

void EditorManager::closeTab(int index){
  removeTab(index);
}

void EditorManager::closeTab(){
  removeTab(currentIndex());
}

void EditorManager::newTab(QString title, QString content){
    Editor *editor = new Editor();
    editor->setFont(defaultFont);
    editor->setTabStopWidth(30);
    QString language = getExtension(title);
    editor->document()->setPlainText(content);
    if(language == "c" || language == "h" || language == "hpp" || language == "c++")
        language = "cpp";
    Highlighter *h;
    if(!highlighterExistsFor(language)){
        h = new Highlighter(editor->document(), language);
    }
    else{
        h = syntaxHighlighters->at(getSyntaxHighlighterIndexFor(language));
    }

    addTab(editor, title);
}

QString EditorManager::getExtension(QString fileName){
    QFileInfo *info = new QFileInfo(fileName);
    return info->suffix();
}

bool EditorManager::highlighterExistsFor(QString language){
    for(int i = 0; i < syntaxHighlighters->size(); i++){
        if(syntaxHighlighters->at(i)->getLanguage().compare(language) == 0)
            return true;
    }
    return false;
}

int EditorManager::getSyntaxHighlighterIndexFor(QString language){
    for(int i = 0; i < syntaxHighlighters->size(); i++){
        if(syntaxHighlighters->at(i)->getLanguage().compare(language) == 0)
            return i;
    }
    return -1;
}
