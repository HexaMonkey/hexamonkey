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

#include "core/containerparser.h"
#include "core/module.h"
#include "core/log/logmanager.h"
#include "core/parsingexception.h"

ContainerParser::ContainerParser(Object &object, const Module &module)
    : Parser(object),
      _module(module),
      _autogrow(false)

{
}

void ContainerParser::addChild(Object *child)
{
    if (child != nullptr) {
        std::streamoff pos = object().pos();
        if (child->size() == -1LL) {
            if (object().size() != -1LL && child->isSetToExpandOnAddition()) {
                child->setSize(object().size() - pos);
            } else {
                child->parse();
                child->setSize(child->_contentSize);
            }
        }



        int64_t newSize = pos + child->size();
        if (_autogrow && newSize > object().size()) {
            object().setSize(newSize);
        }

        std::streamoff newPos;
        const bool fileGood = object().file().good();
        const int64_t objectSize = object().size();
        const int64_t newAbsolutePosition = object().beginningPos() + newSize;
        const int64_t fileSize = object().file().size();
        const bool outOfFile = (!fileGood || newAbsolutePosition > fileSize);
        const bool outOfParent = (objectSize != -1 && newSize > objectSize);
        if (outOfFile || outOfParent) {
            if (object().size() != -1) {
                newPos = object().size();
            } else {
                newPos = 0;
            }
        } else {
            newPos = child->beginningPos() - object().beginningPos();
            if (child->size() != -1LL) {
                newPos += child->size();
            }
        }

        object().setPos(newPos);
        object().seekObjectEnd();

        if (object()._contentSize < newSize) {
            object()._contentSize = newSize;
        }

        if(!child->name().empty()) {
            object()._lookUpTable[child->name()] = child;
        }

        child->_parent = &object();

        object()._children.push_back(child);
        object()._ownedChildren.push_back(std::unique_ptr<Object>(child));
        child->_rank = _object._children.size() - 1;
        object()._lastChild = nullptr;

        if (!(child->isValid())) {
            throwChildError(*child, ParsingException::InvalidChild, "child invalid");
        } else if (outOfFile) {
            throwChildError(*child, ParsingException::OutOfFile, "out of file");
        } else if (outOfParent) {
            throwChildError(*child, ParsingException::OutOfParent, concat("too big ", child->size()));
        }


    }
}

void ContainerParser::addChild(Object *child, const std::string &name)
{
    if(child != nullptr) {
        child->setName(name);
        addChild(child);
    } else {
        Log::error("Child is null");
    }
}

Object *ContainerParser::readVariable(const ObjectType &type)
{
    Object* child = getVariable(type);
    if (child != nullptr && child->size() == -1LL) {
        child->parse();
        child->setSize(child->_contentSize);
    }
    return child;
}

Object *ContainerParser::getVariable(const ObjectType &type)
{
    object().seekObjectEnd();
    return _module.handle(type, object());
}

Object *ContainerParser::addVariable(const ObjectType &type)
{
    Object* child = getVariable(type);
    addChild(child);
    return child;
}

Object *ContainerParser::addVariable(const ObjectType &type, const std::string &name)
{
    object().seekObjectEnd();

    Object* child = getVariable(type);
    addChild(child, name);
    return child;
}

int64_t ContainerParser::findBytePattern(const std::string &pattern)
{
    std::vector<std::vector<unsigned char>> byteList;
    std::vector<std::vector<unsigned char>> maskList;

    parseBytePattern(pattern, byteList, maskList);

    size_t patternCount = byteList.size();
    size_t width = 0;
    for (const auto& bytes : byteList) {
        if (bytes.size() > width) {
            width = bytes.size();
        }
    }
    object().seekObjectEnd();
    size_t pos = object().pos();
    File& file = object().file();
    std::vector<unsigned char> buffer(width);
    for (size_t i = 0; file.good();++i) {
        file.read(reinterpret_cast<char *>(&buffer[i%width]), 8);
        for (size_t j = 0; j < patternCount; ++j) {
            const auto& bytes = byteList[j];
            const auto& masks = maskList[j];
            size_t patternSize = bytes.size();
            if (i >= patternSize - 1) {
                for (size_t k = 0; k < patternSize; ++k) {
                    if (bytes[k] != (masks[k] & buffer[(i - k) % width])) {
                        break;
                    }

                    if (k >= patternSize - 1) {
                        return pos + 8*(i - k);
                    }
                }
            }
        }
    }
    file.clear();
    return -1;
}

const Module &ContainerParser::module() const
{
    return _module;
}


void ContainerParser::setAutogrow()
{
    _autogrow = true;
    if(object().size()<0) {
        object().setSize(0);
    }
}

void ContainerParser::throwChildError(const Object &child, ParsingException::Type type, const std::string reason) const
{
    throw ParsingException(type, concat("Child ", child, " cannot be added to ", object(), " : ", reason));
}

void ContainerParser::parseBytePattern(const std::string &pattern, std::vector<std::vector<unsigned char> >& byteList, std::vector<std::vector<unsigned char> >& maskList)
{
    bool isCurrentMask = false;
    std::vector<unsigned char> current;
    std::string buffer = "00";
    for (int i = 0, n = pattern.size(); i < n; ++i)
    {
        char ch = pattern[i];
        if (ch == '&') {
            byteList.emplace_back(std::move(current));
            current.resize(0);
            isCurrentMask = true;
        } else if (ch == '|') {
            if (isCurrentMask) {
                maskList.emplace_back(std::move(current));
                current.resize(0);
                isCurrentMask = false;
            } else {
                maskList.emplace_back(std::vector<unsigned char>(current.size(), 0xff));
                byteList.emplace_back(std::move(current));
                current.resize(0);
            }
        } else if (ch == ' ') {
            /** ignore **/
        } else if (i < n - 1) {
            buffer[0] = ch;
            ++i;
            buffer[1] = pattern[i];
            current.insert(current.begin(), strtol(buffer.c_str(), nullptr, 16));
        }
    }

    if (isCurrentMask) {
        maskList.emplace_back(std::move(current));
        current.resize(0);
        isCurrentMask = false;
    } else {
        maskList.emplace_back(std::vector<unsigned char>(current.size(), 0xff));
        byteList.emplace_back(std::move(current));
        current.resize(0);
    }
}
