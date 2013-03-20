#include "ebmlmodule.h"

#include <fstream>
#include <streambuf>

#include "bitutil.h"
#include "strutil.h"

#include "defaulttypes.h"
#include "standardtypes.h"
#include "ebmltypes.h"

#include "ebmlmasterparser.h"
#include "ebmlcontainerparser.h"
#include "ebmllargeintegerparser.h"
#include "ebmlsimpleparser.h"
#include "ebmldateparser.h"

using namespace ebmlTypes;

void EbmlModule::addFormatDetection(StandardFormatDetector::Adder &formatAdder)
{
    formatAdder.addMagicNumber("1a 45 df a3");
}

void EbmlModule::requestImportations(std::vector<std::string> &formatRequested)
{
    formatRequested.push_back("bestd");
}

bool EbmlModule::doLoad()
{
    addTemplate(EBMLFile);
    setExtension(EBMLFile, defaultTypes::file());
    setSpecification(defaultTypes::file(), EBMLFile());

    addTemplate(EBMLElement);

    addTemplate(Date);
    setExtension(Date, standardTypes::int64);

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

