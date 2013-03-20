#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QModelIndex>

class Resource;

class Worker : public QObject {
    Q_OBJECT

public:
    Worker(Resource& resource);
    ~Worker();


public slots:
    void process();

signals:
    void finished();
    void error(QString err);
    void updateChildren( QModelIndex index);

private:
    Resource& _resource;

};
#endif // WORKER_H
