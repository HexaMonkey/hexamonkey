#ifndef EBMLTYPES_H
#define EBMLTYPES_H

#include "objecttypetemplate.h"
#include "objecttype.h"


namespace ebmlTypes
{
    const ObjectTypeTemplate EBMLFile("EBMLFile");
    const ObjectTypeTemplate EBMLElement("EBMLElement", {"id"});
    const ObjectTypeTemplate largeInteger("LargeInteger");
    const ObjectTypeTemplate Date("Date");

    const int numberOfTypeElements = 8;
    const std::string typeElements[] = {"MasterElement","IntegerElement","UIntegerElement","FloatElement","StringElement","UTF8StringElement","DateElement","BinaryElement"};
    const std::string typeElementAtributes[] = {"master","integer","uinteger","float","string","utf-8","date","binary"};

    const int numberOfDefaultElements = 8;
    const std::string defaultElements[] = {"EBML", "EBMLVersion", "EBMLReadVersion", "EBMLMaxIDLength", "EBMLMaxSizeLength", "DocType", "DocTypeVersion", "DocTypeReadVersion"};
    const uint32_t defaultElementIds[] = {0xa45dfa3, 0x286, 0x2f7, 0x2f2, 0x2f3, 0x282, 0x287, 0x285};
    const int defaultElementTypes[] = {0,2,2,2,2,4,2,2};
}

#endif // EBMLTYPES_H
