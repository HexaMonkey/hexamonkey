#include "hmcmodule.h"

#include <fstream>

#include "csvreader.h"
#include "strutil.h"
#include "fileutil.h"

#include "objecttypetemplate.h"
#include "ebmltypes.h"

using namespace ebmlTypes;

void HmcModule::addFormatDetection(StandardFormatDetector::Adder &formatAdder)
{
    formatAdder.addMagicNumber("1a 45 df a3 xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx 68 65 78 61 6d 6f 6e 6b 65 79");
    formatAdder.addExtension("hmc");
}

void HmcModule::requestImportations(std::vector<std::string> &formatRequested)
{
    formatRequested.push_back("ebml");
}

bool HmcModule::doLoad()
{
    std::string path = "hmcmodel.csv";
    if(!fileExists(path))
    {
        path = "../core/modules/hmc/hmcmodel.csv";
    }
    else if(!fileExists(path))
    {
        std::cerr<<"HMC model file not found"<<std::endl;
        return false;
    }

    std::ifstream modelFile(path);
    CSVReader modelReader(modelFile);

    for(unsigned int i = 0;modelReader.hasNextLine();++i)
    {
        std::vector<std::string> modelLine;
        modelReader.readNextLine(modelLine);

        if(modelLine.size()<2)
            break;

        std::string name = modelLine[0];
        name[0]= toupper(name[0]);
        std::string type  = modelLine[1];

        addParser(name);
        const ObjectTypeTemplate& typeTemplate = newTemplate(name);
        setSpecification(EBMLElement(i), typeTemplate());

        for(int j =0;j<numberOfTypeElements;++j)
        {
            if(type == typeElementAtributes[j])
            {
                setExtension(typeTemplate, getTemplate(typeElements[j])());
                break;
            }
        }
    }
    return true;
}

