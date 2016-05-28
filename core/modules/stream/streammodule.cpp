
#include "streammodule.h"

#include "core/file/psifragmentedfile.h"
#include "core/file/esfragmentedfile.h"
#include "core/modules/stream/parentpidtemplate.h"
#include "core/variable/objectattributes.h"

FragmentedFile* StreamModule::getFragmentedFile(Object& object)
{
    const ObjectAttributes* attributes = object.attributes();
    if (attributes) {
        const Variant *streamTypeAttribute = attributes->getNamed("_stream_type");

        if (streamTypeAttribute) {
            const std::string& streamType = streamTypeAttribute->toString();
            if (!streamType.compare("psi")) {
                return new PsiFragmentedFile(&object);
            } else if (!streamType.compare("es")) {
                return new EsFragmentedFile(&object);
            }
        }
    }
    return nullptr;
}

std::string StreamModule::getFragmentedModule(Object& object)
{
    const ObjectAttributes* attributes = object.attributes();
    if (attributes) {
        const Variant *streamTypeAttribute = attributes->getNamed("_stream_type");

        if (streamTypeAttribute) {
            const std::string& streamType = streamTypeAttribute->toString();
            if (!streamType.compare("psi")) {
                return "psi_table";
            } else if (!streamType.compare("es")) {
                return "pes";
            }
        }
    }
    return "";
}

bool StreamModule::doLoad()
{
    addTemplate(new ParentPidTemplate());

    return true;
}

Parser *StreamModule::getParser(const ObjectType &type, Object &object, const Module &fromModule) const
{
    return type.parseOrGetParser(static_cast<ParsingOption&>(object), fromModule);
}

bool StreamModule::hasParser(const ObjectType &type) const
{
    return hasTemplate(type.name());
}

int64_t StreamModule::doGetFixedSize(const ObjectType &type, const Module &module) const
{
    return type.fixedSize(module);
}

ObjectType StreamModule::getFatherLocally(const ObjectType &child) const
{
    return child.parent();
}
