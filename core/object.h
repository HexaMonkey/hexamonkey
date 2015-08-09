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

#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>

#include "core/file/realfile.h"
#include "core/objecttype.h"
#include "core/variant.h"
#include "core/util/strutil.h"
#include "core/variable/variable.h"

class Parser;
class ObjectContext;
class ObjectAttributes;

/** @brief Node of the tree structure associated with a \link File file\endlink
 *
 * The object correspond to a contiguous memory area in the \link file() file\endlink represented
 * by a \link beginningPos() beginning position\endlink and a \link size() size\endlink.
 *
 * It is given an \link type() object type\endlink and a \link name() name\endlink,
 * a \link Module module\endlink can then use the \link type() object type\endlink to associate a series
 * of \link Parser parsers\endlink that will extract all the information from the \link file() file\endlink
 * to construct the object.
 *
 * It is part of a tree structure, it can threfore have a \link parent() parent\endlink and be subdivided
 * into children. The children can be access through iteration of the object or by using access functions.
 * It can also have a \link value() value\endlink.
 */
class Object
{
    public:
        enum Endianness {
            bigEndian = 0,
            littleEndian = 1
        };

        typedef std::vector<Object*> container;
        typedef container::iterator iterator;
        typedef container::const_iterator const_iterator;
        typedef container::reverse_iterator reverse_iterator;
        typedef container::const_reverse_iterator const_reverse_iterator;

        typedef std::unordered_map<std::string, Variable> contextContainer;
        typedef contextContainer::iterator contextIterator;
        typedef contextContainer::const_iterator const_contextIterator;

        friend class Parsing;
        /**
         * @brief RAII object used to lock parsing to avoid reentrency
         */
        class Parsing
        {
        public:
            Parsing(Object& object);
            ~Parsing();
            bool isAvailable() const;
        private:
            Object& _object;
            bool _isAvailable;
        };

        ~Object();

        /** @brief Access the file associated. */
        File& file();

        /** @brief Access the file associated. */
        const File& file() const;

        /** @brief Access the begining file position of the object. */
        std::streampos beginningPos() const;

        /** @brief Access the size occupied by the object in the \link file() file\endlink.
         *
         *  If the size is yet unknown then -1 will be returned.
         */
        std::streamoff size() const;
        void setSize(std::streamoff size);
        bool isSetToExpandOnAddition() const;
        void setToExpandOnAddition();

        /** @brief Access the current position of the \link file() file\endlink relative to
         *  the \link beginningPos beginning position\endlink of the \link file() file\endlink.
         */
        std::streamoff pos() const;
        bool includesPos(std::streamoff pos) const;
        void setPos(std::streamoff pos);

        /** @brief Move the \link file() file\endlink current position to the \link beginningPos()
         *  beginning position\endlink of the object.
         */
        void seekBeginning();

        /** @brief Move the \link file() file\endlink current position to the ending position of the
         *  object.
         *
         *  The \link size() size \endlink should be known to use this function.
         */
        void seekEnd();

        /** @brief Move the \link file() file\endlink current position to the ending position of the
         *  last child.
         */
        void seekObjectEnd();

        /**
         * @brief Use the parsers to add the children of the object
         * @param depth is the recursive depth of the exploration. If it is set to -1 the exploration won't stop until all is explored.
         */
        void explore(int depth = 1);

        /**
         * @brief Use the parsers to add a limited number of children of the object
         * @param hint is the desired number of children to add. The actual number of children added depends on the parser's implementation.
         * @return true if the exploration is done
         */
        bool exploreSome(int hint);

        /**
         * @brief Add a parser at the end of the parser list
         *
         * Parsers are executed in the order they have been added. If the parser have a head (i.e. a part that must be parsed as soon as possible,
         * then every parsers added previously execute fully their parsing and then the new parser parses the head of its parsing
         */
        void addParser(Parser* parser);

        /**
         * @brief Check if the parsing is done
         */
        bool parsed();

        /**
         * @brief Type used by the module to generate parsers for the object
         */
        const ObjectType &type() const;
        ObjectType &type();
        void setType(const ObjectType& type);

        /**
         * @brief Name
         */
        const std::string &name() const;
        void setName(const std::string& name);

        /**
         * @brief Value of the object set during parsing
         */
        const Variant& value() const;
        Variant& value();
        void setValue(const Variant& value);

        /**
         * @brief Object links to a point in file
         */
        bool hasLinkTo() const;
        std::streamoff linkTo() const;
        void setLinkTo(std::streamoff linkTo);
        void removeLinkTo();

        /**
         * @brief Attributes values
         */
        ObjectAttributes* attributes(bool createIfNeeded = false);
        const ObjectAttributes* attributes() const;

        /**
         * @brief Context
         */
        ObjectContext* context(bool createIfNeeded = false);
        const ObjectContext* context() const;

        /**
         * @brief Standard representation for the object in an out stream
         */
        std::ostream& display(std::ostream& out, std::string prefix = "") const;

        /**
         * @brief Standard representation for the object in an out stream
         */
        std::ostream& displayTree(std::ostream& out, std::string prefix = "") const;

        /**
         * @brief Pointer to the object parent returns nullptr if the object is top level
         */
        Object* parent();
        const Object* parent() const;

        /**
         * @brief Reference to the root parent
         */
        Object& root();
        const Object& root() const;

        /**
         * @brief Index of the object as a child of its parent
         */
        int64_t rank() const;

        /**
         * @brief Number of children
         */
        int numberOfChildren() const;

        /**
         * @brief Iterator pointing to the beginning of the children container
         */
        iterator begin();
        /**
         * @brief Iterator pointing to the end of the children container
         */
        iterator end();
        /**
         * @brief Iterator pointing to the last member of the children container
         */
        iterator last();

        const_iterator begin() const;
        const_iterator end() const;
        const_iterator last() const;

        reverse_iterator rbegin();
        reverse_iterator rend();
        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;

        /**
         * @brief Access a child by its index
         *
         * If the index is too large, the parsing is not done and forceParse is set then
         * the object will be parsed progressively until the index is reached or the parsing is done
         */
        Object* access(int64_t index, bool forceParse = false);

        /**
         * @brief Access a child by its name
         *
         * If conflict returns the last parsed. If the name is not found, the parsing is not
         * done and forceParse is set then the object will be parsed progressively until the
         * name is found or the parsing is done.
         */
        Object* lookUp(const std::string& name, bool forceParse = false);

        /**
         * @brief Access a child by its type
         *
         * Return the first object whose type extends directly the type given. If the type is not found,
         * the parsing is not done and forceParse is set then the object will be parsed progressively
         * until the type is found or the parsing is done.
         */
        Object* lookForType(const ObjectType& type, bool forceParse = false);

        void dump(std::ostream &outStream) const;

        void dumpToFile(const std::string& path) const;

        bool hasStream() const;

        void dumpStream(std::ostream &outStream);

        void dumpStreamToFile(const std::string& path);


        const Variable& variable();
        const Variable& contextVariable(bool createIfNeeded = false);
        const Variable& attributesVariable(bool createIfNeeded = false);

        Variable parserVariable(Parser* parser);
        Variable parserTypeScope(Parser* parser);
        Variable parserVariableToBeAdded(Parser* parser);
        Variable parserTypeScopeToBeAdded(Parser* parser);

        bool isValid() const;
        void invalidate();

        Endianness endianness() const;
        void setEndianness(const Endianness &endianness);

        inline VariableCollector& collector() {
            return _collector;
        }

        inline const VariableCollector& collector() const {
            return _collector;
        }

    private:
        friend class Module;
        friend class ContainerParser;

        Object(File& file, std::streampos beginningPos, Object* parent, VariableCollector& collector);

        void parse();
        void parseBody();
        bool parseSome(int hint);
        void parseTail();

        File& _file;
        std::streampos _beginningPos;
        std::streamoff _size;
        std::streamoff _contentSize;
        std::streamoff _pos;

        Object* _parent;
        Object* _lastChild;
        Variant _rank;

        ObjectType _type;
        std::string _name;
        Variant _value;
        Variant _linkTo;

        container _children;
        std::vector<std::unique_ptr<Object> > _ownedChildren;
        std::unordered_map<std::string, Object*> _lookUpTable;

        std::vector<std::shared_ptr<Parser> > _parsers;
        bool _expandOnAddition;

        size_t _parsedCount;
        bool _parsingInProgress;

        Variable _variable;

        ObjectContext* _context;
        Variable _contextVariable;

        ObjectAttributes* _attributes;
        Variable _attributesVariable;

        bool _valid;

        Endianness _endianness;

        std::shared_ptr<Parser> _toBeAddedParser;

        VariableCollector& _collector;

        //Non copyable
        Object& operator =(const Object&) = delete;
        Object(const Object&) = delete;
};

std::ostream& operator <<(std::ostream& out, const Object& object);

#endif // OBJECT_H_INCLUDED
