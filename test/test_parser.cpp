#include "test_parser.h"

#include "core/modules/default/defaulttypes.h"

#include "core/util/fileutil.h"

TestParser::TestParser()
{
    moduleSetup.setup();
}

void TestParser::test_asf()
{
    QVERIFY(checkFile("resources/parser/test_asf.asf", 3, 30));
}

bool TestParser::checkFile(const std::string &path, int depth, int width)
{
    RealFile file;
    file.setPath(path);

    if (!file.good())
    {
        std::cerr << "File not found" << std::endl;
        return false;
    }

    const Module& module = moduleSetup.moduleLoader().getModule(file);

    Object* object = module.handle(defaultTypes::file, file);

    if (!object) {
        return false;
    }

    const std::string origPath = path+".orig.txt";
    if (!fileExists(origPath)) {
        writeObject(*object, origPath, depth, width);
        return true;
    } else {
        const std::string newPath = path+".new.txt";
        writeObject(*object, newPath, depth, width);
        return fileCompare(origPath, newPath);
    }

}

void TestParser::writeObject(Object &object, const std::string &path, int depth, int width)
{
    std::ofstream file;
    file.open(path);

    writeObjectRecursive(object, file, 0, depth, width);
}

void TestParser::writeObjectRecursive(Object &object, std::ofstream &file, int currentDepth, int remainingDepth, int width)
{
    for (int i = currentDepth; i > 0; --i) {
        file << "    ";
    }

    file << object.type() << " " << object.name() << " = " << object.value() << "  @" << object.beginningPos() << "x" << object.size() << std::endl;

    if (width != -1) {
        while (!object.parsed() && object.numberOfChildren() < width) {
            object.exploreSome(width);
        }
    } else {
        object.explore();
    }

    int n = object.numberOfChildren();
    if (width != -1 && n > width) {
        n = width;
    }

    int nextCurrentDepth = currentDepth + 1;
    int nextRemainingDepth = remainingDepth - 1;

    for (int i = 0; i < n; ++i) {
        writeObjectRecursive(*object.access(i), file, nextCurrentDepth, nextRemainingDepth, width);
    }
}
