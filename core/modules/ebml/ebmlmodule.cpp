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

#include <fstream>
#include <streambuf>
#include <unordered_set>
#include <array>

#include "core/modules/default/defaultmodule.h"
#include "core/modules/ebml/ebmlmodule.h"
#include "core/modules/ebml/ebmlmasterparser.h"
#include "core/modules/ebml/ebmlcontainerparser.h"
#include "core/modules/ebml/ebmllargeintegerparser.h"
#include "core/modules/ebml/ebmlsimpleparser.h"
#include "core/modules/ebml/ebmldateparser.h"
#include "core/util/bitutil.h"
#include "core/util/strutil.h"

#include "core/modules/ebml/ebmllargeintegertypetemplate.h"
#include "core/modules/ebml/ebmlfiletypetemplate.h"
#include "core/modules/ebml/ebmlelementtypetemplate.h"
#include "core/modules/ebml/ebmldatetypetemplate.h"
#include "core/modules/ebml/ebmlextensiontypetemplate.h"

std::unordered_set<std::string> ebmlRefactored = {"LargeInteger", "EBMLFile", "EBMLElement", "Date", "MasterElement","IntegerElement","UIntegerElement","FloatElement","StringElement","UTF8StringElement","DateElement","BinaryElement"};

const int EbmlModule::numberOfTypeElements = 8;
const std::string EbmlModule::typeElements[] = {"MasterElement","IntegerElement","UIntegerElement","FloatElement","StringElement","UTF8StringElement","DateElement","BinaryElement"};
const std::string EbmlModule::typeElementAtributes[] = {"master","integer","uinteger","float","string","utf-8","date","binary"};

const int EbmlModule::numberOfDefaultElements = 8;
const std::string EbmlModule::defaultElements[] = {"EBML", "EBMLVersion", "EBMLReadVersion", "EBMLMaxIDLength", "EBMLMaxSizeLength", "DocType", "DocTypeVersion", "DocTypeReadVersion"};
const uint32_t EbmlModule::defaultElementIds[] = {0xa45dfa3, 0x286, 0x2f7, 0x2f2, 0x2f3, 0x282, 0x287, 0x285};
const int EbmlModule::defaultElementTypes[] = {0,2,2,2,2,4,2,2};

std::shared_ptr<ObjectType> EbmlModule::elementType(const std::string &type) const
{
    return _elementTypes.find(type)->second;
}

void EbmlModule::addFormatDetection(StandardFormatDetector::Adder &formatAdder)
{
    formatAdder.addMagicNumber("1a 45 df a3");
}

bool EbmlModule::doLoad()
{
    auto file = getType("File");
    auto& EBMLFile = addTemplate(new EbmlFileTypeTemplate(file.typeTemplate()));
    setSpecification(file, ObjectType(EBMLFile));

    auto& EBMLElement = addTemplate(new EbmlElementTypeTemplate);

    addTemplate(new EbmlLargeIntegerTypeTemplate);
    const ObjectTypeTemplate& dateTypeTemplate = addTemplate(new EbmlDateTypeTemplate);

    auto elementType = std::make_shared<ObjectType>(EBMLElement);
    std::shared_ptr<ObjectType> elementTypeTemplates[] = {
        std::make_shared<ObjectType>(addTemplate(new EbmlMasterTypeTemplate(elementType))),
        std::make_shared<ObjectType>(addTemplate(new EbmlIntegerTypeTemplate(elementType, getTemplate("int")))),
        std::make_shared<ObjectType>(addTemplate(new EbmlUIntegerTypeTemplate(elementType, getTemplate("uint")))),
        std::make_shared<ObjectType>(addTemplate(new EbmlFloatTypeTemplate(elementType, getTemplate("float"), getTemplate("double")))),
        std::make_shared<ObjectType>(addTemplate(new EbmlStringTypeTemplate(elementType, getTemplate("String")))),
        std::make_shared<ObjectType>(addTemplate(new EbmlUtf8StringTypeTemplate(elementType, getTemplate("String")))),
        std::make_shared<ObjectType>(addTemplate(new EbmlDateElementTypeTemplate(elementType, dateTypeTemplate))),
        std::make_shared<ObjectType>(addTemplate(new EbmlBinaryTypeTemplate(elementType, getTemplate("Data"))))
    };

    for (int i = 0; i < numberOfTypeElements; ++i)
    {
        _elementTypes[typeElementAtributes[i]] = elementTypeTemplates[i];
    }

    for(int i = 0; i < numberOfDefaultElements; ++i)
    {
        const ObjectTypeTemplate& defaultElementTemplate = addTemplate(new FixedParentTypeTemplate(defaultElements[i], elementTypeTemplates[defaultElementTypes [i]]));
        setSpecification(ObjectType(EBMLElement, defaultElementIds[i]), ObjectType(defaultElementTemplate));
    }

    return true;
}
