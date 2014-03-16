

#ifndef FRAGMENTED_FILE_H
#define FRAGMENTED_FILE_H

#include "core/file/file.h"

class Object;
class Module;

class FragmentedFile : public File
{
public:
    FragmentedFile(Object *object, Module *module);

    // do nothing
    virtual void setPath(const std::string& path) override;

    // do nothing
    virtual const std::string& path() const override;

    // do nothing : the "file" is open since the object is usable
    virtual void open() override;

    // do nothing
    virtual void close() override;

    // do nothing
    virtual void clear() override;

    // FIXME
    virtual void read(char* s, int64_t size) override;

    // FIXME
    virtual void seekg(int64_t off, std::ios_base::seekdir dir) override;

    // FIXME
    virtual int64_t tellg() override;

    // FIXME
    virtual int64_t size() override;

    // FIXME
    virtual bool good() override;


private:
    Object const* _parent;
    Module const* _module;
    std::string   _path;
    std::pair<Object*, int64_t> _fragments;
    FragmentedFile& operator=(const FragmentedFile&) = delete;
    FragmentedFile(const FragmentedFile&) = delete;
};

#endif // FRAGMENTED_FILE_H
