#ifndef RESURCEMANAGER_H
#define RESURCEMANAGER_H

#include <QMap>
#include <QMutex>
#include <QObject>
#include "object.h"

class Resource;
class TreeModel;

class ResourceManager: public QObject {
    Q_OBJECT
public:
    ResourceManager(TreeModel& model);

    void lock(Object& object);

    void unlock(Object& object);

    void insert(File& file);

    Resource& resource(Object& object);

private:
    QMap<File*,Resource*> _map;
    TreeModel& _model;

};

#endif // RESURCEMANAGER_H
