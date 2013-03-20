#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <string>
#include <vector>

bool fileExists(const std::string& path);
void getDirContent(const std::string& path, std::vector<std::string>& content);


#endif // FILEUTIL_H
