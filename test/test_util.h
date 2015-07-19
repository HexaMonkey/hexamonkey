#ifndef TEST_UTIL
#define TEST_UTIL

#include <QtTest/QtTest>

class TestUtil : public QObject
{
    Q_OBJECT
private slots:
    void testCSVReader();
    void testBitUtil_msb();
    void testBitUtil_lsbMask();
    void testBitUtil_popCount();
    void testFileUtil_fileExists();
    void testFileUtil_getFile();
    void testFileUtil_getDirContent();
    void testFileUtil_fileCompare();
    void testStrUtil_strTo();
    void testStrUtil_toStr();
    void testStrUtil_toHex();
    void testStrUtil_intDisplay();
    void testStrUtil_fromHex();
    void testStrUtil_formatDate();
    void testStrUtil_formatDuration();
    void testStrUtil_extension();
    void testStrUtil_sizeDisplay();
    void testStrUtil_defineStyle();
    void testOptOwnPtr();
    void testIterationWrapper();
};

#endif // TEST_UTIL
