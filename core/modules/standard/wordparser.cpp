#include "wordparser.h"

WordParser::WordParser(Object &object, int numberOfChars)
    : SimpleParser(object), numberOfChars(numberOfChars)
{
}

void WordParser::doParseHead()
{
    setSize(numberOfChars * 8 );
    char* _word = new char[numberOfChars+1];
    file().read(_word, object().size());
    _word[numberOfChars] = '\0';
    setValue(std::string(_word));
    setInfo("\""+std::string(_word)+"\"");
}


Utf8StringParser::Utf8StringParser(Object &object)
    : SimpleParser(object)
{
}

void Utf8StringParser::doParseHead()
{
    std::stringstream S;
    std::streamoff stringLength = 0;
    S<<"\"";
    while(file().good())
    {
        char ch;
        file().read(&ch, 8);
        ++stringLength;
        if(ch == '\0')
        {
            break;
        }
        char mask = 1 << 7;
        if((ch & mask) == 0)
        {
            S<<ch;
        }
        else
        {
            for(mask>>=1; ch & mask; mask>>=1)
            {
                file().seekg(8,std::ios::cur);
            }
        }
    }
    S<<"\"";
    setValue(S.str());
    setInfo(S.str());
    setSize(8*stringLength);
}
