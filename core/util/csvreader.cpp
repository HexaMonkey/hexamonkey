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

#include "core/util/csvreader.h"
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
