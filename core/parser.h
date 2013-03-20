#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "object.h"

/*!
 * \class Parser
 * \brief The Parser class
 *
 * This class is used to parse the files.
 *
 */
class Parser
{
public:
    Parser(Object& object);
    virtual ~Parser(){}
    virtual void parseHead();
    virtual void parse();
    virtual bool parseSome(int hint);

    bool parsed();

protected:
    virtual void doParseHead();
    virtual void doParse();
    virtual bool doParseSome(int hint);

    ObjectType& type();
    void setSize(int64_t size);
    void setName(const std::string& name);
    void setInfo(const std::string& info);
    void setValue(const Variant& value);
    Showcase& showcase();
    File& file();

    int64_t availableSize() const;
    void setExpandOnAddition();

    Object* access(int64_t index, bool forceParse = false);
    Object* lookUp(const std::string &name, bool forceParse = false);
    Object* lookForType(const ObjectType& type, bool forceParse = false);

    Object &object();

private:
    friend class SimpleParser;
    friend class ContainerParser;

    Object& _object;

    bool _headParsed;
    bool _parsed;
};

class SimpleParser : public Parser
{
public:
    SimpleParser(Object& object);
    virtual ~SimpleParser() {}

    virtual void parseHead();

protected:
    void doParse();
    bool doParseSome(int hint);
};

#endif // PARSER_H
