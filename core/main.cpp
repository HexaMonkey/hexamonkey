#include <iostream>
#include <iomanip>
#include <fstream>
#include "file.h"
#include "filedata.h"



int main(int argc, char *argv[])
{

	std::string fileName = "C:/sintel.mp4";

	if(argc>1)
	{
		fileName = argv[1];
	}

	File file(fileName);
	if (!file.is_open())
	{
		std::cerr << "File not found" <<std::endl;
		return 1;
	}
	FileData fileData(&file);
	fileData.setInfo(fileName);
	fileData.explore(-1);
	std::cout<<fileData;
	file.close();

	return 0;
}
