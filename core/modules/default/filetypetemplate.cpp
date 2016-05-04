#include "filetypetemplate.h"
#include "core/object.h"
#include "core/variable/objectattributes.h"

FileTypeTemplate::FileTypeTemplate()
    :ObjectTypeTemplate("File")
{
    setVirtual(true);
}

Parser *FileTypeTemplate::parseOrGetParser(const ObjectType &/*objectType*/, ParsingOption &option, const Module&) const
{
    Object::ParsingContext context(option);
    int64_t size = context.object().file().size();
    context.object().setSize(size);
    context.object().attributes(true)->addNamed("path")->setValue(context.object().file().path());

    return nullptr;
}
