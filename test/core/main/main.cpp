
#include <QTest>
#include "test_variant.h"

int main()
{
    int error_count = 0;
    TestVariant testVariant;
    error_count += QTest::qExec(&testVariant);
    return (error_count > 0 ? 1 : 0);
}
