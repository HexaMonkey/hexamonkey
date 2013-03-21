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

#include "interpreter.h"
#include <sstream>
#ifdef USE_QT
#include <QProcess>
#include <QDir>
#include <QFileInfo>
#else
#include <cstdlib>
#include <cstdio>
#endif

#include "defaulttypes.h"
#include "model.h"

#include "unused.h"
#include "hmcmodule.h"
#include "variable.h"
#include "variabledescriptor.h"
#include "scope.h"
#include "objecttypetemplate.h"
#include "parser.h"



Interpreter::Interpreter(const HmcModule &module) : _module(module), _program(NULL), _currentObject(NULL), _fileObject(NULL), emptyString("")
{
    UNUSED(hmcElemNames);
}

Interpreter::~Interpreter()
{
    clear();
}

bool Interpreter::loadFromString(const std::string &exp)
{
    std::ofstream f("temp.hm");
    f<<exp<<std::endl;
    f.close();
    return loadFromHM("temp.hm", expression);
}

bool Interpreter::loadFromHM(const std::string &path, int mode)
{
    const std::string outputName = path+"c";

#ifdef USE_QT
    QProcess process;
    QString base;
    if(mode == file)
         base = "hexacompiler";
    else
         base = "expcompiler";

    #ifdef __MINGW32__
    QString extension = ".exe";
    #else
    QString extension = "";
    #endif
    QFileInfo program;



    program.setFile(QString("./%1%2").arg(base).arg(extension));
    if(!program.exists())
    {
        program.setFile(QString("../compiler/%1%2").arg(base).arg(extension));
    }
    if(!program.exists())
    {
        std::cerr<<"Compiler not found"<<std::endl;
        return false;
    }


    QStringList arguments;
    QFileInfo inputFile(path.c_str());
    QString arg1 = inputFile.absoluteFilePath();
    QString arg2 = QString("%1c").arg(arg1);
    arguments<<arg1<<arg2;
    std::cout<<"Program "<<program.absoluteFilePath().toStdString()<<std::endl;
    std::cout<<"Arguments "<<arg1.toStdString()<<" "<<arg2.toStdString()<<std::endl;
    process.start(program.absoluteFilePath(), arguments);
    process.waitForFinished();
    std::string output =  QString(process.readAllStandardOutput()).toStdString();
    _error = QString(process.readAllStandardError()).toStdString();
    std::cout<<QString(process.readAllStandardOutput()).toStdString()<<std::endl;

    if(!output.empty())
    {
        std::cout<<"Compiler output : "<<output<<std::endl;
    }

    if(!_error.empty())
    {
        std::cerr<<"Compiler error : "<<_error<<std::endl;
        return false;
    }
#else
    std::cout<<"don't use qt"<<std::endl;
    std::stringstream commandStream;
#ifndef linux
    commandStream<<"..\\compiler\\";
#else
    commandStream<<"../compiler/"
#endif

    if(mode==file)
        commandStream<<"hexacompiler";
    else
        commandStream<<"expcompiler";

    commandStream<<" "<<path<<" "<<outputName;
    const char* command = commandStream.str().c_str();

#ifndef linux
    _pclose(_popen(command, "r"));
#else
    pclose(popen(command, "r"));
#endif
#endif
    return loadFromHMC(outputName);
}

bool Interpreter::loadFromHMC(const std::string &path)
{
    clear();
    _file.setPath(path);

    _fileObject = _module.handle(defaultTypes::file(), _file);
    _fileObject->explore(-1);

    if(_fileObject->numberOfChildren())
    {
        delete _program;
        _program = new Program(*_fileObject->access(1));
        return true;
    }
    else
    {
        std::cout << "failure" << std::endl;
        return false;
    }
}

const std::string &Interpreter::error() const
{
    return _error;
}

void Interpreter::setObject(Object& object)
{
    _currentObject = &object;
}

Program& Interpreter::program()
{
    return *_program;
}

Variable& Interpreter::evaluate(const Program& rightValue, const Scope &scope, const Module &module)
{
    Program first = rightValue.elem(0);
    switch(first.id())
    {
    case OPERATOR:
    {
        const int op = first.payload().toInteger();
        switch(operatorParameterCount[op])
        {
            case 1:
                return evaluateUnaryOperation(op,   evaluate(rightValue.elem(1), scope, module));

            case 2:
                return evaluateBinaryOperation(op,  evaluate(rightValue.elem(1), scope, module),
                                                    evaluate(rightValue.elem(2), scope, module));

            case 3:
                return evaluateTernaryOperation(op, evaluate(rightValue.elem(1), scope, module),
                                                    evaluate(rightValue.elem(2), scope, module),
                                                    evaluate(rightValue.elem(3), scope, module));

            default:
                return constReference(null);
        }

    }

    case INT_CONSTANT:
    case UINT_CONSTANT:
    case STRING_CONSTANT:
    case FLOAT_CONSTANT:
        return constReference(first.payload());

    case NULL_CONSTANT:
        return constReference(null);

    case EMPTY_STRING_CONSTANT:
        return constReference(emptyString);

    case VARIABLE:
        return evaluateVariable(first, scope, module);

    case TYPE:
        return copy(evaluateType(first, scope, module));
    }
    return constReference(null);
}

Variable& Interpreter::evaluateBinaryOperation(int op, Variable &a, Variable &b)
{
    Variable *result;
    switch(op)
    {
    case ASSIGN_OP:
        a.value() = b.constValue();
        result = &a;
        break;

    case RIGHT_ASSIGN_OP:
        a.value() >>= b.constValue();
        result = &a;
        break;

    case LEFT_ASSIGN_OP:
        a.value() <<= b.constValue();
        result = &a;
        break;

    case ADD_ASSIGN_OP:
        a.value() += b.constValue();
        result = &a;
        break;

    case SUB_ASSIGN_OP:
        a.value() -= b.constValue();
        result = &a;
        break;

    case MUL_ASSIGN_OP:
        a.value() *= b.constValue();
        result = &a;
        break;

    case DIV_ASSIGN_OP:
        a.value() /= b.constValue();
        result = &a;
        break;

    case MOD_ASSIGN_OP:
        a.value() %= b.constValue();
        result = &a;
        break;

    case AND_ASSIGN_OP:
        a.value() &= b.constValue();
        result = &a;
        break;

    case XOR_ASSIGN_OP:
        a.value() ^= b.constValue();
        result = &a;
        break;

    case OR_ASSIGN_OP:
        a.value() |= b.constValue();
        result = &a;
        break;

    case OR_OP:
        result = &copy(a.constValue() || b.constValue());
        break;

    case AND_OP:
        result = &copy(a.constValue() && b.constValue());
        break;

    case BITWISE_OR_OP:
        result = &copy(a.constValue() | b.constValue());
        break;

    case BITWISE_XOR_OP:
        result = &copy(a.constValue() ^ b.constValue());
        break;

    case BITWISE_AND_OP:
        result = &copy(a.constValue() & b.constValue());
        break;

    case EQ_OP:
        result = &copy(a.constValue() == b.constValue());
        break;

    case NE_OP:
        result = &copy(a.constValue() != b.constValue());
        break;

    case GE_OP:
        result = &copy(a.constValue() >= b.constValue());
        break;

    case GT_OP:
        result = &copy(a.constValue() > b.constValue());
        break;

    case LE_OP:
        result = &copy(a.constValue() <= b.constValue());
        break;

    case LT_OP:
        result = &copy(a.constValue() < b.constValue());
        break;

    case RIGHT_OP:
        result = &copy(a.constValue() >> b.constValue());
        break;

    case LEFT_OP:
        result = &copy(a.constValue() << b.constValue());
        break;

    case ADD_OP:
        result = &copy(a.constValue() + b.constValue());
        break;

    case SUB_OP:
        result = &copy(a.constValue() - b.constValue());
        break;

    case MUL_OP:
        result = &copy(a.constValue() * b.constValue());
        break;

    case DIV_OP:
        result = &copy(a.constValue() / b.constValue());
        break;

    case MOD_OP:
        result = &copy(a.constValue() % b.constValue());
        break;

    default:
        result = &constReference(null);
    }

    const int parameterRelease = operatorParameterRelease[op];
    if(parameterRelease&0x1)
        release(a);
    if(parameterRelease&0x2)
        release(b);

    return *result;
}

Variable &Interpreter::evaluateUnaryOperation(int op, Variable &a)
{
    Variable *result = nullptr;
    switch(op)
    {
    case NOT_OP:
        result = &copy(!a.constValue());
        break;

    case BITWISE_NOT_OP:
        result = &copy(~a.constValue());
        break;

    case OPP_OP:
        result = &copy(-a.constValue());;
        break;

    case PRE_INC_OP:
        ++a.value();
        result = &a;
        break;

    case PRE_DEC_OP:
        --a.value();
        result = &a;
        break;

    case SUF_INC_OP:
        result = &copy(a.value()++);
        break;

    case SUF_DEC_OP:
        result = &copy(a.value()--);
        break;

    case TOINT_OP:
        if(a.value().canConvertTo(Variant::integer))
        {
            result = &copy(a.value().toInteger());
        }
        else if(a.value().canConvertTo(Variant::string))
        {
            std::stringstream S;
            S<<a.value().toString();
            int64_t i;
            if(!(S>>i).fail())
                result = &copy(i);
            else
                result = &constReference(null);
        }
        break;

    case UPPERCASE_OP:
    {
        std::string str = toStr(a.value());
        std::transform(str.begin(), str.end(),str.begin(), ::toupper);
        result = &copy(str);
        break;
    }


    case LOWERCASE_OP:
    {
        std::string str = toStr(a.value());
        std::transform(str.begin(), str.end(),str.begin(), ::tolower);
        result = &copy(str);
        break;
    }

    default:
        result = &constReference(null);
    }

    const int parameterRelease = operatorParameterRelease[op];
    if(parameterRelease)
        release(a);

    return *result;
}

Variable &Interpreter::evaluateTernaryOperation(int op, Variable &a, Variable &b, Variable &c)
{
    Variable *result = nullptr;
    switch(op)
    {
    case SUBSTR_OP:
        if(    a.value().canConvertTo(Variant::string)
            && b.value().canConvertTo(Variant::integer)
            && c.value().canConvertTo(Variant::integer))
        {
            result = &copy(a.value().toString().substr(b.value().toInteger(), c.value().toInteger()));
        }
        break;

    case TOSTR_OP:
    {
        std::stringstream S;
        S<<std::setbase(b.value().toInteger())<<std::setw(c.value().toInteger())<<std::setfill('0')<<a.value();
        result = &copy(S.str());
    }
        break;

    default:
        result = &constReference(null);
    }

    const int parameterRelease = operatorParameterRelease[op];
    if(parameterRelease&0x1)
        release(a);
    if(parameterRelease&0x2)
        release(b);
    if(parameterRelease&0x4)
        release(c);

    return *result;
}

Variable& Interpreter::evaluateVariable(const Program &variable, const Scope &scope, const Module &module)
{
    const Scope* toUseScope = &scope;
    Scope* toDeleteScope    = nullptr;

    VariableDescriptor descriptor;
    buildVariableDescriptor(variable, scope, module, descriptor);

    unsigned int i = 0;
    for(;i < descriptor.size() - 1; ++i)
    {
        Scope* s = toUseScope->getScope(descriptor[i]);
        if(s == nullptr)
            return constReference(null);
        delete toDeleteScope;
        toDeleteScope = s;
        toUseScope = s;
    }

    const Variant& key = descriptor[i];
    Variant* value = toUseScope->get(key);
    if(value != nullptr)
    {
        delete toDeleteScope;
        return reference(*value);
    }

    const Variant* cvalue = toUseScope->cget(key);
    if(cvalue != nullptr)
    {
        delete toDeleteScope;
        return constReference(*cvalue);
    }

    return copy(Variant());
}

ObjectType Interpreter::evaluateType(const Program &program, const Scope &scope, const Module &module)
{
    const std::string& name = program.elem(0).payload().toString();
    const ObjectTypeTemplate& parentTemplate = module.getTemplate(name);
    ObjectType type(parentTemplate);

    Program arguments = program.elem(1);
    for(int i = 0; i < arguments.size(); ++i)
    {
        if(arguments.elem(i).id() == RIGHT_VALUE)
        {
            Variable& var = evaluate(arguments.elem(i), scope, module);
            type.setParameter(i, var.value());
            release(var);
        }
    }
    return type;
}

bool Interpreter::hasDeclaration(const Program &classDefinition)
{
    for(Program program: classDefinition)
    {
        switch(program.id())
        {
            case DECLARATION:
                return true;

            case CONDITIONAL_STATEMENT:
                if(hasDeclaration(program.elem(1)))
                    return true;
                if(hasDeclaration(program.elem(2)))
                    return true;
                break;

            case LOOP:
                if(hasDeclaration(program.elem(1)))
                    return true;
                break;

            default:
                break;
        }
    }
    return false;
}

void Interpreter::buildDependencies(const Program &program, bool modificationOnly, std::set<VariableDescriptor> &descriptors)
{
    switch(program.id())
    {
    case CLASS_DEFINITION:
    case CONDITIONAL_STATEMENT:
    case LOOP:
        for(Program elem:program)
            buildDependencies(elem, modificationOnly, descriptors);
        break;

    case TYPE:
        for(Program arg:program.elem(1))
            buildDependencies(arg, modificationOnly, descriptors);
        break;

    case DECLARATION:
        buildDependencies(program.elem(0), modificationOnly, descriptors);
        break;

    case LOCAL_DECLARATION:
        if(program.size()>=2)
            buildDependencies(program.elem(1), modificationOnly, descriptors);
        break;

    case RIGHT_VALUE:
        if(program.elem(0).id() == OPERATOR)
        {
            int op = program.elem(0).payload().toInteger();
            for(int i = 0; i < operatorParameterCount[op]; ++i)
            {
                if(!modificationOnly || !((1<<i)&operatorParameterRelease[op]))
                {
                    buildDependencies(program.elem(1+i), modificationOnly, descriptors);
                }
            }
        }
        else if(program.elem(0).id() == VARIABLE || program.elem(0).id() == TYPE)
        {
            buildDependencies(program.elem(0), modificationOnly, descriptors);
        }
        break;

    case VARIABLE:
        {
            VariableDescriptor descriptor;
            buildVariableDescriptor(program, EmptyScope(), Module(), descriptor);
            descriptors.insert(descriptor);
        }
        break;

    default:
        break;

    }
}

int64_t Interpreter::guessSize(const Program &classDefinition, const Module &module)
{
    if(classDefinition.size() == 0)
        return 0;

    int64_t size = 0;
    for(const Program& line : classDefinition)
    {

        switch(line.id())
        {
        case DECLARATION:
            {
                const ObjectType type = evaluate(line.elem(0), EmptyScope(), module).value().toObjectType();
                if(type.isNull())
                    return -1;
                int64_t elemSize = module.getFixedSize(type);
                if(elemSize == -1)
                    return -1;

                size += elemSize;
                break;
            }

        case LOOP:
            if(guessSize(line.elem(1), module) != 0)
                return -1;
            break;

        case CONDITIONAL_STATEMENT:
            {
                int64_t size1 = guessSize(line.elem(1), module);
                if(size1 == -1)
                    return -1;
                int64_t size2 = guessSize(line.elem(2), module);
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

Variable &Interpreter::copy(const Variant &value)
{
    Variable* var = Variable::copy(value);
    _vars.insert(var);
    return *var;
}

Variable &Interpreter::constReference(const Variant &value)
{
    Variable* var = Variable::constReference(value);
    _vars.insert(var);
    return *var;
}

Variable &Interpreter::reference(Variant &value)
{
    Variable* var = Variable::reference(value);
    _vars.insert(var);
    return *var;
}

void Interpreter::release(Variable &var)
{
    std::set<Variable*>::iterator it = _vars.find(&var);
    delete *it;
    _vars.erase(it);
}

void Interpreter::garbageCollect()
{
    for(Variable* var : _vars)
    {
        delete var;
    }
    _vars.clear();
}

void Interpreter::clear()
{
    garbageCollect();
    _file.close();
    _file.clear();
    _currentObject = NULL;
    delete _fileObject;
    _fileObject = NULL;
}

void Interpreter::buildVariableDescriptor(const Program &variable, const Scope &scope, const Module &module, VariableDescriptor &variableDescriptor)
{
    for(Program elem : variable)
    {
        switch(elem.id())
        {
            case IDENTIFIER:
                variableDescriptor.push_back(elem.payload());
                break;

            case RIGHT_VALUE:
                {
                    Variable& var = evaluate(elem, scope);
                    variableDescriptor.push_back(var.value());
                    release(var);
                }
                break;

            case TYPE:
                variableDescriptor.push_back(evaluateType(elem, scope, module));
                break;

            default:
                break;
        }
    }
}
