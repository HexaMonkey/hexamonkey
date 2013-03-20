#include "mkvintegerdata.h"

MkvIntegerData::MkvIntegerData(File* _file) : Data(_file)
{
    uint8_t byte;
    file()->read(reinterpret_cast<char*>(&byte), 8);
    int count;
    for(count = 1; count <= 8; ++count)
    {
        if(byte&(1<<(8-count)))
            break;
    }
    int64_t var = byte & ~(1<<(8-count));

    for(int i = 1; i < count; ++i)
    {
        uint8_t byte;
        file()->read(reinterpret_cast<char*>(&byte), 8);
        var = var<<8 | byte;
    }
    setSize(8*count);
    setValue(var);
    setInfo(toStr(var));
    setParsed();
}
