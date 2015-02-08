

#include "parentpidparser.h"
#include "core/file/fragmentedfile.h"

ParentPidParser::ParentPidParser(Object &object) : SimpleParser(object)
{
}

void ParentPidParser::doParseHead()
{
    int pid = reinterpret_cast<FragmentedFile*>(&file())->parent().parent()->lookUp("PID", true)->value().toInteger();
    setSize(0);
    setValue(pid);
}
