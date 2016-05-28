#include "parentpidtemplate.h"

#include "core/object.h"
#include "core/variable/objectattributes.h"
#include "core/file/fragmentedfile.h"

ParentPidTemplate::ParentPidTemplate()
    : ObjectTypeTemplate("ParentPID")
{
}

Parser *ParentPidTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &) const
{
    Object::ParsingContext context(option);

    int pid = static_cast<FragmentedFile*>(&context.object().file())->parent().parent()->lookUp("PID", true)->value().toInteger();
    context.object().setSize(0);
    context.object().setValue(pid);

    return nullptr;
}

int64_t ParentPidTemplate::fixedSize(const ObjectType &, const Module &) const
{
    return 0LL;
}
