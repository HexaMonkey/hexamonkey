#ifndef FORMATDETECTOR_H
#define FORMATDETECTOR_H

#include <string>
#include <vector>
#include <map>
#include "file.h"

/*!
 * \brief The FormatDetector class
 */
class FormatDetector
{
public:
    std::string getFormat(File& file) const;
protected:
    virtual std::string doGetFormat(File& file) const = 0;
};




#endif // FORMATDETECTOR_H
