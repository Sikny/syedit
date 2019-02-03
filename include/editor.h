#ifndef EDITOR_H
#define EDITOR_H

#include <QPlainTextEdit>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QPainter>

#include "highlighter.h"

class Editor : public QPlainTextEdit{
    Q_OBJECT
public:
  Editor(QWidget * parent = nullptr);
  QString getFileName(){
    return fileName;
  }
  QString getFileNameWithoutPath(){
    return QFileInfo(fileName).fileName();
  }
  void setFileName(QString name){
    fileName = name;
  }
  void saveFile();
  void openFile();

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void highlightCurrentLine();

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);

private:
  QString fileName;
  QWidget *lineNumberArea;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(Editor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    Editor *codeEditor;
};

#endif
