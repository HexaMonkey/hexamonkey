#include "defaultmodule.h"

#include "defaulttypes.h"

#include "fileparser.h"
#include "arrayparser.h"
#include "tupleparser.h"

using namespace defaultTypes;

bool DefaultModule::doLoad()
{
    addTemplate(file);
    addParser("File", []parserLambda{return new FileParser(object);});

    addTemplate(array);
    addParser("Array", []parserLambda
    {
        if(type.parameterSpecified(0))
        {
            if(type.parameterSpecified(1))
                return new ArrayParser(object, module, type.parameterValue(0).toObjectType(), type.parameterValue(1).toInteger());
            else
                return new ArrayParser(object, module, type.parameterValue(0).toObjectType(), -1);
        }
        return nullptr;
    });
    setFixedSize("Array", [this]fixedSizeLambda
    {
         if(type.parameterSpecified(1))
         {
             int64_t s = type.parameterValue(1).toInteger();
             if(s>0)
                return s;
         }
         return -1;
    });

    addTemplate(tuple);
    addParser("Tuple", []parserLambda
    {
        if(type.parameterSpecified(0)  && type.parameterSpecified(1))
        {
            return new TupleParser(object, module, type.parameterValue(0).toObjectType(), type.parameterValue(1).toInteger());
        }
        return nullptr;
    });
    setFixedSize("Tuple", [this]fixedSizeLambda
    {
         if(type.parameterSpecified(0) && type.parameterSpecified(1))
         {
             int64_t t = module.getFixedSize(type.parameterValue(0).toObjectType());
             if(t>0)
                return t*type.parameterValue(1).toInteger();
         }
         return -1;
    });

    return true;
}
