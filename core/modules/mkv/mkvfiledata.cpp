#include "mkvfiledata.h"
#include "mkvtypes.h"

using namespace mkvTypes;

MkvFileData::MkvFileData(File *file, const Context *context) : FileData(file, context)
{

}

void MkvFileData::parse()
{
    while ((file()->tellg()<size()) && !file()->fail())
    {
        addVariable(EBMLElement());
    }
}
