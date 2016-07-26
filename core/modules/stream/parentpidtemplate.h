#ifndef PARENTPIDTEMPLATE_H
#define PARENTPIDTEMPLATE_H

#include "core/objecttypetemplate.h"

class ParentPidTemplate : public ObjectTypeTemplate
{
public:
    ParentPidTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType&, ParsingOption&) const;


    virtual int64_t fixedSize(const ObjectType&) const;
};

#endif // PARENTPIDTEMPLATE_H
