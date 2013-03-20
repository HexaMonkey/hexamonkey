#include "formatdetector.h"

#include "strutil.h"
#include <iterator>
#include <sstream>
#include <algorithm>
#include <utility>

std::string FormatDetector::getFormat(File& file) const
{
    int64_t pos = file.tellg();
    const std::string& format = doGetFormat(file);
    file.clear();
    file.close();
    file.open();
    file.seekg(pos, std::ios_base::beg);
    return format;
}
