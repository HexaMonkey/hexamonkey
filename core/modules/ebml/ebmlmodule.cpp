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
#include "core/util/bitutil.h"
#include "core/util/strutil.h"

#include "core/modules/ebml/ebmllargeintegertypetemplate.h"
#include "core/modules/ebml/ebmlfiletypetemplate.h"
#include "core/modules/ebml/ebmlelementtypetemplate.h"
#include "core/modules/ebml/ebmldatetypetemplate.h"
#include "core/modules/ebml/ebmlextensiontypetemplate.h"

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

    const ObjectTypeTemplate& largeIntegerTypeTemplate = addTemplate(new EbmlLargeIntegerTypeTemplate);
    const ObjectTypeTemplate& dateTypeTemplate = addTemplate(new EbmlDateTypeTemplate);
    auto& EBMLElement = addTemplate(new EbmlElementTypeTemplate(getType(largeIntegerTypeTemplate)));

    auto& EBMLFile = addTemplate(new EbmlFileTypeTemplate(file, getType(EBMLElement)));
    setSpecification(file, getType(EBMLFile));



    auto elementType = getSharedType(EBMLElement);
    std::shared_ptr<ObjectType> elementTypeTemplates[] = {
        getSharedType(addTemplate(new EbmlMasterTypeTemplate(elementType, getType(EBMLElement)))),
        getSharedType(addTemplate(new EbmlIntegerTypeTemplate(elementType, getType("int")))),
        getSharedType(addTemplate(new EbmlUIntegerTypeTemplate(elementType, getType("uint")))),
        getSharedType(addTemplate(new EbmlFloatTypeTemplate(elementType, getType("float"), getType("double")))),
        getSharedType(addTemplate(new EbmlStringTypeTemplate(elementType, getType("String")))),
        getSharedType(addTemplate(new EbmlUtf8StringTypeTemplate(elementType, getType("String")))),
        getSharedType(addTemplate(new EbmlDateElementTypeTemplate(elementType, getType(dateTypeTemplate)))),
        getSharedType(addTemplate(new EbmlBinaryTypeTemplate(elementType, getType("Data"))))
    };

    for (int i = 0; i < numberOfTypeElements; ++i)
    {
        _elementTypes[typeElementAtributes[i]] = elementTypeTemplates[i];
    }

    for(int i = 0; i < numberOfDefaultElements; ++i)
    {
        const ObjectTypeTemplate& defaultElementTemplate = addTemplate(new FixedParentTypeTemplate(defaultElements[i], elementTypeTemplates[defaultElementTypes [i]]));
        setSpecification(getType(EBMLElement, defaultElementIds[i]), getType(defaultElementTemplate));
    }

    return true;
}
