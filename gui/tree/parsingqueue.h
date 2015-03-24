#ifndef PARSINGQUEUE_H
#define PARSINGQUEUE_H

#include <QObject>
#include <QQueue>

class ParsingThread;
class Object;

class ParsingQueue : public QObject
{
    Q_OBJECT
public:
    explicit ParsingQueue(QObject* parent);
    
    ParsingThread* addObjectParsing(Object& object, const QModelIndex &index, unsigned int nominalCount, unsigned int minCount, unsigned int maxTries);

private:
    QQueue<ParsingQueue*> _queue;
};

#endif // PARSINGQUEUE_H
