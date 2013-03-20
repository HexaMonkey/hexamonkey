#ifndef EXTENSIONFORMATDETECTOR_H
#define EXTENSIONFORMATDETECTOR_H

#include "formatdetector.h"

/*!
 * \brief The ExtensionFormatDetector class
 */
class ExtensionFormatDetector : public FormatDetector
{
public:
    void addExtension(const std::string& format, const std::string& extension);
protected:
    virtual std::string doGetFormat(File& file) const override;
private:
    std::map<std::string, std::string> _extensions;
};

#endif // EXTENSIONFORMATDETECTOR_H
