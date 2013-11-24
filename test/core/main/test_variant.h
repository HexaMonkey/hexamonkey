#ifndef TEST_VARIANT
#define TEST_VARIANT

#include <QtTest/QtTest>

class TestVariant : public QObject
{
    Q_OBJECT
private slots:
    void unknown();
    void boolean();
    void integer();
    void unsignedInteger();
    void floating();
    void string();
    void objectType();
    void conversion();
};

#endif // TEST_VARIANT