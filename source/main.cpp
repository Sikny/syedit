#include <QApplication>
#include "mainwindow.h"
#include "settings.h"

int main(int argc, char** argv){
  QApplication app(argc, argv);

  MainWindow window;
  window.resize(600, 400);
  qApp->setStyleSheet("QScrollBar:vertical{background: #f1f1f1; width: 10px;}"
"QScrollBar::handle{background: #888;}"
"QScrollBar::handle:hover{background: #555;}"
"QScrollBar::add-page, QScrollBar::sub-page{background: none;}"
"QScrollBar::add-line, QScrollBar::sub-line{background: none;}");
  window.show();

  return app.exec();
}
