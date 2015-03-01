
#include <limits>

#include "compiler/model.h"
#include "core/containerparser.h"
#include "core/interpreter/blockexecution.h"
#include "core/interpreter/evaluator.h"
#include "core/interpreter/programloader.h"
#include "core/interpreter/scope/scope.h"
#include "core/util/unused.h"

BlockExecution::BlockExecution(Program block,
                               const Evaluator &evaluator,
                               Scope& scope,
                               ContainerParser *parser)
    : program(block),
      begin(block.begin()),
      end(block.end()),
      current(block.begin()),
      last(block.end()),
      lineRepeatCount(0),
      eval(evaluator),
      scope(scope),
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
        if(current!=last) {
            lineRepeatCount = 0;
        }
        last = current;

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
                    switch(line.tag())
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
            switch(line.tag())
            {
                case DECLARATION:
                    handleDeclaration(line, parseQuota);
                    break;

                case LOCAL_DECLARATIONS:
                    handleLocalDeclarations(line);
                    break;

                case SUBSCOPE_ASSIGN:
                    handleSubscopeAssign(line);
                    break;

                case REMOVE:
                    handleRemove(line);
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
    subBlock.reset(new BlockExecution(program, eval, scope, _parser));
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
        ObjectType type = eval.rightValue(declaration.node(0)).cvalue().toObjectType();
        std::string name = declaration.node(1).payload().toString();
#ifdef EXECUTION_TRACE
        std::cerr<<"Declaration "<<type<<" "<<name<<std::endl;
#endif
        if (parser().addVariable(type, name) != nullptr) {
            --parseQuota;
        }
    }
    ++current;
}

void BlockExecution::handleLocalDeclarations(const Program &declarations)
{
    for (const Program& declaration : declarations) {
        Variable variable = scope.declare(declaration.node(0).payload());
#ifdef EXECUTION_TRACE
        std::cerr<<"Local declaration "<<declaration.node(0).payload();
#endif

        if (declaration.size() >= 2 && variable.isDefined())
        {
            variable.value() = eval.rightValue(declaration.node(1)).cvalue();
#ifdef EXECUTION_TRACE
            std::cerr<<" = "<<variable.cvalue();
#endif
        }
#ifdef EXECUTION_TRACE
        std::cerr<<std::endl;
#endif
    }
    ++current;
}

void BlockExecution::handleSubscopeAssign(const Program &assign)
{
    const int size = assign.size();

    Scope::Ptr subscope = handleScope(assign.node(size - 1));
    for (int i = size - 2; i >= 0; --i) {
        VariablePath variablePath = eval.variablePath(assign.node(i));
        scope.assignSubscope(variablePath, subscope);
    }
    ++current;
}

void BlockExecution::handleRemove(const Program &remove)
{
    scope.remove(eval.variablePath(remove.node(0)));

    ++current;
}

void BlockExecution::handleRightValue(const Program &rightValue)
{
#ifdef EXECUTION_TRACE
    Variant value = eval.rightValue(rightValue).value();
#else
    eval.rightValue(rightValue);
#endif
#ifdef EXECUTION_TRACE
    std::cerr<<"Right value "<<value<<std::endl;
#endif
    ++current;
}

void BlockExecution::handleCondition(const Program &condition)
{
#ifdef EXECUTION_TRACE
    std::cerr<<"Condition"<<std::endl;
#endif
    if(eval.rightValue(condition.node(0)).cvalue().toBool())
    {
#ifdef EXECUTION_TRACE
        std::cerr<<" then";
#endif
        setSubBlock(condition.node(1), false);
    }
    else
    {
#ifdef EXECUTION_TRACE
        std::cerr<<" else";
#endif
        setSubBlock(condition.node(2), false);
    }
#ifdef EXECUTION_TRACE
    std::cerr<<std::endl;
#endif
}

void BlockExecution::handleLoop(const Program &loop)
{
#ifdef EXECUTION_TRACE
    std::cerr<<"Loop"<<std::endl;
#endif
    if(loopCondition(loop))
    {
#ifdef EXECUTION_TRACE
        std::cerr<<" continue";
#endif
        setSubBlock(loop.node(1), true);
    }
    else
    {
#ifdef EXECUTION_TRACE
        std::cerr<<" done";
#endif
        ++current;
    }
#ifdef EXECUTION_TRACE
    std::cerr<<std::endl;
#endif

}

void BlockExecution::handleDoLoop(const Program &loop)
{
#ifdef EXECUTION_TRACE
    std::cerr<<"Do Loop : repeat count "<<lineRepeatCount;
#endif
    if(lineRepeatCount <= 1 || loopCondition(loop))
    {
#ifdef EXECUTION_TRACE
        std::cerr<<" continue";
#endif
        setSubBlock(loop.node(1), true);
    }
    else
    {
#ifdef EXECUTION_TRACE
        std::cerr<<" done";
#endif
        ++current;
    }
#ifdef EXECUTION_TRACE
    std::cerr<<std::endl;
#endif
}

bool BlockExecution::handleBreak()
{
    if(_inLoop)
    {
#ifdef EXECUTION_TRACE
        std::cerr<<"Break"<<std::endl;
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
        std::cerr<<"Continue"<<std::endl;
        current = end;
        return true;
    }
    ++current;
    return false;
}

void BlockExecution::handleReturn(const Program &line)
{
    const Program& rightValue = line.node(0);
    _returnValue = eval.rightValue(rightValue);
#ifdef EXECUTION_TRACE
    std::cerr<<"Return "<<_returnValue.cvalue()<<std::endl;
#endif
    current = end;
}

bool BlockExecution::loopCondition(const Program &loop)
{
    return eval.rightValue(loop.node(0)).cvalue().toBool()
            && ((!hasDeclaration(loop.node(1)) || (hasParser() && parser().availableSize()!= 0)));
}

bool BlockExecution::hasDeclaration(const Program &instructions)
{
    for(Program program: instructions)
    {
        switch(program.tag())
        {
            case DECLARATION:
                return true;

            case CONDITIONAL_STATEMENT:
                if(hasDeclaration(program.node(1)))
                    return true;
                if(hasDeclaration(program.node(2)))
                    return true;
                break;

            case LOOP:
            case DO_LOOP:
                if(hasDeclaration(program.node(1)))
                    return true;
                break;

            default:
                break;
        }
    }
    return false;
}

Scope::Ptr BlockExecution::handleScope(const Program & s)
{
    Program node = s.node(0);

    switch (node.tag()) {
        case VARIABLE:
            return scope.getScope(eval.variablePath(node));
    }

    return Scope::Ptr();
}
