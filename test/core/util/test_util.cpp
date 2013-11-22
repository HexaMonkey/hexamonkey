
#include <sstream>

#include "test_util.h"
#include "core/util/bitutil.h"
#include "core/util/csvreader.h"
#include "core/util/fileutil.h"

void TestUtil::testCSVReader()
{
    std::stringstream input;
    input << "specification;master;0;" << std::endl;
    input << "executionBlock;master;1;" << std::endl;
    input << "argumentDeclarations;master;1;" << std::endl;
    CSVReader reader(input);
    QCOMPARE(reader.hasNextLine(), true);

    std::vector<std::string> fields;
    reader.readNextLine(fields);
    QCOMPARE(fields.size(), std::vector<std::string>::size_type(3));
    QCOMPARE(fields[0], std::string("specification"));
    QCOMPARE(fields[1], std::string("master"));
    QCOMPARE(fields[2], std::string("0"));
    QCOMPARE(reader.hasNextLine(), true);

    reader.ignoreNextLine();
    QCOMPARE(reader.hasNextLine(), true);

    fields.clear();
    reader.readNextLine(fields);
    QCOMPARE(fields.size(), std::vector<std::string>::size_type(3));
    QCOMPARE(fields[0], std::string("argumentDeclarations"));
    QCOMPARE(fields[1], std::string("master"));
    QCOMPARE(fields[2], std::string("1"));

    QCOMPARE(reader.hasNextLine(), false);
}

// most significant bit
void TestUtil::testBitUtil_msb()
{
    QCOMPARE(msb(0x00), int(0));
    QCOMPARE(msb(0x03), int(1));
    QCOMPARE(msb(0x06), int(2));
    QCOMPARE(msb(0x09), int(3));
    QCOMPARE(msb(0x10), int(4));
    QCOMPARE(msb(0x21), int(5));
    QCOMPARE(msb(0x50), int(6));
    QCOMPARE(msb(0xFF), int(7));
}

// least significant bit mask
void TestUtil::testBitUtil_lsbMask()
{
    QCOMPARE(lsbMask(0),uint8_t(0));
    QCOMPARE(lsbMask(1),uint8_t(1));
    QCOMPARE(lsbMask(2),uint8_t(3));
    QCOMPARE(lsbMask(3),uint8_t(7));
    QCOMPARE(lsbMask(4),uint8_t(15));
    QCOMPARE(lsbMask(5),uint8_t(31));
    QCOMPARE(lsbMask(6),uint8_t(63));
    QCOMPARE(lsbMask(7),uint8_t(127));
    QCOMPARE(lsbMask(8),uint8_t(255));
}

void TestUtil::testBitUtil_popCount()
{
    QCOMPARE(popCount(0), uint8_t(0));
    QCOMPARE(popCount(1), uint8_t(1));
    QCOMPARE(popCount(17), uint8_t(2));
    QCOMPARE(popCount(35), uint8_t(3));
    QCOMPARE(popCount(15), uint8_t(4));
    QCOMPARE(popCount(157), uint8_t(5));
    QCOMPARE(popCount(603), uint8_t(6));
    QCOMPARE(popCount(1234567), uint8_t(11));
    QCOMPARE(popCount(9012384), uint8_t(7));
    QCOMPARE(popCount(68719476736), uint8_t(1)); // 2^36
    QCOMPARE(popCount(68719476735), uint8_t(36)); // 2^36-1
}


void TestUtil::testFileUtil_fileExists()
{
    QCOMPARE(fileExists("../../ressources/fake_zip.zip"), true);
    QCOMPARE(fileExists("../../ressources/some_imaginary_file"), false);
}

void TestUtil::testFileUtil_getFile()
{
    QCOMPARE(getFile({"../../ressources/"}, "fake_zip.zip"),
                                std::string("../../ressources/fake_zip.zip"));
}

void TestUtil::testFileUtil_getDirContent()
{
    std::vector<std::string> content;
    getDirContent("some/imaginary/dir", content);
    QCOMPARE(content.size(), std::vector<std::string>::size_type(0));
    content.clear();
    getDirContent("../../../models/", content);

    // Keep this list up to date.
    QCOMPARE(find(content.begin(), content.end(), "hmcmodel.csv")
                                                     != content.end(),
             true);
    QCOMPARE(find(content.begin(), content.end(), "hmcoperators.csv")
                                                     != content.end(),
             true);
    QCOMPARE(find(content.begin(), content.end(), "mkvmodel.xml")
                                                     != content.end(),
             true);
}

QTEST_APPLESS_MAIN(TestUtil)
