#ifndef PARSINGTHREAD_H
#define PARSINGTHREAD_H

#include <QThread>
#include <QModelIndex>

class Object;
class ParsingQueue;

class ParsingThread : public QThread
{
    Q_OBJECT
public:
    const QModelIndex& index() const;

    explicit ParsingThread(ParsingQueue *parent,
                           Object& object,
                           const QModelIndex &index,
                           unsigned int nominalCount,
                           unsigned int minCount,
                           unsigned int maxTries);

    virtual void run() final;

private:
    Object& _object;

    QModelIndex _index;
    unsigned int _nominalCount;
    unsigned int _minCount;
    unsigned int _maxTries;
};

#endif // PARSINGTHREAD_H
