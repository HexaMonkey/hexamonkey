#include "fromfiletemplate.h"

std::vector<std::string> parseParameters(Program definition)
{
    std::vector<std::string> parameters;
    for(Program arg:definition)
    {
        parameters.push_back(arg.payload().toString());
    }
    return parameters;
}


FromFileTemplate::FromFileTemplate(const Module &module, Program declaration)
    : ObjectTypeTemplate(declaration.node(0).node(0).node(0).payload().toString(), parseParameters(declaration.node(0).node(0).node(1))),
      _module(module),
      _classInfo(declaration.node(0)),
      _classDefinition(declaration.node(1))
{
    if (_classInfo.node(4).payload().toBool()) {
        setVirtual(true);
    }
}
