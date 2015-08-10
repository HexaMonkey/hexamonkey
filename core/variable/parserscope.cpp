#include "parserscope.h"

#include "core/variable/variablecollector.h"
#include "core/containerparser.h"
#include "core/util/unused.h"
#include "core/object.h"

#define A_PEEK 1

const std::unordered_map<std::string, int> reserved = {
    {"peek",             A_PEEK}
};


ParserScope::ParserScope(VariableCollector &collector, std::shared_ptr<ContainerParser *> sharedAccess)
    : VariableImplementation(collector),
      _sharedAccess(sharedAccess)
{
}

Variable ParserScope::doGetField(const Variant &key, bool /*modifiable*/, bool /*createIfNeeded*/)
{
    if (key.type() == Variant::stringType) {
        auto it = reserved.find(key.toString());
        if (it != reserved.end()) {
            const auto id = it->second;
            switch (id) {
                case A_PEEK:
                return collector().lambda(
                    [this](const VariableArgs& args, const VariableKeywordArgs&) ->Variable {
                    if (parser()) {
                        Variant typeArg = args[0].value();
                        if (typeArg.type() == Variant::objectType) {
                            const ObjectType& type = typeArg.toObjectType();
                            std::unique_ptr<Object> object(parser()->readVariable(type));
                            return collector().copy(object->value());
                        }
                    }
                    return Variable();
                }, memory());
            }
        }
    }
    return Variable();
}
