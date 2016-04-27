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

#include <algorithm>
#include <stdexcept>

#include "core/object.h"
#include "core/parser.h"
#include "core/log/logmanager.h"
#include "core/modules/stream/streammodule.h"
#include "core/variable/objectcontext.h"
#include "core/variable/objectattributes.h"
#include "core/variable/objectscope.h"
#include "core/variable/typescope.h"


#define BUFFER_SIZE 1048576

Object::Object(File& file, std::streampos beginningPos, Object *parent, VariableCollector &collector) :
    _file(file),
    _beginningPos(beginningPos),
    _size(-1),
    _contentSize(0),
    _pos(0),
    _parent(parent),
    _lastChild(nullptr),
    _rank(parent ? parent->numberOfChildren() : -1),
    _name("*"),
    _value(Variant::null()),
    _children(0),
    _expandOnAddition(false),
    _parsedCount(0),
    _parsingInProgress(false),
    _context(nullptr),
    _attributes(nullptr),
    _valid(true),
    _endianness(parent ? parent->_endianness : bigEndian),
    _collector(collector)
{
}


Object::iterator Object::begin()
{
    return _children.begin();
}

Object::iterator Object::end()
{
    return _children.end();
}

Object::iterator Object::last()
{
    if(_children.begin() != _children.end())
    {
        Object::iterator it = _children.end();
        --it;
        return  it;
    }
    else
        return _children.end();
}

Object::const_iterator Object::begin() const
{
    return _children.begin();
}

Object::const_iterator Object::end() const
{
    return _children.end();
}

Object::const_iterator Object::last() const
{
    if(_children.begin() != _children.end()) {
        Object::const_iterator it = _children.end();
        --it;
        return  it;
    } else {
        return _children.end();
    }
}

Object::reverse_iterator Object::rbegin()
{
    return _children.rbegin();
}

Object::reverse_iterator Object::rend()
{
    return _children.rend();
}

Object::const_reverse_iterator Object::rbegin() const
{
    return _children.rbegin();
}

Object::const_reverse_iterator Object::rend() const
{
    return _children.rend();
}

int Object::numberOfChildren() const
{
    return _children.size();
}

Object *Object::access(int64_t index, bool forceParse)
{
    if(index >=0 && index < numberOfChildren()) {
        return _children[index];
    } else if(forceParse && !parsed()) {
        int64_t pos = file().tellg();
        int n = numberOfChildren();
        exploreSome(128);
        if(n == numberOfChildren())
        {
            Log::error("Parsing locked for index ", index);
            return nullptr;
        }
        file().seekg(pos, std::ios_base::beg);
        return access(index, true);
    } else {
        Log::error("Requested variable not in range");
        return nullptr;
    }
}

Object* Object::lookUp(const std::string &name, bool forceParse)
{
    auto it = _lookUpTable.find(name);
    if (it != _lookUpTable.end()) {

        return it->second;

    } else if (forceParse && !parsed()) {

        int64_t pos = file().tellg();
        int n = numberOfChildren();
        exploreSome(128);
        if(n == numberOfChildren())
        {
            Log::error("Parsing locked for look up ", name);
            return nullptr;
        }
        file().seekg(pos, std::ios_base::beg);
        return lookUp(name, true);

    } else {

        return nullptr;
    }
}

Object* Object::lookForType(const ObjectType &targetType, bool forceParse)
{
    for (Object::iterator it = begin(); it != end(); ++it) {
        if ((*it)->type().extendsDirectly(targetType)) {
            return *it;
        }
    }

    if (forceParse && !parsed()) {
        int64_t pos = file().tellg();
        exploreSome(128);
        file().seekg(pos, std::ios_base::beg);
        return lookForType(targetType, false);
    } else {
        return nullptr;
    }
}

void Object::dump(std::ostream &out) const
{
    // make sure it is not packetised ?
    std::ifstream in  (file().path(), std::ios::in | std::ios::binary);

    if (size() == -1) {
        return;
    }

    size_t done = 0;
    size_t n = size()/8;

    in.seekg(beginningPos()/8);

    char buffer[BUFFER_SIZE];
    //Copy file part by chunks
    while(done < n) {
        size_t chunkSize = std::min<size_t>(n - done, BUFFER_SIZE);
        in.read(buffer, chunkSize);

        out.write(buffer, chunkSize);
        done += chunkSize;
    }
}

void Object::dumpToFile(const std::string &path) const
{
    std::ofstream out (path, std::ios::out | std::ios::binary);
    dump(out);
}

bool Object::hasStream() const
{
    if (attributes()) {
        const Variant* streamAttribute = attributes()->getNamed("_stream");
        return streamAttribute && !(streamAttribute->isValueless());
    } else {
        return false;
    }
}

void Object::dumpStream(std::ostream &out)
{
    FragmentedFile* file = StreamModule::getFragmentedFile(*this);
    if(file) {
        file->dump(out);
    }
}

void Object::dumpStreamToFile(const std::string &path)
{
    std::ofstream out (path, std::ios::out | std::ios::binary);
    dumpStream(out);
}

const Variable &Object::variable()
{
    if (!_variable.isDefined()) {
        _variable = Variable((VariableImplementation *) new ObjectScope(*this), true);
    }

    return _variable;
}

const Variable &Object::contextVariable(bool createIfNeeded)
{
    if (_context == nullptr) {
        if (createIfNeeded) {
            _context = new ObjectContext(*this);
            _contextVariable = Variable((VariableImplementation *) _context, true);
            return _contextVariable;
        } else {
            for (Object* object = parent(); object; object = object->parent()) {
                if (object->_context) {
                    return object->_contextVariable;
                }
            }
        }
    }

    return _contextVariable;
}

const Variable &Object::attributesVariable(bool createIfNeeded)
{
    if (_attributes == nullptr && createIfNeeded) {
        _attributes = new ObjectAttributes(collector());
        _attributesVariable = Variable((VariableImplementation *) _attributes, true);
    }

    return _attributesVariable;
}

bool Object::isValid() const
{
    return _valid;
}

void Object::invalidate()
{
    _valid = false;
}

void Object::seekBeginning()
{
    _file.seekg(_beginningPos,std::ios::beg);
}

void Object::seekEnd()
{
    std::streampos newPos;
    if(size() != -1) {
        newPos = _beginningPos + size();
    } else {
        newPos = _beginningPos;
    }
    _file.seekg(_beginningPos, std::ios::beg);
}

void Object::seekObjectEnd(std::streamoff offset)
{
    _file.seekg(_beginningPos + static_cast<std::streamoff>(_pos) + offset, std::ios::beg);
}

std::streamoff Object::pos() const
{
    return _pos;
}

bool Object::includesPos(std::streamoff pos) const
{
    return _beginningPos <= pos && pos < _beginningPos + _size;
}

void Object::setPos(std::streamoff pos)
{
    if (pos >= 0) {
        if (size() != -1) {
            if (pos <= size()) {
                _pos = pos;
            } else {
                Log::warning("Trying to set a position ", pos," outside of the bounds of the object");
            }
        } else {
            if (_beginningPos + pos <= file().size()) {
                _pos = pos;
            } else {
                Log::warning("Trying to set a position ", _beginningPos + pos," outside of the bounds of the file");
            }
        }
    } else {
        Log::warning("Trying to set a negative value ", pos, " as position");
    }
}

Variant &Object::value()
{
    return _value;
}

const Variant &Object::value() const
{
    return _value;
}

void Object::setValue(const Variant &value)
{
    _value = value;
}

bool Object::hasLinkTo() const
{
    return !_linkTo.isValueless();
}

std::streamoff Object::linkTo() const
{
    return _linkTo.toUnsignedInteger();
}

void Object::setLinkTo(std::streamoff linkTo)
{
    if (linkTo >= 0LL) {
        _linkTo.setValue(linkTo);
    } else {
        Log::warning("Trying to set a negative value for a linkTo");
    }
}

void Object::removeLinkTo()
{
    _linkTo.setValue(Variant());
}

ObjectAttributes *Object::attributes(bool createIfNeeded)
{
    if (_attributes == nullptr && createIfNeeded) {
        _attributes = new ObjectAttributes(collector());
        _attributesVariable = Variable((VariableImplementation *) _attributes, true);
    }

    return _attributes;
}

const ObjectAttributes *Object::attributes() const
{
    return _attributes;
}

void Object::parse()
{
    parseBody();
    parseTail();
}

void Object::parseBody()
{
    while(_valid && _parsedCount < _parsers.size())
    {
        auto& parser = _parsers[_parsedCount];
        parser->parse();
        ++_parsedCount;
    }
}

bool Object::parseSome(int hint)
{
    size_t initialCount = _children.size();

    while(_valid && _parsedCount < _parsers.size() && _children.size() < initialCount+hint)
    {
        auto& parser = _parsers[_parsedCount];
        if(parser->parseSome(initialCount+hint-_children.size()))
        {
            ++_parsedCount;
        }
        else
        {
            return false;
        }
    }

    if (!_valid) {
        return true;
    } else if(_parsedCount == _parsers.size()) {
        parseTail();
        return true;
    }
    else
    {
        return false;
    }
}

void Object::parseTail()
{
    for(auto& parser : _parsers)
    {
        if(parser)
        {
            if (_valid) {
                parser->parseTail();
            }
            parser.reset();
        }
    }
}

Object::Endianness Object::endianness() const
{
    return _endianness;
}

void Object::setEndianness(const Object::Endianness &endianness)
{
    _endianness = endianness;
}

void throwChildError(const Object& object, const Object &child, ParsingException::Type type, const std::string reason)
{
    throw ParsingException(type, concat("Child ", child, " cannot be added to ", object, " : ", reason));
}

void Object::addChild(Object *child)
{
    if (child != nullptr) {
        std::streamoff curPos = pos();
        if (child->size() == -1LL) {
            if (size() != -1LL && child->isSetToExpandOnAddition()) {
                child->setSize(size() - curPos);
            } else {
                child->parse();
                child->setSize(child->_contentSize);
            }
        }

        int64_t newSize = curPos + child->size();

        std::streamoff newPos;
        const bool fileGood = file().good();
        const int64_t objectSize = size();
        const int64_t newAbsolutePosition = beginningPos() + newSize;
        const int64_t fileSize = file().size();
        const bool outOfFile = (!fileGood || newAbsolutePosition > fileSize);
        const bool outOfParent = (objectSize != -1 && newSize > objectSize);
        if (outOfFile || outOfParent) {
            if (size() != -1) {
                newPos = size();
            } else {
                newPos = 0;
            }
        } else {
            newPos = child->beginningPos() - beginningPos();
            if (child->size() != -1LL) {
                newPos += child->size();
            }
        }

        setPos(newPos);
        seekObjectEnd();

        if (_contentSize < newSize) {
            _contentSize = newSize;
        }

        if(!child->name().empty()) {
            _lookUpTable[child->name()] = child;
        }

        child->_parent = this;

        _children.push_back(child);
        _ownedChildren.push_back(std::unique_ptr<Object>(child));
        child->_rank = _children.size() - 1;
        _lastChild = nullptr;

        if (!(child->isValid())) {
            throwChildError(*this, *child, ParsingException::InvalidChild, "child invalid");
        } else if (outOfFile) {
            throwChildError(*this, *child, ParsingException::OutOfFile, "out of file");
        } else if (outOfParent) {
            throwChildError(*this, *child, ParsingException::OutOfParent, concat("too big ", child->size()));
        }
    }
}


void Object::explore(int depth)
{
    if(depth == 0) {
        return;
    }

    if (!parsed()) {
        if (!file().good()) {
            _file.clear();
            std::cerr<<"clearing file"<<std::endl;
        }
        seekObjectEnd();
        parse();
    }

    for (Object::iterator it = begin(); it != end(); ++it) {
        if(depth == -1) {
            (*it)->explore(-1);
        } else {
            (*it)->explore(depth-1);
        }
    }
}

bool Object::exploreSome(int hint)
{
    if(!parsed()) {
        if(!file().good()) {
            _file.clear();
            std::cerr<<"clearing file"<<std::endl;
        }
        seekObjectEnd();

        return parseSome(hint);
    }
    return true;
}

ObjectContext *Object::context(bool createIfNeeded)
{
    if (_context == nullptr && createIfNeeded) {
        _context = new ObjectContext(*this);
        _contextVariable = Variable((VariableImplementation *) _context, true);
    }

    return _context;
}

const ObjectContext *Object::context() const
{
    return _context;
}

const ObjectType &Object::type() const
{
    return _type;
}

ObjectType &Object::type()
{
    return _type;
}

void Object::setType(const ObjectType &type)
{
    if(!type.isNull()) {
        _type = type;
    }
}

const std::string &Object::name() const
{
    return _name;
}

void Object::setName(const std::string &name)
{
    _name = name;
}

File &Object::file()
{
    return _file;
}

const File &Object::file() const
{
    return _file;
}

std::streampos Object::beginningPos() const
{
    return _beginningPos;
}

std::streamoff Object::size() const
{
    return _size;
}

void Object::setSize(std::streamoff size)
{
    if (size >= 0) {
        _size = size;
    } else {
        Log::warning("Trying to set a negative value for a size");
    }
}

bool Object::isSetToExpandOnAddition() const
{
    return _expandOnAddition;
}

void Object::setToExpandOnAddition()
{
    _expandOnAddition = true;
}

Object* Object::parent()
{
    return _parent;
}

const Object *Object::parent() const
{
    return _parent;
}

Object &Object::root()
{
    Object* pRoot = this;
    while (pRoot->_parent) {
        pRoot = pRoot->_parent;
    }
    return *pRoot;
}

const Object &Object::root() const
{
    const Object* pRoot = this;
    while (pRoot->_parent) {
        pRoot = pRoot->_parent;
    }
    return *pRoot;
}

int64_t Object::rank() const
{
    return _rank.toInteger();
}

void Object::addParser(Parser *parser)
{
    if(_valid && parser != nullptr)
    {
        if(!parser->headParsed())
        {
            parseBody();
            parser->parseHead();
        }
        _parsers.emplace_back(parser);
    }
}

std::ostream& Object::display(std::ostream& out, std::string prefix) const
{
    out << prefix << type() << " " << name();
    if(!value().isValueless())
        out << " = " << value();

    return out;
}

std::ostream &Object::displayTree(std::ostream &out, std::string prefix) const
{
    out << prefix;
    display(out);
    out<<std::endl;

    if(numberOfChildren() < 20)
    {
        for(Object::const_iterator it = begin(); it != end(); ++it)
        {
            (*it)->displayTree(out, prefix+"    ");
        }
    }
    return out;
}

bool Object::parsed()
{
    if (!_valid) {
        return true;
    }

    for(int i = _parsers.size() - 1; i >= 0; --i)
    {
        auto& parser = _parsers[i];
        if (parser && !parser->tailParsed())
        {
            return false;
        }
    }
    return true;
}

std::ostream& operator <<(std::ostream& out, const Object& object)
{
    return object.display(out);
}


Object::ParsingContext::ParsingContext(Object &object)
    :_object(object),
      _isAvailable(!object._parsingInProgress)
{
    if (!_isAvailable)
    {
        throw LockException(_object);
    }
    _object._parsingInProgress = true;
}

Object::ParsingContext::ParsingContext(ParsingOption &parsingOption)
    :_object(static_cast<Object&>(parsingOption)),
      _isAvailable(!_object._parsingInProgress)
{
    if (!_isAvailable)
    {
        throw LockException(_object);
    }
    _object.parseBody();
    _object._parsingInProgress = true;
}


Object::ParsingContext::~ParsingContext()
{
    if (_isAvailable) {
        _object._parsingInProgress = false;
    }
}

bool Object::ParsingContext::isAvailable() const
{
    return _isAvailable;
}


Object::ParsingContext::LockException::LockException(const Object &object)
    :_object(object)
{
}

const char *Object::ParsingContext::LockException::what() const noexcept
{
    return "Object locked for parsing";
}


ParsingOption::ParsingOption()
{
}

