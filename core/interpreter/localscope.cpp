#include "localscope.h"

#include "variant.h"

void LocalScope::clear()
{
    _map.clear();
}

Variant* LocalScope::doDeclare(const Variant &key) const
{
    _map.insert(std::make_pair(key.toString(), Variant()));
    return &_map[key.toString()];
}

Variant *LocalScope::doGet(const Variant &key) const
{
    if(key.canConvertTo(Variant::string))
    {
        auto it = _map.find(key.toString());
        if(it != _map.end())
        {
            return &(it->second);
        }
    }
    return nullptr;
}
