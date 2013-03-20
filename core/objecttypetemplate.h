#ifndef OBJECTTYPETEMPLATE_H
#define OBJECTTYPETEMPLATE_H

#include <string>
#include <vector>
#include <map>

#include "variant.h"
#include "objecttype.h"

/*!
 * \brief The ObjectTypeTemplate class
 */
class ObjectTypeTemplate
{
public:
    ObjectTypeTemplate(const std::string &name, const std::vector<std::string>& parameterNames);
    ObjectTypeTemplate(const std::string &name);

    const std::string& name() const;
    int numberOfParameters() const;
    const std::string& parameterName(int index) const;
    int parameterNumber(const std::string& name) const;
    bool isNull() const;

    void addParameter(const std::string& parameterName);

    template<typename... Args> ObjectType operator()(Args... args) const
    {
        ObjectType type(*this);
        type.setParameters(args...);
        return type;
    }
    friend bool operator==(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);
    friend bool operator< (const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);

private:
    std::string                _name;
    std::vector<std::string>   _parametersNames;
    std::map<std::string, int>      _parametersNumbers;

    ObjectTypeTemplate(const ObjectTypeTemplate&) = delete;
    ObjectTypeTemplate& operator=(const ObjectTypeTemplate&) = delete;
};

const ObjectTypeTemplate nullTypeTemplate("");

bool operator!=(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);
bool operator<=(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);

#endif // OBJECTTYPETEMPLATE_H
