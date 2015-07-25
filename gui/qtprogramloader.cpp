#include "qtprogramloader.h"

#include <QProcess>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>

QtProgramLoader::QtProgramLoader(const HmcModule &module, const std::vector<std::string> &compilerDirs, const std::string userDir)
    : ProgramLoader(module, compilerDirs, userDir)
{
}

bool QtProgramLoader::executeCommand(const std::string &program, const std::vector<std::string> &arguments) const
{
    QProcess process;
    QStringList argumentList;

    for(const std::string& argument : arguments)
    {
        argumentList << QString(argument.c_str());
    }

    process.start(QString(program.c_str()), argumentList);
    process.waitForFinished();
    std::string error  = QString(process.readAllStandardError()).toStdString();

    if(!error.empty())
    {
        std::cerr<<"Program error for : "<<program;
        for (const std::string& argument : arguments) {
            std::cerr<<" "<<argument;
        }
        std::cerr<<std::endl;
        std::cerr<<error<<std::endl;
        return false;
    }

    return true;
}

long long QtProgramLoader::lastModified(const std::string &file) const
{

    QFileInfo fileInfo(file.c_str());
    if(fileInfo.exists())
    {
        return fileInfo.lastModified().toMSecsSinceEpoch();
    }

    return -1LL;
}
