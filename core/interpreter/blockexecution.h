#ifndef BLOCKEXECUTION_H
#define BLOCKEXECUTION_H

class Module;
class Interpreter;
class ContainerParser;
class Scope;

#include "program.h"

#include <memory>

class BlockExecution
{
public:
    BlockExecution(Program::const_iterator begin,
                   Program::const_iterator end,
                   const Module& module,
                   Interpreter& interpreter,
                   Scope& scope,
                   ContainerParser* parser = nullptr);

    void execute();
    void execute(Program::const_iterator breakpoint);
    void execute(size_t& parseQuota);
    void execute(Program::const_iterator breakpoint, size_t& parseQuota);

    bool done();

    const Module& module();
    Interpreter& interpreter();
    Scope& scope();

    bool hasParser();
    ContainerParser& parser();

private:
    void setSubBlock(Program::const_iterator subBegin, Program::const_iterator subEnd);
    void resetSubBlock();

    void handleDeclaration(const Program& declaration, size_t& parseQuota);
    void handleLocalDeclaration(const Program& declaration);
    void handleRightValue(const Program& rightValue);
    void handleCondition(const Program& condition);
    void handleLoop(const Program& loop);


    const Program::const_iterator begin;
    const Program::const_iterator end;
    Program::const_iterator current;

    const Module& _module;
    Interpreter& _interpreter;
    Scope& _scope;
    ContainerParser* _parser;

    std::unique_ptr<BlockExecution> subBlock;
};

#endif // BLOCKEXECUTION_H
