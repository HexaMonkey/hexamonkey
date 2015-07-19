#include "qtmodulesetup.h"
#include "qtprogramloader.h"


ProgramLoader *QtModuleSetup::createProgramLoader(const HmcModule &hmcModule, const std::vector<std::string> &compilerDir, const std::string &userDir)
{
    return new QtProgramLoader(hmcModule, compilerDir, userDir);
}
