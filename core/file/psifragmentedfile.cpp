
#include "psifragmentedfile.h"

PsiFragmentedFile::PsiFragmentedFile(Object *object) : FragmentedFile(object)
{
    _pid = object->parent()->lookUp("PID", true)->value().toInteger();
    _n = object->lookUp("psi_syntax_section", true)
                  ->lookUp("last_section_number", true)
                  ->value().toInteger();
    object->explore(-1);
    _fragments.push_back(object);
}

bool PsiFragmentedFile::importNextFragment() {
    if(_n<=0)
        return false;

    auto main_obj = _parent->parent()->parent();
    int previousRank = 0;
    if(_fragments.size()==1) {
        previousRank = _fragments.back()->parent()->rank();
    } else {
        previousRank = _fragments.back()->parent()->parent()->rank();
    }
    int currentRank = previousRank+1;
    while(true) {
        if(currentRank >= main_obj->numberOfChildren()) {
            main_obj->exploreSome(128);
            if(currentRank >= main_obj->numberOfChildren())
                return false;
        }
        auto it = main_obj->begin()+currentRank;
        if((*it)->lookUp("PID", true)->value().toInteger() == _pid) {
            _fragments.push_back((*it)->lookUp("psi_fragment", true));
            _n--;
            return true;
        }
        currentRank++;
    }
    return false;
}