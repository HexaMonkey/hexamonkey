#include "tsmodule.h"

#include "defaulttypes.h"
#include "standardtypes.h"
#include "tstypes.h"

#include "bitutil.h"
#include "strutil.h"

#include "tsadaptationfieldparser.h"
#include "tsfileparser.h"
#include "tsjunkparser.h"
#include "tspacketparser.h"

using namespace standardTypes;
using namespace tsTypes;

void TsModule::addFormatDetection(StandardFormatDetector::Adder &formatAdder)
{
    formatAdder.addSyncbyte(0x47, 188);
}

void TsModule::requestImportations(std::vector<std::string> &formatRequested)
{
    formatRequested.push_back("bestd");
}

bool TsModule::doLoad()
{
    setExtension(tsFile,defaultTypes::file());
    setSpecification(defaultTypes::file(), tsFile());

    addParser("TsFile", []parserLambda{return new TsFileParser(object, module);});

    addParser("transport_packet", []parserLambda{return new TsPacketParser(object, module);});

    addParser("junk", []parserLambda{return new TsJunkParser(object, module);});

    addParser("adaptation_field", []parserLambda{return new TsAdaptationFieldParser(object, module);});

    setExtension(bslbf, bitset(), {{0,0}});
    setExtension(uimsbf, uinteger(), {{0,0}});

    addParser("bslbf");
    addParser("uimsbf");

    return true;
}
