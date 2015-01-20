//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <fstream>

#include "core/objecttypetemplate.h"
#include "core/modules/ebml/ebmltypes.h"
#include "core/modules/hmc/hmcmodule.h"
#include "core/util/csvreader.h"
#include "core/util/strutil.h"
#include "core/util/fileutil.h"
#include "core/error/errormanager.h"

using namespace ebmlTypes;

HmcModule::HmcModule(std::string modelPath) : _modelPath(modelPath)
{
}

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

    if(!fileExists(_modelPath))
    {
        Log::error("HMC model file not found");
        return false;
    }

    std::ifstream modelFile(_modelPath);
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

