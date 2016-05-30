#ifndef FROMFILETEMPLATE_H
#define FROMFILETEMPLATE_H

#include "core/objecttypetemplate.h"

#include "core/interpreter/program.h"

class Module;

class FromFileTemplate : public ObjectTypeTemplate
{
public:
    FromFileTemplate(const Module& module, Program declaration);

private:
    const Module& _module;
    Program _classInfo;
    Program _classDefinition;
};

#endif // FROMFILETEMPLATE_H
