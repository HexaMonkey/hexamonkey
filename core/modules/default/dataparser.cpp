#include "dataparser.h"

DataParser::DataParser(Object &object, int64_t size)
    : SimpleParser(object),
      size(size)
{
}

void DataParser::doParseHead()
{
    setSize(size);
}
