#include "csvreader.h"
#include <sstream>


CSVReader::CSVReader(std::istream &input) : _input(input)
{

}

bool CSVReader::hasNextLine()
{
    return !(_input.eof()||_input.fail()||_input.peek() == std::istream::traits_type::eof());
}

void CSVReader::readNextLine(std::vector<std::string> &fields)
{
    std::string line;
    std::getline(_input,line);
    if(!line.empty() && line[line.size()-1] == '\r')
    {
        line.resize(line.size()-1);
    }
    std::stringstream  lineStream(line);
    std::string        cell;
    while(std::getline(lineStream,cell,';'))
    {

        fields.push_back(cell);
    }
}

void CSVReader::ignoreNextLine()
{
    std::string line;
    std::getline(_input,line);
}
