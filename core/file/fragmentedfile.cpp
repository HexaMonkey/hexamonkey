
#include "core/file/fragmentedfile.h"
#include "core/object.h"
#include "core/module.h"

FragmentedFile::FragmentedFile(Object *object, Module *module) : File(), _parent(object), _module(module) {
    _module.handle(object)
}


void FragmentedFile::setPath(const std::string& path) {}

const std::string& FragmentedFile::path() const {
    return _path;
}

void FragmentedFile::open() {}

void FragmentedFile::close() {}

void FragmentedFile::clear() {}

void FragmentedFile::read(char* s, int64_t size) {
    throw "fixme";
}

void FragmentedFile::seekg(int64_t off, std::ios_base::seekdir dir) {
    throw "fixme";
}

int64_t FragmentedFile::tellg() {
    throw "fixme";
    return 0;
}

int64_t FragmentedFile::size() {
    throw "fixme";
}

bool FragmentedFile::good() {
    throw "fixme";
}

void FragmentedFile::dump(std::ostream &out) {
    for(auto &p : _fragments) {
        p->dump(out);
    }
}