
#ifndef ES_FRAGMENTED_FILE_H
#define ES_FRAGMENTED_FILE_H

#include "core/file/fragmentedfile.h"
#include "core/object.h"

class EsFragmentedFile : public FragmentedFile {
public:
    EsFragmentedFile(Object *object);
private:
    virtual bool importNextFragment() override;
    int _pid;
    uint32_t _n;
    static uint32_t maxFragmentNumber;
};

#endif // ES_FRAGMENTED_FILE