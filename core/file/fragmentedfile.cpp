
#include <stdexcept>

#include "core/file/fragmentedfile.h"
#include "core/object.h"
#include "core/module.h"

FragmentedFile::FragmentedFile(Object *object) :
    File(), _parent(object), _parentFile(object->file()), _tellg(0) {}


void FragmentedFile::setPath(const std::string& path) {}

const std::string& FragmentedFile::path() const {
    return _path;
}

void FragmentedFile::open() {}

void FragmentedFile::close() {}

void FragmentedFile::clear() {}

void FragmentedFile::read(char* s, int64_t count) {
    if(count == 0)
        return;

    int64_t beginFrag = 0;
    auto it = _fragments.begin();
    while(beginFrag+(*it)->size()<_tellg) {
        if(it==_fragments.end()) {
            if(!importNextFragment()) {
                // requesting out of range fragment
                return;
            }
        }
        beginFrag += (*it)->size();
        it++;
    }

    while(count > 0) {
        if(it==_fragments.end()) {
            if(!importNextFragment()) {
                // requesting out of range fragment
                return;
            }
        }
        int64_t fragmentCount = std::min(count, beginFrag+(*it)->size()-_tellg);
        _parentFile.seekg( (*it)->beginningPos()+_tellg-beginFrag,
                           std::ios_base::beg);

        // TODO : accept also %8=0 fragments sizes

        _parentFile.read(s, fragmentCount);
        s += fragmentCount/8;
        _tellg += fragmentCount;
        count -= fragmentCount;

        beginFrag += (*it)->size();
        it++;
    }
}

void FragmentedFile::seekg(int64_t off, std::ios_base::seekdir dir) {
    switch (dir)
    {
        case std::ios_base::beg :
            _tellg = off;
        break;
        case std::ios_base::end :
            _tellg = size()-1;
        break;
        default:
            _tellg += off;
        break;
    }
}

int64_t FragmentedFile::tellg() {
    return _tellg;
}

int64_t FragmentedFile::size() {
    while(importNextFragment());
    int64_t size = 0;
    for(auto &p : _fragments) {
        size += p->size();
    }
    return size;
}

bool FragmentedFile::good() {
    return true;
}

void FragmentedFile::dump(std::ostream &out) {
    while(importNextFragment());
    for(auto &p : _fragments) {
        p->dump(out);
    }
}
