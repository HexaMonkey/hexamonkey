#ifndef TEST_UTIL
#define TEST_UTIL

#include <QtTest/QtTest>

class TestUtil : public QObject
{
    Q_OBJECT
private slots:
    void testCSVReader();
};

#endif // TEST_UTIL