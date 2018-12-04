#ifndef EDITORMANAGER_H
#define EDITORMANAGER_H

#include <QPlainTextEdit>
#include <QTabWidget>
#include <QShortcut>
#include <QSignalMapper>
#include <QFont>
#include <QFileInfo>
#include <QList>

#include "editor.h"
#include "highlighter.h"

class EditorManager : public QTabWidget{
Q_OBJECT
public:
  EditorManager(QWidget* parent = nullptr);
  void mapSignals();
public slots:
  void closeTab(int index);
  void closeTab();
  void newTab(QString title, QString content);
  QString getExtension(QString fileName);
  bool highlighterExistsFor(QString language);
  int getSyntaxHighlighterIndexFor(QString language);

private:
  QSignalMapper *signalMapper;
  int tabSize;
  QFont defaultFont;
  QList<Highlighter*> *syntaxHighlighters;
};


#endif  // EDITORMANAGER_H
