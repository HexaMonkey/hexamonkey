
#include <QTest>
#include <vector>
#include <ostream>

#include "test_variant.h"
#include "test_formatdetector.h"
#include "test_util.h"
#include "test_variable.h"
#include "test_parser.h"

#include "core/util/strutil.h"

#include "core/log/streamlogger.h"

int main(int argc, char** argv)
{
    std::ofstream logFile;
    logFile.open("test.txt");
    StreamLogger streamLogger(logFile);
    TestVariant testVariant;
    TestFormatDetector testFormatDetector;
    TestUtil testUtil;
    TestVariable testVariable;
    TestParser testParser;

    std::vector<QObject*> testList = {&testVariant, &testFormatDetector, &testUtil, &testVariable, &testParser};

    int errorCount = 0;
    for (QObject* test : testList) {
        errorCount += QTest::qExec(test, argc, argv) ? 1 : 0;
        qDebug(" ");
    }

    qDebug(concat("Totals: ", (testList.size() - errorCount), " passed, ", errorCount, " failed").c_str());
    return (errorCount > 0 ? EXIT_FAILURE : EXIT_SUCCESS);
}
