#ifndef STREAM_MODULE_H
#define STREAM_MODULE_H

#include "core/object.h"
#include "core/mapmodule.h"
#include "core/file/fragmentedfile.h"

const ObjectTypeTemplate parentPID("ParentPID");

class StreamModule : public MapModule
{
public:
    static FragmentedFile* getFragmentedFile(Object &object);
    static std::string getFragmentedModule(Object &object);
protected:
    bool doLoad() override;
};


#endif // STREAM_MODULE_H
