#ifndef PARSINGQUEUE_H
#define PARSINGQUEUE_H

#include <QObject>
#include <QQueue>
#include <QModelIndex>

class ParsingThread;
class Object;

class ParsingQueue : public QObject
{
    Q_OBJECT
public:
    explicit ParsingQueue(QObject* parent);

    void addObjectParsing(Object& object, const QModelIndex &index, unsigned int nominalCount, unsigned int minCount, unsigned int maxTries);

public slots:
    void onThreadFinished();

signals:
    void started(QModelIndex);
    void finished(QModelIndex);

private:
    void onQueueUpdated();

    QQueue<ParsingThread*> _threadQueue;
    ParsingThread* _currentThread;
};

#endif // PARSINGQUEUE_H
