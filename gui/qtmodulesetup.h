#ifndef QTMODULESETUP_H
#define QTMODULESETUP_H

#include "core/modulesetup.h"

class QtModuleSetup : public ModuleSetup
{
public:
    virtual ProgramLoader* createProgramLoader(const HmcModule &hmcModule, const std::vector<std::string> &compilerDir, const std::string &userDir) override;
};

#endif // QTMODULESETUP_H
