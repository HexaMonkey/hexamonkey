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

class ParsingException;

#include "core/object.h"

/**
 * @brief Parse a bit of \link File file\endlink into an \link Object object\endlink
 *
 * The parsing is done in two phases : first the head is parse, which correspond to the
 * operations that must be executed as soon as possible which donne with parseHead
 * and then the rest can be parsed whenever either full with parse or little by little
 * with parseSome. For a \link Parser parser\endlink that is always parsed fully as soon
 * as possible subclass SimpleParser for convenience.
 *
 * The \link Parser parser\endlink can modify most of the \link Object object\endlink's
 * attributes, however it cannot add children : for this functionnality subclass
 * ContainerParser.
 *
 * The class is virtual and needs to be reimplemented
 */
class Parser
{
public:
    virtual ~Parser(){}

    /**
     * @brief Parse what must be parsed as soon as possible
     */
    virtual void parseHead();

    /**
     * @brief Parse everything except the tail
     */
    virtual void parse();

    /**
     * @brief Parse the amount indicated by the hint
     */
    virtual bool parseSome(int hint);

    /**
     * @brief parse the tail
     */
    virtual void parseTail();

    /**
     * @brief Check if the head has been parsed
     */
    bool headParsed() const;

    /**
     * @brief Check if the parsing is done except the tail
     */
    bool parsed() const;

    /**
     * @brief Check if the tail has been parsed
     */
    bool tailParsed() const;

    /**
     * @brief Get the \link Object object\endlink manipulated by the \link Parser parser\endlink
     */
    Object &object();
    const Object &object() const;

    /**
     * @brief Get the \link Object::type type\endlink of the \link Object object\endlink
     */
    ObjectType *modifiableType();
    const ObjectType& constType() const;

protected:
    Parser(Object& object);

    /**
     * @brief Get the number of bits between the current position in the \link File file\endlink and
     * the end of the \link Object object\endlink
     */
    int64_t availableSize() const;

    /**
     * @brief Mark the head as parsed
     */
    void setHeadParsed();

    /**
     * @brief Mark the parser as done
     */
    void setParsed();

    /**
     * @brief Mark the parser as headless, meaning that no operation have to be executed as soon as possible
     */
    void setNoHead();

    /**
     * @brief Mark the parser as tailless
     */
    void setNoTail();


    /**
     * @brief [Virtual] Parse what must be parsed as soon as possible (do nothing by default)
     */
    virtual void doParseHead();

    /**
     * @brief [Virtual] Parse everything remaining except the tail, assuming that the head and only the head as been parsed (do nothing by default)
     */
    virtual void doParse();

    /**
     * @brief [Virtual] Parse the amount indicated by the hint (if not reimplemented calls doParse)
     * @param hint The exact meaning of the hint is to be defined by the subclass implementing the function, for most
     * \link ContainerParser container parsers\endlink it simply is the number of children to be added. However the
     * only requirement is that for any hint>0, "while(doParseSome(hint)){}" finishes.
     * @return true if the parsing is done false otherwise
     */
    virtual bool doParseSome(int hint);

    /**
     * @brief [Virtual] Parse the tail
     *
     * The tail will be parsed after every subsequent parsers have been parsed including their tails
     */
    virtual void doParseTail();


private:
    friend class SimpleParser;
    friend class ContainerParser;
    friend class BlockExecution;
    friend class ParserTypeScope;

    Object& _object;
    std::unique_ptr<ObjectType> _type;

    bool lockObject();
    void unlockObject();

    void handleParsingException(const ParsingException& exception);

    bool _headParsed;
    bool _parsed;
    bool _tailParsed;
    bool _hasHead;
    bool _hasTail;
};

/**
 * @brief Parser that is that is always parsed fully as soon
 * as possible.
 *
 * Only doParseHead needs to be reimplemented.
 */
class SimpleParser : public Parser
{
public:
    virtual ~SimpleParser() {}

    void parseHead() override;

protected:
    SimpleParser(Object& object);

private:
    void doParse() override;
    bool doParseSome(int hint) override;
};

#endif // PARSER_H
