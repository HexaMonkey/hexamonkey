#include "standardformatdetector.h"

StandardFormatDetector::StandardFormatDetector()
{
    addDetector(_magicDetector);
    addDetector(_syncbyteDetector);
    addDetector(_extensionDetector);
}

ExtensionFormatDetector &StandardFormatDetector::extensionDetector()
{
    return _extensionDetector;
}

MagicFormatDetector &StandardFormatDetector::magicDetector()
{
    return _magicDetector;
}

SyncbyteFormatDetector &StandardFormatDetector::syncbyteDetector()
{
    return _syncbyteDetector;
}

StandardFormatDetector::Adder *StandardFormatDetector::newAdder(const std::string &key)
{
    return new Adder(*this, key);
}


void StandardFormatDetector::Adder::addMagicNumber(const std::string &magicNumber)
{
    detector.magicDetector().addMagicNumber(format, magicNumber);
}

void StandardFormatDetector::Adder::addMagicNumber(const std::string &magicNumber, int offset)
{
    detector.magicDetector().addMagicNumber(format, magicNumber, offset);
}

void StandardFormatDetector::Adder::addSyncbyte(uint8_t syncbyte, int packetlength)
{
    detector.syncbyteDetector().addSyncbyte(format, syncbyte, packetlength);
}

void StandardFormatDetector::Adder::addExtension(const std::string &extension)
{
    detector.extensionDetector().addExtension(format, extension);
}

StandardFormatDetector::Adder::Adder(StandardFormatDetector &detector, const std::string &format)
    :detector(detector), format(format)
{
}
