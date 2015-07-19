#include "modulesetup.h"

#include <vector>
#include <string>

#include "core/modules/standard/standardmodule.h"
#include "core/modules/ebml/ebmlmodule.h"
#include "core/modules/mkv/mkvmodule.h"
#include "core/modules/hmc/hmcmodule.h"
#include "core/modules/stream/streammodule.h"
#include "core/util/fileutil.h"
#include "core/util/osutil.h"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_APPLE)

#include <sys/types.h>
#include <sys/stat.h>

#endif

void ModuleSetup::setup()
{
#if defined(PLATFORM_WIN32)
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string installDir = std::string(buffer);
    std::string::size_type pos = installDir.find_last_of( "\\/" );
    installDir = installDir.substr( 0, pos)+"\\";

    std::string userDir = installDir+"scripts\\";

    std::vector<std::string> modelsDirs = {installDir, "..\\models\\"};
    std::vector<std::string> scriptsDirs = {installDir+"scripts\\", "..\\scripts\\"};
    std::vector<std::string> compilerDirs = {installDir, "..\\compiler\\"};
    _logoDirs = {installDir, "..\\logo\\"};

#elif defined(PLATFORM_LINUX) || defined(PLATFORM_APPLE)
    /* XXX: use autotools ? */

    std::string installDir = "/usr/share/hexamonkey/";
    std::string userDir = std::string(getenv("HOME"))+"/.hexamonkey/";
    mkdir(userDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    std::vector<std::string> modelsDirs = {installDir, "../models/"};
    std::vector<std::string> scriptsDirs = {installDir+"scripts/", userDir, "../scripts/"};
    std::vector<std::string> compilerDirs = {"/usr/bin/", "../compiler/"};
    _logoDirs = {installDir, "../logo/"};

#else
    std::cerr<<"Error: unsuported operating system"<<std::endl;
#endif

    _moduleLoader.addModule("bestd", new StandardModule(true));
    _moduleLoader.addModule("lestd", new StandardModule(false));
    _moduleLoader.addModule("ebml",  new EbmlModule);
    _moduleLoader.addModule("mkv",   new MkvModule(getFile(modelsDirs, "mkvmodel.xml")));
    _moduleLoader.addModule("hmc",   new HmcModule(getFile(modelsDirs, "hmcmodel.csv")));
    _moduleLoader.addModule("stream",new StreamModule);

    _programLoader.reset(createProgramLoader(static_cast<const HmcModule&>(_moduleLoader.getModule("hmc")), compilerDirs, userDir));

    _moduleLoader.setDirectories(scriptsDirs, programLoader());
}

ModuleSetup::~ModuleSetup()
{
}

ProgramLoader *ModuleSetup::createProgramLoader(const HmcModule &hmcModule, const std::vector<std::string> &compilerDir, const std::string &userDir)
{
    return new ProgramLoader(hmcModule, compilerDir, userDir);
}

ModuleLoader &ModuleSetup::moduleLoader()
{
    return _moduleLoader;
}

const ModuleLoader &ModuleSetup::moduleLoader() const
{
    return _moduleLoader;
}

const ProgramLoader &ModuleSetup::programLoader() const
{
    return *_programLoader;
}

const std::string &ModuleSetup::logoPath() const
{
    return getFile(_logoDirs, "logo.svg");
}
