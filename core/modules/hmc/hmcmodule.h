#ifndef HMCMODULE_H
#define HMCMODULE_H

#include "mapmodule.h"

/*!
 * \brief The HmcModule class
 */
class HmcModule : public MapModule
{
protected:
    void addFormatDetection(StandardFormatDetector::Adder& formatAdder) override;
    void requestImportations(std::vector<std::string>& formatRequested) override;
    bool doLoad() override;
};

#endif // HMCMODULE_H
