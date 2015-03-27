#ifndef PARSINGQUEUE_H
#define PARSINGQUEUE_H

#include <QObject>
#include <QQueue>
#include <QModelIndex>

#include <functional>

class QThread;
class Object;

class ParsingQueue : public QObject
{
    Q_OBJECT
public:
    explicit ParsingQueue(QObject* parent);

    void addObjectParsing(Object& object, unsigned int nominalCount, unsigned int minCount, unsigned int maxTries, const std::function<void(int)>& registerId);

public slots:
    void onThreadFinished();

signals:
    void started(int);
    void finished(int);

private:
    void addThread(QThread* thread, const std::function<void (int)> &registerId);
    void onQueueUpdated();

    QQueue<std::pair<int, QThread*> > _threadQueue;
    QThread* _currentThread;
    int _currentId;
    int _freeId;
};

#endif // PARSINGQUEUE_H
