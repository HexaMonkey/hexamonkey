
#include <sstream>

#include "test_util.h"
#include "core/util/bitutil.h"
#include "core/util/csvreader.h"
#include "core/util/fileutil.h"
#include "core/util/iterutil.h"
#include "core/util/ptrutil.h"
#include "core/util/strutil.h"

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
    QCOMPARE(fileExists("resources/fake_zip.zip"), true);
    QCOMPARE(fileExists("resources/some_imaginary_file"), false);
}

void TestUtil::testFileUtil_getFile()
{
    QCOMPARE(getFile({"resources/"}, "fake_zip.zip"),
                                std::string("resources/fake_zip.zip"));
}

void TestUtil::testFileUtil_getDirContent()
{
    std::vector<std::string> content;
    getDirContent("some/imaginary/dir", content);
    QCOMPARE(content.size(), std::vector<std::string>::size_type(0));
    content.clear();
    getDirContent("../models/", content);

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

void TestUtil::testFileUtil_fileCompare()
{
    QVERIFY( fileCompare("resources/util/file_compare.orig.txt", "resources/util/file_compare.same.txt"));
    QVERIFY(!fileCompare("resources/util/file_compare.orig.txt", "resources/util/file_compare.different.txt"));
    QVERIFY(!fileCompare("resources/util/file_compare.orig.txt", "resources/util/file_compare.longer.txt"));
    QVERIFY(!fileCompare("resources/util/file_compare.orig.txt", "resources/util/file_compare.shorter.txt"));
}

void TestUtil::testStrUtil_strTo()
{
    QCOMPARE(strTo<int>(std::string("10.0")), int(10));
    QCOMPARE(strTo<int>(std::string("-23.1")), int(-23));
    QCOMPARE(strTo<char>(std::string("c")), char('c'));
}

void TestUtil::testStrUtil_toStr()
{
    QCOMPARE(toStr<int>(125), std::string("125"));
    QCOMPARE(toStr<char>('b'), std::string("b"));
    QCOMPARE(toStr<int>(-23), std::string("-23"));
}

void TestUtil::testStrUtil_toHex()
{
    // maybe unwanted behaviour for char ?
    QCOMPARE(toHex<char>('c'), std::string("c"));

    QCOMPARE(toHex<int>(20), std::string("14"));
}

void TestUtil::testStrUtil_intDisplay()
{
    QCOMPARE(intDisplay<int>(12345, 10), std::string("12345"));
    QCOMPARE(intDisplay<int>(12345, 8), std::string("030071"));
}

void TestUtil::testStrUtil_fromHex()
{
    QCOMPARE(fromHex('3'), int(3));
    QCOMPARE(fromHex('c'), int(12));
    QCOMPARE(fromHex(std::string("10")), uint64_t(16));
    QCOMPARE(fromHex(std::string("12345")), uint64_t(74565));
}

void TestUtil::testStrUtil_formatDate()
{
    QCOMPARE(formatDate(0), std::string("01-01-1970 00:00:00 GMT"));
    QCOMPARE(formatDate(1385306654), std::string("24-11-2013 15:24:14 GMT"));
}

void TestUtil::testStrUtil_formatDuration()
{
    QCOMPARE(formatDuration(2), std::string("2s"));
    QCOMPARE(formatDuration(10000), std::string("2h46m40s"));
}

void TestUtil::testStrUtil_extension()
{
    QCOMPARE(extension("example.avi"), std::string("avi"));
    QCOMPARE(extension("example2.test.avi"), std::string("avi"));
}

void TestUtil::testStrUtil_sizeDisplay()
{
    QCOMPARE(sizeDisplay(0), std::string("0 byte"));
    QCOMPARE(sizeDisplay(8), std::string("1 byte"));
    QCOMPARE(sizeDisplay(10), std::string("10 bits"));
    QCOMPARE(sizeDisplay(2048*8), std::string("2 kB"));
    QCOMPARE(sizeDisplay(1234567890), std::string("147.2 MB"));
    QCOMPARE(sizeDisplay(1234567890000), std::string("143.7 GB"));
}

void TestUtil::testStrUtil_defineStyle()
{
    QCOMPARE(defineStyle("thisShouldWorkJustFine"),
             std::string("THIS_SHOULD_WORK_JUST_FINE"));
}

void TestUtil::testStrUtil_splitByChar()
{
    {
        const std::vector<std::string> test = splitByChar("#aa#bbb##c#", '#');
        const std::vector<std::string> result = {"", "aa", "bbb", "", "c", ""};
        QCOMPARE(test, result);
    }

    {
        const std::vector<std::string> test = splitByChar("aabbbc", 'e');
        const std::vector<std::string> result = {"aabbbc"};
        QCOMPARE(test, result);
    }

    {
        const std::vector<std::string> result = {""};
        QCOMPARE(splitByChar("", 'a'), result);
    }
}

void TestUtil::testStrUtil_join()
{
    {
        const std::vector<std::string> input = {"", "aa", "bbb", "", "c", ""};

        const std::string test1 = join(input);
        const std::string test2 = join(input, "#");
        const std::string test3 = join(input, "dig");

        QCOMPARE(test1, std::string("aabbbc"));
        QCOMPARE(test2, std::string("#aa#bbb##c#"));
        QCOMPARE(test3, std::string("digaadigbbbdigdigcdig"));
    }

    QCOMPARE(join(std::vector<std::string>()), std::string());
    QCOMPARE(join(std::vector<std::string>(), "dig"), std::string());
}

void TestUtil::testOptOwnPtr()
{
    // since no operation is done on T, using an int
    OptOwnPtr<int> ptr;
    QCOMPARE(ptr.get() == nullptr, true);
    QCOMPARE(ptr == false, true); // bool() operator

    OptOwnPtr<int> ptr2(OptOwnPtr<int>::move(new int(3)));
    QCOMPARE(ptr2 == true, true);
    QCOMPARE(*ptr2, 3);
    *ptr2 += 2;
    QCOMPARE(*ptr2, 5);

    int val3 = 12;
    OptOwnPtr<int> ptr3(OptOwnPtr<int>::ref(val3));
    QCOMPARE(ptr3 == true, true);
    QCOMPARE(ptr3.get(), &val3);
    QCOMPARE(*ptr3, val3);
    ++val3;
    QCOMPARE(*ptr3, val3);
}

void TestUtil::testIterationWrapper()
{
    std::vector<std::string> strings({"a", "b", "c", "d"});
    auto str_it = reverse(strings).begin();
    QCOMPARE(*str_it, std::string("d"));
    QCOMPARE(*(++str_it), std::string("c"));
    QCOMPARE(*(++str_it), std::string("b"));
    QCOMPARE(*(++str_it), std::string("a"));
    QCOMPARE(++str_it, reverse(strings).end());
}
