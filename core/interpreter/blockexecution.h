#ifndef BLOCKEXECUTION_H
#define BLOCKEXECUTION_H

class Module;
class ContainerParser;
class Scope;
class Evaluator;

#include "program.h"


#include <memory>

/**
 * @brief Controls the execution of a program block
 *
 * The program block can either be part of a class definition or
 * a function defintion.
 */
class BlockExecution
{
public:
    /**
     * @param block An execution block tagged program
     * @param evaluator Used to evaluate right values.
     * @param scope Used to declare local variables.
     * @param parser Used to handle member declarations in
     * a class definition.
     */
    BlockExecution(Program block,
                   const Evaluator& evaluator,
                   Scope& scope,
                   ContainerParser* parser = nullptr);

    /**
     * @brief Codes signifying the reason that an execution terminated
     */
    enum class ExitCode
    {
        /// Invalid exit code
        NoExit,
        /// Has exited when the end of the program block is reached
        EndReached,
        /// Has exited when the breakpoint defined has been reached
        BreakPointReached,
        /// Has exited when the quota of members declarations have been
        /// exhausted.
        QuotaExhausted,
        /// Has exited when a break statement has been reached
        Broken,
        /// Has exited when a continue statement has been reached
        Continued,
        /// Has exited when a return statement has been reached
        Returned
    };

    /**
     * @brief Execute the block until it is done
     */
    ExitCode execute();

    /**
     * @brief Execute the block until the breakpoint is reached or it is done.
     */
    ExitCode execute(Program::const_iterator breakpoint);

    /**
     * @brief Execute the block until it is done as long as the number of members
     * declared doesn't exceed the parseQuota
     */
    ExitCode execute(size_t& parseQuota);

    /**
     * @brief Execute the block until the breakpoint is reached or it is done
     * as long as the number of members declared doesn't exceed the parseQuota
     */
    ExitCode execute(Program::const_iterator breakpoint, size_t& parseQuota);

    /**
     * @brief Check if the execution of the block is done
     */
    bool done();

    /**
     * @brief Get the value returned by the block in case the execution has exited
     * with a return statement.
     */
    Variable returnValue();

private:
    bool hasParser();
    ContainerParser& parser();

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
    static bool hasDeclaration(const Program& instructions);

    Program program;
    const Program::const_iterator begin;
    const Program::const_iterator end;
    Program::const_iterator current;
    Program::const_iterator last;
    unsigned int lineRepeatCount;

    const Evaluator& eval;
    Scope& scope;
    ContainerParser* _parser;

    Variable _returnValue;

    bool _inLoop;

    std::unique_ptr<BlockExecution> subBlock;
    ExitCode _subBlockExitCode;
};

#endif // BLOCKEXECUTION_H
