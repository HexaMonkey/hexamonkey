
#include <QTest>
#include <vector>

#include "test_variant.h"
#include "test_formatdetector.h"
#include "test_util.h"

#include "core/util/strutil.h"

int main(int argc, char** argv)
{
    TestVariant testVariant;
    TestFormatDetector testFormatDetector;
    TestUtil testUtil;

    std::vector<QObject*> testList = {&testVariant, &testFormatDetector, &testUtil};

    int errorCount = 0;
    for (QObject* test : testList) {
        errorCount += QTest::qExec(test, argc, argv);
        qDebug(" ");
    }

    qDebug(concat("Totals: ", (testList.size() - errorCount), " passed, ", errorCount, " failed").c_str());
    return (errorCount > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
