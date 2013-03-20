#include "fileparser.h"
#include "object.h"

FileParser::FileParser(Object &object) : SimpleParser(object)
{
}

void FileParser::doParseHead()
{
    int64_t size = file().size();
    setSize(size);

    setInfo("\""+file().path()+"\"");
}
