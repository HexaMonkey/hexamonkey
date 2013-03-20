#include "containerparser.h"

#include "module.h"

ContainerParser::ContainerParser(Object &object, const Module &module)
    : Parser(object),
      _module(module),
      _autogrow(false)

{
}

void ContainerParser::addChild(Object *child)
{
    if(child != nullptr && child->size() != 0)
    {
        if(child->size() == -1)
        {
            if(_object.size() != -1 && child->_expandOnAddition)
            {
                child->_size.setValue(_object.size()-_object.pos());
            }
            else
            {
                child->parse();
                child->_size.setValue(child->_contentSize);
            }
        }


        int64_t newSize = _object._contentSize + child->size();
        if(_autogrow && newSize > _object._size)
        {
            _object._size = newSize;
        }

        if(newSize <= _object.size() || _object.size() == -1)
        {
            _object._contentSize = newSize;
            if(!child->name().empty())
            {
                _object._lookUpTable[child->name()] = child;
            }
            child->_parent = &_object;
            child->seekEnd();

            _object._children.push_back(child);
            _object._ownedChildren.push_back(std::unique_ptr<Object>(child));
            child->_rank = _object._children.size() - 1;
        }
        else
        {
            _object.seekEnd();
            std::cerr<<"too big "<<child->type()<<" "<<child->name()<<std::endl;
        }
    }
}

void ContainerParser::addChild(Object *child, const std::string &name)
{
    if(child != nullptr)
    {
        child->setName(name);
        addChild(child);
    }
    else
        std::cerr<<"child is NULL"<<std::endl;
}

Object *ContainerParser::getVariable(const ObjectType &type)
{
    return _module.handle(type, file(), &_object);
}

Object *ContainerParser::addVariable(const ObjectType &type)
{
    Object* child = getVariable(type);
    addChild(child);
    return child;
}

Object *ContainerParser::addVariable(const ObjectType &type, const std::string &name)
{
    _object.seekObjectEnd();
    Object* child = getVariable(type);
    addChild(child, name);
    return child;
}

void ContainerParser::addLoop(const std::function<void (ContainerParser &)> &body, const std::string &name)
{
    addLoop([](){return true;}, body, name);
}

void ContainerParser::addLoop(const std::function<bool ()> &guard, const std::function<void (ContainerParser &)> &body, const std::string &name)
{
    Object* loop = new Object(object().file());
    ContainerParser* parser = new ContainerParser(*loop, module());
    parser->setAutogrow();
    loop->addParser(static_cast<Parser*>(parser));
    while(parser->availableSize() && guard())
    {
        body(*parser);
    }
    addChild(loop, name);
}

void ContainerParser::addForLoop(int64_t max, const std::function<void (ContainerParser &, int64_t)> &body, const std::string &name)
{
    Object* loop = new Object(object().file());
    ContainerParser* parser = new ContainerParser(*loop, module());
    parser->setAutogrow();
    loop->addParser(static_cast<Parser*>(parser));
    for(int64_t i = 0;availableSize() && i < max; ++i)
    {
        body(*parser, i);
    }
    addChild(loop, name);
}

const Module &ContainerParser::module() const
{
    return _module;
}

ContainerParser &ContainerParser::containerParser(Object &object)
{
    return static_cast<ContainerParser&>(*object._parsers.back());
}


void ContainerParser::setAutogrow()
{
    _autogrow = true;
    if(object().size()<0)
        setSize(0);
}
