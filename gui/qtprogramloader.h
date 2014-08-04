#ifndef QTPROGRAMLOADER_H
#define QTPROGRAMLOADER_H

#include "core/interpreter/programloader.h"

class QtProgramLoader : public ProgramLoader
{
public:
    QtProgramLoader(const HmcModule &module, const std::vector<std::string> &compilerDirs, const std::string userDir);

protected:
    virtual bool executeCommand(const std::string& program, const std::vector<std::string>& arguments) const override;
    virtual long long lastModified(const std::string& file) const override;
};

#endif // QTPROGRAMLOADER_H
