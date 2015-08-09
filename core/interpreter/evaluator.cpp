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
const Module emptyModule;


Evaluator::Evaluator(const Variable &scope)
    : scope(scope),
      module(emptyModule)
{
    UNUSED(hmcElemNames);
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
        case HMC_OPERATOR:
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

        case HMC_FIELD_ASSIGN:
            return assignField(first[0], first[1]);

        case HMC_FUNCTION_EVALUATION:
            return function(first);

        case HMC_INT_CONSTANT:
        case HMC_UINT_CONSTANT:
        case HMC_STRING_CONSTANT:
        case HMC_FLOAT_CONSTANT:
            return collector().copy(first.payload());

        case HMC_NULL_CONSTANT:
            return collector().null();

        case HMC_UNDEFINED_CONSTANT:
            return Variable();

        case HMC_EMPTY_STRING_CONSTANT:
            return collector().copy(emptyString);

        case HMC_VARIABLE:
            return variable(first, modifiable, createIfNeeded);

        case HMC_TYPE:
            return collector().copy(type(first));

        case HMC_ARRAY_SCOPE:
            return arrayScope(first);

        case HMC_MAP_SCOPE:
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
            case HMC_IDENTIFIER:
                path.push_back(elem.payload());
                break;

            case HMC_RIGHT_VALUE:
                path.push_back(rightValue(elem).value());
                break;

            case HMC_TYPE:
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
        Log::error("Type not found ", name);
        return type;
    }

    Program arguments = program.node(1);
    for(int i = 0; i < arguments.size(); ++i)
    {
        if(arguments.node(i).tag() == HMC_RIGHT_VALUE)
        {
            type.setParameter(i, rightValue(arguments.node(i)).value());
        }
    }
    return type;
}

VariableCollector &Evaluator::collector() const
{
    return scope.collector();
}

Variable Evaluator::unaryOperation(int op, const Variable& a) const
{
    switch(op)
    {
        case HMC_NOT_OP:
            return collector().copy(!a.value());

        case HMC_BITWISE_NOT_OP:
            return collector().copy(~a.value());

        case HMC_OPP_OP:
            return collector().copy(-a.value());;

        case HMC_PRE_INC_OP:
            a.setValue(a.value()+1);
            return a;

        case HMC_PRE_DEC_OP:
            a.setValue(a.value()-1);
            return a;

        case HMC_SUF_INC_OP:
        {
            const Variant& value = a.value();
            Variable aCopy = collector().copy(value);
            a.setValue(value + 1);
            return aCopy;
        }

        case HMC_SUF_DEC_OP:
        {
            const Variant& value = a.value();
            Variable aCopy = collector().copy(value);
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
        case HMC_ASSIGN_OP:
            a.setValue(b.value());
            return a;

        case HMC_RIGHT_ASSIGN_OP:
            a.setValue(a.value() >> b.value());
            return a;

        case HMC_LEFT_ASSIGN_OP:
            a.setValue(a.value() << b.value());
            return a;

        case HMC_ADD_ASSIGN_OP:
            a.setValue(a.value() + b.value());
            return a;

        case HMC_SUB_ASSIGN_OP:
            a.setValue(a.value() - b.value());
            return a;

        case HMC_MUL_ASSIGN_OP:
            a.setValue(a.value() * b.value());
            return a;

        case HMC_DIV_ASSIGN_OP:
            a.setValue(a.value() / b.value());
            return a;

        case HMC_MOD_ASSIGN_OP:
            a.setValue(a.value() % b.value());
            return a;

        case HMC_AND_ASSIGN_OP:
            a.setValue(a.value() & b.value());
            return a;

        case HMC_XOR_ASSIGN_OP:
            a.setValue(a.value() ^ b.value());
            return a;

        case HMC_OR_ASSIGN_OP:
            a.setValue(a.value() | b.value());
            return a;

        case HMC_OR_OP:
            return collector().copy(a.value() || b.value());

        case HMC_AND_OP:
            return collector().copy(a.value() && b.value());

        case HMC_BITWISE_OR_OP:
            return collector().copy(a.value() | b.value());

        case HMC_BITWISE_XOR_OP:
            return collector().copy(a.value() ^ b.value());

        case HMC_BITWISE_AND_OP:
            return collector().copy(a.value() & b.value());

        case HMC_EQ_OP:
            return collector().copy(a.value() == b.value());

        case HMC_NE_OP:
            return collector().copy(a.value() != b.value());

        case HMC_GE_OP:
            return collector().copy(a.value() >= b.value());

        case HMC_GT_OP:
            return collector().copy(a.value() > b.value());

        case HMC_LE_OP:
            return collector().copy(a.value() <= b.value());

        case HMC_LT_OP:
            return collector().copy(a.value() < b.value());

        case HMC_RIGHT_OP:
            return collector().copy(a.value() >> b.value());

        case HMC_LEFT_OP:
            return collector().copy(a.value() << b.value());

        case HMC_ADD_OP:
            return collector().copy(a.value() + b.value());

        case HMC_SUB_OP:
            return collector().copy(a.value() - b.value());

        case HMC_MUL_OP:
            return collector().copy(a.value() * b.value());

        case HMC_DIV_OP:
            return collector().copy(a.value() / b.value());

        case HMC_MOD_OP:
            return collector().copy(a.value() % b.value());

        default:
            break;
    }
    return Variable();
}

Variable Evaluator::ternaryOperation(int op, const Variable& a, const Variable& b, const Variable& c) const
{
    switch(op)
    {
        case HMC_TERNARY_OP:
            if(a.value().toBool())
                return collector().copy(b.value());
            else
                return collector().copy(c.value());

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

    FunctionScope* functionScope = new FunctionScope(collector());
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
        functionScope->addNamedParameter(collector().constRef(parametersDefaults[i]), parametersNames[i]);
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
    ArrayScope* arrayScope = new ArrayScope(collector());
    for (const Program& item : program) {
        arrayScope->addField(rightValue(item));
    }

    return Variable(arrayScope, true);
}

Variable Evaluator::mapScope(const Program &program) const
{
    MapScope* mapScope = new MapScope(collector());
    for (const Program& item : program) {
        mapScope->setField(item[0].payload(), rightValue(item[1], true));
    }

    return Variable(mapScope, true);
}
