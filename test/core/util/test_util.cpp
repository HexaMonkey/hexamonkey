
#include <sstream>

#include "test_util.h"
#include "core/util/csvreader.h"

void TestUtil::testCSVReader()
{
    std::stringstream input;
    input << "specification;master;0;" << std::endl;
    input << "executionBlock;master;1;" << std::endl;
    input << "argumentDeclarations;master;1;" << std::endl;
    CSVReader reader(input);
    QCOMPARE(reader.hasNextLine(), true);

    std::vector<std::string> fields;
    reader.readNextLine(fields);
    QCOMPARE(fields.size(), std::vector<std::string>::size_type(3));
    QCOMPARE(fields[0], std::string("specification"));
    QCOMPARE(fields[1], std::string("master"));
    QCOMPARE(fields[2], std::string("0"));
    QCOMPARE(reader.hasNextLine(), true);

    reader.ignoreNextLine();
    QCOMPARE(reader.hasNextLine(), true);

    fields.clear();
    reader.readNextLine(fields);
    QCOMPARE(fields.size(), std::vector<std::string>::size_type(3));
    QCOMPARE(fields[0], std::string("argumentDeclarations"));
    QCOMPARE(fields[1], std::string("master"));
    QCOMPARE(fields[2], std::string("1"));

    QCOMPARE(reader.hasNextLine(), false);
}

QTEST_MAIN(TestUtil)