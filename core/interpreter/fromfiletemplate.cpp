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
#include "core/log/logmanager.h"

std::vector<std::string> parseParameters(Program definition)
{
    std::vector<std::string> parameters;
    for(Program arg:definition)
    {
        parameters.push_back(arg.payload().toString());
    }
    return parameters;
}


std::unordered_map<std::string, ObjectTypeTemplate::Attribute> attributeMapping = {
    {"elemType", ObjectTypeTemplate::Attribute::elementType},
    {"elemCount", ObjectTypeTemplate::Attribute::elementCount},
    {"name", ObjectTypeTemplate::Attribute::name},
    {"displayMode", ObjectTypeTemplate::Attribute::displayMode},
    {"displayAs", ObjectTypeTemplate::Attribute::displayAs}
};


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

    Program typeAttributes = _classInfo.node(3);
    for (Program typeAttribute : typeAttributes) {
        const std::string& attributeName = typeAttribute.node(0).payload().toString();
        Program program = typeAttribute.node(1);

        auto it = attributeMapping.find(attributeName);
        if (it != attributeMapping.end()) {
            _attributeExpressions[it->second] = program;
        } else {
            Log::error("Unrecognized attribute ", attributeName, " for ", name());
        }
    }
}

Parser *FromFileTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    if(_classDefinition.node(0).size() == 0 && _classDefinition.node(1).size() == 0) {
        return nullptr;
    }

    return new FromFileParser(option, _module, _classDefinition, headerEnd(), needTailParsing());
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

int64_t FromFileTemplate::fixedSize(const ObjectType &type) const
{
    if (! (_flag & _sizeComputed)) {
        _fixedSize = unknownSize;
        std::set<VariablePath> dependencies = variableDependencies(_classDefinition, true);
        if (dependencies.find(sizeDescriptor) == dependencies.end())
        {
            if (!isVirtual() && type.parent().isNull()) {
                VariableCollectionGuard guard(_collector);

                int64_t size = guessSize(_classDefinition);
                if(size>=0)
                {
    #ifdef LOAD_TRACE
                    std::cerr<<type.typeTemplate().name()<<" guessed size "<<size<<std::endl;
    #endif
                    _fixedSize = size;
                }
            } else {
        #ifdef LOAD_TRACE
                std::cerr<<type.typeTemplate().name()<<" father's size"<<std::endl;
        #endif

                _flag |= _isParentSize;
            }
        }

        _flag |= _sizeComputed;
    }

    if (_flag & _isParentSize) {
        return type.parent().fixedSize();
    } else {
        return _fixedSize;
    }
}

std::shared_ptr<ObjectType> FromFileTemplate::parent(const ObjectType &type) const
{
    if (_parentInfo.size() == 0) {
        return fromFileNullParent;
    } else {
        VariableCollectionGuard guard(_collector);
        return Evaluator(Variable(new TypeScope(_collector, type), false), _module).sharedType(_parentInfo.node(0));
    }
}

Variant FromFileTemplate::attributeValue(const ObjectType &type, ObjectTypeTemplate::Attribute attribute) const
{
    auto it = _attributeExpressions.find(attribute);

    if (it != _attributeExpressions.end())
    {
        VariableCollectionGuard guard(_collector);
        return Evaluator(Variable(new TypeScope(_collector, type), false), _module).rightValue(it->second).value();
    } else {
        return Variant();
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

int64_t FromFileTemplate::guessSize(const Program &instructions) const
{
    int64_t size = 0;
    for(const Program& line : instructions)
    {
        switch(line.tag())
        {
        case HMC_EXECUTION_BLOCK:
            size += guessSize(line);
            break;

        case HMC_DECLARATION:
            {
                if(!variableDependencies(line.node(0),false).empty())
                    return unknownSize;

                ObjectType type = _evaluator.rightValue(line.node(0)).value().toObjectType();
                if(type.isNull())
                    return unknownSize;

                int64_t elemSize = type.fixedSize();
                if(elemSize == -1)
                    return unknownSize;

                size += elemSize;
                break;
            }

        case HMC_LOOP:
        case HMC_DO_LOOP:
            if(guessSize(line.node(1)) != 0)
                return unknownSize;
            break;

        case HMC_CONDITIONAL_STATEMENT:
            {
                int64_t size1 = guessSize(line.node(1));
                if(size1 == -1)
                    return unknownSize;
                int64_t size2 = guessSize(line.node(2));
                if(size2 == -1 ||size2 != size1)
                    return unknownSize;
                size += size1;
                break;
            }

        case HMC_RIGHT_VALUE:
        case HMC_LOCAL_DECLARATION:
            break;

        default:
            return unknownSize;
        }
    }
    return size;
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
