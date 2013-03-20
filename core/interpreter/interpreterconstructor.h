#ifndef INTERPRETERCONSTRUCTOR_H
#define INTERPRETERCONSTRUCTOR_H

class HmcModule;
class Interpreter;

class InterpreterConstructor
{
public:
    InterpreterConstructor(const HmcModule& hmcModule);
    Interpreter* newInstance() const;

private:
    const HmcModule& _hmcModule;
};

#endif // INTERPRETERCONSTRUCTOR_H
