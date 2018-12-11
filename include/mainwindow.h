#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include "editor.h"

class MainWindow : public QMainWindow{
public:
  MainWindow(QWidget* parent = nullptr);
public slots:
  void handleNew();
  void handleOpen();
  void handleSave();
private:
  Editor* editor;
  QMenuBar* menuBar;
};

#endif
