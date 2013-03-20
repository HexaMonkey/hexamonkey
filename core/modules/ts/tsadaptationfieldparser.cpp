#include "tsadaptationfieldparser.h"

#include "standardtypes.h"
#include "tstypes.h"
using namespace standardTypes;
using namespace tsTypes;

TsAdaptationFieldParser::TsAdaptationFieldParser(Object &object, const Module &module)
    :ContainerParser(object, module)
{
}

void TsAdaptationFieldParser::doParseHead()
{
    Object* adaptation_field_length = addVariable(uimsbf(8),"adaptation_field_length");
    setSize(8+8 * adaptation_field_length->value().toUnsignedInteger());
}

void TsAdaptationFieldParser::doParse()
{
    if (object().size() > 8 )
    {
        addVariable(bslbf(1),"discontinuity_indicator");
        addVariable(bslbf(1),"random_access_indicator");
        addVariable(bslbf(1),"elementary_stream_priority_indicator");
        Object* PCR_flag = addVariable(bslbf(1),"PCR_flag");
        Object* OPCR_flag = addVariable(bslbf(1),"OPCR_flag");
        Object* splicing_point_flag= addVariable(bslbf(1),"splicing_point_flag");
        Object* transport_private_data_flag = addVariable(bslbf(1),"transport_private_data_flag");
        Object* adaptation_field_extension_flag = addVariable(bslbf(1),"adaptation_field_extension_flag");
        if (PCR_flag->value().toUnsignedInteger() == 1)
        {
            addVariable(uimsbf(33),"program_clock_reference_base");
            addVariable(bslbf(6),"reserved");
            addVariable(uimsbf(9),"program_clock_reference_extension");
        }
        if (OPCR_flag->value().toUnsignedInteger() == 1)
        {
            addVariable(uimsbf(33),"original_program_clock_reference_base");
            addVariable(bslbf(6),"reserved");
            addVariable(uimsbf(9),"original_program_clock_reference_extension");
        }
        if (splicing_point_flag->value().toUnsignedInteger() == 1)
        {
            addVariable(tcimsbf(8),"splice_countdown");
        }
        if (transport_private_data_flag->value().toUnsignedInteger() == 1)
        {
            Object* transport_private_data_length = addVariable(uimsbf(8),"transport_private_data_length");
            for (unsigned int i = 0 ; i < transport_private_data_length->value().toUnsignedInteger() ; i++)
            {
                addVariable(bslbf(8),"private_data_byte");
            }
        }
        if (adaptation_field_extension_flag->value().toUnsignedInteger() == 1)
        {
            addVariable(uimsbf(8),"elementary_stream_priority_indicator");
            Object* ltw_flag = addVariable(bslbf(1),"ltw_flag");
            Object* piecewise_rate_flag = addVariable(bslbf(1),"piecewise_rate_flag");
            Object* seamless_splice_flag = addVariable(bslbf(1),"seamless_splice_flag");
            addVariable(bslbf(5),"reserved");
            if (ltw_flag->value().toUnsignedInteger() == 1) //size 16
            {
                addVariable(bslbf(1),"ltw_valid_flag");
                addVariable(uimsbf(15),"ltw_offset");
            }
            if(piecewise_rate_flag->value().toUnsignedInteger() == 1) //size  24
            {
                addVariable(bslbf(1),"ltw_valid_flag");
                addVariable(uimsbf(22),"piecewise_rate");
            }
            if(seamless_splice_flag->value().toUnsignedInteger() == 1) //size 36
            {
                addVariable(bslbf(1),"splice_type");
                addVariable(bslbf(3),"DTS_next_AU_32_30");
                addVariable(bslbf(1),"marker_bit");
                addVariable(bslbf(15),"DTS_next_AU_29_15");
                addVariable(bslbf(1),"marker_bit");
                addVariable(bslbf(15),"DTS_next_AU_14_0");
                addVariable(bslbf(1),"marker_bit");
            }

            addVariable(data(16*(1 - ltw_flag->value().toUnsignedInteger())
                            +24*(1 - piecewise_rate_flag->value().toUnsignedInteger())
                            +36*(1 - seamless_splice_flag->value().toUnsignedInteger())),"stuffing_bytes");
        }
        addVariable(data(),"stuffing_bytes");
    }
}
