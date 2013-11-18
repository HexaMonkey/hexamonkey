#include <QTest>
#include "test_formatdetector.h"

int main()
{
    int error_count = 0;
    TestFormatDetector testFormatDetector;
    error_count += QTest::qExec(&testFormatDetector);
    return (error_count > 0 ? 1 : 0);
}
