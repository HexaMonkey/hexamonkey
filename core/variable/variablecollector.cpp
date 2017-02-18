#include "variablecollector.h"
#include "core/variable/variable.h"
#include "core/log/logmanager.h"

#include <algorithm>

Variant VariableCollector::nullVariant = Variant::null();

VariableCollector::VariableCollector()
    : _destroying(false)
{
}

VariableCollector::~VariableCollector()
{
    _destroying = true;
    if (!_directlyAccessible.empty()) {
        Log::warning("Destroying variable collector while there are still directly accessible variables");
    }
    for (auto it = _accessibility.begin(); it != _accessibility.end(); ++it)
    {
        delete it->first;
    }
}

void VariableCollector::collect()
{
    compact();

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
            _directlyAccessible[i]->collect([this] (VariableMemory& variable) {
                if (variable._tag.flags.defined) {
                    VariableImplementation* implementation = variable._implementation;
                    auto& isAccessible = _accessibility[implementation];
                    if (!isAccessible) {
                        isAccessible = true;
                        _directlyAccessible.push_back(implementation);
                    }
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

void VariableCollector::removeDirectlyAccessible(VariableImplementation *variable)
{
    auto rit = _directlyAccessible.rbegin();
    auto rend = _directlyAccessible.rend();

    if (rit != rend && !_destroying) {
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


VariableCollectionGuard::VariableCollectionGuard(VariableCollector &collector)
    : _collector(collector)
{
}

VariableCollectionGuard::~VariableCollectionGuard()
{
    _collector.collect();
}
