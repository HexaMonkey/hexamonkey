#ifndef TSMODULE_H
#define TSMODULE_H

#include "mapmodule.h"

/*!
 * \brief The TsModule class
 */
class TsModule : public MapModule
{
protected:
    void addFormatDetection(StandardFormatDetector::Adder& formatAdder) override;
    void requestImportations(std::vector<std::string>& formatRequested) override;
    bool doLoad() override;
};

#endif // TSMODULE_H
