//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

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


protected:
    virtual void doParseHead();
    virtual void doParse();
    virtual bool doParseSome(int hint);
    virtual void cleanUp();


private:
    friend class SimpleParser;
    friend class ContainerParser;
    friend class ParsingInProgress;

    Object& _object;

    bool lockObject();
    void unlockObject();

    bool _headParsed;
    bool _parsed;

    class Parsing
    {
    private:
        Parser& parser;
        bool hasLock;
    public:
        Parsing(Parser& parser) : parser(parser), hasLock(parser.lockObject()){}
        ~Parsing() {if(hasLock) parser.unlockObject();}
        bool available() {return hasLock;}
    };

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
