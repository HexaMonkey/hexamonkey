#include "enumparser.h"

#include <algorithm>

#include "core/variable/objectattributes.h"

EnumParser::EnumParser(Object& object, const Module& module, const ObjectType &type)
    :ContainerParser(object, module), _type(type)
{
    setNoTail();
}

void EnumParser::addElement(const Variant &key, const Variant &value)
{
    _elements.emplace_back(std::make_pair(key, value));
}

void EnumParser::doParseHead()
{
    std::unique_ptr<Object> childPtr(readVariable(_type));
    Object& child = *childPtr;

    const Variant& value = child.value();
    auto it = std::find_if(_elements.cbegin(), _elements.cend(), [&value](const std::pair<Variant,Variant>& elem) {
        return elem.first == value;
    });

    if (it != _elements.cend()) {
        object().attributes(true)->addNumbered().setValue(it->second);
    }

    object().setValue(value);
    object().setSize(child.size());
    setParsed();
}
