

#ifndef FRAGMENTED_FILE_H
#define FRAGMENTED_FILE_H

#include <vector>

#include "core/file/file.h"

class Object;
class Module;

class FragmentedFile : public File
{
public:
    FragmentedFile(Object *object);

    // do nothing
    virtual void setPath(const std::string& path) override;

    // do nothing
    virtual const std::string& path() const override;

    // do nothing : the "file" is open since the object is usable
    virtual void open() override;

    // do nothing
    virtual void close() override;

    // do nothing
    virtual void clear() override;

    // Read count bits from the fragments, beginning at the position of _tellg
    virtual void read(char* s, int64_t count) override;

    // Move the _tellg position item in the fragmented file, like in a regular
    // one
    virtual void seekg(int64_t off, std::ios_base::seekdir dir) override;

    // Returns the current stream position
    virtual int64_t tellg() override;

    // Returns the sum of the fragments size
    virtual int64_t size() override;

    // Always returns true...
    virtual bool good() const override;

    void dump(std::ostream &out);

    Object& parent();

protected:
    virtual bool importNextFragment() = 0;

    Object*       _parent;
    File&         _parentFile;
    std::string   _path;
    int64_t       _tellg;
    std::vector<Object*> _fragments;
    FragmentedFile& operator=(const FragmentedFile&) = delete;
    FragmentedFile(const FragmentedFile&) = delete;
};

#endif // FRAGMENTED_FILE_H
