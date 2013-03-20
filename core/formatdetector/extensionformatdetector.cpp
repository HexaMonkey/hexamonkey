#include "extensionformatdetector.h"
#include "strutil.h"

void ExtensionFormatDetector::addExtension(const std::string& format, const std::string &extension)
{
    _extensions[extension] = format;
}

std::string ExtensionFormatDetector::doGetFormat(File &file) const
{
    auto it = _extensions.find(extension(file.path()));
    if(it != _extensions.end())
    {
        return it->second;
    }
    return "";
}

