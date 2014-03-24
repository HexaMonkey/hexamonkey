
#include "core/file/fragmentedfile.h"
#include "core/object.h"
#include "core/module.h"

FragmentedFile::FragmentedFile(Object *object, Module *module) : File(), _parent(object), _module(module) {
    //_module->handle(object);
    // should use the module, somehow
    // test with psi fragments

    auto pid = object->parent()->lookUp("PID", true)->value().toInteger();
    int n = object->lookUp("psi_syntax_section", true)
                  ->lookUp("last_section_number", true)
                  ->value().toInteger();
    std::cout << "last_section_number: " << n << ", pid: " << pid  << std::endl;

    if(!object->lookUp("psi_begin", true))
    {
        std::cout << "psi_begin null!" << std::endl;
    }
    _fragments.push_back(object->lookUp("psi_begin", true));

    auto main_obj = object->parent()->parent();
    auto it = main_obj->begin()+object->parent()->rank()+1;
    while(n>0) {
        for(;it!=main_obj->end();it++) {

            if((*it)->lookUp("PID", true)->value().toInteger() == pid) {
                if(!(*it)->lookUp("psi_fragment", true)) {
                    std::cout << "rank: " << (*it)->rank() << std::endl;
                    std::cout << "whowhowho!" << std::endl;
                }
                _fragments.push_back((*it)->lookUp("psi_fragment", true));
                n--;
                continue;
            }
        }
        if(it == main_obj->end()) {
            main_obj->exploreSome(128);
            if(it == main_obj->end())
                break;
        }
    }
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
