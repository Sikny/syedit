#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QPlainTextEdit>
#include <QSet>
#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QList>
#include <QTextCharFormat>
#include <QStringList>

#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>

/**
 * Class for text syntax highighting
 */
class Highlighter : public QSyntaxHighlighter{
public:
    Highlighter(QTextDocument* document, QString language);
    void highlightBlock(const QString &text);
    void highlightMultiLine(const QString &text, const QString &start,
                        const QString &end, const QBrush &color);
    void highlightLine(const QString &text, const QString &expression,
                       const QBrush &color);
    QString getLanguage();

private:
    QString language;
    QString primKeyRegex;
};

#endif
