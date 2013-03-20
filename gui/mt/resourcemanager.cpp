#include "resourcemanager.h"
#include "resource.h"

ResourceManager::ResourceManager(TreeModel &model) : _model(model)
{
}

void ResourceManager::lock(Object &object)
{
    File* file = &object.file();
    QMap<File*, Resource*>::iterator it = _map.find(file);
    if (it != _map.end())
    {
        it.value()->lock();
    }
}

void ResourceManager::unlock(Object &object)
{
    File* file = &object.file();
    QMap<File*, Resource*>::iterator it = _map.find(file);
    if (it != _map.end())
    {
        it.value()->unlock();
    }
}

void ResourceManager::insert(File &file)
{
    QMap<File*, Resource*>::iterator it = _map.find(&file);
    if (it == _map.end())
    {
        Resource* resource = new Resource(_model);
        _map.insert(&file, resource);
    }
}

Resource &ResourceManager::resource(Object &object)
{
    File* file = &object.file();
    QMap<File*, Resource*>::iterator it = _map.find(file);
    if (it != _map.end())
    {
        return *(it.value());
    }
    else
    {
        insert(object.file());
        return resource(object);
    }
}

