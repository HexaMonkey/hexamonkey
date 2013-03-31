#ifndef HOLDER_H
#define HOLDER_H

#include <map>
#include "module.h"
class Interpreter;
class Variable;
class Variant;
class Program;

class Holder
{
public:
    Holder(Interpreter& interpreter);
    ~Holder();

    Variant& getNew();
    Variant& evaluate(const Program& rightValue, const Scope& scope, const Module& module = Module());
    const Variant& cevaluate(const Program& rightValue, const Scope& scope, const Module& module = Module());


private:
    friend class Interpreter;
    friend class BlockExecution;
    friend class FromFileModule;
    Variable& add(Variable& var);
    Variable& extract(Variable& var);

    Interpreter& _interpreter;
    std::multimap<const Variant*, Variable*> _held;

};

#endif // HOLDER_H
