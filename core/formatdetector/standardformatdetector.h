#ifndef STANDARDFORMATDETECTOR_H
#define STANDARDFORMATDETECTOR_H

#include "compositeformatdetector.h"
#include "magicformatdetector.h"
#include "syncbyteformatdetector.h"
#include "extensionformatdetector.h"


class StandardFormatDetector : public CompositeFormatDetector
{
public:
    class Adder
    {
    public:
        void addMagicNumber(const std::string& magicNumber);
        void addMagicNumber(const std::string& magicNumber, int offset);
        void addSyncbyte(uint8_t syncbyte, int packetlength);
        void addExtension(const std::string& extension);
    private:
        friend class StandardFormatDetector;
        Adder(StandardFormatDetector& detector, const std::string& format);
        StandardFormatDetector& detector;
        const std::string& format;
    };


    StandardFormatDetector();
    ExtensionFormatDetector& extensionDetector();
    MagicFormatDetector&     magicDetector();
    SyncbyteFormatDetector&  syncbyteDetector();

    Adder* newAdder(const std::string& key);


private:
    ExtensionFormatDetector _extensionDetector;
    MagicFormatDetector     _magicDetector;
    SyncbyteFormatDetector  _syncbyteDetector;
};

#endif // STANDARDFORMATDETECTOR_H
