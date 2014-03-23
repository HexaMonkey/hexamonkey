
#ifndef PSI_FRAGMENTED_FILE_H
#define PSI_FRAGMENTED_FILE_H

#include "core/file/fragmentedfile.h"
#include "core/object.h"

class PsiFragmentedFile : public FragmentedFile {
public:
    PsiFragmentedFile(Object *object);
private:
    virtual bool importNextFragment() override;
    int _pid;
    int _n;
};

#endif // PSI_FRAGMENTED_FILE_H
