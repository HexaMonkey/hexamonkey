#include "mp4boxparser.h"

#include "module.h"

#include "defaulttypes.h"
#include "standardtypes.h"
#include "mp4types.h"

using namespace defaultTypes;
using namespace standardTypes;
using namespace mp4Types;

Mp4BoxParser::Mp4BoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4BoxParser::doParseHead()
{
    Object* p_size_var = addVariable(uint32, "size");
    Object* p_type_var = addVariable(string(4), "type");

    if(p_size_var->value().toUnsignedInteger() == 1)
    {
        Object* p_largesize_var = addVariable(uint64,"largesize");
        setSize( 8 * p_largesize_var->value().toUnsignedInteger());
    }
    else if(p_size_var->value().toUnsignedInteger() == 0)
    {
        setSize(file().size() - object().beginningPos());
    }
    else
    {
        setSize( 8 * p_size_var->value().toInteger());
    }

    if(p_type_var->value().toString() == "uuid")
    {
        addVariable(string(16), "user_type");
    }


    type().setParameterByName("box_type", p_type_var->value());
}


Mp4FullBoxParser::Mp4FullBoxParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void Mp4FullBoxParser::doParseHead()
{
    addVariable(uint8, "version");
    addVariable(bitset(24), "flags");
}


Mp4ContainerParser::Mp4ContainerParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void Mp4ContainerParser::doParse()
{
    while(availableSize()>0)
    {
        addVariable(Box());
    }
}

bool Mp4ContainerParser::doParseSome(int hint)
{
    int count = 0;
    while(true)
    {
        if(availableSize()<=0)
            return true;

        if(count>=hint)
            return false;

        addVariable(Box());
        ++count;
    }
}

Mp4FileTypeBoxParser::Mp4FileTypeBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4FileTypeBoxParser::doParse()
{
    addVariable(string(4), "major_brand");
    addVariable(uint32, "minor_version");
    addVariable(array(string(4)), "compatible_brands");
}

Mp4MovieHeaderBoxParser::Mp4MovieHeaderBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4MovieHeaderBoxParser::doParse()
{
    bool version = (lookUp("version")->value()).toInteger();
    addVariable((version)? Date64 : Date32, "creation_time");
    addVariable((version)? Date64 : Date32, "modification_time");
    Object* p_time_scale = addVariable(uint32, "time_scale");
    addVariable(Duration((version)? 64 : 32, p_time_scale->value()), "duration");
    addVariable(fixed32, "rate");
    addVariable(fixed16, "volume");
    addVariable(uint16, "reserved");
    addVariable(tuple(uint32, 2), "reserved");
    addVariable(tuple(fixed32, 9), "matrix");
    addVariable(tuple(uint32, 6), "predefined");
    addVariable(uint32, "next_track_ID");
}

Mp4ProgressiveDownloadInfoBoxParser::Mp4ProgressiveDownloadInfoBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ProgressiveDownloadInfoBoxParser::doParse()
{
    addVariable(array(module().getTemplate("ProgressiveDownloadInfoEntry")()));
}

Mp4FreeSpaceBoxParser::Mp4FreeSpaceBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4FreeSpaceBoxParser::doParse()
{
    addVariable(data(),"junk");
}

Mp4MediaDataBoxParser::Mp4MediaDataBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4MediaDataBoxParser::doParse()
{
    addVariable(data(),"data");
}

Mp4TrackHeaderBoxParser::Mp4TrackHeaderBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4TrackHeaderBoxParser::doParse()
{
    bool version = lookUp("version")->value().toInteger();
    addVariable((version)? Date64 : Date32, "creation_time");
    addVariable((version)? Date64 : Date32, "modification_time");
    addVariable(uint32, "trackID");
    addVariable(uint32, "reserved");
    Object* p_time_scale =    object()
                         .  parent()
                         -> parent()
                         -> lookForType(module().getTemplate("MovieHeaderBox")(), true)
                         -> lookUp("time_scale", true);
    addVariable(Duration((version)? 64 : 32, p_time_scale->value()), "duration");
    addVariable(tuple(fixed32, 2), "reserved");
    addVariable(int16, "layer");
    addVariable(int16, "alternate_group");
    addVariable(fixed16, "volume");
    addVariable(int16,"reserved");
    addVariable(tuple(fixed32, 9), "matrix");
    addVariable(fixed32, "width");
    addVariable(fixed32, "height");
}

Mp4TrackReferenceTypeBoxParser::Mp4TrackReferenceTypeBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4TrackReferenceTypeBoxParser::doParse()
{
    addVariable(array(uint32), "track_IDs");
}

Mp4MediaHeaderBoxParser::Mp4MediaHeaderBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4MediaHeaderBoxParser::doParse()
{
    bool version = lookUp("version")->value().toInteger();
    addVariable((version)? Date64 : Date32, "creation_time");
    addVariable((version)? Date64 : Date32, "modification_time");
    Object* p_time_scale = addVariable(uint32, "time_scale");
    addVariable(Duration((version)? 64 : 32, p_time_scale->value()), "duration");
    addVariable(Language(), "langage");
    addVariable(uint16,"predefined");
}

Mp4HandlerBoxParser::Mp4HandlerBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4HandlerBoxParser::doParse()
{
    addVariable(uint32, "pre_defined");
    addVariable(string(4), "handler_type");
    addVariable(tuple(uint32,3), "reserved");
    addVariable(string(),"name");
}

Mp4VideoMediaHeaderBoxParser::Mp4VideoMediaHeaderBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{

}

void Mp4VideoMediaHeaderBoxParser::doParse()
{
    addVariable(uint16, "graphicmode");
    addVariable(tuple(uint16, 3), "opcolor");
}

Mp4SoundMediaHeaderBoxParser::Mp4SoundMediaHeaderBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4SoundMediaHeaderBoxParser::doParse()
{
    addVariable(uint16, "balance");
    addVariable(uint16, "reserved");
}

Mp4HintMediaHeaderBoxParser::Mp4HintMediaHeaderBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4HintMediaHeaderBoxParser::doParse()
{
    addVariable(uint16, "maxPDUsize");
    addVariable(uint16, "avgPDUsize");
    addVariable(uint32, "maxbitrate");
    addVariable(uint32, "avgbitrate");
    addVariable(uint32, "reserved");
}

Mp4NullMediaHeaderBoxParser::Mp4NullMediaHeaderBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4NullMediaHeaderBoxParser::doParse()
{

}

Mp4SampleDescriptionBoxParser::Mp4SampleDescriptionBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4SampleDescriptionBoxParser::doParse()
{
    Object* p_entry_count = addVariable(uint32, "entry_count");
    Object* p_handler_type =
                             object().parent()
                           ->parent()
                           ->parent()
                           ->lookForType(module().getTemplate("HandlerBox")())
                           ->lookUp("handler_type", true);
    for(int i = 0; i < p_entry_count->value().toInteger(); ++i)
    {
        addVariable(SampleEntry(p_handler_type->value()));
    }
}

Mp4PixelAspectRatioBoxParser::Mp4PixelAspectRatioBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4PixelAspectRatioBoxParser::doParse()
{
    addVariable(uint32, "hSpacing");
    addVariable(uint32, "vSpacing");
}

Mp4CleanApertureBoxParser::Mp4CleanApertureBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4CleanApertureBoxParser::doParse()
{
    addVariable(uint32, "cleanApertureWidthN");
    addVariable(uint32, "cleanApertureWidthD");

    addVariable(uint32, "cleanApertureHeightN");
    addVariable(uint32, "cleanApertureHeightD");

    addVariable(uint32, "horizOffN");
    addVariable(uint32, "horizOffD");

    addVariable(uint32, "vertOffN");
    addVariable(uint32, "vertOffD");
}

Mp4BitRateBoxParser::Mp4BitRateBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4BitRateBoxParser::doParse()
{
    addVariable(uint32, "bufferSizeDB");
    addVariable(uint32, "maxBitrate");
    addVariable(uint32, "avgBitrate");
}

Mp4DegradationPriorityBoxParser::Mp4DegradationPriorityBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4DegradationPriorityBoxParser::doParse()
{
    Object *p_sample_size_box = object().parent()->lookForType(module().getTemplate("SampleSizeBox")());
    if(p_sample_size_box == NULL) p_sample_size_box = object().parent()->lookForType(module().getTemplate("CompactSampleSizeBox")());
    Object *p_sample_count = p_sample_size_box->lookUp("sample_count", true);
    addVariable(tuple(uint16, p_sample_count->value().toInteger()), "priority");
}

Mp4SampleScaleBoxParser::Mp4SampleScaleBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4SampleScaleBoxParser::doParse()
{
    addVariable(bitset(8), "constraint_flag");
    addVariable(uint8,  "scale_method");
    addVariable(int16,  "display_center_x");
    addVariable(int16,  "display_center_y");
}

Mp4TimeToSampleBoxParser::Mp4TimeToSampleBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4TimeToSampleBoxParser::doParse()
{
    Object* p_entry_count = addVariable(uint32, "entry_count");
    addVariable(tuple(module().getTemplate("TimeToSampleEntry")(), p_entry_count->value().toInteger()));
}

Mp4CompositionOffsetBoxParser::Mp4CompositionOffsetBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4CompositionOffsetBoxParser::doParse()
{
    Object* p_entry_count = addVariable(uint32, "entry_count");
    addVariable(tuple(module().getTemplate("CompositionOffsetEntry")(), p_entry_count->value().toInteger()));
}

Mp4SyncSampleBoxParser::Mp4SyncSampleBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4SyncSampleBoxParser::doParse()
{
    Object* p_entry_count = addVariable(uint32, "entry_count");
    addVariable(tuple(uint32, p_entry_count->value().toInteger()), "sample_numbers");
}

Mp4ShadowedSyncSampleBoxParser::Mp4ShadowedSyncSampleBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ShadowedSyncSampleBoxParser::doParse()
{
    Object* p_entry_count = addVariable(uint32, "entry_count");
    addVariable(tuple(module().getTemplate("ShadowedSyncSampleEntry")(), p_entry_count->value().toInteger()));
}

Mp4SampleDependencyTypeBoxParser::Mp4SampleDependencyTypeBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4SampleDependencyTypeBoxParser::doParse()
{
    addVariable(bitset(8), "sample_dependancy_flags");
}

Mp4EditListBoxParser::Mp4EditListBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4EditListBoxParser::doParse()
{
    bool version = lookUp("version")->value().toInteger();
    Object* p_entry_count = addVariable(uint32, "entry_count");
    Object* p_time_scale =    object().parent()
                         -> parent()
                         -> parent()
                         -> lookForType(module().getTemplate("MovieHeaderBox")(), true)
                         -> lookUp("time_scale", true);
    addForLoop(p_entry_count->value().toInteger(),[&](ContainerParser& loop, int64_t)
    {
        loop.addVariable(Duration((version)? 64 : 32, p_time_scale->value()), "segment_duration");
        loop.addVariable((version)?int64:int32, "media_time");
        loop.addVariable(uint16, "media_rate_integer");
        loop.addVariable(uint16, "media_rate_fraction");
    });
}

Mp4DataReferenceBoxParser::Mp4DataReferenceBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4DataReferenceBoxParser::doParse()
{
    Object* p_entry_count = addVariable(uint32, "entry_count");

    addForLoop(p_entry_count->value().toInteger(),[&](ContainerParser& loop, int64_t)
    {
        loop.addVariable(Box(), "data_entry");
    });
}

Mp4DataEntryUrlBoxParser::Mp4DataEntryUrlBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4DataEntryUrlBoxParser::doParse()
{
    addVariable(string(), "location");
}

Mp4DataEntryUrnBoxParser::Mp4DataEntryUrnBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4DataEntryUrnBoxParser::doParse()
{
    addVariable(string(), "name");
    addVariable(string(), "location");
}

Mp4SampleSizeBoxParser::Mp4SampleSizeBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4SampleSizeBoxParser::doParse()
{
    Object* p_sample_size = addVariable(uint32, "sample_size");
    Object* p_sample_count = addVariable(uint32, "sample_count");
    if(p_sample_size->value().toInteger() == 0)
        addVariable(tuple(int32, p_sample_count->value().toInteger()), "sample_sizes");
}

Mp4CompactSampleSizeBoxParser::Mp4CompactSampleSizeBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4CompactSampleSizeBoxParser::doParse()
{
    addVariable(bitset(8), "reserved");
    Object* p_field_size = addVariable(uint8, "sample_count");
    Object* p_sample_count = addVariable(uint32, "sample_count");
    addVariable(tuple(uinteger(p_field_size->value()), p_sample_count->value().toUnsignedInteger()), "entry_sizes");

    if(p_field_size->value().toInteger() == 4 && p_sample_count->value().toInteger() % 2 == 1)
        addVariable(uinteger(4), "reserved");
}

Mp4SampleToChunkBoxParser::Mp4SampleToChunkBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4SampleToChunkBoxParser::doParse()
{
    Object* p_entry_count = addVariable(uint32, "entry_count");
    addVariable(tuple(module().getTemplate("SampleToChunkEntry")(), p_entry_count->value().toInteger()), "sample_to_chunks");
}

Mp4ChunkOffsetBoxParser::Mp4ChunkOffsetBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ChunkOffsetBoxParser::doParse()
{
    Object* p_entry_count = addVariable(uint32, "entry_count");
    addVariable(tuple(uint32, p_entry_count->value().toInteger()), "chunk_offsets");
}

Mp4ChunkLargeOffsetBoxParser::Mp4ChunkLargeOffsetBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ChunkLargeOffsetBoxParser::doParse()
{
    Object* p_entry_count = addVariable(uint32, "entry_count");
    addVariable(tuple(uint64, p_entry_count->value().toInteger()), "chunk_offsets");
}

Mp4PaddingBitsBoxParser::Mp4PaddingBitsBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4PaddingBitsBoxParser::doParse()
{
    Object* p_sample_count = addVariable(uint32, "sample_count");
    addVariable(tuple(bitset(8), p_sample_count->value().toInteger()), "pads");
}

Mp4SubSampleInformationBoxParser::Mp4SubSampleInformationBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4SubSampleInformationBoxParser::doParse()
{
    bool version = lookUp("version")->value().toInteger();
    Object* p_entry_count = addVariable(uint32, "entry_count");
    addForLoop(p_entry_count->value().toInteger(),[&](ContainerParser& loop, int64_t)
    {
        loop.addVariable(uint32, "sample_delta");
        Object* p_subsample_count = loop.addVariable(uint32, "subsample_count");
        loop.addForLoop(p_subsample_count->value().toInteger(), [&](ContainerParser& subloop, int64_t)
        {
            subloop.addVariable((version)?uint32:uint16, "subsample_size");
            subloop.addVariable(uint8, "subsample_priority");
            subloop.addVariable(uint8, "discardable");
            subloop.addVariable(uint32, "reserved");
        });
    });
}

Mp4MovieExtendsHeaderBoxParser::Mp4MovieExtendsHeaderBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4MovieExtendsHeaderBoxParser::doParse()
{
    addVariable((lookUp("version")->value().toInteger())?uint64:uint32 , "fragment_duration");
}

Mp4TrackExtendsBoxParser::Mp4TrackExtendsBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4TrackExtendsBoxParser::doParse()
{
    addVariable(uint32 , "track_ID");
    addVariable(uint32 , "default_sample_description_index");
    addVariable(uint32 , "default_sample_duration");
    addVariable(uint32 , "default_sample_size");
    addVariable(uint32 , "default_sample_flags");
}

Mp4MovieFragmentHeaderBoxParser::Mp4MovieFragmentHeaderBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4MovieFragmentHeaderBoxParser::doParse()
{
    addVariable(uint32 , "sequence_number");
}

Mp4TrackFragmentHeaderBoxParser::Mp4TrackFragmentHeaderBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4TrackFragmentHeaderBoxParser::doParse()
{
    addVariable(uint32 , "track_ID");
    Object* p_flags = lookUp("flags");

    if(p_flags->value().toUnsignedInteger() & 0x000001)
        addVariable(uint64, "base_data_offset");
    if(p_flags->value().toUnsignedInteger() & 0x000002)
        addVariable(uint32, "sample_description_index");
    if(p_flags->value().toUnsignedInteger() & 0x000008)
        addVariable(uint32, "default_sample_description");
    if(p_flags->value().toUnsignedInteger() & 0x000010)
        addVariable(uint32, "default_sample_size");
    if(p_flags->value().toUnsignedInteger() & 0x000020)
        addVariable(uint32, "default_sample_flags");
}

Mp4TrackRunBoxParser::Mp4TrackRunBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4TrackRunBoxParser::doParse()
{
    Object* p_flags = lookUp("flags");

    Object* p_sample_count = addVariable(uint32, "sample_count");

    if(p_flags->value().toUnsignedInteger() & 0x000001)
        addVariable(uint32, "data_offset_present");
    if(p_flags->value().toUnsignedInteger() & 0x000004)
        addVariable(uint32, "first_sample_flags");

    addForLoop(p_sample_count->value().toInteger(), [&](ContainerParser& loop, int64_t)
    {
        if(p_flags->value().toUnsignedInteger() & 0x000100)
            loop.addVariable(uint32, "sample_duration");
        if(p_flags->value().toUnsignedInteger() & 0x000200)
            loop.addVariable(uint32, "sample_size");
        if(p_flags->value().toUnsignedInteger() & 0x000400)
            loop.addVariable(uint32, "sample_flags");
        if(p_flags->value().toUnsignedInteger() & 0x000800)
            loop.addVariable(uint32, "sample_composition_time_offset");
    });
}

Mp4TrackFragmentRandomAccessBoxParser::Mp4TrackFragmentRandomAccessBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4TrackFragmentRandomAccessBoxParser::doParse()
{
    bool version = lookUp("version")->value().toInteger();
    addVariable(uint32 , "track_ID");
    addVariable(bitset(26), "reserved");
    Object* p_length_size_of_traf_num = addVariable(uinteger(2), "length_size_of_traf_num");
    Object* p_length_size_of_trun_num = addVariable(uinteger(2), "length_size_of_trun_num");
    Object* p_length_size_of_sample_num = addVariable(uinteger(2), "length_size_of_sample_num");
    Object* p_number_of_entry = addVariable(uint32, "number_of_entry");

    addForLoop(p_number_of_entry->value().toInteger(), [&](ContainerParser& loop, int64_t)
    {
        loop.addVariable((version)?uint64:uint32, "time");
        loop.addVariable((version)?uint64:uint32, "moof_offset");
        loop.addVariable(uinteger(p_length_size_of_traf_num->value().toInteger()+1), "traf_number");
        loop.addVariable(uinteger(p_length_size_of_trun_num->value().toInteger()+1), "trun_number");
        loop.addVariable(uinteger(p_length_size_of_sample_num->value().toInteger()+1), "sample_number");
    });
}

Mp4MovieFragmentRandomAccessOffsetBoxParser::Mp4MovieFragmentRandomAccessOffsetBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4MovieFragmentRandomAccessOffsetBoxParser::doParse()
{
    addVariable(uint32, "mfra_size");
}

Mp4SampleToGroupBoxParser::Mp4SampleToGroupBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4SampleToGroupBoxParser::doParse()
{
    addVariable(uint32, "grouping_type");
    Object* p_number_of_entry = addVariable(uint32, "number_of_entry");
    addVariable(tuple(module().getTemplate("Mp4SampleToGroupEntry")(), p_number_of_entry->value().toInteger()));
}

Mp4CopyrightBoxParser::Mp4CopyrightBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4CopyrightBoxParser::doParse()
{
    addVariable(Language(), "langage");
    addVariable(uint16,"predefined");
    addVariable(string(),"notice");
}

Mp4TrackSelectionBoxParser::Mp4TrackSelectionBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4TrackSelectionBoxParser::doParse()
{
    addVariable(int32, "switch_group");
    addVariable(array(string(4)), "attribute_list");
}

Mp4XMLBoxParser::Mp4XMLBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4XMLBoxParser::doParse()
{
    addVariable(string(),"notice");
}

Mp4BinaryXMLBoxParser::Mp4BinaryXMLBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4BinaryXMLBoxParser::doParse()
{
    addVariable(data(),"data");
}

Mp4ItemLocationBoxParser::Mp4ItemLocationBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ItemLocationBoxParser::doParse()
{
    addVariable(uinteger(4), "reserved");
    Object* p_offset_size = addVariable(uinteger(4), "offset_size");
    Object* p_length_size = addVariable(uinteger(4), "length_size");
    Object* p_base_offset_size = addVariable(uinteger(4), "base_offset_size");
    Object* p_item_count = addVariable(uint16, "item_count");

    addForLoop(p_item_count->value().toInteger(), [&](ContainerParser loop, int64_t)
    {
        loop.addVariable(uint16, "item_ID");
        loop.addVariable(uint16, "data_reference_index");
        loop.addVariable(uinteger(p_base_offset_size->value().toInteger()), "base_offset");
        Object* p_extent_count = loop.addVariable(uint16, "extent_count");

        addForLoop(p_extent_count->value().toInteger(), [&](ContainerParser subloop, int64_t)
        {
            subloop.addVariable(uinteger(p_offset_size->value().toInteger()), "extent_offset");
            subloop.addVariable(uinteger(p_length_size->value().toInteger()), "extent_length");
        });
    });
}

Mp4PrimaryItemBoxParser::Mp4PrimaryItemBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4PrimaryItemBoxParser::doParse()
{
    addVariable(uint16,"item_ID");
}

Mp4ItemProtectionBoxParser::Mp4ItemProtectionBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ItemProtectionBoxParser::doParse()
{
    Object* p_protection_count = addVariable(uint16, "protection_count");

    addForLoop(p_protection_count->value().toInteger(), [&](ContainerParser& loop, int64_t)
    {
        loop.addVariable(Box(), "protection_information");
    });
}

Mp4FDItemInfoExtensionParser::Mp4FDItemInfoExtensionParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4FDItemInfoExtensionParser::doParse()
{

}

Mp4ItemInfoBoxParser::Mp4ItemInfoBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ItemInfoBoxParser::doParse()
{
    Object* p_entry_count = addVariable(uint16, "entry_count");

    addForLoop(p_entry_count->value().toInteger(), [&](ContainerParser& loop, int64_t)
    {
        loop.addVariable(Box(), "item_infos");
    });
}

Mp4ItemInfoEntryParser::Mp4ItemInfoEntryParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ItemInfoEntryParser::doParse()
{
    addVariable(uint16, "item_ID");
    addVariable(string(), "item_name");
    addVariable(string(), "content_type");
    addVariable(string(), "content_encoding");
    if(lookUp("version")->value().toInteger() == 1 && availableSize()>0)
    {
        Object* p_extension_type = addVariable(string(4),"extension_type");

        if(p_extension_type->value().toString() == "fdel")
        {
            addVariable(string(), "content_location");
            addVariable(string(), "content_MD5");
            addVariable(uint64, "content_length");
            addVariable(uint64, "transfert_length");
            Object* p_entry_count = addVariable(uint8, "entry_count");
            addVariable(tuple(uint32, p_entry_count->value().toInteger()), "group_ids");
        }
    }
}

Mp4ItunesDescriptionBoxParser::Mp4ItunesDescriptionBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ItunesDescriptionBoxParser::doParse()
{
    addVariable(array(module().getTemplate("ItunesTagBox")()), "tags");
}

Mp4ItunesTagBoxParser::Mp4ItunesTagBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ItunesTagBoxParser::doParse()
{
    addVariable(module().getTemplate("ItunesDataBox")(), "data");
}

Mp4ItunesDataBoxParser::Mp4ItunesDataBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4ItunesDataBoxParser::doParse()
{
    Object* p_data_type = addVariable(uint32,"data_type");

    addVariable(uint32, "reserved");

    std::string tag_type = object().parent()->lookUp("type")->value().toString();
    if((p_data_type->value().toInteger() == 0) && (tag_type == "trkn" || tag_type == "disk"))
    {
        addVariable(uint32, "payload");
    }
    else if(p_data_type->value().toInteger() == 1)
    {
        addVariable(string(availableSize()/8), "payload");
    }
    else
    {
        addVariable(data(), "payload");
    }
}

Mp4AVCConfigurationBoxParser::Mp4AVCConfigurationBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4AVCConfigurationBoxParser::doParse()
{
    addVariable(uint8, "configurationVersion");
    addVariable(uint8, "AVCProfileIndication");
    addVariable(uint8, "profile_compatibility");
    addVariable(uint8, "AVCLevelIndication");
    addVariable(bitset(6), "reserved");
    addVariable(uinteger(2), "lengthSizeMinusOne");
    addVariable(bitset(3), "reserved");
    Object* p_numOfSequenceParameterSets = addVariable(uinteger(5), "numOfSequenceParameterSets");
    addForLoop(p_numOfSequenceParameterSets->value().toInteger(), [&](ContainerParser& loop, int64_t)
    {
        Object*  p_sequenceParameterSetLength = loop.addVariable(uint16, "sequenceParameterSetLength");
        loop.addVariable(data(8*p_sequenceParameterSetLength->value().toInteger())  , "sequenceParameterSetNALUnit");
    });

    Object* p_numOfPictureParameterSets = addVariable(uinteger(8), "numOfPictureParameterSets");
    addForLoop(p_numOfPictureParameterSets->value().toInteger(), [&](ContainerParser& loop, int64_t)
    {
        Object*  p_pictureParameterSetLength = loop.addVariable(uint16, "pictureParameterSetLength");
        loop.addVariable(data(8*p_pictureParameterSetLength->value().toInteger())  , "pictureParameterSetNALUnit");
    });

}

Mp4MetaboxRelationBoxParser::Mp4MetaboxRelationBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4MetaboxRelationBoxParser::doParse()
{
    addVariable(uint32, "first_metabox_handler_type");
    addVariable(uint32, "second_metabox_handler_type");
    addVariable(uint8,  "metabox_relation");
}

Mp4OriginalFormatBoxParser::Mp4OriginalFormatBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4OriginalFormatBoxParser::doParse()
{
    addVariable(string(4), "data_format");
}

Mp4IPMPInfoBoxParser::Mp4IPMPInfoBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4IPMPInfoBoxParser::doParse()
{
    //ToDo  ISO/IEC 14496-1
}

Mp4IPMPControlBoxParser::Mp4IPMPControlBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4IPMPControlBoxParser::doParse()
{
    //ToDo
}

Mp4SchemeInformationBoxParser::Mp4SchemeInformationBoxParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void Mp4SchemeInformationBoxParser::doParse()
{
    addVariable(uint32, "scheme_type");
    addVariable(uint32, "scheme_version");
    if(lookUp("flags")->value().toUnsignedInteger() & 0x000001)
    {
        addVariable(string(availableSize()), "scheme_uri");
    }
}


Mp4ProgressiveDownloadInfoEntryParser::Mp4ProgressiveDownloadInfoEntryParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void Mp4ProgressiveDownloadInfoEntryParser::doParseHead()
{
    setSize(64);
    addVariable(uint32, "rate");
    addVariable(uint32, "initial_delay");
}


Mp4TimeToSampleEntryParser::Mp4TimeToSampleEntryParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void Mp4TimeToSampleEntryParser::doParseHead()
{
    setSize(64);
    addVariable(uint32, "sample_count");
    addVariable(uint32, "sample_delta");
}


Mp4CompositionOffsetEntryParser::Mp4CompositionOffsetEntryParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void Mp4CompositionOffsetEntryParser::doParseHead()
{
    setSize(64);
    addVariable(uint32, "sample_count");
    addVariable(uint32, "sample_offset");
}


Mp4ShadowedSyncSampleEntryParser::Mp4ShadowedSyncSampleEntryParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void Mp4ShadowedSyncSampleEntryParser::doParseHead()
{
    setSize(64);
    addVariable(uint32, "shadowed_sample_number");
    addVariable(uint32, "sync_sample_number");
}


Mp4SampleToGroupEntryParser::Mp4SampleToGroupEntryParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void Mp4SampleToGroupEntryParser::doParseHead()
{
    setSize(64);
    addVariable(uint32, "sample_count");
    addVariable(uint32, "group_description_index");
}


Mp4SampleToChunkEntryParser::Mp4SampleToChunkEntryParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void Mp4SampleToChunkEntryParser::doParseHead()
{
    setSize(96);
    addVariable(uint32, "first_chunk");
    addVariable(uint32, "sample_per_chunk");
    addVariable(uint32, "sample_description_index");
}
