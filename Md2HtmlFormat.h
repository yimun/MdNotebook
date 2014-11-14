#pragma once

#include <string>
#include <boost/regex.hpp>
#include <iostream>
using namespace std;
using namespace boost;

namespace FPT{
	// end must not include enter ｛$|$(?!\\n)｝是测试器所有行的结尾，这里用$就行了
	const char* const FIRST_TITLE = "^#([^#]+?)$"; 
	const char* const SECOND_TITLE = "^##([^#]+?)$";
	const char* const THIRD_TITLE = "^###([^#]+?)$";
	const char* const FORTH_TITLE = "^####([^#]+?)$";
	const char* const HR = "(-{3,})";
	const char* const BOLD = "\\*\\*([^\\*]+?)\\*\\*";
	const char* const ITALIC = "(?<!\\*)\\*([^\\*]+?)\\*";
	const char* const STRING = "`([^`]+)`";
	// 1name 2link
	const char* const LINK = "\\[(.*?)\\]\\(((http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?)\\)";
	//const char* const EMAIL = "\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*";
	const char* const QUOTE = "^>(.*?)(?=\\n\\n)";
	const char* const CODE = "((^((\\s{4,})|\\t).*?$\\n){1,})";

	const char* const PARA = "(?<=\\n)(^[^\\n#].+?)(?=\\n\\n|\\n#)";
	const char* const LIST_ITEM = "^-([^-]*?)(?=\\n)";
	const char* const LIST_BLOCK = "(<LI>.*?)(?=\\n\\n)";

}

namespace FT{
	const char* const FIRST_TITLE =  "<H1>\\1</H1>";
	const char* const SECOND_TITLE = "<H2>\\1</H2>";
	const char* const THIRD_TITLE =  "<H3>\\1</H3>";
	const char* const FORTH_TITLE =  "<H4>\\1</H4>";
	const char* const HR = "<HR>";
	const char* const BOLD = "<STRONG>\\1</STRONG>";
	const char* const ITALIC = "<EM>\\1</EM>";
	const char* const STRING = "<CODE>\\1</CODE>";
	const char* const LINK = "<A href=\"\\2\">\\1</A>";
	const char* const QUOTE = "<BLOCKQUOTE><P>\\1</P></BLOCKQUOTE>";
	const char* const CODE = "<PRE><CODE>\\1</CODE></PRE>";
	const char* const PARA = "<P>\\1</P>";
	const char* const LIST_ITEM = "<LI>\\1</LI>";
	const char* const LIST_BLOCK = "<UL>\\1</UL>";

}


class Md2HtmlFormat
{
public:
	Md2HtmlFormat();
	virtual ~Md2HtmlFormat();
	string format(const string &srctext);
	void setStyleFile(const string &name);
	string render(string &src);
private:
	string text;
	string styleFile;
	void addRules(string &text,const char *pattern, const char *foramt);
};

