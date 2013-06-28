#include "blockexecution.h"
#include "interpreter.h"
#include "model.h"
#include "holder.h"
#include "scope.h"
#include "containerparser.h"
#include "unused.h"

#include <limits>

BlockExecution::BlockExecution(Program program,
                               const Module &module,
                               Scope& scope,
                               ContainerParser *parser)
    : program(program),
      begin(program.begin()),
      end(program.end()),
      current(program.begin()),
      last(program.end()),
      lineRepeatCount(0),
      _module(module),
      _scope(scope),
      _parser(parser),
      returnHolder(program),
      returnValue(nullptr),
      inLoop(false),
      subBlockExitCode(ExitCode::NoExit)
{
    UNUSED(hmcElemNames);
}

BlockExecution::ExitCode BlockExecution::execute()
{
    size_t parseQuota = std::numeric_limits<size_t>::max();
    return execute(end, parseQuota);
}

BlockExecution::ExitCode BlockExecution::execute(Program::const_iterator breakpoint)
{
    size_t parseQuota = std::numeric_limits<size_t>::max();
    return execute(breakpoint, parseQuota);
}

BlockExecution::ExitCode BlockExecution::execute(size_t &parseQuota)
{
    return execute(end, parseQuota);
}

BlockExecution::ExitCode BlockExecution::execute(Program::const_iterator breakpoint, size_t &parseQuota)
{

    while(current != end && current != breakpoint && parseQuota > 0)
    {
        if(current!=last)
            lineRepeatCount = 0;

        const Program& line = *current;
        if(subBlock)
        {
            if(subBlock->done())
            {


                if(subBlockExitCode == ExitCode::Returned)
                {
                    returnValue = &returnHolder.add(subBlock->extractReturnValue());
                    resetSubBlock();
                    return ExitCode::Returned;
                }
                else
                {
                    resetSubBlock();
                    switch(line.id())
                    {

                        case LOOP:
                        case DO_LOOP:
                            if(subBlockExitCode == ExitCode::Broken)
                            {
                                ++current;
                            }
                            break;

                        default:
                            if(subBlockExitCode == ExitCode::Broken && handleBreak())
                            {
                                return ExitCode::Broken;
                            }

                            if(subBlockExitCode == ExitCode::Continued && handleContinue())
                            {
                                return ExitCode::Continued;
                            }

                            ++current;
                            break;
                    }
                }
            }
            else
            {
                subBlockExitCode = subBlock->execute(parseQuota);
            }
        }
        else
        {
            ++lineRepeatCount;
            switch(line.id())
            {
                case DECLARATION:
                    handleDeclaration(line, parseQuota);
                    break;

                case LOCAL_DECLARATION:
                    handleLocalDeclaration(line);
                    break;

                case RIGHT_VALUE:
                    handleRightValue(line);
                    break;

                case CONDITIONAL_STATEMENT:
                    handleCondition(line);
                    break;

                case LOOP:
                    handleLoop(line);
                    break;

                case DO_LOOP:
                    handleDoLoop(line);
                    break;

                case BREAK:
                if(handleBreak())
                    return ExitCode::Broken;

                case CONTINUE:
                if(handleContinue())
                    return ExitCode::Continued;

                case RETURN:
                    handleReturn(line);
                    return ExitCode::Returned;

                default:
                    ++current;
                    break;
            }
        }
        last = current;
    }
    if(current == end)
        return ExitCode::EndReached;

    if(current == breakpoint)
        return ExitCode::BreakPointReached;

    return ExitCode::QuotaExhausted;
}

bool BlockExecution::done()
{
    return current == end;
}

const Module &BlockExecution::module()
{
    return _module;
}

Scope &BlockExecution::scope()
{
    return _scope;
}

bool BlockExecution::hasParser()
{
    return (_parser != nullptr);
}

ContainerParser &BlockExecution::parser()
{
    return *_parser;
}

Variable& BlockExecution::extractReturnValue()
{
    if(returnValue == nullptr)
        return program.null();
    else
        return returnHolder.extract(*returnValue);
}

void BlockExecution::setSubBlock(Program program, bool loop)
{
    subBlock.reset(new BlockExecution(program, module(), scope(), _parser));
    if(loop || inLoop)
        subBlock->inLoop = true;
    subBlockExitCode = ExitCode::NoExit;
}

void BlockExecution::resetSubBlock()
{
    subBlock.reset();
}

void BlockExecution::handleDeclaration(const Program &declaration, size_t &parseQuota)
{
    if(hasParser())
    {
        Holder holder(declaration.elem(0));
        ObjectType type = holder.cevaluate(scope(), module()).toObjectType();
        std::string name = declaration.elem(1).payload().toString();
        bool showcased = declaration.elem(2).payload().toInteger();
        if(parser().addVariable(type, name) != nullptr)
            --parseQuota;
        if(showcased)
            parser().showcase().add(name);
    }
    ++current;
}

void BlockExecution::handleLocalDeclaration(const Program &declaration)
{
    Variant* variant = scope().declare(declaration.elem(0).payload());
    if(declaration.size() >= 2 && variant != nullptr)
    {
        Holder holder(declaration.elem(1));
        *variant = holder.evaluate(scope(), module());
    }
    ++current;
}

void BlockExecution::handleRightValue(const Program &rightValue)
{
    Holder holder(rightValue);
    holder.evaluate(scope(), module());
    ++current;
}

void BlockExecution::handleCondition(const Program &condition)
{
    Holder holder(condition.elem(0));
    if(holder.cevaluate(scope(), module()).toBool())
    {
        setSubBlock(condition.elem(1), false);
    }
    else
    {
        setSubBlock(condition.elem(2), false);
    }
}

void BlockExecution::handleLoop(const Program &loop)
{
    if(loopCondition(loop))
    {
        setSubBlock(loop.elem(1), true);
    }
    else
    {
        ++current;
    }
}

void BlockExecution::handleDoLoop(const Program &loop)
{
    if(lineRepeatCount <= 1 || loopCondition(loop))
    {
        setSubBlock(loop.elem(1), true);
    }
    else
    {
        ++current;
    }
}

bool BlockExecution::handleBreak()
{
    if(inLoop)
    {
        current = end;
        return true;
    }
    ++current;
    return false;
}

bool BlockExecution::handleContinue()
{
    if(inLoop)
    {
        current = end;
        return true;
    }
    ++current;
    return false;
}

void BlockExecution::handleReturn(const Program &line)
{
    const Program& rightValue = line.elem(0);
    returnValue = &returnHolder.add(rightValue.evaluate(scope(), module()));
    current = end;
}

bool BlockExecution::loopCondition(const Program &loop)
{
    Holder holder(loop.elem(0));

    return holder.cevaluate(scope()).toBool()
           && ((!loop.elem(1).hasDeclaration() || parser().availableSize()> 0));
}
