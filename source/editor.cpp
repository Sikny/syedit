#include "editor.h"

/**
 * @brief Editor::Editor
 * Builds an editor and sets to it line numbers, current line highlighting
 * @param parent
 */
Editor::Editor(QWidget * parent) : QPlainTextEdit(parent){
    lineNumberArea = new LineNumberArea(this);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    setFont(Settings::Instance().getFont());

    QFontMetrics metrics(font());
    this->setTabStopWidth(Settings::Instance().getTabSize()*metrics.width(' '));
}

/**
 * @brief Editor::saveFile
 * Saves the current file opened in the editor. If file with fileName name
 * was not existing, creates it
 */
void Editor::saveFile(){
  QFile file(fileName);
  if(file.open(QFile::WriteOnly)){
    QTextStream out(&file);
	  out << this->toPlainText().toStdString().c_str();
  }
}

/**
 * @brief Editor::openFile
 * Opens one file in the editor @see MainWindow::handleOpen
 */
void Editor::openFile(){
  std::cout << fileName.toStdString() << std::endl;
  QFile file(fileName);
  if(file.open(QFile::ReadOnly)){
    QTextStream in(&file);
    QString line;
    QString content("");
    while(in.readLineInto(&line)){
      content.append(line+"\n");
    }
    this->setPlainText(content);
  }
}

/**
 * @brief Editor::lineNumberAreaWidth
 * Calculates the appropriate width needed to write the line number
 * character(s)
 * @return the space needed
 */
int Editor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = font().pointSize() + 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

/**
 * @brief Editor::updateLineNumberAreaWidth
 */
void Editor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

/**
 * @brief Editor::updateLineNumberArea
 * @param rect
 * @param dy
 */
void Editor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

/**
 * @brief Editor::resizeEvent
 * @param e
 */
void Editor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

/**
 * @brief Editor::highlightCurrentLine
 */
void Editor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = Settings::Instance().color("currentline").lighter(120);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

/**
 * @brief Editor::lineNumberAreaPaintEvent
 * @param event
 */
void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Settings::Instance().color("currentline"));
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
            if (block.isVisible() && bottom >= event->rect().top()) {
                QString number = QString::number(blockNumber + 1) + " ";
                painter.setPen(Settings::Instance().color("text"));
                painter.setFont(this->font());
                painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                 Qt::AlignRight, number);
            }

            block = block.next();
            top = bottom;
            bottom = top + static_cast<int>(blockBoundingRect(block).height());
            ++blockNumber;
    }
}
