#include "objecttypetemplate.h"
#include "objecttype.h"

ObjectTypeTemplate::ObjectTypeTemplate(const std::string &name, const std::vector<std::string> &parameterNames)
    :_name(name), _parametersNames(parameterNames)

{
    for(unsigned int i = 0; i < _parametersNames.size(); ++i)
    {
        _parametersNumbers[_parametersNames[i]] = i;
    }
}

ObjectTypeTemplate::ObjectTypeTemplate(const std::string& name)
    : ObjectTypeTemplate(name, std::vector<std::string>())
{
}


const std::string& ObjectTypeTemplate::name() const
{
    return _name;
}

int ObjectTypeTemplate::numberOfParameters() const
{
    return _parametersNames.size();
}

const std::string& ObjectTypeTemplate::parameterName(int index) const
{
    return _parametersNames[index];
}

int ObjectTypeTemplate::parameterNumber(const std::string &name) const
{
    const auto it = _parametersNumbers.find(name);
    if(it != _parametersNumbers.end())
        return it->second;
    return -1;
}

bool ObjectTypeTemplate::isNull() const
{
    return _name.empty();
}

void ObjectTypeTemplate::addParameter(const std::string& parameterName)
{
    _parametersNames.push_back(parameterName);
}

bool operator==(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b)
{
    return (a._name == b._name);
}

bool operator!=(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b)
{
    return !(a == b);
}

bool operator< (const ObjectTypeTemplate& a, const ObjectTypeTemplate& b)
{
    return (a._name < b._name);
}

bool operator<= (const ObjectTypeTemplate& a, const ObjectTypeTemplate& b)
{
    return (a<b)||(a==b);
}
