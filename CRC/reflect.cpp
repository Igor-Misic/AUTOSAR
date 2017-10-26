#include <stdint.h>

uint32_t reflect(uint32_t data, uint8_t nBits)
{
    uint32_t  reflection = 0;


    /*
    * Reflect the data about the center bit.
    */
    for (uint8_t bit = 0; bit < nBits; ++bit)
    {
        /*
        * If the LSB bit is set, set the reflection of it.
        */
        if (data & 0x01)
        {
            reflection |= (1 << ((nBits - 1) - bit));
        }

        data = (data >> 1);
    }

    return (reflection);

}   /* reflect() */