#include "fixedparenttypetemplate.h"

FixedParentTypeTemplate::FixedParentTypeTemplate(const std::string &name, const std::vector<std::string> &parameterNames, std::shared_ptr<ObjectType> parent)
    : ObjectTypeTemplate(name, parameterNames),
      _parent(parent)
{
}

FixedParentTypeTemplate::FixedParentTypeTemplate(const std::string &name, std::shared_ptr<ObjectType> parent)
    : ObjectTypeTemplate(name),
      _parent(parent)
{
}

std::shared_ptr<ObjectType> FixedParentTypeTemplate::parent(const ObjectType &) const
{
    return _parent;
}


