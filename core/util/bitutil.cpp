#include "bitutil.h"

int msb(uint8_t byte)
{
    for(int i = 7; i >=1;--i)
    {
        if(byte&(1<<i))
            return i;
    }
    return 0;
}

uint8_t msbMask(int min)
{
    uint8_t mask = 0;
    for(int i = min; i<8; ++i)
    {
        mask|= 1<<i;
    }
    return mask;
}

uint8_t lsbMask(int max)
{
    uint8_t mask = 0;
    for(int i = 0; i<max; ++i)
    {
        mask|= 1<<i;
    }
    return mask;
}
