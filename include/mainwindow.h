#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include <QStandardPaths>

#include <iostream>

#include "editor.h"

class MainWindow : public QMainWindow{
Q_OBJECT
public:
  MainWindow(QWidget* parent = nullptr);
  void setHighlighter(QString& fileName);
public slots:
  void handleNew();
  void handleOpen();
  void handleSave();
private:
  Editor* editor;
  QMenuBar* menuBar;
  Highlighter *highlighter;
};

#endif
