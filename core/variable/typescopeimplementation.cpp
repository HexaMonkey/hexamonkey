#include "core/objecttype.h"
#include "core/objecttypetemplate.h"
#include "core/parser.h"
#include "core/log/logmanager.h"

#include "core/variable/typescopeimplementation.h"

#define A_COUNT 0
#define A_ELEMENT_TYPE 1
#define A_ELEMENT_COUNT 2
#define A_NAME 3

const std::map<std::string, int> reserved = {
    {"@count", A_COUNT},
    {"@elementType", A_ELEMENT_TYPE},
    {"@elementCount", A_ELEMENT_COUNT},
    {"@name", A_NAME}
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
                // TODO
                return Variable();

            case A_ELEMENT_COUNT:
                // TODO
                return Variable();

            case A_NAME:
                // TODO
                return Variable();
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
