#include "magicformatdetector.h"
#include <sstream>
#include <algorithm>
#include <iterator>

#include "strutil.h"


void MagicFormatDetector::addMagicNumber(const std::string& format, const std::string &magicNumber)
{
    _magicNumbers[format] = magicNumber;
}

void MagicFormatDetector::addMagicNumber(const std::string &format, const std::string &magicNumber, int offset)
{
    std::stringstream magicStream;
    for(int i = 0; i < offset; ++i)
    {
        magicStream<<"xx ";
    }
    magicStream<<magicNumber;
    addMagicNumber(format, magicStream.str());
}

std::string MagicFormatDetector::doGetFormat(File &file) const
{
    std::string bestFormat = "";
    unsigned int bestMagicLength = 0;
    for(const auto& entry:_magicNumbers)
    {
        const std::string& format = entry.first;
        std::stringstream magicNumber(entry.second);
        file.seekg(0, std::ios_base::beg);

        if(std::all_of(std::istream_iterator<std::string>(magicNumber) , std::istream_iterator<std::string>(), [&](const std::string& token)
        {

            if(token == "xx")
            {
                file.seekg(8, std::ios_base::cur);
                return true;
            }
            else
            {

                uint8_t byte;
                file.read(reinterpret_cast<char*>(&byte), 8);
                return (byte == fromHex(token));
            }
        }))
        {
            if(entry.second.size() > bestMagicLength)
            {
                bestFormat = format;
                bestMagicLength = entry.second.size();
            }
        }
    }
    return bestFormat;
}
