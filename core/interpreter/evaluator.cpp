#include "evaluator.h"

#include "compiler/model.h"
#include "core/log/logmanager.h"
#include "core/interpreter/evaluator.h"
#include "core/interpreter/program.h"
#include "core/variable/functionscope.h"
#include "core/util/unused.h"
#include "core/variable/arrayscope.h"
#include "core/variable/mapscope.h"

const Variant emptyString("");
Variable emptyScope;
const Module emptyModule;

Evaluator::Evaluator()
    : scope(emptyScope),
      module(emptyModule)
{
    UNUSED(hmcElemNames);
}

Evaluator::Evaluator(const Variable &scope)
    : scope(scope),
      module(emptyModule)
{
}

Evaluator::Evaluator(const Module &module)
    : scope(emptyScope),
      module(module)
{
}

Evaluator::Evaluator(const Variable& scope, const Module &module)
    : scope(scope),
      module(module)
{
}

Variable Evaluator::rightValue(const Program &program, int modifiable, int createIfNeeded) const
{
    Program first = program.node(0);
    switch(first.tag())
    {
        case OPERATOR:
        {
            const int op = first.payload().toInteger();
            const int release = operatorParameterRelease[op];
            switch(operatorParameterCount[op])
            {
                case 1:
                {
                    const bool create1 = !(1&release);
                    const Variable value1 = rightValue(program[1], create1, create1);
                    return unaryOperation(op, value1);
                }

                case 2:
                {
                    const bool create1 = !(1&release);
                    const bool create2 = !(2&release);
                    const Variable value1 = rightValue(program[1], create1, create1);
                    const Variable value2 = rightValue(program[2], create2, create2);
                    return binaryOperation (op, value1, value2);
                }

                case 3:
                {
                    const bool create1 = !(1&release);
                    const bool create2 = !(2&release);
                    const bool create3 = !(4&release);
                    const Variable value1 = rightValue(program[1], create1, create1);
                    const Variable value2 = rightValue(program[2], create2, create2);
                    const Variable value3 = rightValue(program[3], create3, create3);
                    return ternaryOperation(op, value1, value2, value3);
                }

                default:
                    return Variable();
            }

        }

        case FIELD_ASSIGN:
            return assignField(first[0], first[1]);

        case FUNCTION_EVALUATION:
            return function(first);

        case INT_CONSTANT:
        case UINT_CONSTANT:
        case STRING_CONSTANT:
        case FLOAT_CONSTANT:
            return Variable::copy(first.payload());

        case NULL_CONSTANT:
            return Variable::null();

        case UNDEFINED_CONSTANT:
            return Variable();

        case EMPTY_STRING_CONSTANT:
            return Variable::copy(emptyString);

        case VARIABLE:
            return variable(first, modifiable, createIfNeeded);

        case TYPE:
            return Variable::copy(type(first));

        case ARRAY_SCOPE:
            return arrayScope(first);

        case MAP_SCOPE:
            return mapScope(first);
    }
    return Variable();
}

VariablePath Evaluator::variablePath(const Program &program) const
{
    VariablePath path;
    for(Program elem : program)
    {
        switch(elem.tag())
        {
            case IDENTIFIER:
                path.push_back(elem.payload());
                break;

            case RIGHT_VALUE:
                path.push_back(rightValue(elem).value());
                break;

            case TYPE:
                path.push_back(type(elem));
                break;

            default:
                break;
        }
    }
    return path;
}

ObjectType Evaluator::type(const Program &program) const
{
    const std::string& name = program.node(0).payload().toString();
    const ObjectTypeTemplate& parentTemplate = module.getTemplate(name);
    ObjectType type(parentTemplate);
    if(type.isNull())
    {
        Log::error("Type not found", name);
        return type;
    }

    Program arguments = program.node(1);
    for(int i = 0; i < arguments.size(); ++i)
    {
        if(arguments.node(i).tag() == RIGHT_VALUE)
        {
            type.setParameter(i, rightValue(arguments.node(i)).value());
        }
    }
    return type;
}

Variable Evaluator::unaryOperation(int op, const Variable& a) const
{
    switch(op)
    {
        case NOT_OP:
            return Variable::copy(!a.value());

        case BITWISE_NOT_OP:
            return Variable::copy(~a.value());

        case OPP_OP:
            return Variable::copy(-a.value());;

        case PRE_INC_OP:
            a.setValue(a.value()+1);
            return a;

        case PRE_DEC_OP:
            a.setValue(a.value()-1);
            return a;

        case SUF_INC_OP:
        {
            const Variant& value = a.value();
            Variable aCopy = Variable::copy(value);
            a.setValue(value + 1);
            return aCopy;
        }

        case SUF_DEC_OP:
        {
            const Variant& value = a.value();
            Variable aCopy = Variable::copy(value);
            a.setValue(value - 1);
            return aCopy;
        }

        default:
            break;
    }
    return Variable();
}

Variable Evaluator::binaryOperation(int op, const Variable& a, const Variable& b) const
{
    switch(op)
    {
        case ASSIGN_OP:
            a.setValue(b.value());
            return a;

        case RIGHT_ASSIGN_OP:
            a.setValue(a.value() >> b.value());
            return a;

        case LEFT_ASSIGN_OP:
            a.setValue(a.value() << b.value());
            return a;

        case ADD_ASSIGN_OP:
            a.setValue(a.value() + b.value());
            return a;

        case SUB_ASSIGN_OP:
            a.setValue(a.value() - b.value());
            return a;

        case MUL_ASSIGN_OP:
            a.setValue(a.value() * b.value());
            return a;

        case DIV_ASSIGN_OP:
            a.setValue(a.value() / b.value());
            return a;

        case MOD_ASSIGN_OP:
            a.setValue(a.value() % b.value());
            return a;

        case AND_ASSIGN_OP:
            a.setValue(a.value() & b.value());
            return a;

        case XOR_ASSIGN_OP:
            a.setValue(a.value() ^ b.value());
            return a;

        case OR_ASSIGN_OP:
            a.setValue(a.value() | b.value());
            return a;

        case OR_OP:
            return Variable::copy(a.value() || b.value());

        case AND_OP:
            return Variable::copy(a.value() && b.value());

        case BITWISE_OR_OP:
            return Variable::copy(a.value() | b.value());

        case BITWISE_XOR_OP:
            return Variable::copy(a.value() ^ b.value());

        case BITWISE_AND_OP:
            return Variable::copy(a.value() & b.value());

        case EQ_OP:
            return Variable::copy(a.value() == b.value());

        case NE_OP:
            return Variable::copy(a.value() != b.value());

        case GE_OP:
            return Variable::copy(a.value() >= b.value());

        case GT_OP:
            return Variable::copy(a.value() > b.value());

        case LE_OP:
            return Variable::copy(a.value() <= b.value());

        case LT_OP:
            return Variable::copy(a.value() < b.value());

        case RIGHT_OP:
            return Variable::copy(a.value() >> b.value());

        case LEFT_OP:
            return Variable::copy(a.value() << b.value());

        case ADD_OP:
            return Variable::copy(a.value() + b.value());

        case SUB_OP:
            return Variable::copy(a.value() - b.value());

        case MUL_OP:
            return Variable::copy(a.value() * b.value());

        case DIV_OP:
            return Variable::copy(a.value() / b.value());

        case MOD_OP:
            return Variable::copy(a.value() % b.value());

        default:
            break;
    }
    return Variable();
}

Variable Evaluator::ternaryOperation(int op, const Variable& a, const Variable& b, const Variable& c) const
{
    switch(op)
    {
        case TERNARY_OP:
            if(a.value().toBool())
                return Variable::copy(b.value());
            else
                return Variable::copy(c.value());

        default:
            break;
    }
    return Variable();
}

Variable Evaluator::function(const Program &program) const
{
    const std::string& name = program.node(0).payload().toString() ;
    Program arguments = program.node(1);
    const std::vector<std::string>& parametersNames = module.getFunctionParameterNames(name);
    const std::vector<bool>& parameterModifiables = module.getFunctionParameterModifiables(name);
    const std::vector<Variant>& parametersDefaults = module.getFunctionParameterDefaults(name);

    FunctionScope* functionScope = new FunctionScope;
    unsigned int size = parametersNames.size();
    size_t i = 0;
    for(Program argument:arguments)
    {
        if(i>=size) {
            break;
        }

        bool modifiable = parameterModifiables[i];
        Variable argumentVariable = rightValue(argument, modifiable);
        if(!modifiable) {
            argumentVariable.setConstant();
        }
        functionScope->addNamedParameter(argumentVariable, parametersNames[i]);

        ++i;
    }

    while (i < parametersDefaults.size())
    {
        functionScope->addNamedParameter(Variable::constRef(parametersDefaults[i]), parametersNames[i]);
        ++i;
    }

    while (i < size)
    {
        functionScope->addNamedParameter(Variable(), parametersNames[i]);
        ++i;
    }

    return module.executeFunction(name, Variable(functionScope, true));
}

Variable Evaluator::variable(const Program &program, bool modifiable, bool createIfNeeded) const
{
    return scope.field(variablePath(program), modifiable, createIfNeeded);
}

Variable Evaluator::assignField(const Program &pathProgram, const Program &rightValueProgram) const
{
    Variable value = rightValue(rightValueProgram, true);

    scope.setField(variablePath(pathProgram), value);

    return value;
}

Variable Evaluator::arrayScope(const Program &program) const
{
    ArrayScope* arrayScope = new ArrayScope;
    for (const Program& item : program) {
        arrayScope->addField(rightValue(item));
    }

    return Variable(arrayScope, true);
}

Variable Evaluator::mapScope(const Program &program) const
{
    MapScope* mapScope = new MapScope;
    for (const Program& item : program) {
        mapScope->setField(item[0].payload(), rightValue(item[1], true));
    }

    return Variable(mapScope, true);
}
