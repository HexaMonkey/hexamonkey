#ifndef TEST_FORMATDETECTOR
#define TEST_FORMATDETECTOR

#include <QtTest/QtTest>

class TestFormatDetector : public QObject
{
    Q_OBJECT
private slots:
    void testExtensionFormatDetector();
    void testMagicFormatDetector();
    void testSyncbyteFormatDetector();
    void testCompositeFormatDetector();
    void testStandardFormatDetector();
};

#endif // TEST_FORMATDETECTOR