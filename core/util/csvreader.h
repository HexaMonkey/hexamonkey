#ifndef CSVREADER_H
#define CSVREADER_H

#include <iostream>
#include <string>
#include <vector>

class CSVReader {
public:
    CSVReader(std::istream &input);
    bool hasNextLine();
    void readNextLine(std::vector<std::string> &fields);
    void ignoreNextLine();
private:
    std::istream &_input;
};

#endif // CSVREADER_H
