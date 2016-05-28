#ifndef PARENTPIDTEMPLATE_H
#define PARENTPIDTEMPLATE_H

#include "core/objecttypetemplate.h"

class ParentPidTemplate : public ObjectTypeTemplate
{
public:
    ParentPidTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType&, ParsingOption&, const Module &module) const;


    virtual int64_t fixedSize(const ObjectType&, const Module& module) const;
};

#endif // PARENTPIDTEMPLATE_H
