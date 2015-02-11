
#include "streammodule.h"

#include "core/file/psifragmentedfile.h"
#include "core/file/esfragmentedfile.h"
#include "core/modules/stream/parentpidparser.h"

FragmentedFile* StreamModule::getFragmentedFile(Object& object)
{
    const Variant *streamTypeAttribute = object.attributeValue("_stream_type");

    if (streamTypeAttribute) {
        const std::string& streamType = streamTypeAttribute->toString();
        if (!streamType.compare("psi")) {
            return new PsiFragmentedFile(&object);
        } else if (!streamType.compare("es")) {
            return new EsFragmentedFile(&object);
        }
    }
    return nullptr;
}

std::string StreamModule::getFragmentedModule(Object& object)
{
    const Variant *streamTypeAttribute = object.attributeValue("_stream_type");

    if (streamTypeAttribute) {
        const std::string& streamType = streamTypeAttribute->toString();
        if (!streamType.compare("psi")) {
            return "psi_table";
        } else if (!streamType.compare("es")) {
            return "pes";
        }
    }
    return "";
}

bool StreamModule::doLoad()
{
    addTemplate(parentPID);
    addParser("ParentPID", [this]parserLambda
    {
        return new ParentPidParser(object);
    });
    setFixedSize("ParentPID", 0);
    return true;
}
