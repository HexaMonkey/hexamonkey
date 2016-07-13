#ifndef PARSERSCOPE_H
#define PARSERSCOPE_H

#include "core/variable/variable.h"

#include <memory>

class Parser;

class ParserScope : public VariableImplementation
{
public:
    ParserScope(VariableCollector& collector, std::shared_ptr<Parser*> sharedAccess);

protected:
    virtual Variable doGetField(const Variant &key, bool, bool);

private:
    inline Parser* parser() {
        return *_sharedAccess;
    }
    std::shared_ptr<Parser*> _sharedAccess;
};

#endif // PARSERSCOPE_H
