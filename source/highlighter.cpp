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
        doHighlight = true;
    } else {
        std::cout << "File " << file << " not found" << std::endl;
        doHighlight = false;
    }
    is.close();
}

/**
 * @brief Highlighter::highlightBlock
 * Sets the highlighting rules for text block
 * @param text
 */
void Highlighter::highlightBlock(const QString &text){
    if(doHighlight){
        highlightLine(text, "\\b[+-]?[0-9]*[.]?[0-9]+\\b",
                  Settings::Instance().color("number"));
        highlightLine(text, "\\b0x[0-9A-Fa-f]+\\b",
                  Settings::Instance().color("number"));
        highlightLine(text, primKeyRegex, Settings::Instance().color("primary"));
        highlightLine(text, "[+=;\\*[\\]{}()/<>:,-]*",
                  Settings::Instance().color("operator"));
        highlightLine(text, "#.*$", Settings::Instance().color("secondary"));
        highlightLine(text, "\".*\"", Settings::Instance().color("string"));
        highlightLine(text, "//[^\n]*", Settings::Instance().color("comment"));
        // multiline, different delimiters
        highlightMultiLine(text, "/\\*", "\\*/", Settings::Instance().color("comment"));
    }
}

/**
 * @brief Highlighter::highlightMultiLine
 * Highlights a regex that is multiline
 * @param text
 * @param start
 * @param end
 * @param color
 */
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

/**
 * @brief Highlighter::highlightLine
 * Highlights a regex that is supposed to fit inline
 * @param text
 * @param expression
 * @param color
 */
void Highlighter::highlightLine(const QString &text, const QString &expression,
        const QBrush &color){
    QTextCharFormat myClassFormat;
    myClassFormat.setForeground(color);

    QRegularExpression exp(expression, QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatchIterator i = exp.globalMatch(text);
    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(), match.capturedLength(), myClassFormat);
    }
}
