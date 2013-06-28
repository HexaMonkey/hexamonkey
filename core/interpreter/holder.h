#ifndef HOLDER_H
#define HOLDER_H

#include <map>
#include "module.h"
#include "program.h"
class Variable;
class Variant;

class Holder
{
public:
    Holder(Program program);
    ~Holder();

    Variant& getNew();
    Variant& copy(const Variant& value);
    Variant& evaluate(const Scope& scope, const Module& module = Module());
    const Variant& cevaluate(const Scope& scope, const Module& module = Module());


private:
    Holder(const Holder&) = delete;
    Holder& operator=(Holder other) = delete;

    friend class Program;
    friend class BlockExecution;
    friend class FromFileModule;
    Variable& add(Variable& variable);
    Variable& extract(Variable& variable);


    Program _program;
    std::multimap<const Variant*, Variable*> _held;

};

#endif // HOLDER_H
