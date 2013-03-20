#ifndef MAGICFORMATDETECTOR_H
#define MAGICFORMATDETECTOR_H

#include "formatdetector.h"

/*!
 * \brief The MagicFormatDetector class
 */
class MagicFormatDetector : public FormatDetector
{
public:
    void addMagicNumber(const std::string &format, const std::string& magicNumber);
    void addMagicNumber(const std::string &format, const std::string& magicNumber, int offset);
protected:
    virtual std::string doGetFormat(File& file) const override;
private:
    std::map<std::string, std::string> _magicNumbers;
};

#endif // MAGICFORMATDETECTOR_H
