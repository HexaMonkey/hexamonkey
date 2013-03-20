#ifndef TASK_H
#define TASK_H

#include <QModelIndex>
#include <QObject>

#include "object.h"


class ParsingTask
{

public:
    ParsingTask(Object& object,  QModelIndex index, int hint = 1 );


    Object& object();

    QModelIndex index();

    int hint();

private:
    Object& _object;
    QModelIndex _index;
    int _hint;
};

#endif // TASK_H
