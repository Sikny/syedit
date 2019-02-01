#include <QApplication>
#include "mainwindow.h"
#include "theme.h"

int main(int argc, char** argv){
  QApplication app(argc, argv);

  MainWindow window;
  qApp->setStyleSheet("MainWindow, MainWindow * {background-color: "
                        + Theme::Instance().color("background").name() + ";"
                        + "color: "
                        + Theme::Instance().color("text").name() + ";}"
+"QScrollBar:vertical{background: #f1f1f1; width: 10px;}"
+"QScrollBar::handle{background: #888;}"
+"QScrollBar::handle:hover{background: #555;}"
+"QScrollBar::add-page, QScrollBar::sub-page{background: none;}"
+"QScrollBar::add-line, QScrollBar::sub-line{background: none;}");
  window.show();

  return app.exec();
}
