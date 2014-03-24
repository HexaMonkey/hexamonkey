
#include "esfragmentedfile.h"

uint32_t EsFragmentedFile::maxFragmentNumber = 1000;

EsFragmentedFile::EsFragmentedFile(Object *object) : FragmentedFile(object), _n(0)
{
    _pid = object->lookUp("PID", true)->value().toInteger();
    auto main_obj = _parent->parent();
    // _fragments.push_back(object->lookUp("payload", true));
    int currentRank = 0;
    while(true) {
        if(currentRank >= main_obj->numberOfChildren()) {
            main_obj->exploreSome(128);
            if(currentRank >= main_obj->numberOfChildren())
                return;
        }
        auto it = main_obj->begin()+currentRank;
        if( (*it)->lookUp("PID", true)->value().toInteger() == _pid
            && (*it)->lookUp("payload_unit_start_indicator", true)->value().toInteger() == 1)
        {
            _fragments.push_back((*it)->lookUp("payload", true));
            _n++;
            return;
        }
        currentRank++;
    }
}

bool EsFragmentedFile::importNextFragment() {
    if(_n>=maxFragmentNumber)
        return false;

    if((_fragments.size() % 100) == (maxFragmentNumber % 100))
        std::cout << _fragments.size() << "/" << maxFragmentNumber << " packets imported." << std::endl;

    auto main_obj = _parent->parent();
    int previousRank = 0;
    previousRank = _fragments.back()->parent()->rank();
    int currentRank = previousRank+1;
    while(true) {
        if(currentRank >= main_obj->numberOfChildren()) {
            main_obj->exploreSome(128);
            if(currentRank >= main_obj->numberOfChildren())
                return false;
        }
        auto it = main_obj->begin()+currentRank;
        if((*it)->lookUp("PID", true)->value().toInteger() == _pid) {
            Object* payload = (*it)->lookUp("payload", true);
            if(payload) {
                _fragments.push_back(payload);
                _n++;
                return true;
            }
        }
        currentRank++;
    }
    return false;
}