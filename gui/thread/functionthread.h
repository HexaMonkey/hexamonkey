#ifndef FUNCTIONTHREAD_H
#define FUNCTIONTHREAD_H

#include <QThread>

#include <functional>

class FunctionThread : public QThread
{
    Q_OBJECT
public:
    explicit FunctionThread(QObject *parent, std::function<void()> functionToRun);

    virtual void run() final;
    
private:
    std::function<void()> functionToRun;
    
};

#endif // FUNCTIONTHREAD_H
