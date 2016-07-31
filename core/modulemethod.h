#ifndef MODULEMETHOD_H
#define MODULEMETHOD_H

#include "variable/variable.h"
#include "variable/variablecollector.h"

class MethodArgument
{
public:
    MethodArgument()
    {
    }

    template <typename TName, typename TDefault>
    MethodArgument(TName&& name, bool constant, TDefault&& defaultValue)
        : _name(name),
          _constant(constant),
          _defaultValue(defaultValue)
    {

    }

    inline const std::string& name() const
    {
        return _name;
    }

    inline bool constant() const
    {
        return _constant;
    }

    inline const Variant& defaultValue() const
    {
        return _defaultValue;
    }

private:
    std::string _name;
    bool _constant;
    Variant _defaultValue;
};

class ModuleMethod
{
public:
    ModuleMethod();

    template <typename TSignature>
    ModuleMethod(TSignature&& signature) : _signature(signature)
    {
    }

    virtual Variable call(VariableArgs& args, VariableKeywordArgs& kwargs, VariableCollector& collector) const;


    void fillNumberedArgs(VariableArgs& args, const VariableKeywordArgs& kwargs, VariableCollector& collector) const;

    void fillArgs(VariableArgs& args, VariableKeywordArgs& kwargs, VariableCollector& collector) const;

private:
    std::vector<MethodArgument> _signature;
};

#endif // MODULEMETHOD_H
