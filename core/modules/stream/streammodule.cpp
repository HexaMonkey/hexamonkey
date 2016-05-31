
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

