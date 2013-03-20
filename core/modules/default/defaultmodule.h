#ifndef DEFAULTMODULE_H
#define DEFAULTMODULE_H

#include "mapmodule.h"

class DefaultModule : public MapModule
{
protected:
    bool doLoad() override;
};

#endif // DEFAULTMODULE_H
