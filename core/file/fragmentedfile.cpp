
#include <stdexcept>

#include "core/file/fragmentedfile.h"
#include "core/object.h"
#include "core/module.h"

FragmentedFile::FragmentedFile(Object *object) :
    File(), _parent(object), _parentFile(object->file()), _tellg(0)
{
    // test with psi fragments

    _pid = object->parent()->lookUp("PID", true)->value().toInteger();
    _n = object->lookUp("psi_syntax_section", true)
                  ->lookUp("last_section_number", true)
                  ->value().toInteger();

    if(!object->lookUp("psi_begin", true))
    {
        std::cout << "psi_begin null!" << std::endl;
    }
    object->explore(-1);
    _fragments.push_back(object);
}


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

bool FragmentedFile::importNextFragment() {
    if(_n<=0)
        return false;

    auto main_obj = _parent->parent()->parent();
    int previousRank = 0;
    if(_fragments.size()==1) {
        previousRank = _fragments.back()->parent()->rank();
    } else {
        previousRank = _fragments.back()->parent()->parent()->rank();
    }
    auto it = main_obj->begin()+previousRank+1;
    while(true) {
        for(;it!=main_obj->end();it++) {
            if((*it)->lookUp("PID", true)->value().toInteger() == _pid) {
                _fragments.push_back((*it)->lookUp("psi_fragment", true));
                _n--;
                return true;
            }
        }
        if(it == main_obj->end()) {
            main_obj->exploreSome(128);
            if(it == main_obj->end())
                return false;
        }
    }
    return false;
}