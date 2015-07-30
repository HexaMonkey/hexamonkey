#ifndef MODULESETUP_H
#define MODULESETUP_H

#include <memory>

#include "core/moduleloader.h"
#include "core/interpreter/programloader.h"

class ModuleSetup
{
public:
    void setup();
    virtual ~ModuleSetup();

    void addScriptDirectory(const std::string& dir);
    virtual ProgramLoader* createProgramLoader(const HmcModule& hmcModule, const std::vector<std::string>& compilerDir, const std::string& userDir);

    ModuleLoader& moduleLoader();
    const ModuleLoader& moduleLoader() const;
    const ProgramLoader& programLoader() const;
    const std::string& logoPath() const;
private:
    std::vector<std::string> _scriptsDirs;
    std::unique_ptr<ProgramLoader> _programLoader;
    ModuleLoader _moduleLoader;
    std::string _logoPath;
};

#endif // MODULESETUP_H
