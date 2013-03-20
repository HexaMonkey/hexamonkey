#include "fileutil.h"

#include <fstream>
#include <dirent.h>

bool fileExists(const std::string& path)
{
    std::ifstream file(path);
    return file.is_open();
}

void getDirContent(const std::string& path, std::vector<std::string>& content)
{
    DIR *dir = NULL;
    struct dirent *drnt = NULL;
    dir=opendir(path.c_str());
    if(dir)
    {
        while((drnt = readdir(dir)))
        {
            content.push_back(drnt->d_name);
        }
        closedir(dir);
    }
}
