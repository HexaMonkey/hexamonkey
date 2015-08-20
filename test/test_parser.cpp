#include "test_parser.h"

#include "core/modules/default/defaultmodule.h"
#include "core/variable/variablecollector.h"

#include "core/util/fileutil.h"
#include "core/log/logmanager.h"

TestParser::TestParser() : path("resources/parser/")
{
    moduleSetup.addScriptDirectory(path+"scripts/");
    moduleSetup.setup();

}

void TestParser::test_default()
{
    QVERIFY(checkFile("test_default.bin", -1, -1, "test_default"));
}

void TestParser::test_find()
{
    QVERIFY(checkFile("test_find.bin", -1, -1, "test_find"));
}

void TestParser::test_asf()
{
    QVERIFY(checkFile("test_asf.asf", 3, 22));
}

void TestParser::test_avi()
{
    QVERIFY(checkFile("test_avi.avi", -1, 15));
}

void TestParser::test_bmp()
{
    QVERIFY(checkFile("test_bmp_4.bmp"));
    QVERIFY(checkFile("test_bmp_16b565.bmp"));
    QVERIFY(checkFile("test_bmp_24.bmp"));
    QVERIFY(checkFile("test_bmp_32b8888.bmp"));
}

void TestParser::test_gif()
{
    QVERIFY(checkFile("test_gif.gif", -1, 20));
}

void TestParser::test_jpg()
{
    QVERIFY(checkFile("test_jpg.jpg", -1, 20));
}

void TestParser::test_pe()
{
    QVERIFY(checkFile("test_pe.exe", -1, 15));
}

void TestParser::test_png()
{
    QVERIFY(checkFile("test_png.png"));
}

void TestParser::test_messagepack()
{
    QVERIFY(checkFile("test_msgpack.msgpack"));
}

void TestParser::test_mp3()
{
    QVERIFY(checkFile("test_mp3.mp3", -1, 15));
}

void TestParser::test_mp4()
{
    QVERIFY(checkFile("test_mp4.mp4", -1, 20));
}

void TestParser::test_sqlite()
{
    QVERIFY(checkFile("test_sqlite.sqlite"));
}

void TestParser::test_tiff()
{
    QVERIFY(checkFile("test_tiff.tif", 2, 30));
}

void TestParser::test_zip()
{
    QVERIFY(checkFile("test_zip.zip"));
}

bool TestParser::checkFile(const std::string &fileName, int depth, int width, const std::string &moduleKey)
{
    VariableCollector collector;

    Log::info("Checking ", fileName);

    RealFile file;
    file.setPath(path+fileName);

    if (!file.good())
    {
        Log::error("File not found ", fileName);
        return false;
    }

    ModuleLoader& moduleLoader = moduleSetup.moduleLoader();
    const Module& module = moduleKey.empty() ? moduleLoader.getModule(file) : moduleLoader.getModule(moduleKey);

    Object* object = module.handleFile(DefaultModule::file, file, collector);

    if (!object) {
        return false;
    }

    const std::string origPath = path+"orig/"+fileName+".txt";
    if (!fileExists(origPath)) {
        writeObject(*object, origPath, depth, width);
        return true;
    } else {
        const std::string newPath = path+"new/"+fileName+".txt";
        writeObject(*object, newPath, depth, width);
        return fileCompare(origPath, newPath);
    }

}

void TestParser::writeObject(Object &object, const std::string &outputPath, int depth, int width)
{
    std::ofstream file;
    file.open(outputPath);

    writeObjectRecursive(object, file, 0, depth, width);
}

void TestParser::writeObjectRecursive(Object &object, std::ofstream &file, int currentDepth, int remainingDepth, int width)
{
    const std::string firstRow = concat(object.beginningPos(),"x",object.size());
    file << firstRow;

    for (int i = 4*currentDepth + 13 - firstRow.size(); i > 0; --i) {
        file << " ";
    }

    object.type().simpleDisplay(file) << " ";

    const std::string& objectName = object.name();
    if (!objectName.empty() && ((objectName[0] >= 'a' && objectName[0] <= 'z') || (objectName[0] >= 'A' && objectName[0] <= 'Z'))) {
        file << objectName << " ";
    }

    if (!object.value().isValueless()) {
        file << "= ";
        object.value().simpleDisplay(file);
    }

    file << std::endl;

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
