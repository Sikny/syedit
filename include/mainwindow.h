#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>

#include <iostream>

#include "editor.h"
#include "settings.h"
#include "settingswindow.h"

class MainWindow : public QMainWindow{
Q_OBJECT
public:
  MainWindow(QStringList argv, QWidget* parent = nullptr);
  void setHighlighter(Editor* editor, QString& fileName);

public slots:
  void handleNew();
  void handleOpen();
  void handleSave();
  void loadTheme();
  void closeTab(int);
  void textEdited(bool);

private:
  QTabWidget* editors;
  QMenuBar* menuBar;
  QList<Highlighter*> highlighters;
  SettingsWindow *settingsWin;

  QAction* saveAction;
};

#endif
