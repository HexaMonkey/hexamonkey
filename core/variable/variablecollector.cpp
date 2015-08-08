#include "variablecollector.h"
#include "core/variable/variable.h"

#include <algorithm>

VariableCollector::VariableCollector()
{
}

VariableCollector::~VariableCollector()
{
    for (auto it = _accessibility.begin(); it != _accessibility.end(); ++it)
    {
        delete it->first;
    }
}

void VariableCollector::collect()
{
    size_t i = 0;
    size_t originalSize = _directlyAccessible.size();

    std::for_each(_directlyAccessible.begin(), _directlyAccessible.end(), [this] (VariableImplementation* variable) {
        _accessibility[variable] = true;
    });

    while (true) {
        size_t size = _directlyAccessible.size();
        if (i >= size) {
            break;
        }

        do {
            _directlyAccessible[i]->collect([this] (VariableImplementation* variable) {
                auto& isAccessible = _accessibility[variable];
                if (!isAccessible) {
                    isAccessible = true;
                    _directlyAccessible.push_back(variable);
                }
            });
            ++i;
        } while (i < size);
    }

    _directlyAccessible.resize(originalSize);

    for (auto it = _accessibility.begin(); it != _accessibility.end() /* not hoisted */; /* no increment */)
    {
        auto& isAccessible = it->second;
        if (isAccessible) {
            isAccessible = false; // reset until next collection
            ++it;
        } else {
            delete it->first;
            _accessibility.erase(it++);
        }
    }
}

void VariableCollector::registerVariable(VariableImplementation *variable)
{
    _accessibility.emplace(std::make_pair(variable, false));
}


void VariableCollector::pushDirectlyAccessible(VariableImplementation *variable)
{
    _directlyAccessible.push_back(variable);
}

void VariableCollector::popDirectlyAccessible(VariableImplementation *variable)
{
    auto rit = _directlyAccessible.rbegin();
    auto rend = _directlyAccessible.rend();

    if (rit != rend) {
        if (*rit == variable) {
            ++rit;
            while (rit != rend && *rit == nullptr) {
                ++rit;
            }
            _directlyAccessible.resize(std::distance(rit, rend));
        } else {
            ++rit;
            while (rit != rend) {
                if (*rit == variable) {
                    *rit = nullptr;
                    break;
                }
                ++rit;
            }
        }
    }
}

void VariableCollector::compact()
{
    size_t newSize = 0;
    auto inputIt = _directlyAccessible.begin();
    auto outputIt = _directlyAccessible.begin();
    auto endIt = _directlyAccessible.end();

    while (inputIt != endIt) {
        auto& inputValue = *inputIt;
        if (inputValue) {
            *outputIt = inputValue;
            ++outputIt;
            ++newSize;
        }
        ++inputIt;
    }

    _directlyAccessible.resize(newSize);
}
