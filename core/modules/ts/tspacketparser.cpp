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
