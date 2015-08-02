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

#include "core/modules/default/defaultmodule.h"
#include "core/modules/ebml/ebmlmodule.h"
#include "core/modules/ebml/ebmlmasterparser.h"
#include "core/modules/ebml/ebmlcontainerparser.h"
#include "core/modules/ebml/ebmllargeintegerparser.h"
#include "core/modules/ebml/ebmlsimpleparser.h"
#include "core/modules/ebml/ebmldateparser.h"
#include "core/util/bitutil.h"
#include "core/util/strutil.h"

const ObjectTypeTemplate EbmlModule::EBMLFile("EBMLFile");
const ObjectTypeTemplate EbmlModule::EBMLElement("EBMLElement", {"id"}, [](ObjectTypeTemplate& typeTemplate) {
    typeTemplate.setVirtual(true);
});
const ObjectTypeTemplate EbmlModule::largeInteger("LargeInteger");
const ObjectTypeTemplate EbmlModule::Date("Date");

const int EbmlModule::numberOfTypeElements = 8;
const std::string EbmlModule::typeElements[] = {"MasterElement","IntegerElement","UIntegerElement","FloatElement","StringElement","UTF8StringElement","DateElement","BinaryElement"};
const std::string EbmlModule::typeElementAtributes[] = {"master","integer","uinteger","float","string","utf-8","date","binary"};

const int EbmlModule::numberOfDefaultElements = 8;
const std::string EbmlModule::defaultElements[] = {"EBML", "EBMLVersion", "EBMLReadVersion", "EBMLMaxIDLength", "EBMLMaxSizeLength", "DocType", "DocTypeVersion", "DocTypeReadVersion"};
const uint32_t EbmlModule::defaultElementIds[] = {0xa45dfa3, 0x286, 0x2f7, 0x2f2, 0x2f3, 0x282, 0x287, 0x285};
const int EbmlModule::defaultElementTypes[] = {0,2,2,2,2,4,2,2};

void EbmlModule::addFormatDetection(StandardFormatDetector::Adder &formatAdder)
{
    formatAdder.addMagicNumber("1a 45 df a3");
}

bool EbmlModule::doLoad()
{
    addTemplate(EBMLFile);
    setExtension(EBMLFile, DefaultModule::file());
    setSpecification(DefaultModule::file(), EBMLFile());

    addTemplate(EBMLElement);

    addTemplate(Date);
    setExtension(Date, DefaultModule::int64);

    addTemplate(largeInteger);

    for(int i = 0; i < numberOfTypeElements; ++i)
    {
        const ObjectTypeTemplate& TypeElementTemplate = newTemplate(typeElements[i]);
        setExtension(TypeElementTemplate, EBMLElement());
    }

    for(int i = 0; i < numberOfDefaultElements; ++i)
    {
        const ObjectTypeTemplate& DefaultElementTemplate = newTemplate(defaultElements[i]);
        addParser(defaultElements[i]);
        setExtension(DefaultElementTemplate, getTemplate(typeElements[defaultElementTypes[i]])());
        setSpecification(EBMLElement(defaultElementIds[i]), DefaultElementTemplate());
    }

    addParser("EBMLFile", []parserLambda{return new EbmlMasterParser(object, module);});
    addParser("EBMLElement", []parserLambda{return new EbmlContainerParser(object, module);});

    addParser("Date", []parserLambda{return new EbmlDateParser(object);});
    addParser("LargeInteger", []parserLambda{return new EbmlLargeIntegerParser(object);});

    addParser("MasterElement", []parserLambda{return new EbmlMasterParser(object, module);});
    addParser("IntegerElement", []parserLambda{return new EbmlIntegerParser(object, module);});
    addParser("UIntegerElement", []parserLambda{return new EbmlUIntegerParser(object, module);});
    addParser("FloatElement", []parserLambda{return new EbmlFloatParser(object, module);});
    addParser("StringElement", []parserLambda{return new EbmlStringParser(object, module);});
    addParser("UTF8StringElement", []parserLambda{return new EbmlStringParser(object, module);});
    addParser("DateElement", []parserLambda{return new EbmlDateContainerParser(object, module);});
    addParser("BinaryElement", []parserLambda{return new EbmlBinaryParser(object, module);});

    return true;
}

