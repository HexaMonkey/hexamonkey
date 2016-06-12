#ifndef FROMFILETEMPLATE_H
#define FROMFILETEMPLATE_H

#include "core/objecttypetemplate.h"

#include "core/interpreter/program.h"

#include "core/interpreter/evaluator.h"

class Module;

class FromFileTemplate : public ObjectTypeTemplate
{
public:
    FromFileTemplate(Program declaration, const Module& module, VariableCollector& collector, const Evaluator& evaluator);

private:
    virtual Parser* parseOrGetParser(const ObjectType&, ParsingOption&, const Module &module) const override;
    virtual int64_t fixedSize(const ObjectType& type) const override;
    virtual std::shared_ptr<ObjectType> parent(const ObjectType&) const override;


    Program::const_iterator headerEnd() const;
    bool needTailParsing() const;

    bool checkHeaderOnlyVar(const Program& line) const;
    int64_t guessSize(const Program& instructions) const;
    std::set<VariablePath> variableDependencies(const Program& instructions, bool modificationOnly) const;
    void buildDependencies(const Program& instructions, bool modificationOnly, std::set<VariablePath>& descriptors, bool areVariablesModified = false) const;





    const Module& _module;
    Program _classInfo;
    Program _classDefinition;
    Program _parentInfo;

    VariableCollector&  _collector;
    const Evaluator& _evaluator;

    mutable unsigned int _flag;

    static const unsigned int _sizeComputed = 0x1;
    static const unsigned int _isParentSize = 0x2;
    static const unsigned int _headerEndComputed = 0x4;
    static const unsigned int _needTailParsingComputed = 0x8;

    mutable int64_t _fixedSize;
    mutable Program::const_iterator _headerEnd;
    mutable bool _needTailParsing;
};

#endif // FROMFILETEMPLATE_H
