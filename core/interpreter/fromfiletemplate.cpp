#include "fromfiletemplate.h"


#include "compiler/model.h"
#include "core/interpreter/programloader.h"
#include "core/interpreter/fromfiletemplate.h"
#include "core/variable/variablepath.h"
#include "core/variable/variable.h"
#include "core/interpreter/fromfileparser.h"
#include "core/interpreter/blockexecution.h"
#include "core/variable/functionscope.h"
#include "core/variable/localscope.h"
#include "core/variable/typescope.h"
#include "core/variable/variablecollector.h"
#include "core/util/fileutil.h"
#include "core/util/unused.h"

std::vector<std::string> parseParameters(Program definition)
{
    std::vector<std::string> parameters;
    for(Program arg:definition)
    {
        parameters.push_back(arg.payload().toString());
    }
    return parameters;
}


FromFileTemplate::FromFileTemplate(Program declaration, const Module &module, VariableCollector& collector, const Evaluator& evaluator)
    : ObjectTypeTemplate(declaration.node(0).node(0).node(0).payload().toString(), parseParameters(declaration.node(0).node(0).node(1))),
      _module(module),
      _classInfo(declaration.node(0)),
      _classDefinition(declaration.node(1)),
      _collector(collector),
      _evaluator(evaluator),
      _flag(0)
{
    UNUSED(hmcElemNames);

    if (_classInfo.node(4).payload().toBool()) {
        setVirtual(true);
    }

    _parentInfo = _classInfo.node(1);
    //If there is no "parent" then "specification" is taken
    if(_parentInfo.size() == 0)
    {
        _parentInfo = _classInfo.node(2);
    }
}



const VariablePath sizeDescriptor = {"@size"};
const std::vector<VariablePath> headerOnlyVars = {
    sizeDescriptor,
    {"@args"},
    {"@value"},
    {"@info"},
    {"@linkTo"},
    {"@attr"}
};

std::shared_ptr<ObjectType> fromFileNullParent(new ObjectType);

std::shared_ptr<ObjectType> FromFileTemplate::parent(const ObjectType &type) const
{
    if (_parentInfo.size() == 0) {
        return fromFileNullParent;
    } else {
        VariableCollectionGuard guard(_collector);
        return Evaluator(Variable(new TypeScope(_collector, type), false), _module).sharedType(_parentInfo.node(0));
    }
}

Program::const_iterator FromFileTemplate::headerEnd() const
{
    if (! (_flag & _headerEndComputed))
    {
        Program bodyBlock = _classDefinition.node(0);

        for (Program::const_iterator headerEnd = bodyBlock.begin(); headerEnd != bodyBlock.end(); ++headerEnd) {
            const Program& line = *headerEnd;
            //Check header mark
            if (line.tag() == HMC_HEADER_MARK) {
                return ++headerEnd;
            }
        }

        Program::const_reverse_iterator reverseHeaderEnd = bodyBlock.rbegin();
        for(;reverseHeaderEnd != bodyBlock.rend(); ++reverseHeaderEnd)
        {
            if(checkHeaderOnlyVar(*reverseHeaderEnd)) {
                break;
            }
        }

        _headerEnd = bodyBlock.begin();
        std::advance(_headerEnd, std::distance(reverseHeaderEnd, bodyBlock.rend()));

        _flag |= _headerEndComputed;
    }
    return _headerEnd;
}

bool FromFileTemplate::needTailParsing() const
{
    if (! (_flag & _needTailParsingComputed))
    {
        Program tailBlock = _classDefinition.node(1);

        _needTailParsing = false;
        for(Program line : tailBlock)
        {
            if(checkHeaderOnlyVar(line)) {
                _needTailParsing = true;
            }
        }

        _flag |= _needTailParsingComputed;
    }
    return _needTailParsing;
}



bool FromFileTemplate::checkHeaderOnlyVar(const Program &line) const
{
    //Check dependencies
    std::set<VariablePath> variableSet = variableDependencies(line, true);

    return std::any_of(headerOnlyVars.begin(), headerOnlyVars.end(), [&variableSet](const VariablePath& headerOnlyVar)
    {
        auto find = variableSet.find(headerOnlyVar);
        if(find != variableSet.end())
            return true;

        auto upper = variableSet.upper_bound(headerOnlyVar);
        if(upper == variableSet.end())
            return false;

        if((*upper).inScopeOf(headerOnlyVar))
            return true;

        return false;

    });
}


void FromFileTemplate::buildDependencies(const Program &instructions, bool modificationOnly, std::set<VariablePath> &descriptors, bool areVariablesModified) const
{
    switch(instructions.tag())
    {
        case HMC_EXECUTION_BLOCK:
        case HMC_CONDITIONAL_STATEMENT:
        case HMC_LOOP:
        case HMC_DO_LOOP:
            for(Program elem : instructions) {
                buildDependencies(elem, modificationOnly, descriptors);
            }
            break;

        case HMC_TYPE:
            for(Program arg : instructions.node(1)) {
                buildDependencies(arg, modificationOnly, descriptors);
            }
            break;

        case HMC_DECLARATION:
            buildDependencies(instructions.node(0), modificationOnly, descriptors);
            break;

        case HMC_LOCAL_DECLARATIONS:
            for (const Program& localDeclaration : instructions) {
                if(localDeclaration.size()>=2) {
                    buildDependencies(localDeclaration.node(1), modificationOnly, descriptors);
                }
            }
            break;

        case HMC_RIGHT_VALUE:
            if(instructions.node(0).tag() == HMC_OPERATOR) {
                int op = instructions.node(0).payload().toInteger();
                for(int i = 0; i < operatorParameterCount[op]; ++i) {
                    buildDependencies(instructions.node(1+i), modificationOnly, descriptors, areVariablesModified || !((1<<i)&operatorParameterRelease[op]));
                }
            } else if(instructions.node(0).tag() == HMC_VARIABLE) {
                buildDependencies(instructions.node(0), modificationOnly, descriptors, areVariablesModified);
            } else if(instructions.node(0).tag() == HMC_TYPE) {
                buildDependencies(instructions.node(0), modificationOnly, descriptors);
            }
            break;

        case HMC_VARIABLE:
            if (!modificationOnly || areVariablesModified) {
                descriptors.insert(_evaluator.variablePath(instructions));
            }
            break;

        default:
            break;

    }
}

std::set<VariablePath> FromFileTemplate::variableDependencies(const Program &instructions, bool modificationOnly) const
{
    std::set<VariablePath> dependencies;
    VariableCollectionGuard guard(_collector);
    buildDependencies(instructions, modificationOnly, dependencies);
    return dependencies;
}
