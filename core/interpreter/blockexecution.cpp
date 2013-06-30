#include "blockexecution.h"
#include "interpreter.h"
#include "model.h"
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
      _inLoop(false),
      _subBlockExitCode(ExitCode::NoExit)
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
                if(_subBlockExitCode == ExitCode::Returned)
                {
                    _returnValue = subBlock->returnValue();
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
                            if(_subBlockExitCode == ExitCode::Broken)
                            {
                                ++current;
                            }
                            break;

                        default:
                            if(_subBlockExitCode == ExitCode::Broken && handleBreak())
                            {
                                return ExitCode::Broken;
                            }

                            if(_subBlockExitCode == ExitCode::Continued && handleContinue())
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
                _subBlockExitCode = subBlock->execute(parseQuota);
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

Variable BlockExecution::returnValue()
{
    return _returnValue;
}

void BlockExecution::setSubBlock(Program program, bool loop)
{
    subBlock.reset(new BlockExecution(program, module(), scope(), _parser));
    if(loop || _inLoop)
        subBlock->_inLoop = true;
    _subBlockExitCode = ExitCode::NoExit;
}

void BlockExecution::resetSubBlock()
{
    subBlock.reset();
}

void BlockExecution::handleDeclaration(const Program &declaration, size_t &parseQuota)
{
    if(hasParser())
    {
        ObjectType type = declaration.elem(0).evaluateValue(scope(), module()).toObjectType();
        std::string name = declaration.elem(1).payload().toString();
        bool showcased = declaration.elem(2).payload().toInteger();
#ifdef EXECUTION_TRACE
        std::cout<<"Declaration "<<type<<" "<<name<<std::endl;
#endif
        if(parser().addVariable(type, name) != nullptr){
            --parseQuota;
        }
        if(showcased)
            parser().showcase().add(name);
    }
    ++current;
}

void BlockExecution::handleLocalDeclaration(const Program &declaration)
{
    Variable variable = scope().declare(declaration.elem(0).payload());
#ifdef EXECUTION_TRACE
    std::cout<<"Local declaration "<<declaration.elem(0).payload();
#endif

    if(declaration.size() >= 2 && variable.isDefined())
    {
        variable.value() = declaration.elem(1).evaluateValue(scope(), module());
        std::cout<<" = "<<variable.cvalue();
    }
    std::cout<<std::endl;
    ++current;
}

void BlockExecution::handleRightValue(const Program &rightValue)
{
#ifdef EXECUTION_TRACE
    Variant value =
#endif
    rightValue.evaluateValue(scope(), module());
#ifdef EXECUTION_TRACE
    std::cout<<"Right value "<<value<<std::endl;
#endif
    ++current;
}

void BlockExecution::handleCondition(const Program &condition)
{
#ifdef EXECUTION_TRACE
    std::cout<<"Condition"<<std::endl;
#endif
    if(condition.elem(0).evaluateValue(scope(), module()).toBool())
    {
#ifdef EXECUTION_TRACE
        std::cout<<" then";
#endif
        setSubBlock(condition.elem(1), false);
    }
    else
    {
#ifdef EXECUTION_TRACE
        std::cout<<" else";
#endif
        setSubBlock(condition.elem(2), false);
    }
#ifdef EXECUTION_TRACE
    std::cout<<std::endl;
#endif
}

void BlockExecution::handleLoop(const Program &loop)
{
#ifdef EXECUTION_TRACE
    std::cout<<"Loop"<<std::endl;
#endif
    if(loopCondition(loop))
    {
#ifdef EXECUTION_TRACE
        std::cout<<" continue";
#endif
        setSubBlock(loop.elem(1), true);
    }
    else
    {
#ifdef EXECUTION_TRACE
        std::cout<<" done";
#endif
        ++current;
    }
    std::cout<<std::endl;
}

void BlockExecution::handleDoLoop(const Program &loop)
{
#ifdef EXECUTION_TRACE
    std::cout<<"Do Loop";
#endif
    if(lineRepeatCount <= 1 || loopCondition(loop))
    {
#ifdef EXECUTION_TRACE
        std::cout<<" continue";
#endif
        setSubBlock(loop.elem(1), true);
    }
    else
    {
#ifdef EXECUTION_TRACE
        std::cout<<" done";
#endif
        ++current;
    }
#ifdef EXECUTION_TRACE
    std::cout<<std::endl;
#endif
}

bool BlockExecution::handleBreak()
{
    if(_inLoop)
    {
#ifdef EXECUTION_TRACE
        std::cout<<"Break"<<std::endl;
#endif
        current = end;
        return true;
    }
    ++current;
    return false;
}

bool BlockExecution::handleContinue()
{
    if(_inLoop)
    {
        std::cout<<"Continue"<<std::endl;
        current = end;
        return true;
    }
    ++current;
    return false;
}

void BlockExecution::handleReturn(const Program &line)
{
    const Program& rightValue = line.elem(0);
    _returnValue = rightValue.evaluate(scope(), module());
#ifdef EXECUTION_TRACE
    std::cout<<"Return "<<_returnValue.cvalue()<<std::endl;
#endif
    current = end;
}

bool BlockExecution::loopCondition(const Program &loop)
{
    return loop.elem(0).evaluateValue(scope()).toBool()
           && ((!loop.elem(1).hasDeclaration() || parser().availableSize()> 0));
}
