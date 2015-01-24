#ifndef STRUCTPARSER_H
#define STRUCTPARSER_H

#include "core/containerparser.h"

#include <vector>

/**
 * @brief Parser for C-like struct object
 */
class StructParser : public ContainerParser
{
public:
    StructParser(Object& object, const Module& module);
    void addElement(const ObjectType& type, const std::string& name);

protected:
    void doParseHead();
    void doParse();

private:
    std::vector<ObjectType> _types;
    std::vector<std::string> _names;

    bool _parsedInHead;
};

#endif // STRUCTPARSER_H
