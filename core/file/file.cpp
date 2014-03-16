#include "core/file/file.h"

File::File() : _bitPosition(0) {}

FileAnchor::FileAnchor(File &file)
    :file(file),
     position(file.tellg())
{

}

FileAnchor::~FileAnchor()
{
    if(!file.good())
    {
        file.clear();
        file.close();
        file.open();
    }
    file.seekg(position, std::ios_base::beg);
}
