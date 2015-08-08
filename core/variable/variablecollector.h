#ifndef VARIABLECOLLECTOR_H
#define VARIABLECOLLECTOR_H

#include <vector>
#include <unordered_map>
class VariableImplementation;

class VariableCollector
{
public:
    VariableCollector();
    ~VariableCollector();

    void collect();

    void registerVariable(VariableImplementation* variable);
    void pushDirectlyAccessible(VariableImplementation* variable);
    void popDirectlyAccessible(VariableImplementation* variable);
private:
    void compact();

    std::vector<VariableImplementation*> _directlyAccessible;
    std::unordered_map<VariableImplementation*, bool> _accessibility;
};

#endif // VARIABLECOLLECTOR_H
