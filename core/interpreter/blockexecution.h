#ifndef BLOCKEXECUTION_H
#define BLOCKEXECUTION_H

class Module;
class ContainerParser;
class Scope;

#include "program.h"
#include "holder.h"

#include <memory>

class BlockExecution
{
public:
    BlockExecution(Program program,
                   const Module& module,
                   Scope& scope,
                   ContainerParser* parser = nullptr);

    enum class ExitCode {NoExit, EndReached, BreakPointReached, QuotaExhausted, Broken, Continued, Returned};

    ExitCode execute();
    ExitCode execute(Program::const_iterator breakpoint);
    ExitCode execute(size_t& parseQuota);
    ExitCode execute(Program::const_iterator breakpoint, size_t& parseQuota);

    bool done();

    const Module& module();
    Scope& scope();

    bool hasParser();
    ContainerParser& parser();

    Variable &extractReturnValue();

private:
    void setSubBlock(Program program, bool loop);
    void resetSubBlock();

    void handleDeclaration(const Program& declaration, size_t& parseQuota);
    void handleLocalDeclaration(const Program& declaration);
    void handleRightValue(const Program& rightValue);
    void handleCondition(const Program& condition);
    void handleLoop(const Program& loop);
    void handleDoLoop(const Program& loop);
    bool handleBreak();
    bool handleContinue();
    void handleReturn(const Program& line);

    bool loopCondition(const Program& loop);

    Program program;
    const Program::const_iterator begin;
    const Program::const_iterator end;
    Program::const_iterator current;
    Program::const_iterator last;
    unsigned int lineRepeatCount;

    const Module& _module;
    Scope& _scope;
    ContainerParser* _parser;

    Holder returnHolder;
    Variable* returnValue;


    bool inLoop;

    std::unique_ptr<BlockExecution> subBlock;
    ExitCode subBlockExitCode;
};

#endif // BLOCKEXECUTION_H
