#include "highlighter.h"

Highlighter::Highlighter(QTextDocument* parent,
                         QString &language) : QSyntaxHighlighter(parent){
    this->language = language;
    primKeyRegex = "";
    std::ifstream is;
    std::string wordBuf;
    std::string file = "resources/colh/prim" + language.toStdString();
    is.open(file);
    int i = 0;
    if(is.is_open()){
        while(is >> wordBuf){
            primKeyRegex += "\\b"+QString::fromStdString(wordBuf)+"\\b";
            primKeyRegex += "|";
            i++;
        }
    } else {
        std::cout << "File " << file << " not found" << std::endl;
    }
    is.close();
}

void Highlighter::highlightBlock(const QString &text){
    highlightLine(text, "\\b[+-]?[0-9]*[.]?[0-9]+\\b", QColor(240, 0, 240));
    highlightLine(text, "\\b0x[0-9A-Fa-f]+\\b", QColor(240, 0, 240));
    highlightLine(text, primKeyRegex, QColor(0, 0, 160));
    highlightLine(text, "[+=;\\*[\\]{}()/<>:,-]*", QColor(255, 0, 0));
    highlightLine(text, "#.*$", QColor(0, 160, 0));
    highlightLine(text, "\".*\"", QColor(0, 0, 255));
    //highlightLine(text, "\"([^\"]*)\"", QColor(0, 0, 255));
    highlightLine(text, "//[^\n]*", Qt::gray);
    // multiline, different delimiters
    highlightMultiLine(text, "/\\*", "\\*/", Qt::gray);
}

void Highlighter::highlightMultiLine(const QString &text, const QString &start,
        const QString &end, const QBrush &color){
    QTextCharFormat multiCommentFormat;
    multiCommentFormat.setForeground(color);
    QRegularExpression startExpression(start);
    QRegularExpression endExpression(end);

    setCurrentBlockState(0);

    int startIndex = 0;
    if(previousBlockState() != 1)
        startIndex = text.indexOf(startExpression);

    while(startIndex >= 0){
        QRegularExpressionMatch endMatch;
        int endIndex = text.indexOf(endExpression, startIndex, &endMatch);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + endMatch.capturedLength();
        }
        setFormat(startIndex, commentLength, multiCommentFormat);
        startIndex = text.indexOf(startExpression,
                                     startIndex + commentLength);
    }
}


void Highlighter::highlightLine(const QString &text, const QString &expression,
        const QBrush &color){
    QTextCharFormat myClassFormat;
    myClassFormat.setForeground(color);

    QRegularExpression exp(expression);
    QRegularExpressionMatchIterator i = exp.globalMatch(text);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(), match.capturedLength(), myClassFormat);
    }
}
