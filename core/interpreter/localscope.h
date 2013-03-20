#ifndef LOCALSCOPE_H
#define LOCALSCOPE_H

#include <map>
#include <string>

#include "scope.h"

class LocalScope : public MutableScope
{
public:
    void clear();

protected:
    Variant* doDeclare(const Variant &key) const;
    Variant* doGet(const Variant &key) const;
private:
    mutable std::map<std::string, Variant> _map;
};

#endif // LOCALSCOPE_H
