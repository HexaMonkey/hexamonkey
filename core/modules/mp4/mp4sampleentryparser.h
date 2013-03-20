#ifndef MP4SAMPLEENTRYPARSER_H
#define MP4SAMPLEENTRYPARSER_H

#include "containerparser.h"

class Mp4SampleEntryParser : public ContainerParser
{
public:
    Mp4SampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4AudioSampleEntryParser : public ContainerParser
{
public:
    Mp4AudioSampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4VisualSampleEntryParser : public ContainerParser
{
public:
    Mp4VisualSampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4XMLMetaSampleEntryParser : public ContainerParser
{
public:
    Mp4XMLMetaSampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4TextMetaSampleEntryParser : public ContainerParser
{
public:
    Mp4TextMetaSampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

class Mp4HintSampleEntryParser : public ContainerParser
{
public:
    Mp4HintSampleEntryParser(Object& object, const Module& module);
protected:
    void doParse();
};

#endif // MP4SAMPLEENTRYPARSER_H
