#include "functionthread.h"

FunctionThread::FunctionThread(QObject *parent, std::function<void()> functionToRun) :
    QThread(parent),
    functionToRun(functionToRun)
{
}

void FunctionThread::run()
{
    functionToRun();
}
