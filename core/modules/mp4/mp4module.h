#ifndef MP4MODULE_H
#define MP4MODULE_H

#include "mapmodule.h"

class Mp4Module : public MapModule
{
protected:
    void addFormatDetection(StandardFormatDetector::Adder& formatAdder) override;
    void requestImportations(std::vector<std::string>& formatRequested) override;
    bool doLoad() override;

private:
    void loadCSVFile(std::ifstream& modelFile);
};

#endif // MP4MODULE_H
