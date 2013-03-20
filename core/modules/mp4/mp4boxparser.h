#ifndef MP4BOXPARSER_H
#define MP4BOXPARSER_H

#include "containerparser.h"

/*!
 * \brief The Mp4BoxParser class
 */
class Mp4BoxParser : public ContainerParser
{
public:
    Mp4BoxParser(Object& object, const Module& module);
protected:
    void doParseHead();
};

class Mp4FullBoxParser : public ContainerParser
{
public:
    Mp4FullBoxParser(Object& object, const Module& module);
protected:
    void doParseHead();
};

class Mp4ContainerParser : public ContainerParser
{
public:
    Mp4ContainerParser(Object& object, const Module& module);
protected:
    void doParse();
    bool doParseSome(int hint);
};

class Mp4FileTypeBoxParser : public ContainerParser
{
public:
    Mp4FileTypeBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4MovieHeaderBoxParser : public ContainerParser
{
public:
    Mp4MovieHeaderBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ProgressiveDownloadInfoBoxParser : public ContainerParser
{
public:
    Mp4ProgressiveDownloadInfoBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4FreeSpaceBoxParser : public ContainerParser
{
public:
    Mp4FreeSpaceBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4MediaDataBoxParser : public ContainerParser
{
public:
    Mp4MediaDataBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4TrackHeaderBoxParser : public ContainerParser
{
public:
    Mp4TrackHeaderBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4TrackReferenceTypeBoxParser : public ContainerParser
{
public:
    Mp4TrackReferenceTypeBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4MediaHeaderBoxParser : public ContainerParser
{
public:
    Mp4MediaHeaderBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4HandlerBoxParser : public ContainerParser
{
public:
    Mp4HandlerBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4VideoMediaHeaderBoxParser : public ContainerParser
{
public:
    Mp4VideoMediaHeaderBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4SoundMediaHeaderBoxParser : public ContainerParser
{
public:
    Mp4SoundMediaHeaderBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4HintMediaHeaderBoxParser : public ContainerParser
{
public:
    Mp4HintMediaHeaderBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4NullMediaHeaderBoxParser : public ContainerParser
{
public:
    Mp4NullMediaHeaderBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4SampleDescriptionBoxParser : public ContainerParser
{
public:
    Mp4SampleDescriptionBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4PixelAspectRatioBoxParser : public ContainerParser
{
public:
    Mp4PixelAspectRatioBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4CleanApertureBoxParser : public ContainerParser
{
public:
    Mp4CleanApertureBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4BitRateBoxParser : public ContainerParser
{
public:
    Mp4BitRateBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4DegradationPriorityBoxParser : public ContainerParser
{
public:
    Mp4DegradationPriorityBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4SampleScaleBoxParser : public ContainerParser
{
public:
    Mp4SampleScaleBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4TimeToSampleBoxParser : public ContainerParser
{
public:
    Mp4TimeToSampleBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4CompositionOffsetBoxParser : public ContainerParser
{
public:
    Mp4CompositionOffsetBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4SyncSampleBoxParser : public ContainerParser
{
public:
    Mp4SyncSampleBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ShadowedSyncSampleBoxParser : public ContainerParser
{
public:
    Mp4ShadowedSyncSampleBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4SampleDependencyTypeBoxParser : public ContainerParser
{
public:
    Mp4SampleDependencyTypeBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4EditListBoxParser : public ContainerParser
{
public:
    Mp4EditListBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4DataReferenceBoxParser : public ContainerParser
{
public:
    Mp4DataReferenceBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4DataEntryUrlBoxParser : public ContainerParser
{
public:
    Mp4DataEntryUrlBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4DataEntryUrnBoxParser : public ContainerParser
{
public:
    Mp4DataEntryUrnBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4SampleSizeBoxParser : public ContainerParser
{
public:
    Mp4SampleSizeBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4CompactSampleSizeBoxParser : public ContainerParser
{
public:
    Mp4CompactSampleSizeBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4SampleToChunkBoxParser : public ContainerParser
{
public:
    Mp4SampleToChunkBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ChunkOffsetBoxParser : public ContainerParser
{
public:
    Mp4ChunkOffsetBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ChunkLargeOffsetBoxParser : public ContainerParser
{
public:
    Mp4ChunkLargeOffsetBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4PaddingBitsBoxParser : public ContainerParser
{
public:
    Mp4PaddingBitsBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4SubSampleInformationBoxParser : public ContainerParser
{
public:
    Mp4SubSampleInformationBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4MovieExtendsHeaderBoxParser : public ContainerParser
{
public:
    Mp4MovieExtendsHeaderBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4TrackExtendsBoxParser : public ContainerParser
{
public:
    Mp4TrackExtendsBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4MovieFragmentHeaderBoxParser : public ContainerParser
{
public:
    Mp4MovieFragmentHeaderBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4TrackFragmentHeaderBoxParser : public ContainerParser
{
public:
    Mp4TrackFragmentHeaderBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4TrackRunBoxParser : public ContainerParser
{
public:
    Mp4TrackRunBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4TrackFragmentRandomAccessBoxParser : public ContainerParser
{
public:
    Mp4TrackFragmentRandomAccessBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4MovieFragmentRandomAccessOffsetBoxParser : public ContainerParser
{
public:
    Mp4MovieFragmentRandomAccessOffsetBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4SampleToGroupBoxParser : public ContainerParser
{
public:
    Mp4SampleToGroupBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4CopyrightBoxParser : public ContainerParser
{
public:
    Mp4CopyrightBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4TrackSelectionBoxParser : public ContainerParser
{
public:
    Mp4TrackSelectionBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4XMLBoxParser : public ContainerParser
{
public:
    Mp4XMLBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4BinaryXMLBoxParser : public ContainerParser
{
public:
    Mp4BinaryXMLBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ItemLocationBoxParser : public ContainerParser
{
public:
    Mp4ItemLocationBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4PrimaryItemBoxParser : public ContainerParser
{
public:
    Mp4PrimaryItemBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ItemProtectionBoxParser : public ContainerParser
{
public:
    Mp4ItemProtectionBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4FDItemInfoExtensionParser : public ContainerParser
{
public:
    Mp4FDItemInfoExtensionParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ItemInfoBoxParser : public ContainerParser
{
public:
    Mp4ItemInfoBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ItemInfoEntryParser : public ContainerParser
{
public:
    Mp4ItemInfoEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ItunesDescriptionBoxParser : public ContainerParser
{
public:
    Mp4ItunesDescriptionBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ItunesTagBoxParser : public ContainerParser
{
public:
    Mp4ItunesTagBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ItunesDataBoxParser : public ContainerParser
{
public:
    Mp4ItunesDataBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4AVCConfigurationBoxParser : public ContainerParser
{
public:
    Mp4AVCConfigurationBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4MetaboxRelationBoxParser : public ContainerParser
{
public:
    Mp4MetaboxRelationBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4OriginalFormatBoxParser : public ContainerParser
{
public:
    Mp4OriginalFormatBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4IPMPInfoBoxParser : public ContainerParser
{
public:
    Mp4IPMPInfoBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4IPMPControlBoxParser : public ContainerParser
{
public:
    Mp4IPMPControlBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4SchemeInformationBoxParser : public ContainerParser
{
public:
    Mp4SchemeInformationBoxParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4ProgressiveDownloadInfoEntryParser : public ContainerParser
{
public:
    Mp4ProgressiveDownloadInfoEntryParser(Object& object, const Module& module);
protected:
    void doParseHead();
};

class Mp4TimeToSampleEntryParser : public ContainerParser
{
public:
    Mp4TimeToSampleEntryParser(Object& object, const Module& module);
protected:
    void doParseHead();
};

class Mp4CompositionOffsetEntryParser : public ContainerParser
{
public:
    Mp4CompositionOffsetEntryParser(Object& object, const Module& module);
protected:
    void doParseHead();
};

class Mp4ShadowedSyncSampleEntryParser : public ContainerParser
{
public:
    Mp4ShadowedSyncSampleEntryParser(Object& object, const Module& module);
protected:
    void doParseHead();
};

class Mp4SampleToGroupEntryParser : public ContainerParser
{
public:
    Mp4SampleToGroupEntryParser(Object& object, const Module& module);
protected:
    void doParseHead();
};

class Mp4SampleToChunkEntryParser : public ContainerParser
{
public:
    Mp4SampleToChunkEntryParser(Object& object, const Module& module);
protected:
    void doParseHead();
};


#endif // MP4BOXPARSER_H
