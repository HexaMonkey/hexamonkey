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
};

#endif // TEST_UTIL