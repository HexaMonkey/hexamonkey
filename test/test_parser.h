#ifndef TEST_PARSER_H
#define TEST_PARSER_H

#include <QtTest/QtTest>

#include <QObject>
#include <fstream>

#include "gui/qtmodulesetup.h"

class TestParser : public QObject
{
    Q_OBJECT
public:
    TestParser();

private slots:
    void test_default();
    void test_find();

    void test_asf();
    void test_avi();
    void test_bmp();
    void test_gif();
    void test_jpg();
    void test_pe();
    void test_png();
    void test_messagepack();
    void test_mp4();
    void test_sqlite();
    void test_tiff();
    void test_zip();

private:

    bool checkFile(const std::string& fileName, int depth = -1, int width = -1, const std::string &moduleKey = "");

    void writeObject(Object& object, const std::string& outputPath, int depth, int width);
    void writeObjectRecursive(Object& object, std::ofstream& file, int currentDepth, int remainingDepth, int width);

    QtModuleSetup moduleSetup;
    const std::string path;
};

#endif // TEST_PARSER_H
