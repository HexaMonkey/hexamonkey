
#include <memory>

#include "test_formatdetector.h"
#include "core/file/realfile.h"
#include "core/formatdetector/extensionformatdetector.h"
#include "core/formatdetector/compositeformatdetector.h"
#include "core/formatdetector/magicformatdetector.h"
#include "core/formatdetector/standardformatdetector.h"
#include "core/formatdetector/syncbyteformatdetector.h"

void TestFormatDetector::testExtensionFormatDetector()
{
    std::string zip_str("zip");
    std::string empty_str("");

    RealFile fake_zip_file, fake_avi_file;
    fake_zip_file.setPath("resources/format_detector/fake_zip.zip");
    fake_avi_file.setPath("resources/format_detector/fake_avi.avi");

    ExtensionFormatDetector fDetector;
    fDetector.addExtension("mp4", "mp4");
    fDetector.addExtension("zip", "zip");

    // getFormat should return zip
    QCOMPARE(fDetector.getFormat(fake_zip_file), zip_str);
    // since avi extension is unknown, getFormat should return an empty string
    QCOMPARE(fDetector.getFormat(fake_avi_file), empty_str);
}

void TestFormatDetector::testMagicFormatDetector()
{
    std::string zip_str("zip"), empty_str(""), mov_str("mov");

    RealFile fake_avi_file, true_zip_file, mov_file;
    fake_avi_file.setPath("resources/format_detector/fake_avi.avi");
    true_zip_file.setPath("resources/format_detector/true_zip.zip");
    mov_file.setPath("resources/format_detector/magic_mov.mov");

    MagicFormatDetector fDetector;
    fDetector.addMagicNumber("zip", "50 4b 03 04");
    fDetector.addMagicNumber("mov", "6d 6f 6f 76", 4);

    QCOMPARE(fDetector.getFormat(true_zip_file), zip_str);
    // only extension should not work
    QCOMPARE(fDetector.getFormat(fake_avi_file), empty_str);
    // testing magic number offset
    QCOMPARE(fDetector.getFormat(mov_file), mov_str);
}

void TestFormatDetector::testSyncbyteFormatDetector()
{
    std::string ts_str("ts");
    std::string empty_str("");

    RealFile ts_file;
    ts_file.setPath("resources/format_detector/magic_ts.ts");

    SyncbyteFormatDetector fDetector(3);
    fDetector.addSyncbyte("ts", 0x47, 188);

    SyncbyteFormatDetector tooManyPeriodDetector(64);
    tooManyPeriodDetector.addSyncbyte("ts", 0x47, 188);

    // magic_ts.ts is big enough to have 3 188-bytes-long packets
    QCOMPARE(fDetector.getFormat(ts_file), ts_str);
    // magic_ts.ts is truncated and too small, there should not be enough
    // periods (< 64) to conclude.
    QCOMPARE(tooManyPeriodDetector.getFormat(ts_file), empty_str);
}

void TestFormatDetector::testCompositeFormatDetector()
{
    std::string zip_str("zip");
    std::string mov_str("mov");

    // fake_zip.zip contains a truncated mov file.
    RealFile fake_zip_file;
    fake_zip_file.setPath("resources/format_detector/fake_zip.zip");

    ExtensionFormatDetector extensionDetector;
    extensionDetector.addExtension("zip", "zip");
    MagicFormatDetector magicDetector;
    magicDetector.addMagicNumber("zip", "50 4b 03 04");
    magicDetector.addMagicNumber("mov", "6d 6f 6f 76", 4);

    // The detector with highest priority is the extension one. It should
    // detect the file as a zip file, regardless of its content.
    CompositeFormatDetector extensionFirstDetector;
    extensionFirstDetector.addDetector(extensionDetector);
    extensionFirstDetector.addDetector(magicDetector);
    QCOMPARE(extensionFirstDetector.getFormat(fake_zip_file), zip_str);

    // The detector with highest priority is the magic number one. It should
    // detect the file as a mov file.
    CompositeFormatDetector magicFirstDetector;
    magicFirstDetector.addDetector(magicDetector);
    magicFirstDetector.addDetector(extensionDetector);
    QCOMPARE(magicFirstDetector.getFormat(fake_zip_file), mov_str);
}

void TestFormatDetector::testStandardFormatDetector()
{
    std::string zip_str("zip");

    RealFile fake_zip_file, true_zip_file;
    fake_zip_file.setPath("resources/format_detector/fake_zip.zip");
    true_zip_file.setPath("resources/format_detector/true_zip.zip");

    StandardFormatDetector detector;
    std::unique_ptr<StandardFormatDetector::Adder> adder(detector.newAdder(zip_str));
    adder->addMagicNumber("50 4b 03 04");
    adder->addExtension("zip");

    QCOMPARE(detector.getFormat(true_zip_file), zip_str);
    // Since the extension detector is added first in a standard format detector,
    // it should detect the file only using extension.
    QCOMPARE(detector.getFormat(fake_zip_file), zip_str);
}

