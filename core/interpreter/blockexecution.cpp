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
      _module(module),
      _interpreter(interpreter),
      _scope(scope),
      _parser(parser)
{
    UNUSED(hmcElemNames);
}

void BlockExecution::execute()
{
    size_t parseQuota = 1000;//std::numeric_limits<size_t>::max();
    execute(end, parseQuota);
}

void BlockExecution::execute(Program::const_iterator breakpoint)
{
    size_t parseQuota = 1000;//std::numeric_limits<size_t>::max();
    execute(breakpoint, parseQuota);
}

void BlockExecution::execute(size_t &parseQuota)
{
    execute(end, parseQuota);
}

void BlockExecution::execute(Program::const_iterator breakpoint, size_t &parseQuota)
{

    while(current != breakpoint && parseQuota > 0)
    {
        const size_t line_number = std::distance(begin, current);
        const Program& line = *current;
        if(subBlock)
        {
            if(subBlock->done())
            {
                resetSubBlock();

                switch(line.id())
                {

                    case LOOP:
                        break;

                    default:
                        ++current;
                        break;
                }
            }
            else
            {
                subBlock->execute(parseQuota);
            }
        }
        else
        {
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

                default:
                    ++current;
                    break;
            }
        }
    }
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

void BlockExecution::setSubBlock(Program::const_iterator subBegin, Program::const_iterator subEnd)
{
    subBlock.reset(new BlockExecution(subBegin, subEnd, module(), interpreter(), scope(), _parser));
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
        if(parser().addVariable(type, name) != nullptr)
            --parseQuota;
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

        setSubBlock(condition.elem(1).begin(), condition.elem(1).end());
    }
    else
    {
        setSubBlock(condition.elem(2).begin(), condition.elem(2).end());
    }
}

void BlockExecution::handleLoop(const Program &loop)
{
    Holder holder(interpreter());

    if(((!interpreter().hasDeclaration(loop.elem(1)) || parser().availableSize()> 0))
            &&  holder.cevaluate(loop.elem(0), scope()).toBool())
    {
        setSubBlock(loop.elem(1).begin(), loop.elem(1).end());
    }
    else
    {
        ++current;
    }
}
