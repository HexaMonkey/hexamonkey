#ifndef EBMLMODULE_H
#define EBMLMODULE_H

#include "mapmodule.h"

class EbmlModule : public MapModule
{
protected:
    void addFormatDetection(StandardFormatDetector::Adder& formatAdder) override;
    void requestImportations(std::vector<std::string>& formatRequested) override;
    bool doLoad() override;

private:
    static int64_t parseId(char* str);
};

#endif // EBMLMODULE_H
