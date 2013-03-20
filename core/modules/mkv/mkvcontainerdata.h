#ifndef MKVCONTAINERDATA_H
#define MKVCONTAINERDATA_H

#include "datacontainer.h"

class MkvContainerData : public DataContainer
{
public:
    MkvContainerData(File* file, const Context* context);

    virtual void parse();
    virtual bool parseSome(int hint);
};

#endif // MKVCONTAINERDATA_H
