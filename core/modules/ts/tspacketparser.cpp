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

#include "tspacketparser.h"
#include "tstypes.h"
#include "standardtypes.h"

TsPacketParser::TsPacketParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void TsPacketParser::doParseHead()
{
    setSize(8*188);
    addVariable(tsTypes::bslbf(8),"syncbyte");
    addVariable(tsTypes::bslbf(1),"transport_error_indicator");
    addVariable(tsTypes::bslbf(1),"payload_unit_start_indicator");
    addVariable(tsTypes::bslbf(1),"transport_priority");
    addVariable(tsTypes::uimsbf(13),"PID");
    addVariable(tsTypes::bslbf(2),"transport_scrambling_control");
    addVariable(tsTypes::bslbf(2),"adaptation_field_control");
    addVariable(tsTypes::uimsbf(4),"continuity_counter");

    showcase().add("PID");
    showcase().add("continuity_counter");
}

void TsPacketParser::doParse()
{
    Object* adaptation_field_control = lookUp("adaptation_field_control");
    if (adaptation_field_control->value().toUnsignedInteger() & 0b10)
    {
        addVariable(tsTypes::adaptation_field(),"adaptation_field");
    }
    if (adaptation_field_control->value().toUnsignedInteger() & 0b01)
    {
        addVariable(standardTypes::data(), "data_bytes");
    }
}
