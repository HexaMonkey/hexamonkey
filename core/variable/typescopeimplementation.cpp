#include "core/objecttype.h"
#include "core/objecttypetemplate.h"
#include "core/parser.h"
#include "core/log/logmanager.h"

#include "core/variable/typescopeimplementation.h"

#define A_COUNT 0
#define A_ELEMENT_TYPE 1
#define A_ELEMENT_COUNT 2
#define A_NAME 3

const std::unordered_map<std::string, int> reserved = {
    {"@count", A_COUNT},
    {"@elementType", A_ELEMENT_TYPE},
    {"@elementCount", A_ELEMENT_COUNT},
    {"@name", A_NAME}
};

class TypeNameVariableImplementation : public VariableImplementation
{
public:
    TypeNameVariableImplementation(ObjectType& type) : _type(type) {}

protected:
    virtual void doSetValue(const Variant& value) override {
        if (value.type() == Variant::stringType) {
            _type.setName(value.toString());
        } else {
            Log::warning("Trying to set an invalid value ", value, " as name for type ", _type);
        }
    }

    virtual Variant doGetValue() override {
        return Variant(_type.name());
    }
private:
    ObjectType& _type;
};

class ConstTypeNameVariableImplementation : public VariableImplementation
{
public:
    ConstTypeNameVariableImplementation(const ObjectType& type) : _type(type) {}

protected:
    virtual Variant doGetValue() override {
        return Variant(_type.name());
    }
private:
    const ObjectType& _type;
};

class TypeElementTypeVariableImplementation : public VariableImplementation
{
public:
    TypeElementTypeVariableImplementation(ObjectType& type) : _type(type) {}

protected:
    virtual void doSetValue(const Variant& value) override {
        if (value.hasNumericalType()) {
            _type.setElementCount(value.toInteger());
        } else {
            Log::warning("Trying to set an invalid value ", value, " as element count for type ", _type);
        }
    }

    virtual Variant doGetValue() override {
        return Variant(_type.elementCount());
    }
private:
    ObjectType& _type;
};

class ConstTypeElementTypeVariableImplementation : public VariableImplementation
{
public:
    ConstTypeElementTypeVariableImplementation(const ObjectType& type) : _type(type) {}

protected:
    virtual Variant doGetValue() override {
        return Variant(_type.elementType());
    }
private:
    const ObjectType& _type;
};

class TypeElementCountVariableImplementation : public VariableImplementation
{
public:
    TypeElementCountVariableImplementation(ObjectType& type) : _type(type) {}

protected:
    virtual void doSetValue(const Variant& value) override {
        if (value.hasNumericalType()) {
            _type.setElementType(value.toObjectType());
        } else {
            Log::warning("Trying to set an invalid value ", value, " as element type for type ", _type);
        }
    }

    virtual Variant doGetValue() override {
        return Variant(_type.elementType());
    }
private:
    ObjectType& _type;
};

class ConstTypeElementCountVariableImplementation : public VariableImplementation
{
public:
    ConstTypeElementCountVariableImplementation(const ObjectType& type) : _type(type) {}

protected:
    virtual Variant doGetValue() override {
        return Variant(_type.elementCount());
    }
private:
    const ObjectType& _type;
};

Variable AbstractTypeScopeImplementation::doGetField(const Variant &key, bool modifiable)
{
    ObjectType* mType = modifiable ? modifiableType() : nullptr;
    const ObjectType& cType = constType();

    int64_t parameterIndex = -1LL;
    const size_t numberOfParameters = cType.numberOfParameters();

    if (key.hasNumericalType()) {
        parameterIndex = key.toInteger();
        if (parameterIndex < 0 || parameterIndex >= numberOfParameters) {
            if (modifiable) {
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
                return Variable::copy(numberOfParameters, false);

            case A_ELEMENT_TYPE:
                if (mType != nullptr) {
                    return Variable(new TypeElementTypeVariableImplementation(*mType), true);
                } else {
                    return Variable(new ConstTypeElementTypeVariableImplementation(cType), false);
                }

            case A_ELEMENT_COUNT:
                if (mType != nullptr) {
                    return Variable(new TypeElementCountVariableImplementation(*mType), true);
                } else {
                    return Variable(new ConstTypeElementCountVariableImplementation(cType), false);
                }

            case A_NAME:
                if (mType != nullptr) {
                    return Variable(new TypeNameVariableImplementation(*mType), true);
                } else {
                    return Variable(new ConstTypeNameVariableImplementation(cType), false);
                }
            }

            return Variable();
        } else {
            parameterIndex = cType.typeTemplate().parameterNumber(str);

            if (parameterIndex == -1) {
                if (modifiable) {
                    Log::error("Trying to modify an unknown named parameter ", str, " for type ", cType);
                }
                return Variable();
            }

        }
    } else {
        if (modifiable) {
            Log::error("Trying to modify an invalid parameter ", key, " for type ", cType);
        }
        return Variable();
    }

    if (parameterIndex != -1) {
        if(mType) {
            return Variable::ref(mType->_parametersValue[parameterIndex]);
        } else {
            return Variable::constRef(cType._parametersValue[parameterIndex]);
        }
    } else {
        return Variable();
    }
}

TypeScopeImplementation::TypeScopeImplementation(ObjectType &type, bool modifiable)
    : _type(modifiable? &type : nullptr),
      _constType(type)
{
}

TypeScopeImplementation::TypeScopeImplementation(const ObjectType &type)
    : _type(nullptr),
      _constType(type)
{
}

ObjectType *TypeScopeImplementation::modifiableType()
{
    return _type;
}

const ObjectType &TypeScopeImplementation::constType()
{
    return _constType;
}

ParserTypeScopeImplementation::ParserTypeScopeImplementation(Parser &parser)
    : _parser(parser)
{
}

ObjectType *ParserTypeScopeImplementation::modifiableType()
{
    return _parser.modifiableType();
}

const ObjectType &ParserTypeScopeImplementation::constType()
{
    return _parser.constType();
}
