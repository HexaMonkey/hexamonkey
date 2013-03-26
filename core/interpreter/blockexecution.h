#ifndef BLOCKEXECUTION_H
#define BLOCKEXECUTION_H

class Module;
class Interpreter;
class ContainerParser;

#include "program.h"


class BlockExecution
{
public:
    BlockExecution(Program::const_iterator begin,
                   Program::const_iterator end,
                   const Module& module,
                   Interpreter& interpreter,
                   ContainerParser* parser = nullptr);

private:
    const Program::const_iterator begin;
    const Program::const_iterator end;
    Program::const_iterator current;

    const Module& module;
    Interpreter& interpreter;
    ContainerParser* parser;

};

#endif // BLOCKEXECUTION_H
