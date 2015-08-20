#include "parserscope.h"

#include "core/variable/variablecollector.h"
#include "core/containerparser.h"
#include "core/util/unused.h"
#include "core/object.h"

#define A_PEEK 1
#define A_READ 2
#define A_FIND_BYTE_PATTERN 3

const std::unordered_map<std::string, int> reserved = {
    {"peek",             A_PEEK},
    {"read",             A_READ},
    {"findBytePattern",  A_FIND_BYTE_PATTERN}
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
                    if (parser() && args.size() >= 1) {
                        Variant typeArg = args[0].value();

                        if (typeArg.type() == Variant::objectType) {
                            std::streamoff offset = args.size() > 1 ? args[1].value().toInteger() : 0;
                            const ObjectType& type = typeArg.toObjectType();
                            std::unique_ptr<Object> child(parser()->readVariable(type, offset));
                            if (child) {
                                return collector().copy(child->value());
                            }
                        }
                    }
                    return Variable();
                }, memory());

                case A_READ:
                return collector().lambda(
                    [this](const VariableArgs& args, const VariableKeywordArgs&) ->Variable {
                    if (parser()) {
                        Variant typeArg = args[0].value();
                        if (typeArg.type() == Variant::objectType) {
                            const ObjectType& type = typeArg.toObjectType();
                            std::unique_ptr<Object> child(parser()->readVariable(type));
                            if (child) {
                                parser()->object().setPos(parser()->object().pos()+child->size());
                                return collector().copy(child->value());
                            }
                        }
                    }
                    return Variable();
                }, memory());

                case A_FIND_BYTE_PATTERN:
                return collector().lambda(
                    [this](const VariableArgs& args, const VariableKeywordArgs&) ->Variable {
                        if (args.size() < 1) {
                            return Variable();
                        }
                        const Variant& value = args[0].value();
                        if (value.type() != Variant::stringType) {
                            return Variable();
                        }
                        if (parser()) {
                            return collector().copy(parser()->findBytePattern(value.toString()));
                        }

                        return Variable();
                    }
                );
            }
        }
    }
    return Variable();
}
