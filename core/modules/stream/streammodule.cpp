
#include "streammodule.h"

#include "core/file/psifragmentedfile.h"
#include "core/file/esfragmentedfile.h"
#include "core/modules/stream/parentpidparser.h"

FragmentedFile* StreamModule::getFragmentedFile(Object* object) {
    std::string streamType = object->getState("_stream_type").toString();
    if(!streamType.compare("psi")) {

        return new PsiFragmentedFile(object);
    }
    else if(!streamType.compare("es")) {
        return new EsFragmentedFile(object);
    }
    else {
        return nullptr;
    }
}

std::string StreamModule::getFragmentedModule(Object* object) {
    std::string streamType = object->getState("_stream_type").toString();
    if(!streamType.compare("psi")) {

        return "psi_table";
    }
    else if(!streamType.compare("es")) {
        return "pes";
    }
    else {
        return nullptr;
    }

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