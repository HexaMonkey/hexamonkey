#include "objectscopeimplementation.h"

#include "core/log/logmanager.h"

#include "core/object.h"

#define A_SIZE 0
#define A_PARENT 1
#define A_ROOT 2
#define A_VALUE 3
#define A_RANK 4
#define A_POS 5
#define A_REM 6
#define A_NUMBER_OF_CHILDREN 7
#define A_BEGINNING_POS 8
#define A_LINK_TO 9
#define A_ATTR 10
#define A_CONTEXT 11
#define A_GLOBAL 12

const std::unordered_map<std::string, int> reserved = {
    {"@size",             A_SIZE},
    {"@value",            A_VALUE},
    {"@parent",           A_PARENT},
    {"@root",             A_ROOT},
    {"@rank",             A_RANK},
    {"@pos",              A_POS},
    {"@rem",              A_REM},
    {"@numberOfChildren", A_NUMBER_OF_CHILDREN},
    {"@beginningPos",     A_BEGINNING_POS},
    {"@linkTo",           A_LINK_TO},
    {"@attr",             A_ATTR},
    {"@context",          A_CONTEXT},
    {"@global",           A_GLOBAL}
};

class ObjectPosVariableImplementation : public VariableImplementation
{
public:
    ObjectPosVariableImplementation(Object& object) : _object(object) {}

protected:
    virtual void doSetValue(const Variant& value) override {
        if (value.hasNumericalType()) {
            _object.setPos(value.toInteger());
        } else {
            Log::warning("Trying to set a non numerical value for a pos");
        }
    }

    virtual Variant doGetValue() override {
        return Variant((long long) _object.pos());
    }
private:
    Object& _object;
};

class ObjectSizeVariableImplementation : public VariableImplementation
{
public:
    ObjectSizeVariableImplementation(Object& object) : _object(object) {}

protected:
    virtual void doSetValue(const Variant& value) override {
        if (value.hasNumericalType()) {
            _object.setSize(value.toInteger());
        } else {
            Log::warning("Trying to set a non numerical value for a size");
        }
    }

    virtual Variant doGetValue() override {
        return Variant((long long) _object.size());
    }
private:
    Object& _object;
};

class ObjectLinkToVariableImplementation : public VariableImplementation
{
public:
    ObjectLinkToVariableImplementation(Object& object) : _object(object) {}

protected:
    virtual void doSetValue(const Variant& value) override {
        if (value.hasNumericalType()) {
            _object.setLinkTo(value.toInteger());
        } else if (value.isNull()) {
            _object.removeLinkTo();
        } else {
            Log::warning("Trying to set a non numerical value for a linkTo");
        }
    }

    virtual Variant doGetValue() override {
        return _object.hasLinkTo() ? Variant((long long) _object.linkTo()) : Variant();
    }
private:
    Object& _object;
};


ObjectScopeImplementation::ObjectScopeImplementation(Object &object)
    : _object(object)
{
}

Variable ObjectScopeImplementation::doGetField(const Variant &key, bool modifiable)
{
    if (key.isValueless()) {

        int numberOfChildren = _object.numberOfChildren();
        if (numberOfChildren > 0) {
            Object* elem = _object.access(numberOfChildren - 1, true);
            if(elem != nullptr) {
                return elem->variable();
            } else {
                return Variable();
            }
        } else {
            return Variable();
        }

    } else if (key.type() == Variant::stringType) {

        const std::string& name = key.toString();
        if (name[0] == '@')
        {
            auto it = reserved.find(name);
            if (it == reserved.end()) {
                return Variable();
            } else {

                switch (it->second) {
                    case A_SIZE:
                        return Variable(new ObjectSizeVariableImplementation(_object), modifiable);

                    case A_VALUE:
                        return Variable::ref(_object.value(), modifiable);

                    case A_PARENT:
                    {
                        Object* parent = _object.parent();

                        if (parent != nullptr) {
                            return parent->variable();
                        } else {
                            return Variable();
                        }
                    }

                    case A_ROOT:
                        return _object.root().variable();

                    case A_RANK:
                        return Variable::copy(_object.rank());

                    case A_POS:
                        return Variable(new ObjectPosVariableImplementation(_object), modifiable);

                    case A_REM:
                        return Variable::copy(_object.size() - _object.pos());

                    case A_NUMBER_OF_CHILDREN:
                        _object.explore(1);
                        return Variable::copy(_object.numberOfChildren());

                    case A_BEGINNING_POS:
                        return Variable::copy((long long) _object.beginningPos());

                    case A_LINK_TO:
                        return Variable(new ObjectLinkToVariableImplementation(_object), modifiable);

                    case A_ATTR:
                        return _object.attributesVariable(modifiable);

                    case A_CONTEXT:
                        return _object.contextVariable(modifiable);

                    case A_GLOBAL:
                        return _object.root().contextVariable(modifiable);

                    default:
                        return Variable();
                }
            }
        } else {

            Object* elem = _object.lookUp(name, true);
            if (elem != nullptr) {
                return elem->variable();
            } else {
                return Variable();
            }
        }

    } else if (key.hasNumericalType()) {

        Object* elem = _object.access(key.toInteger(), true);
        if (elem != nullptr) {
            return elem->variable();
        } else {
            return Variable();
        }

    } else if (key.type() == Variant::objectType) {

        Object* elem = _object.lookForType(key.toObjectType(), true);
        if (elem != nullptr) {
            return elem->variable();
        } else {
            return Variable();
        }

    } else {

        return Variable();

    }
}

void ObjectScopeImplementation::doSetValue(const Variant &value)
{
    _object.setValue(value);
}

Variant ObjectScopeImplementation::doGetValue()
{
    return _object.value();
}
