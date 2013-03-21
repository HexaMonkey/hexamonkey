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
