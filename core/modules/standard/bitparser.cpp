#include "bitparser.h"
#include <bitset>

BitParser::BitParser(Object &object, int64_t size)
    : SimpleParser(object),
      size(size)
{
}

void BitParser::doParseHead()
{
    if(size>64)
    {
        std::cerr<<"Warning size to large for bitset"<<std::endl;
    }
    setSize(size);

    std::bitset<64> flag;
    char bit;
    for(unsigned int i = 0; i < size; i+=1)
    {
        file().read(&bit, 1);
        flag <<= 1;
        flag |= bit;
    }
    setInfo("0b"+flag.to_string().substr(64 - size, size));
    setValue(flag.to_ullong());
}


ContentParser::ContentParser(Object &object, int64_t size)
    : SimpleParser(object),
      size(size)
{
    setExpandOnAddition();
}

void ContentParser::doParseHead()
{
    setSize(size);
}
