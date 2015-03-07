#include "core/modules/default/dataparser.h"

DataParser::DataParser(Object &object, int64_t size)
    : SimpleParser(object),
      size(size)
{
    if(size==-1) {
        object.setToExpandOnAddition();
    }
}

void DataParser::doParseHead()
{
    if (size != -1) {
        object().setSize(size);
    }
}
