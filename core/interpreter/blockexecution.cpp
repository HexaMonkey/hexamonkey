#include "blockexecution.h"
#include "interpreter.h"
#include "model.h"
#include "holder.h"
#include "scope.h"
#include "containerparser.h"
#include "unused.h"

#include <limits>

BlockExecution::BlockExecution(Program::const_iterator begin,
                               Program::const_iterator end,
                               const Module &module,
                               Interpreter &interpreter,
                               Scope& scope,
                               ContainerParser *parser)
    : begin(begin),
      end(end),
      current(begin),
      last(end),
      lineRepeatCount(0),
      _module(module),
      _interpreter(interpreter),
      _scope(scope),
      _parser(parser),
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

Interpreter &BlockExecution::interpreter()
{
    return _interpreter;
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

void BlockExecution::setSubBlock(Program::const_iterator subBegin, Program::const_iterator subEnd, bool loop)
{
    subBlock.reset(new BlockExecution(subBegin, subEnd, module(), interpreter(), scope(), _parser));
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
        Holder holder(interpreter());
        ObjectType type = holder.cevaluate(declaration.elem(0), scope(), module()).toObjectType();
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
        Holder holder(interpreter());
        *variant = holder.evaluate(declaration.elem(1), scope(), module());
    }
    ++current;
}

void BlockExecution::handleRightValue(const Program &rightValue)
{
    Holder holder(interpreter());
    holder.evaluate(rightValue, scope(), module());
    ++current;
}

void BlockExecution::handleCondition(const Program &condition)
{
    Holder holder(interpreter());
    if(holder.cevaluate(condition.elem(0), scope(), module()).toBool())
    {
        setSubBlock(condition.elem(1).begin(), condition.elem(1).end(), false);
    }
    else
    {
        setSubBlock(condition.elem(2).begin(), condition.elem(2).end(), false);
    }
}

void BlockExecution::handleLoop(const Program &loop)
{
    if(loopCondition(loop))
    {
        setSubBlock(loop.elem(1).begin(), loop.elem(1).end(), true);
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
        setSubBlock(loop.elem(1).begin(), loop.elem(1).end(), true);
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

bool BlockExecution::loopCondition(const Program &loop)
{
    Holder holder(interpreter());

    return holder.cevaluate(loop.elem(0), scope()).toBool()
           && ((!interpreter().hasDeclaration(loop.elem(1)) || parser().availableSize()> 0));
}
