#ifndef PARSINGQUEUE_H
#define PARSINGQUEUE_H

#include <QObject>
#include <QQueue>
#include <QModelIndex>

#include <functional>

class QThread;
class Object;

class ThreadQueue : public QObject
{
    Q_OBJECT
public:
    explicit ThreadQueue(QObject* parent);

    void add(QThread* thread, const std::function<void (int)> &registerId);
    void add(std::function<void ()> functionToRun, const std::function<void (int)> &registerId);

public slots:
    void onThreadFinished();

signals:
    void started(int);
    void finished(int);

private:

    void onQueueUpdated();

    QQueue<std::pair<int, QThread*> > _threadQueue;
    QThread* _currentThread;
    int _currentId;
    int _freeId;
};

#endif // PARSINGQUEUE_H
