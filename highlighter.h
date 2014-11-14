#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

#include <boost/regex.hpp>

using namespace boost;

// Define Regular
namespace PT{
    const char* const FIRST_TITLE = "^#[^#]+";
    const char* const SECOND_TITLE = "^##[^#]+";
    const char* const THIRD_TITLE = "^###[^#]+";
    const char* const FORTH_TITLE = "^####[^#]+";
    const char* const HR = "^-{3,}";
    const char* const BOLD = "\\*\\*[^\\*]+?\\*\\*";
    const char* const ITALIC = "(?<!\\*)\\*[^\\*]+?\\*";
    const char* const STRING = "`[^`]+`";
    const char* const LINK = "(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?";
    const char* const EMAIL = "\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*";
    const char* const QUOTE_START = "^>.*";
    const char* const QUOTE_END = "^\\n";

}

const int STATE_NORMAL = 0;
const int STATE_QUOTE = 1;
const int STATE_CODE = 2;

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE


//! [0]
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

    void setupFormat(QTextCharFormat &format, Qt::GlobalColor color,
                     QFont::Weight fontWeight, const char* pattern);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        boost::regex pattern;
        QTextCharFormat format;
    };
    HighlightingRule rule;
    QList<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat firsecTitleFormat;
    QTextCharFormat thiforTitleFormat;
    QTextCharFormat italicFormat;
    QTextCharFormat boldFormat;
    QTextCharFormat emailFormat;
    QTextCharFormat linkFormat;
    QTextCharFormat hrFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat quoteFormat;

    regex regQuateStart;
    regex regQuateEnd;
};
//! [0]

#endif // HIGHLIGHTER_H
