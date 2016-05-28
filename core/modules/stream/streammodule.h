#ifndef STREAM_MODULE_H
#define STREAM_MODULE_H

#include "core/object.h"
#include "core/module.h"
#include "core/file/fragmentedfile.h"

const ObjectTypeTemplate parentPID("ParentPID");

class StreamModule : public Module
{
public:
    static FragmentedFile* getFragmentedFile(Object &object);
    static std::string getFragmentedModule(Object &object);
protected:
    bool doLoad() override;

private:
    virtual Parser* getParser(const ObjectType &type, Object& object, const Module& fromModule) const override;
    virtual bool hasParser(const ObjectType &type) const override;
    virtual int64_t doGetFixedSize(const ObjectType& type, const Module& module) const override;
    virtual ObjectType getFatherLocally(const ObjectType &child) const override;
};


#endif // STREAM_MODULE_H
