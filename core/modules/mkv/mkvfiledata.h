#ifndef MKVFILEDATA_H
#define MKVFILEDATA_H

#include "filedata.h"

class MkvFileData : public FileData
{
public:
    MkvFileData(File* file, const Context* context);

    virtual void parse();
};

#endif // MKVFILEDATA_H
