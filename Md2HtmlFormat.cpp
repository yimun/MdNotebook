#include "Md2HtmlFormat.h"
#include <fstream>


Md2HtmlFormat::Md2HtmlFormat()
{
}


Md2HtmlFormat::~Md2HtmlFormat()
{
}


string Md2HtmlFormat::format(const string &srctext)
{
	string text = srctext;
    addRules(text, FPT::FIRST_TITLE, FT::FIRST_TITLE);
    addRules(text, FPT::SECOND_TITLE, FT::SECOND_TITLE);
    addRules(text, FPT::THIRD_TITLE, FT::THIRD_TITLE);
    addRules(text, FPT::FORTH_TITLE, FT::FORTH_TITLE);
    addRules(text, FPT::HR, FT::HR);
    addRules(text, FPT::ITALIC, FT::ITALIC);
    addRules(text, FPT::BOLD, FT::BOLD);
    addRules(text, FPT::STRING, FT::STRING);
    addRules(text, FPT::LINK, FT::LINK);
    addRules(text, FPT::QUOTE, FT::QUOTE);
    addRules(text, FPT::LIST_ITEM, FT::LIST_ITEM);
    addRules(text, FPT::LIST_BLOCK, FT::LIST_BLOCK);
    addRules(text, FPT::CODE, FT::CODE);
	//addRules(text, PT::PARA, FT::PARA);
	addRules(text, "\\n\\n(^[^<].*?)\\n\\n", "\\1<br>"); // 段落整理
	return this->render(text);
	

}

void Md2HtmlFormat::addRules(string &text, const char *cpattern, const char *cforamt)
{
	regex pattern(cpattern);
	string format(cforamt);
	text = regex_replace(text, pattern, format);
}

void Md2HtmlFormat::setStyleFile(const string &name)
{
	this->styleFile = name;

}

std::string Md2HtmlFormat::render(string &src)
{
	fstream fin;
	fin.open("I:/Users/linwei/Desktop/style.html", ifstream::in);
    char ch;
    string sty;
	int i = 0;
	while (fin.good())
	{
        fin.get(ch);
        sty += ch;
	}
//	ch[i] = '\0';
	fin.close();
//	string sty(ch);
	return regex_replace(sty, regex("%CONTENT%"), src);
}

int main11()
{
	fstream fin;
	fin.open("I:/Users/linwei/Desktop/ch.md",ifstream::in);
	int k = 0;
	string src;
	char ch;
//    while (!fin.eof())
    while (fin.good())
	{
		fin.get(ch);
		src = src + ch;
	}
	cout << src << endl;
	fin.close();
	Md2HtmlFormat f;
	string s = f.format(src);
	fstream fout("I:/Users/linwei/Desktop/out.html",ifstream::out);
	fout << s <<endl;
	fout.close();
	//cout << s << endl;
	system("pause");
	return 0;
}
