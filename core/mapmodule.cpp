#include "mapmodule.h"

#include "objecttypetemplate.h"

const MapModule::ParserGenerator& MapModule::nullGenerator = []parserLambda{return nullptr;};

void MapModule::addParser(const std::string &name, const MapModule::ParserGenerator &parserGenerator)
{
    _map[name] = parserGenerator;
}

void MapModule::addParser(const std::string &name)
{
    addParser(name, nullGenerator);
    setFixedSize(name, 0);
}

void MapModule::setFixedSize(const std::string &name, const FixedSizeGenerator &fixedSizeFunction)
{
    _sizes[name] = fixedSizeFunction;
}

void MapModule::setFixedSize(const std::string &name, int64_t fixedSize)
{
    _sizes[name] = [fixedSize] fixedSizeLambda {return fixedSize;};
}

void MapModule::setFixedSizeFromArg(const std::string &name, int arg)
{
    _sizes[name] = [arg]fixedSizeLambda
        {
             if(type.parameterSpecified(arg))
                 return type.parameterValue(arg).toInteger();
             return -1;
        };
}

bool MapModule::hasParser(const ObjectType &type) const
{
    return _map.find(type.typeTemplate().name()) != _map.end();
}

Parser *MapModule::getParser(const ObjectType &type, Object &object, const Module &fromModule) const
{
    auto it = _map.find(type.typeTemplate().name());
    if(it != _map.end())
    {
        return (it->second)(type, object, fromModule);
    }
    else
    {
        return nullptr;
    }
}

int64_t MapModule::doGetFixedSize(const ObjectType &type, const Module &module) const
{
    auto it = _sizes.find(type.typeTemplate().name());
    if(it != _sizes.end())
    {
        return (it->second)(type, module);
    }
    else
    {
        return -1;
    }
}
