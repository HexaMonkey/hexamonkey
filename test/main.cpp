
#include <QTest>
#include "test_variant.h"
#include "test_formatdetector.h"
#include "test_util.h"
#include "core/util/strutil.h"

int main()
{
    TestVariant testVariant;
    TestFormatDetector testFormatDetector;
    TestUtil testUtil;
    int error_count =   QTest::qExec(&testVariant)
                      + QTest::qExec(&testFormatDetector)
                      + QTest::qExec(&testUtil);
    qDebug(" ");
    qDebug(concat("Totals: ", (3 - error_count), " passed, ", error_count, " failed").c_str());
    return (error_count > 0 ? 1 : 0);
}
