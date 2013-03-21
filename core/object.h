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

#include "file.h"
#include "strutil.h"
#include "variant.h"
#include "objecttype.h"
#include "showcase.h"

class Parser;

/*!
 * \brief The Object class
 */
class Object
{
    public:
        typedef std::vector<Object*> container;
        typedef container::iterator iterator;
        typedef container::const_iterator const_iterator;
        typedef container::reverse_iterator reverse_iterator;
        typedef container::const_reverse_iterator const_reverse_iterator;

        Object(File& file);

        iterator begin();
        iterator end();
        iterator last();
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator last() const;
        reverse_iterator rbegin();
        reverse_iterator rend();
        const_reverse_iterator rbegin() const;
        const_reverse_iterator rend() const;


        int numberOfChildren() const;

        Object* access(int64_t index, bool forceParse = false);
        Object* lookUp(const std::string& name, bool forceParse = false);
        Object* lookForType(const ObjectType& type, bool forceParse =false);

        std::streamoff pos() const;

        void seekBeginning();
        void seekEnd();
        void seekObjectEnd();

        void explore(int level = 1);
        bool exploreSome(int hint);

        void setType(const ObjectType& type);
        void setName(const std::string& name);
        void setInfo(const std::string& info);
        Showcase& showcase();

        const ObjectType &type() const;
        const std::string &name() const;
        const std::string &info() const;
        const Showcase &showcase() const;

        std::ostream& display(std::ostream& out, std::string prefix = "") const;

        bool parsed();

        Variant& value();
        const Variant& value() const;

        File& file();
        std::streampos beginningPos() const;
        std::streamoff size() const;

        Object* parent();
        const Object* parent() const;
        int64_t rank() const;

        void clear();

        void addParser(Parser* parser);

    private:
        friend class Parser;
        friend class SimpleParser;
        friend class ContainerParser;

        friend class Module;

        friend class ConstObjectScope;
        friend class MutableObjectScope;

        void parse();
        bool parseSome(int hint);

        File& _file;
        std::streampos _beginningPos;
        Variant _size;
        std::streamoff _contentSize;

        Object* _parent;
        Variant _rank;

        Variant _type;
        Variant _name;
        Variant _info;
        Variant _value;
        Showcase _showcase;

        container _children;
        std::vector<std::unique_ptr<Object> > _ownedChildren;
        std::map<std::string, Object*> _lookUpTable;


        std::vector<std::unique_ptr<Parser> > _parsers;
        bool _expandOnAddition;

        bool _parsingInProgress;

        //Non copyable
        Object& operator =(const Object&) = delete;
        Object(const Object&) = delete;
};

std::ostream& operator <<(std::ostream& out, const Object& object);

#endif // OBJECT_H_INCLUDED
