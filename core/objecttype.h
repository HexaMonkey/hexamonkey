#ifndef OBJECTTYPE_H
#define OBJECTTYPE_H

#include <vector>

#include "variant.h"

class ObjectTypeTemplate;

/*!
 * \brief The ObjectType class
 */
class ObjectType
{
public:
    ObjectType();
    ObjectType(const ObjectTypeTemplate& typeTemplate);

    const ObjectTypeTemplate &typeTemplate() const;
    const Variant& parameterValue(int index) const;
    bool parameterSpecified(int index) const;
    void setParameter(int index, const Variant& value);
    template<typename... Args> void setParameters(Args... args){return _setParameters(0, args...);}

    void setParameterByName(const std::string& parameterName, const Variant& value);

    const ObjectType &importParameters(const ObjectType& type);

    bool isNull() const;

    bool extendsDirectly(const ObjectType& other) const;

    std::ostream& display(std::ostream& out) const;

    friend void swap(ObjectType& a, ObjectType& b);
    ObjectType& operator=(ObjectType other);

private:
    friend class MutableTypeScope;
    friend class ConstTypeScope;
    const ObjectTypeTemplate* _typeTemplate;
    std::vector<Variant> _parametersValue;

    void _setParameters(int first)
    {
    }

    template<typename... Args>
    void _setParameters(int first, const Variant& v, Args... args)
    {
        setParameter(first, v);
        _setParameters(first+1, args...);
    }
};

bool operator==(const ObjectType& a, const ObjectType& b);
bool operator!=(const ObjectType& a, const ObjectType& b);
bool operator< (const ObjectType& a, const ObjectType& b);

std::ostream& operator<<(std::ostream& out, const ObjectType& type);

#endif // OBJECTTYPE_H
