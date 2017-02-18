#include "parserscope.h"

#include "core/variable/variablecollector.h"
#include "core/parser.h"
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


ParserScope::ParserScope(VariableCollector &collector, std::shared_ptr<Parser *> sharedAccess)
    : VariableImplementation(collector),
      _sharedAccess(sharedAccess)
{
}

class PeekVariableImplementation : public VariableImplementation
{
public:
    PeekVariableImplementation(VariableCollector & collector, const std::shared_ptr<Parser*>& sharedAccess)
        : VariableImplementation(collector),
          _sharedAccess(sharedAccess)
    {
    }

protected:
    virtual Variable doCall(VariableArgs &args, VariableKeywordArgs &) override
    {
        Parser* parser = *_sharedAccess;

        if (parser && args.size() >= 1) {
            Variant typeArg = args[0].value();

            if (typeArg.type() == Variant::objectType) {
                std::streamoff offset = args.size() > 1 ? args[1].value().toInteger() : 0;
                const ObjectType& type = typeArg.toObjectType();
                std::unique_ptr<Object> child(parser->object().readVariable(type, offset));
                if (child) {
                    return collector().copy(child->value());
                }
            }
        }
        return Variable();
    }
private:
    std::shared_ptr<Parser*> _sharedAccess;
};

class ReadVariableImplementation : public VariableImplementation
{
public:
    ReadVariableImplementation(VariableCollector & collector, const std::shared_ptr<Parser*>& sharedAccess)
        : VariableImplementation(collector),
          _sharedAccess(sharedAccess)
    {
    }

protected:
    virtual Variable doCall(VariableArgs &args, VariableKeywordArgs &) override
    {
        Parser* parser = *_sharedAccess;

        if (parser) {
            Variant typeArg = args[0].value();
            if (typeArg.type() == Variant::objectType) {
                const ObjectType& type = typeArg.toObjectType();
                std::unique_ptr<Object> child(parser->object().readVariable(type));
                if (child) {
                    parser->object().setPos(parser->object().pos()+child->size());
                    return collector().copy(child->value());
                }
            }
        }
        return Variable();
    }
private:
    std::shared_ptr<Parser*> _sharedAccess;
};

class FindBytePatternVariableImplementation : public VariableImplementation
{
public:
    FindBytePatternVariableImplementation(VariableCollector & collector, const std::shared_ptr<Parser*>& sharedAccess)
        : VariableImplementation(collector),
          _sharedAccess(sharedAccess)
    {
    }

protected:
    virtual Variable doCall(VariableArgs &args, VariableKeywordArgs &) override
    {
        if (args.size() < 1) {
            return Variable();
        }
        const Variant& value = args[0].value();
        if (value.type() != Variant::stringType) {
            return Variable();
        }

        Parser* parser = *_sharedAccess;
        if (parser) {
            return collector().copy(parser->findBytePattern(value.toString()));
        }

        return Variable();
    }
private:
    std::shared_ptr<Parser*> _sharedAccess;
};

Variable ParserScope::doGetField(const Variant &key, bool /*modifiable*/, bool /*createIfNeeded*/)
{
    if (key.type() == Variant::stringType) {
        auto it = reserved.find(key.toString());
        if (it != reserved.end()) {
            const auto id = it->second;
            switch (id) {
                case A_PEEK:
                return Variable(new PeekVariableImplementation(collector(), _sharedAccess), false);

                case A_READ:
                return Variable(new ReadVariableImplementation(collector(), _sharedAccess), false);

                case A_FIND_BYTE_PATTERN:
                return Variable(new FindBytePatternVariableImplementation(collector(), _sharedAccess), false);
            }
        }
    }
    return Variable();
}

bool ParserScope::isByRefOnly()
{
    return true;
}
