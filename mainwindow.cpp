#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent) {
    createActions();
}

MainWindow::~MainWindow() {

}

void MainWindow::createActions(){
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
}
