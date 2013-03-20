#ifndef FROMFILEPARSER_H
#define FROMFILEPARSER_H

#include "containerparser.h"

#include "program.h"

#include "objectscope.h"
#include "localscope.h"
#include "compositescope.h"

class Interpreter;

class FromFileParser : public ContainerParser
{
public:
    FromFileParser(Object& object, const Module& module, Interpreter& interpreter, Program program, Program::const_iterator headerEnd);

protected:
    void doParseHead() override;
    void doParse() override;
    bool doParseSome(int hint) override;

private:
    Program::const_iterator executeProgram(const Program::const_iterator& start, const Program::const_iterator& end, int64_t hint = -1);
    void handleDeclaration(const Program& declaration);
    void handleLocalDeclaration(const Program& declaration);
    void handleRightValue(const Program& rightValue);
    void handleCondition(const Program& condition);
    bool handleLoop(const Program& loop);

    Interpreter& interpreter();

    Interpreter& _interpreter;
    Program _program;
    Program::const_iterator _index;
    Program::const_iterator _headerEnd;

    MutableObjectScope _objectScope;
    LocalScope _localScope;
    CompositeScope _scope;

};

#endif // FROMFILEPARSER_H
