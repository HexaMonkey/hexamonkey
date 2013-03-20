#ifndef MKVMODULE_H
#define MKVMODULE_H

#include "mapmodule.h"

/*!
 * \brief The MkvModule class
 */
class MkvModule : public MapModule
{
protected:
    void addFormatDetection(StandardFormatDetector::Adder& formatAdder) override;
    void requestImportations(std::vector<std::string>& formatRequested) override;
    bool doLoad() override;

private:
    static int64_t parseId(char* str);
};

#endif // MKVMODULE_H
