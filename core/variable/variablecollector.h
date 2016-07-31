#ifndef VARIABLECOLLECTOR_H
#define VARIABLECOLLECTOR_H

#include <vector>
#include <unordered_map>

#include "core/variable/commonvariable.h"

class VariableImplementation;

class VariableCollector
{
public:
    VariableCollector();
    ~VariableCollector();

    void collect();

    inline void registerVariable(VariableImplementation* variable) {
        _accessibility.insert(std::make_pair(variable, false));
        addDirectlyAccessible(variable);
    }
    inline void addDirectlyAccessible(VariableImplementation* variable) {
        _directlyAccessible.push_back(variable);
    }
    void removeDirectlyAccessible(VariableImplementation* variable);


    /**
     * @brief Construct a \link Variable variable\endlink copying and then owning a value
     */
    inline Variable copy(const Variant& value, bool modifiable = true)
    {
        return Variable(new OwningVariableImplementation(*this, value), modifiable);
    }

    /**
     * @brief Construct a \link Variable variable\endlink referencing a value (modifiable or constant reference)
     */
    inline Variable ref(Variant& value, bool modifiable = true)
    {
        return Variable(new RefVariableImplementation(*this, value), modifiable);
    }

    /**
     * @brief Construct a \link Variable variable\endlink referencing a value if the pointer is not nullptr (modifiable or constant reference)
     */
    inline Variable refIfNotNull(Variant* value, bool modifiable = true)
    {
        if (value != nullptr) {
            return Variable(new RefVariableImplementation(*this, *value), modifiable);
        } else {
            return Variable();
        }
    }

    /**
     * @brief Construct a \link Variable variable\endlink referencing a value (always constant reference)
     */
    inline Variable constRef(const Variant& value)
    {
        return Variable(new ConstRefVariableImplementation(*this, value), false);
    }

    /**
     * @brief Construct a \link Variable variable\endlink referencing a value if the pointer is not nullptr (modifiable or constant reference)
     */
    inline Variable constRefIfNotNull(const Variant* value)
    {
        if (value != nullptr) {
            return Variable(new ConstRefVariableImplementation(*this, *value), false);
        } else {
            return Variable();
        }
    }

    /**
     * @brief Construct a \link Variable variable\endlink owning a null value
     */
    inline Variable null()
    {
        return Variable(new OwningVariableImplementation(*this, nullVariant), true);
    }

private:
    void compact();

    bool _destroying;
    std::vector<VariableImplementation*> _directlyAccessible;
    std::unordered_map<VariableImplementation*, bool> _accessibility;


    static Variant nullVariant;
};

class VariableCollectionGuard
{
public:
    VariableCollectionGuard(VariableCollector& collector);
    ~VariableCollectionGuard();

private:
    VariableCollector& _collector;

};

#endif // VARIABLECOLLECTOR_H
