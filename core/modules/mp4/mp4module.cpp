#include "mp4module.h"

#include <fstream>

#include "csvreader.h"

#include "defaulttypes.h"
#include "standardtypes.h"
#include "mp4types.h"

#include "mp4boxparser.h"
#include "mp4sampleentryparser.h"
#include "mp4simpleparser.h"

#include "fileutil.h"

using namespace standardTypes;
using namespace mp4Types;

void Mp4Module::addFormatDetection(StandardFormatDetector::Adder &formatAdder)
{
    formatAdder.addMagicNumber("00 00 00 xx 66 74 79 70");
    formatAdder.addExtension("mp4");
    formatAdder.addExtension("mov");
    formatAdder.addExtension("m4a");
}

void Mp4Module::requestImportations(std::vector<std::string> &formatRequested)
{
    formatRequested.push_back("bestd");
}

bool Mp4Module::doLoad()
{
    addTemplate(Mp4File);
    setExtension(Mp4File, defaultTypes::file());
    setSpecification(defaultTypes::file(), Mp4File());
    addParser("Mp4File", []parserLambda{return new Mp4ContainerParser(object, module);});

    addTemplate(Box);
    addParser("Box", []parserLambda{return new Mp4BoxParser(object, module);});

    addTemplate(FullBox);
    addParser("FullBox", []parserLambda{return new Mp4FullBoxParser(object, module);});

    /*****Simple Data*****/
    addTemplate(Date);
    setExtension(Date, uinteger(), {{0,0}});
    addParser("Date", []parserLambda
    {
        if(type == Date32 || type == Date64)
            return new Mp4DateParser(object);
        return nullptr;
    });

    addTemplate(Duration);
    setExtension(Duration, uinteger(), {{0,0}});
    addParser("Duration", []parserLambda
    {
        if(type.parameterSpecified(0) && type.parameterSpecified(1))
        {
                int size = type.parameterValue(0).toInteger();
                if(size == 32 || size == 64)
                    return new Mp4DurationParser(object, type.parameterValue(1).toInteger());
        }
        return nullptr;
    });

    addTemplate(Language);
    setExtension(Language, uint16);
    addParser("Language", []parserLambda{return new Mp4LanguageParser(object);});


    /*****Sample Entries*****/
    addTemplate(SampleEntry);
    const ObjectTypeTemplate& AudioSampleEntry        = newTemplate("AudioSampleEntry",        {"box_type"});
    const ObjectTypeTemplate& VisualSampleEntry       = newTemplate("VisualSampleEntry",       {"box_type"});
    const ObjectTypeTemplate& HintSampleEntry         = newTemplate("HintSampleEntry",         {"box_type"});
    const ObjectTypeTemplate& MetaDataSampleEntry     = newTemplate("MetaDataSampleEntry",     {"box_type"});
    const ObjectTypeTemplate& XMLMetaDataSampleEntry  = newTemplate("XMLMetaDataSampleEntry");
    const ObjectTypeTemplate& TextMetaDataSampleEntry = newTemplate("TextMetaDataSampleEntry");

    addParser("SampleEntry", []parserLambda{return new Mp4SampleEntryParser(object, module);});
    addParser("AudioSampleEntry", []parserLambda{return new Mp4AudioSampleEntryParser(object, module);});
    addParser("VisualSampleEntry", []parserLambda{return new Mp4VisualSampleEntryParser(object, module);});
    addParser("MetaDataSampleEntry");
    addParser("XMLMetaDataSampleEntry", []parserLambda{return new Mp4XMLMetaSampleEntryParser(object, module);});
    addParser("TextMetaDataSampleEntry", []parserLambda{return new Mp4TextMetaSampleEntryParser(object, module);});

    setExtension(SampleEntry    , Box());
    setExtension(AudioSampleEntry       , SampleEntry("soun"));
    setExtension(VisualSampleEntry      , SampleEntry("vide"));
    setExtension(HintSampleEntry        , SampleEntry("hint"));
    setExtension(MetaDataSampleEntry    , SampleEntry("meta"));
    setExtension(XMLMetaDataSampleEntry , MetaDataSampleEntry("metx"));
    setExtension(TextMetaDataSampleEntry, MetaDataSampleEntry("mett"));

    setSpecification(SampleEntry("soun"), AudioSampleEntry());
    setSpecification(SampleEntry("vide"), VisualSampleEntry());
    setSpecification(SampleEntry("hint"), HintSampleEntry());
    setSpecification(SampleEntry("meta"), MetaDataSampleEntry());
    setSpecification(SampleEntry("meta", "metx"), XMLMetaDataSampleEntry());
    setSpecification(SampleEntry("meta", "mett"), TextMetaDataSampleEntry());

    /*****Little entries*****/
    newTemplate("ProgressiveDownloadInfoEntry");
    newTemplate("TimeToSampleEntry");
    newTemplate("CompositionOffsetEntry");
    newTemplate("ShadowedSyncSampleEntry");
    newTemplate("SampleToGroupEntry");
    newTemplate("SampleToChunkEntry");


    addParser("ProgressiveDownloadInfoEntry", []parserLambda{return new Mp4ProgressiveDownloadInfoEntryParser(object, module);});
    addParser("TimeToSampleEntry", []parserLambda{return new Mp4TimeToSampleEntryParser(object, module);});
    addParser("CompositionOffsetEntry", []parserLambda{return new Mp4CompositionOffsetEntryParser(object, module);});
    addParser("ShadowedSyncSampleEntry", []parserLambda{return new Mp4ShadowedSyncSampleEntryParser(object, module);});
    addParser("SampleToGroupEntry", []parserLambda{return new Mp4SampleToGroupEntryParser(object, module);});
    addParser("SampleToChunkEntry", []parserLambda{return new Mp4SampleToChunkEntryParser(object, module);});


    setFixedSize("ProgressiveDownloadInfoEntry", 64);
    setFixedSize("TimeToSampleEntry", 64);
    setFixedSize("CompositionOffsetEntry", 64);
    setFixedSize("ShadowedSyncSampleEntry", 64);
    setFixedSize("SampleToGroupEntry", 64);
    setFixedSize("SampleToChunkEntry", 96);

    /*****Itunes Boxes*****/
    const ObjectTypeTemplate& ItunesTagBox  = newTemplate("ItunesTagBox", {"box_type"});
    const ObjectTypeTemplate& ItunesDataBox = newTemplate("ItunesDataBox");

    addParser("ItunesDataBox", []parserLambda{return new Mp4ItunesDataBoxParser(object, module);});
    addParser("ItunesDescriptionBox", []parserLambda{return new Mp4ItunesDescriptionBoxParser(object, module);});

    setExtension(ItunesTagBox  , Box());
    setExtension(ItunesDataBox , Box());

    /*****Importing model*****/
    std::string path = "mp4model.csv";
    if(!fileExists(path))
    {
        path = "../core/modules/mp4/mp4model.csv";
    }
    else if(!fileExists(path))
    {
        std::cerr<<"MP4 model file not found"<<std::endl;
        return false;
    }
    std::ifstream modelFile(path);
    loadCSVFile(modelFile);

    /*****Container Boxes*****/
    addParser("MovieBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("TrackBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("TrackReferenceBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("MediaBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("MediaInformationBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("SampleTableBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("EditBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("DataInformationBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("MovieExtendsBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("MovieFragmentBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("TrackFragmentBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("MovieFragmentRandomAccessBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("UserDataBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("MetaBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("AdditionalMetadataContainerBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("ProtectionSchemeInfoBox", []parserLambda{return new Mp4ContainerParser(object, module);});
    addParser("SchemeTypeBox", []parserLambda{return new Mp4ContainerParser(object, module);});

    /*****Specific Boxes*****/
    addParser("FileTypeBox", []parserLambda{return new Mp4FileTypeBoxParser(object, module);});
    addParser("MovieHeaderBox", []parserLambda{return new Mp4MovieHeaderBoxParser(object, module);});
    addParser("ProgressiveDownloadInfoBox", []parserLambda{return new Mp4ProgressiveDownloadInfoBoxParser(object, module);});
    addParser("FreeSpaceBox", []parserLambda{return new Mp4FreeSpaceBoxParser(object, module);});
    addParser("MediaDataBox", []parserLambda{return new Mp4MediaDataBoxParser(object, module);});
    addParser("TrackHeaderBox", []parserLambda{return new Mp4TrackHeaderBoxParser(object, module);});
    addParser("TrackReferenceTypeBox", []parserLambda{return new Mp4TrackReferenceTypeBoxParser(object, module);});
    addParser("MediaHeaderBox", []parserLambda{return new Mp4MediaHeaderBoxParser(object, module);});
    addParser("HandlerBox", []parserLambda{return new Mp4HandlerBoxParser(object, module);});
    addParser("VideoMediaHeaderBox", []parserLambda{return new Mp4VideoMediaHeaderBoxParser(object, module);});
    addParser("SoundMediaHeaderBox", []parserLambda{return new Mp4SoundMediaHeaderBoxParser(object, module);});
    addParser("HintMediaHeaderBox", []parserLambda{return new Mp4HintMediaHeaderBoxParser(object, module);});
    addParser("NullMediaHeaderBox", []parserLambda{return new Mp4NullMediaHeaderBoxParser(object, module);});
    addParser("SampleDescriptionBox", []parserLambda{return new Mp4SampleDescriptionBoxParser(object, module);});
    addParser("PixelAspectRatioBox", []parserLambda{return new Mp4PixelAspectRatioBoxParser(object, module);});
    addParser("CleanApertureBox", []parserLambda{return new Mp4CleanApertureBoxParser(object, module);});
    addParser("BitRateBox", []parserLambda{return new Mp4BitRateBoxParser(object, module);});
    addParser("DegradationPriorityBox", []parserLambda{return new Mp4DegradationPriorityBoxParser(object, module);});
    addParser("SampleScaleBox", []parserLambda{return new Mp4SampleScaleBoxParser(object, module);});
    addParser("TimeToSampleBox", []parserLambda{return new Mp4TimeToSampleBoxParser(object, module);});
    addParser("CompositionOffsetBox", []parserLambda{return new Mp4CompositionOffsetBoxParser(object, module);});
    addParser("SyncSampleBox", []parserLambda{return new Mp4SyncSampleBoxParser(object, module);});
    addParser("ShadowedSyncSampleBox", []parserLambda{return new Mp4ShadowedSyncSampleBoxParser(object, module);});
    addParser("SampleDependencyTypeBox", []parserLambda{return new Mp4SampleDependencyTypeBoxParser(object, module);});
    addParser("EditListBox", []parserLambda{return new Mp4EditListBoxParser(object, module);});
    addParser("DataReferenceBox", []parserLambda{return new Mp4DataReferenceBoxParser(object, module);});
    addParser("DataEntryUrlBox", []parserLambda{return new Mp4DataEntryUrlBoxParser(object, module);});
    addParser("DataEntryUrnBox", []parserLambda{return new Mp4DataEntryUrnBoxParser(object, module);});
    addParser("SampleSizeBox", []parserLambda{return new Mp4SampleSizeBoxParser(object, module);});
    addParser("CompactSampleSizeBox", []parserLambda{return new Mp4CompactSampleSizeBoxParser(object, module);});
    addParser("SampleToChunkBox", []parserLambda{return new Mp4SampleToChunkBoxParser(object, module);});
    addParser("ChunkOffsetBox", []parserLambda{return new Mp4ChunkOffsetBoxParser(object, module);});
    addParser("ChunkLargeOffsetBox", []parserLambda{return new Mp4ChunkLargeOffsetBoxParser(object, module);});
    addParser("PaddingBitsBox", []parserLambda{return new Mp4PaddingBitsBoxParser(object, module);});
    addParser("SubSampleInformationBox", []parserLambda{return new Mp4SubSampleInformationBoxParser(object, module);});
    addParser("MovieExtendsHeaderBox", []parserLambda{return new Mp4MovieExtendsHeaderBoxParser(object, module);});
    addParser("TrackExtendsBox", []parserLambda{return new Mp4TrackExtendsBoxParser(object, module);});
    addParser("MovieFragmentHeaderBox", []parserLambda{return new Mp4MovieFragmentHeaderBoxParser(object, module);});
    addParser("TrackFragmentHeaderBox", []parserLambda{return new Mp4TrackFragmentHeaderBoxParser(object, module);});
    addParser("TrackRunBox", []parserLambda{return new Mp4TrackRunBoxParser(object, module);});
    addParser("TrackFragmentRandomAccessBox", []parserLambda{return new Mp4TrackFragmentRandomAccessBoxParser(object, module);});
    addParser("MovieFragmentRandomAccessOffsetBox", []parserLambda{return new Mp4MovieFragmentRandomAccessOffsetBoxParser(object, module);});
    addParser("SampleToGroupBox", []parserLambda{return new Mp4SampleToGroupBoxParser(object, module);});
    addParser("CopyrightBox", []parserLambda{return new Mp4CopyrightBoxParser(object, module);});
    addParser("TrackSelectionBox", []parserLambda{return new Mp4TrackSelectionBoxParser(object, module);});
    addParser("XMLBox", []parserLambda{return new Mp4XMLBoxParser(object, module);});
    addParser("BinaryXMLBox", []parserLambda{return new Mp4BinaryXMLBoxParser(object, module);});
    addParser("ItemLocationBox", []parserLambda{return new Mp4ItemLocationBoxParser(object, module);});
    addParser("PrimaryItemBox", []parserLambda{return new Mp4PrimaryItemBoxParser(object, module);});
    addParser("ItemProtectionBox", []parserLambda{return new Mp4ItemProtectionBoxParser(object, module);});
    addParser("FDItemInfoExtension", []parserLambda{return new Mp4FDItemInfoExtensionParser(object, module);});
    addParser("ItemInfoBox", []parserLambda{return new Mp4ItemInfoBoxParser(object, module);});
    addParser("ItemInfoEntry", []parserLambda{return new Mp4ItemInfoEntryParser(object, module);});
    addParser("ItunesDescriptionBox", []parserLambda{return new Mp4ItunesDescriptionBoxParser(object, module);});
    addParser("AVCConfigurationBox", []parserLambda{return new Mp4AVCConfigurationBoxParser(object, module);});
    addParser("MetaboxRelationBox", []parserLambda{return new Mp4MetaboxRelationBoxParser(object, module);});
    addParser("OriginalFormatBox", []parserLambda{return new Mp4OriginalFormatBoxParser(object, module);});
    addParser("IPMPInfoBox", []parserLambda{return new Mp4IPMPInfoBoxParser(object, module);});
    addParser("IPMPControlBox", []parserLambda{return new Mp4IPMPControlBoxParser(object, module);});
    addParser("SchemeInformationBox", []parserLambda{return new Mp4SchemeInformationBoxParser(object, module);});

    return true;
}

void Mp4Module::loadCSVFile(std::ifstream& modelFile)
{
    CSVReader modelReader(modelFile);
    modelReader.ignoreNextLine();
    while(modelReader.hasNextLine())
    {
        std::vector<std::string> modelLine;
        modelReader.readNextLine(modelLine);

        std::string boxType = modelLine[0];
        std::string boxName = modelLine[1];
        bool uniqueBoxName  = (modelLine[2] == "1");
        bool fullBox        = (modelLine[3] == "1");

        if(uniqueBoxName)
        {
            const ObjectTypeTemplate& typeTemplate = newTemplate(boxName);
            ObjectType objectType(typeTemplate());
            setSpecification(Box(boxType), objectType);
            if(fullBox)
            {
                setExtension(typeTemplate, FullBox(boxType));
            }
            else
            {
                setExtension(typeTemplate, Box(boxType));
            }
        }
        else
        {
            const ObjectTypeTemplate& typeTemplate = (hasTemplate(boxName))?getTemplate(boxName):newTemplate(boxName, {"box_type"});

            ObjectType objectType(typeTemplate(boxType));
            setSpecification(Box(boxType), objectType);
            if(fullBox)
            {
                setExtension(typeTemplate, FullBox(), {{0,0}});
            }
            else
            {
                setExtension(typeTemplate, Box(), {{0,0}});
            }
        }
    }
}
