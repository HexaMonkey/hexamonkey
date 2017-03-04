#include "core/objecttype.h"
#include "core/objecttypetemplate.h"
#include "core/parser.h"
#include "core/log/logmanager.h"
#include "core/variable/variablecollector.h"

#include "core/variable/typescope.h"

#define A_COUNT 0
#define A_ELEMENT_TYPE 1
#define A_ELEMENT_COUNT 2
#define A_NAME 3

const std::unordered_map<std::string, int> reserved = {
    {"@count", A_COUNT},
    {"@elemType", A_ELEMENT_TYPE},
    {"@elemCount", A_ELEMENT_COUNT},
    {"@name", A_NAME}
};

/**
 * @brief Basic variable implentation owning a \link Variant value\endlink by copy
 */
class TypeParameterVariableImplementation : public VariableImplementation
{
public:
    TypeParameterVariableImplementation(VariableCollector& variableCollector, VariableMemory memory, AbstractTypeScope& typeScope, int index) :
        VariableImplementation(variableCollector),
        _memory(memory),
        _typeScope(typeScope),
        _index(index)
    {

    }

protected:
    virtual void doSetValue(const Variant& value) override
    {
        _typeScope.modifiableType()->setParameter(_index, value);
    }

    virtual Variant doGetValue() override
    {
        return _typeScope.constType().parameterValue(_index);
    }

    virtual void collect(const std::function<void(VariableMemory&)>& addAccessible)
    {
        addAccessible(_memory);
    }

private:
    VariableMemory _memory;
    AbstractTypeScope& _typeScope;
    int _index;
};

class ConstTypeNameVariableImplementation : public VariableImplementation
{
public:
    ConstTypeNameVariableImplementation(VariableCollector& collector, const ObjectType& type)
        : VariableImplementation(collector),
          _type(type)
    {

    }

protected:
    virtual Variant doGetValue() override {
        return Variant(_type.name());
    }
private:
    const ObjectType& _type;
};

class ConstTypeElementTypeVariableImplementation : public VariableImplementation
{
public:
    ConstTypeElementTypeVariableImplementation(VariableCollector& collector, const ObjectType& type)
        : VariableImplementation(collector),
          _type(type)
    {

    }

protected:
    virtual Variant doGetValue() override {
        return _type.attributeValue(ObjectTypeTemplate::Attribute::elementType);
    }
private:
    const ObjectType& _type;
};

class ConstTypeElementCountVariableImplementation : public VariableImplementation
{
public:
    ConstTypeElementCountVariableImplementation(VariableCollector& collector, const ObjectType& type)
        : VariableImplementation(collector),
          _type(type) {}

protected:
    virtual Variant doGetValue() override {
        return _type.attributeValue(ObjectTypeTemplate::Attribute::elementCount);
    }
private:
    const ObjectType& _type;
};

AbstractTypeScope::AbstractTypeScope(VariableCollector &collector)
    : VariableImplementation(collector)
{
}

Variant AbstractTypeScope::doGetValue()
{
    return constType();
}

Variable AbstractTypeScope::doGetField(const Variant &key, bool modifiable, bool createIfNeeded)
{
    const ObjectType& cType = constType();

    int64_t parameterIndex = -1LL;
    const size_t numberOfParameters = cType.numberOfParameters();

    if (key.hasNumericalType()) {
        parameterIndex = key.toInteger();
        if (parameterIndex < 0 || parameterIndex >= numberOfParameters) {
            if (createIfNeeded) {
                Log::error("Trying to assign a value to an out of bounds parameter");
            }

            return Variable();
        }
    } else if (key.type() == Variant::stringType) {
        const std::string str = key.toString();



        if (!str.empty() && str[0]=='@') {
            auto it = reserved.find(str);
            if(it == reserved.end()) {
                Log::error("Unknown reserved field", str," for type ", cType);

                return Variable();
            }

            const int tag = it->second;

            switch(tag) {
            case A_COUNT:
                return collector().copy(numberOfParameters, false);

            case A_ELEMENT_TYPE:
                return Variable(new ConstTypeElementTypeVariableImplementation(collector(), cType), false);

            case A_ELEMENT_COUNT:
                return Variable(new ConstTypeElementCountVariableImplementation(collector(), cType), false);

            case A_NAME:
                return Variable(new ConstTypeNameVariableImplementation(collector(), cType), false);
            }

            return Variable();
        } else {
            parameterIndex = cType.typeTemplate().parameterNumber(str);

            if (parameterIndex == -1) {
                if (createIfNeeded) {
                    Log::error("Trying to modify an unknown named parameter ", str, " for type ", cType);
                }
                return Variable();
            }

        }
    } else {
        if (createIfNeeded) {
            Log::error("Trying to modify an invalid parameter ", key, " for type ", cType);
        }
        return Variable();
    }

    if (parameterIndex != -1) {
        return Variable(new TypeParameterVariableImplementation(collector(), memory(), *this, parameterIndex), modifiable);
    } else {
        return Variable();
    }
}

TypeScope::TypeScope(VariableCollector& collector, ObjectType &type, bool modifiable)
    : AbstractTypeScope(collector),
      _type(modifiable? &type : nullptr),
      _constType(type)
{
}

TypeScope::TypeScope(VariableCollector& collector, const ObjectType &type)
    : AbstractTypeScope(collector),
      _type(nullptr),
      _constType(type)
{
}

ObjectType *TypeScope::modifiableType()
{
    return _type;
}

const ObjectType &TypeScope::constType()
{
    return _constType;
}

ParserTypeScope::ParserTypeScope(Parser& parser)
    : AbstractTypeScope(parser.object().collector()),
      _object(parser.object()),
      _sharedType(parser.sharedType())
{
}

ObjectType *ParserTypeScope::modifiableType()
{
    if (_sharedType->first) {
        Log::error("Cannot modify parser type's once it has been parsed");
        return nullptr;
    } else {
        return &_object.type();
    }
}

const ObjectType &ParserTypeScope::constType()
{
    if (_sharedType->first) {
        return _sharedType->second;
    } else {
        return _object.type();
    }
}
