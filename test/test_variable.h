#ifndef TEST_VARIABLE_H
#define TEST_VARIABLE_H

#include <QtTest/QtTest>

class TestVariable : public QObject
{
    Q_OBJECT
private slots:
    void testCollector();
    
};

#endif // TEST_VARIABLE_H
