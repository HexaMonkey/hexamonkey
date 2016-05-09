#ifndef FIXEDPARENTTYPETEMPLATE_H
#define FIXEDPARENTTYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class FixedParentTypeTemplate : public ObjectTypeTemplate
{
public:
    FixedParentTypeTemplate(const std::string &name,
                            const std::vector<std::string>& parameterNames,
                            std::shared_ptr<ObjectType> parent);
    FixedParentTypeTemplate(const std::string &name,
                            std::shared_ptr<ObjectType> parent);

private:
    virtual std::shared_ptr<ObjectType> parent(const ObjectType&) const override;

    std::shared_ptr<ObjectType> _parent;

};

#endif // FIXEDPARENTTYPETEMPLATE_H
