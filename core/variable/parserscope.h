#ifndef PARSERSCOPE_H
#define PARSERSCOPE_H

#include "core/variable/variable.h"

#include <memory>

class ContainerParser;

class ParserScope : public VariableImplementation
{
public:
    ParserScope(VariableCollector& collector, std::shared_ptr<ContainerParser*> sharedAccess);

protected:
    virtual Variable doGetField(const Variant &key, bool, bool);

private:
    inline ContainerParser* parser() {
        return *_sharedAccess;
    }
    std::shared_ptr<ContainerParser*> _sharedAccess;
};

#endif // PARSERSCOPE_H
