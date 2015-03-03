

#include "parentpidparser.h"
#include "core/file/fragmentedfile.h"

ParentPidParser::ParentPidParser(Object &object) : SimpleParser(object)
{
}

void ParentPidParser::doParseHead()
{
    int pid = reinterpret_cast<FragmentedFile*>(&object().file())->parent().parent()->lookUp("PID", true)->value().toInteger();
    object().setSize(0);
    object().setValue(pid);
}
