#include "mkvcontainerdata.h"
#include "standardtypes.h"

#include "mkvtypes.h"
using namespace standardTypes;
using namespace mkvTypes;

MkvContainerData::MkvContainerData(File *_file, const Context *_context) : DataContainer(_file, _context)
{
    Data* p_id = addVariable(largeInteger(), "id");
    Data* p_size = addVariable(largeInteger(), "size");


    setSize(8*p_size->value().toInteger()+p_id->size()+p_size->size());

    setType(EBMLElement(p_id->value()));
}

void MkvContainerData::parse()
{
    seekDataEnd();
    if(extends("MasterElement"))
    {
        while ((pos()<size()) && !file()->fail())
        {
            addVariable(EBMLElement());
        }

    }
    else if(extends("IntegerElement"))
    {
        addVariable(integer(size()-pos()),"payload");
    }
    else if(extends("UIntegerElement"))
    {
        addVariable(uinteger(size()-pos()),"payload");
    }
    else if(extends("StringElement")|| extends("UTF8StringElement"))
    {
        addVariable(string((size()-pos())/8),"payload");
    }
    else if(extends("BinaryElement"))
    {
        addVariable(data(size()-pos()), "payload");
    }
    else if(extends("DateElement"))
    {
        addVariable(mkvTypes::Date(), "payload");
    }
    else if(extends("FloatElement"))
    {
        if(size()-pos() == 32)
            addVariable(singleFloat(),"payload");
        else
            addVariable(doubleFloat(),"payload");
    }
}

bool MkvContainerData::parseSome(int hint)
{
    if(extends("MasterElement"))
    {
        int count = 0;
        while(true)
        {
            addVariable(EBMLElement());
            ++count;

            if(pos()>=size() || file()->fail())
                return true;

            if(count>=hint)
                return false;
        }
    }
    else
    {
        parse();
        return true;
    }
}
