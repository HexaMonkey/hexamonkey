#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

#include "../core/util/strutil.h"
#include "../core/util/csvreader.h"

const std::string modelFileName = "../models/hmcmodel.csv";
const std::string opFileName = "../models/hmcoperators.csv";

int main()
{
	
	std::ifstream modelFile(modelFileName.c_str());
	std::ofstream headerFile("model.h");
	CSVReader modelReader(modelFile);

	headerFile<<"#ifndef MODEL_H"<<std::endl;
	headerFile<<"#define MODEL_H"<<std::endl;
	headerFile<<"/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*\\"<<std::endl;
	headerFile<<"File automaticaly generated from "<<modelFileName<<" and "<<std::endl;
	headerFile<<opFileName<<". Do not modify directly"<<std::endl;
	headerFile<<"\\*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/"<<std::endl;
	headerFile<<std::endl;
	
	headerFile<<"#define MASTER   0"<<std::endl;
	headerFile<<"#define INTEGER  1"<<std::endl;
	headerFile<<"#define UINTEGER 2"<<std::endl;
	headerFile<<"#define STRING   3"<<std::endl;
	headerFile<<"#define FLOAT    4"<<std::endl;
    headerFile<<std::endl;
	
	headerFile<<"#define INT_TYPE    0"<<std::endl;
	headerFile<<"#define UINT_TYPE   1"<<std::endl;
	headerFile<<"#define STRING_TYPE 2"<<std::endl;
	headerFile<<"#define FLOAT_TYPE  3"<<std::endl;
    headerFile<<std::endl;
	
	std::vector<std::string> names;
	std::vector<std::string> types;
	std::vector<bool> assoc;
    for(unsigned int i = 0; modelReader.hasNextLine(); ++i)
    {
        std::vector<std::string> modelLine;
        modelReader.readNextLine(modelLine);

		if(modelLine.size()<3)
			break;
			
        std::string name = modelLine[0];
		std::string macroName = defineStyle(name);
		
        std::string type  = modelLine[1];
		std::transform(type.begin(), type.end(),type.begin(), ::toupper);
		
		headerFile<<"#define "<<macroName<<" "<<i<<std::endl;
		names.push_back(name);
		types.push_back(type);
		assoc.push_back(modelLine[2] == "1");
		
		if(name == "root")
			headerFile<<"#define ROOT_ID "<<i<<std::endl; 
    }
	headerFile<<std::endl;

	std::ifstream opFile(opFileName.c_str());
	CSVReader opReader(opFile);
	
	std::vector<int> parameterCounts;
	std::vector<int> parameterReleases;
    for(unsigned int i = 0; opReader.hasNextLine(); ++i)
    {
        std::vector<std::string> opLine;
        opReader.readNextLine(opLine);

		if(opLine.size()<3)
			break;
				
		headerFile<<"#define "<<defineStyle(opLine[0])<<"_OP "<<i<<std::endl;
		parameterCounts.push_back(strTo<int>(opLine[1]));
		parameterReleases.push_back(strTo<int>(opLine[2]));
    }
	headerFile<<std::endl;

	headerFile<<"const int hmcElemTypes[] = {";
	for(unsigned int i =0; ; ++i)
	{
		headerFile<<types[i];
		if(i >= types.size() - 1)
			break;
		headerFile<<", ";
	}
	headerFile<<"};"<<std::endl;
	
	headerFile<<"static const char* hmcElemNames[] = {";
	for(unsigned int i =0; ; ++i)
	{
		headerFile<<"\""<<names[i]<<"\"";
		if(i >= names.size() - 1)
			break;
		headerFile<<", ";
	}
	headerFile<<"};"<<std::endl;
	
	headerFile<<"const int hmcElemAssoc[] = {";
	for(unsigned int i =0; ; ++i)
	{
		headerFile<<assoc[i];
		if(i >= assoc.size() - 1)
			break;
		headerFile<<", ";
	}
	headerFile<<"};"<<std::endl;
	
	headerFile<<"const int operatorParameterCount[] = {";
	for(unsigned int i =0; ; ++i)
	{
		headerFile<<parameterCounts[i];
		if(i >= parameterCounts.size() - 1)
			break;
		headerFile<<", ";
	}
	headerFile<<"};"<<std::endl;
	
	headerFile<<"const int operatorParameterRelease[] = {";
	for(unsigned int i =0; ; ++i)
	{
		headerFile<<parameterReleases[i];
		if(i >= parameterReleases.size() - 1)
			break;
		headerFile<<", ";
	}
	headerFile<<"};"<<std::endl;
	
	headerFile<<"#endif"<<std::endl;
	
	return 0;
}
