#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <iostream>
#include <fstream>

#include <QSyntaxHighlighter>
#include <QRegularExpression>

#include "settings.h"

class Highlighter : public QSyntaxHighlighter{
public:
    Highlighter(QTextDocument* parent, QString& language);
    void highlightBlock(const QString &text);
    void highlightLine(const QString &text, const QString &expression,
            const QBrush &color);
    void highlightMultiLine(const QString &text, const QString &start,
            const QString &end, const QBrush &color);
    QString getLanguage(){
        return language;
    }

private:
    QString language;
    QString primKeyRegex;
};

#endif // HIGHLIGHTER_H
