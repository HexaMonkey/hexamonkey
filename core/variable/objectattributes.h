#ifndef OBJECTATTRIBUTES_H
#define OBJECTATTRIBUTES_H

#include "core/variable/variable.h"

#include <unordered_map>
#include <string>

class ObjectAttributes : public VariableImplementation
{
public:
    ObjectAttributes();

    Variant& addNumbered();
    Variant& getNumbered(size_t number);
    const Variant& getNumbered(size_t numbered) const;
    size_t numberedCount() const;

    Variant* addNamed(const std::string& name);
    Variant* getNamed(const std::string& name);
    const Variant* getNamed(const std::string &name) const;
    const std::vector<std::string>& fieldNames() const;

protected:
    virtual Variable doGetField(const Variant &key, bool modifiable) override;

private:
    std::unordered_map<std::string, Variant> _namedFields;
    std::vector<std::string> _fieldNames;
    std::vector<Variant> _numberedFields;
};

#endif // OBJECTATTRIBUTES_H
