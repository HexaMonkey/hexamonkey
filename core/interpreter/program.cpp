//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "program.h"
#include "model.h"
#include "variable.h"
#include "variant.h"
#include "scope.h"
#include "functionscope.h"
#include "parser.h"
#include "unused.h"

const Variant nullVariant;
const Variant emptyString("");

Program::Program()
    : _object(nullptr)
{
    UNUSED(hmcElemNames);
}

Program::Program(Object &object, std::shared_ptr<Memory> memory)
    : _object(&object),
      _memory(memory)
{
}

bool Program::isValid() const
{
    return _object != nullptr;
}

uint32_t Program::id() const
{
    return _object->lookUp("id")->value().toInteger();
}

const Variant &Program::payload() const
{
    Object* object = _object->lookUp("payload", true);
    if(object == nullptr)
        object = _object;
    return object->value();
}

int Program::size() const
{
   // _object->explore();
    return _object->numberOfChildren() - 2;
}

Program Program::elem(int index) const
{
    return Program(*_object->access(index+2, true), _memory);
}


Program::const_iterator Program::begin() const
{
    //_object->explore();
    Object::iterator it = _object->begin();
    std::advance(it, 2);
    return const_iterator(it, _memory);
}

Program::const_iterator Program::end() const
{
    return const_iterator(_object->end(), _memory);
}

Program::const_reverse_iterator Program::rbegin() const
{
    //_object->explore();
    return const_reverse_iterator(_object->rbegin(), _memory);
}

Program::const_reverse_iterator Program::rend() const
{
    Object::reverse_iterator it = _object->rend();
    std::advance(it, -2);
    return const_reverse_iterator(it, _memory);
}

Variant Program::evaluateValue(const Scope &scope, const Module &module) const
{
    Variable variable = evaluate(scope, module);
    Variant value = variable.cvalue();
    return value;
}

ObjectType Program::evaluateType(const Scope &scope, const Module &module) const
{
    const std::string& name = elem(0).payload().toString();
    const ObjectTypeTemplate& parentTemplate = module.getTemplate(name);
    ObjectType type(parentTemplate);
    if(type.isNull())
    {
        std::cerr<<"Type not found : "<<name<<std::endl;
        return type;
    }

    Program arguments = elem(1);
    for(int i = 0; i < arguments.size(); ++i)
    {
        if(i >= type.typeTemplate().numberOfParameters())
        {
            std::cerr<<"For type "<<name<<" too many parameters given";
        }
        if(arguments.elem(i).id() == RIGHT_VALUE)
        {
            type.setParameter(i, arguments.elem(i).evaluateValue(scope, module));
        }
    }
    return type;
}

bool Program::hasDeclaration() const
{
    for(Program program: *this)
    {
        switch(program.id())
        {
            case DECLARATION:
                return true;

            case CONDITIONAL_STATEMENT:
                if(program.elem(1).hasDeclaration())
                    return true;
                if(program.elem(2).hasDeclaration())
                    return true;
                break;

            case LOOP:
            case DO_LOOP:
                if(program.elem(1).hasDeclaration())
                    return true;
                break;

            default:
                break;
        }
    }
    return false;
}

void Program::buildDependencies(bool modificationOnly, std::set<VariableDescriptor> &descriptors) const
{
    switch(id())
    {
        case EXECUTION_BLOCK:
        case CONDITIONAL_STATEMENT:
        case LOOP:
        case DO_LOOP:
            for(Program elem : *this)
                elem.buildDependencies(modificationOnly, descriptors);
            break;

        case TYPE:
            for(Program arg : elem(1))
                arg.buildDependencies(modificationOnly, descriptors);
            break;

        case DECLARATION:
            elem(0).buildDependencies(modificationOnly, descriptors);
            break;

        case LOCAL_DECLARATION:
            if(size()>=2)
                elem(1).buildDependencies(modificationOnly, descriptors);
            break;

        case RIGHT_VALUE:
            if(elem(0).id() == OPERATOR)
            {
                int op = elem(0).payload().toInteger();
                for(int i = 0; i < operatorParameterCount[op]; ++i)
                {
                    if(!modificationOnly || !((1<<i)&operatorParameterRelease[op]))
                    {
                        elem(1+i).buildDependencies(modificationOnly, descriptors);
                    }
                }
            }
            else if(elem(0).id() == VARIABLE || elem(0).id() == TYPE)
            {
                elem(0).buildDependencies(modificationOnly, descriptors);
            }
            break;

        case VARIABLE:
            {
                VariableDescriptor descriptor;
                buildVariableDescriptor(Scope(), Module(), descriptor);
                descriptors.insert(descriptor);
            }
            break;

        default:
            break;

    }
}

int64_t Program::guessSize(const Module &module) const
{
    if(size() == 0)
        return 0;

    int64_t size = 0;
    for(const Program& line : *this)
    {
        switch(line.id())
        {
        case DECLARATION:
            {
                ObjectType type = line.elem(0).evaluateValue(Scope(), module).toObjectType();
                if(type.isNull())
                    return -1;
                int64_t elemSize = module.getFixedSize(type);
                if(elemSize == -1)
                    return -1;

                size += elemSize;
                break;
            }

        case LOOP:
        case DO_LOOP:
            std::cout<<"Guess loop"<<std::endl;
            if(line.elem(1).guessSize(module) != 0)
                return -1;
            break;

        case CONDITIONAL_STATEMENT:
            {
                std::cout<<"Guess condition"<<std::endl;
                int64_t size1 = line.elem(1).guessSize(module);
                if(size1 == -1)
                    return -1;
                int64_t size2 = line.elem(2).guessSize(module);
                if(size2 == -1 ||size2 != size1)
                    return -1;
                size += size1;
                break;
            }

        case RIGHT_VALUE:
        case LOCAL_DECLARATION:
            break;

        default:
            return -1;
        }
    }
    return size;
}

Variable Program::evaluate(const Scope &scope, const Module &module) const
{
    Program first = elem(0);
    switch(first.id())
    {
        case OPERATOR:
        {
            const int op = first.payload().toInteger();
            switch(operatorParameterCount[op])
            {
                case 1:
                    return evaluateUnaryOperation  (op, elem(1).evaluate(scope, module));

                case 2:
                    return evaluateBinaryOperation (op, elem(1).evaluate(scope, module),
                                                        elem(2).evaluate(scope, module));

                case 3:
                    return evaluateTernaryOperation(op, elem(1).evaluate(scope, module),
                                                        elem(2).evaluate(scope, module),
                                                        elem(3).evaluate(scope, module));
                default:
                    return Variable();
            }

        }

        case FUNCTION_EVALUATION:
            return first.evaluateFunction(scope, module);

        case INT_CONSTANT:
        case UINT_CONSTANT:
        case STRING_CONSTANT:
        case FLOAT_CONSTANT:
            return Variable::constRef(first.payload());

        case NULL_CONSTANT:
            return Variable::nullConstant();

        case EMPTY_STRING_CONSTANT:
            return Variable::constRef(emptyString);

        case VARIABLE:
            return first.evaluateVariable(scope, module);

        case TYPE:
            return Variable::copy(first.evaluateType(scope, module));
    }
    return Variable();
}

Variable Program::evaluateUnaryOperation(int op, Variable a) const
{
    switch(op)
    {
        case NOT_OP:
            return Variable::copy(!a.cvalue());

        case BITWISE_NOT_OP:
            return Variable::copy(~a.cvalue());

        case OPP_OP:
            return Variable::copy(-a.cvalue());;
            break;

        case PRE_INC_OP:
            ++a.value();
            return a;

        case PRE_DEC_OP:
            --a.value();
            return a;

        case SUF_INC_OP:
            return Variable::copy(a.value()++);

        case SUF_DEC_OP:
            return Variable::copy(a.value()--);

        default:
            break;
    }
    return Variable();

}

Variable Program::evaluateBinaryOperation(int op, Variable a, Variable b) const
{
    switch(op)
    {
        case ASSIGN_OP:
            a.value() = b.cvalue();
            return a;

        case RIGHT_ASSIGN_OP:
            a.value() >>= b.cvalue();
            return a;

        case LEFT_ASSIGN_OP:
            a.value() <<= b.cvalue();
            return a;

        case ADD_ASSIGN_OP:
            a.value() += b.cvalue();
            return a;

        case SUB_ASSIGN_OP:
            a.value() -= b.cvalue();
            return a;

        case MUL_ASSIGN_OP:
            a.value() *= b.cvalue();
            return a;

        case DIV_ASSIGN_OP:
            a.value() /= b.cvalue();
            return a;

        case MOD_ASSIGN_OP:
            a.value() %= b.cvalue();
            return a;

        case AND_ASSIGN_OP:
            a.value() &= b.cvalue();
            return a;

        case XOR_ASSIGN_OP:
            a.value() ^= b.cvalue();
            return a;

        case OR_ASSIGN_OP:
            a.value() |= b.cvalue();
            return a;

        case OR_OP:
            return Variable::copy(a.cvalue() || b.cvalue());

        case AND_OP:
            return Variable::copy(a.cvalue() && b.cvalue());

        case BITWISE_OR_OP:
            return Variable::copy(a.cvalue() | b.cvalue());

        case BITWISE_XOR_OP:
            return Variable::copy(a.cvalue() ^ b.cvalue());

        case BITWISE_AND_OP:
            return Variable::copy(a.cvalue() & b.cvalue());

        case EQ_OP:
            return Variable::copy(a.cvalue() == b.cvalue());

        case NE_OP:
            return Variable::copy(a.cvalue() != b.cvalue());

        case GE_OP:
            return Variable::copy(a.cvalue() >= b.cvalue());

        case GT_OP:
            return Variable::copy(a.cvalue() > b.cvalue());

        case LE_OP:
            return Variable::copy(a.cvalue() <= b.cvalue());

        case LT_OP:
            return Variable::copy(a.cvalue() < b.cvalue());

        case RIGHT_OP:
            return Variable::copy(a.cvalue() >> b.cvalue());

        case LEFT_OP:
            return Variable::copy(a.cvalue() << b.cvalue());

        case ADD_OP:
            return Variable::copy(a.cvalue() + b.cvalue());

        case SUB_OP:
            return Variable::copy(a.cvalue() - b.cvalue());

        case MUL_OP:
            return Variable::copy(a.cvalue() * b.cvalue());

        case DIV_OP:
            return Variable::copy(a.cvalue() / b.cvalue());

        case MOD_OP:
            return Variable::copy(a.cvalue() % b.cvalue());

        default:
            break;
    }

    return Variable();

}

Variable Program::evaluateTernaryOperation(int op, Variable a, Variable b, Variable c) const
{
    switch(op)
    {
        case TERNARY_OP:
            if(a.cvalue().toBool())
                return Variable::copy(b.cvalue());
            else
                return Variable::copy(c.cvalue());

        default:
            break;
    }

    return Variable();
}

Variable Program::evaluateFunction(const Scope &scope, const Module &module) const
{
    const std::string& name = elem(0).payload().toString() ;
    Program arguments = elem(1);
    const std::vector<std::string>& parametersNames = module.getFunctionParameterNames(name);
    const std::vector<bool>& parameterModifiables = module.getFunctionParameterModifiables(name);
    const std::vector<Variant>& parametersDefaults = module.getFunctionParameterDefaults(name);

    FunctionScope functionScope;
    unsigned int size = parametersNames.size();
    size_t i = 0;
    for(Program argument:arguments)
    {
        if(i>=size)
            break;

        Variable argumentVariable = argument.evaluate(scope, module);
        const std::string& argName = parametersNames[i];
        if(!parameterModifiables[i])
        {
            argumentVariable.setConstant();
        }
        functionScope.addParameter(argName, argumentVariable);

        ++i;
    }

    while(i < parametersDefaults.size())
    {
        functionScope.addParameter(parametersNames[i], Variable::constRef(parametersDefaults[i]));
        ++i;
    }

    while(i < size)
    {
        functionScope.addParameter(parametersNames[i], Variable());
        ++i;
    }

    return module.executeFunction(name, functionScope);
}

Variable Program::evaluateVariable(const Scope &scope, const Module &module) const
{
    const Scope* toUseScope = &scope;
    std::unique_ptr<Scope> toDeleteScope;

    VariableDescriptor descriptor;
    buildVariableDescriptor(scope, module, descriptor);

    unsigned int i = 0;
    for(;i < descriptor.size() - 1; ++i)
    {
        Scope* s = toUseScope->getScope(descriptor[i]);
        if(s == nullptr)
            return Variable();
        toUseScope = s;
        toDeleteScope.reset(s);
    }

    const Variant& key = descriptor[i];
    return toUseScope->get(key);
}

void Program::buildVariableDescriptor(const Scope &scope, const Module &module, VariableDescriptor &variableDescriptor) const
{
    for(Program elem : *this)
    {
        switch(elem.id())
        {
            case IDENTIFIER:
                variableDescriptor.push_back(elem.payload());
                break;

            case RIGHT_VALUE:
                variableDescriptor.push_back(elem.evaluateValue(scope, module));
                break;

            case TYPE:
                variableDescriptor.push_back(elem.evaluateType(scope, module));
                break;

            default:
                break;
        }
    }
}

Program::Memory &Program::memory() const
{
    return *_memory.get();
}

File &Program::Memory::file()
{
    return _file;
}

Object &Program::Memory::setFileObject(Object *fileObject)
{
    _fileObject.reset(fileObject);
    return *fileObject;
}
