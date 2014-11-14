
#include "highlighter.h"


Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    // first title
    setupFormat(firsecTitleFormat,Qt::red,QFont::Bold,PT::FIRST_TITLE);
    // second title
    setupFormat(firsecTitleFormat,Qt::darkBlue,QFont::Bold,PT::SECOND_TITLE);
    // third title
    setupFormat(thiforTitleFormat,Qt::blue,QFont::Bold,PT::THIRD_TITLE);
    // forth title
    setupFormat(thiforTitleFormat,Qt::blue,QFont::Normal,PT::FORTH_TITLE);
    // italic
    italicFormat.setFontItalic(true);
    setupFormat(italicFormat,Qt::darkGray,QFont::Normal,PT::ITALIC);
    // bold
    setupFormat(boldFormat,Qt::darkGreen,QFont::Bold,PT::BOLD);
    // email address
    emailFormat.setFontItalic(true);
    emailFormat.setFontUnderline(true);
    setupFormat(emailFormat,Qt::black,QFont::Normal,PT::EMAIL);
    // url link
    linkFormat.setFontItalic(true);
    linkFormat.setFontUnderline(true);
    setupFormat(linkFormat,Qt::darkYellow,QFont::Normal,PT::LINK);
    // horizon line
    setupFormat(hrFormat,Qt::blue,QFont::Normal,PT::HR);
    // string
    setupFormat(stringFormat,Qt::darkYellow,QFont::Normal,PT::STRING);

    qDebug("rules length %d" , highlightingRules.length());
    regQuateStart = regex(PT::QUOTE_START);
    regQuateEnd = regex(PT::QUOTE_END);

    quoteFormat.setForeground(Qt::green);
    quoteFormat.setFontWeight(QFont::Normal);

}

void Highlighter::setupFormat(QTextCharFormat &format, Qt::GlobalColor color,
                              QFont::Weight fontWeight, const char *pattern)
{
    format.setForeground(color);
    format.setFontWeight(fontWeight);
    rule.format = format;
    rule.pattern = boost::regex(pattern);
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{

    QList<HighlightingRule>::iterator iter;
    const char *str;
    QByteArray ba = text.toLatin1();  //全部转化为单字节编码
    cmatch what;

    for (iter = highlightingRules.begin(); iter!=highlightingRules.end(); iter++)
    {
        str = ba.data();
        int index = 0;
        while(regex_search(str, what, (*iter).pattern))
        {
            size_t i = 0;
            index = index + what.position(i);
            setFormat(index , what.length(i), (*iter).format);
            str = str + what.position(i)+what.length(i);
            index += what.length(i);
        }
    }

     // match quote block
    setCurrentBlockState(STATE_NORMAL);
    str = ba.data();
    if(regex_search(str, what, regQuateStart))
    {
        setCurrentBlockState(STATE_QUOTE);
    }
    if(previousBlockState() == STATE_QUOTE)
    {
        if(strlen(str) != 0)
            setCurrentBlockState(STATE_QUOTE);
    }
    if(currentBlockState() == STATE_QUOTE)
    {
        setFormat(0,text.length(), quoteFormat);
    }

}
