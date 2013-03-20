#include "compositeformatdetector.h"

void CompositeFormatDetector::addDetector(FormatDetector &detector)
{
    _detectors.push_back(&detector);
}

std::string CompositeFormatDetector::doGetFormat(File& file) const
{
    for(FormatDetector* detector: _detectors)
    {
        const std::string& format = detector->getFormat(file);
        if(!format.empty())
        {
            return format;
        }
    }
    return "";
}
