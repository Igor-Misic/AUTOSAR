#include <stdio.h>
#include <stdint.h>

//This source code is helper for implementing CRC by AutoSar documentation: Specification of CRC Routines.
// https://www.autosar.org/fileadmin/files/standards/classic/

// Good page for checking your CRC:
// http://www.sunshine2k.de/coding/javascript/crc/crc_js.html


extern uint32_t reflect(uint32_t data, uint8_t nBits);

void Crc16TableGenerator(uint16_t polynomial, uint16_t crcTable[256])
{
    uint16_t remainder;

    uint16_t topBit = 0x8000;
    uint32_t ui32Dividend;

    for (ui32Dividend = 0; ui32Dividend < 256; ui32Dividend++)
    {
        remainder = ui32Dividend << 8;

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (0 == (remainder & topBit))
            {
                remainder <<= 1;
            }
            else
            {
                remainder = (remainder << 1) ^ polynomial;
            }
        }

        crcTable[ui32Dividend] = remainder;

        //remove comments if you want print CRC 256 lookup table 
        /*
        if (0 == dividend % 8)
        {
            printf("\n");
        }
        printf("0x%04xU, ", remainder);

        */

    }
}

uint16_t CalculateCRC16(uint16_t crcTable[256], const uint8_t *crc_DataPtr, uint32_t crc_Length, uint16_t crc_InitialValue, uint16_t crc_XorValue, bool reflectedOutput, bool reflectedInput)
{
    uint32_t ui32Counter;
    uint8_t temp;
    uint16_t crc = crc_InitialValue;

    for (ui32Counter = 0U; ui32Counter < crc_Length; ui32Counter++)
    {
        if (reflectedInput)
        {
            temp = reflect(*crc_DataPtr, 8);
        }
        else
        {
            temp = *crc_DataPtr;
        }

        crc = (crc << 8) ^ crcTable[(uint8_t)((crc >> 8) ^ temp)];
        crc_DataPtr++;

    }

    crc ^= crc_XorValue;
    if (reflectedOutput)
    {
        crc = reflect(crc, 16);
    }
    return crc;
}

void TestCRC16(uint16_t calculatedCrc, uint16_t expectedCrc)
{
    if (expectedCrc != calculatedCrc)
    {
        printf("Error for CRC16 0x%x\n", calculatedCrc);
    }
    else
    {
        printf("CRC16 0x%02x OK!\n", calculatedCrc);
    }
}
