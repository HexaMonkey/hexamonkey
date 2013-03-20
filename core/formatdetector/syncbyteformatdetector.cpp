#include "syncbyteformatdetector.h"
#include <algorithm>

void SyncbyteFormatDetector::addSyncbyte(const std::string& format, uint8_t syncbyte, int packetlength)
{
    _formats[format] = std::make_pair(syncbyte, packetlength);
}

std::string SyncbyteFormatDetector::doGetFormat(File &file) const
{
    for(const auto& entry:_formats)
    {
        file.seekg(0, std::ios_base::beg);
        std::string format = entry.first;
        uint8_t syncbyte = entry.second.first;
        int packetlength = entry.second.second;

        std::vector<int> occurences(packetlength,0);
        for(int i = 0; i < check; ++i)
        {
            for(int j = 0; j < packetlength; ++j)
            {
                uint8_t byte;
                file.read(reinterpret_cast<char*>(&byte), 8);
                if(byte == syncbyte)
                    occurences[j]++;
            }
        }

        if(std::any_of(occurences.begin(), occurences.end(), [](int i){return i==check;}))
        {
            return format;
        }
    }
    return "";
}
