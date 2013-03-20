#ifndef STANDARDMODULE_H
#define STANDARDMODULE_H

#include "mapmodule.h"

class StandardModule : public MapModule
{
public:
    StandardModule(bool bigEndian);

protected:
    bool doLoad() override;

private:
    bool bigEndian;
};

#endif // STANDARDMODULE_H
