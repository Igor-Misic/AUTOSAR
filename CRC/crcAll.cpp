#include <stdio.h>
#include <stdint.h>

//This source code is helper for implementing CRC by AutoSar documentation: Specification of CRC Routines.
// https://www.autosar.org/fileadmin/files/standards/classic/

// Good page for checking your CRC:
// http://www.sunshine2k.de/coding/javascript/crc/crc_js.html


extern uint32_t reflect(uint32_t data, uint8_t nBits);

template <class T>
void CrcTableGenerator(T polynomial, T (&crcTable)[256])
{
    T remainder;

    T topBit = 0x01 << (8*sizeof(T) - 1);
    uint32_t ui32Dividend;

    for (ui32Dividend = 0; ui32Dividend  < 256; ui32Dividend++)
    {
        remainder = ui32Dividend << (8*sizeof(T) - 8);

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
        if (sizeof(T) == sizeof(uint8_t))
        {
            printf("0x%02xU, ", remainder);
        }
        else if (sizeof(T) == sizeof(uint16_t))
        {
            printf("0x%04xU, ", remainder);
        }
        else
        {
            printf("0x%08xU, ", remainder);
        }
        */

    }
}

template <class T>
T CalculateCRC(T(&crcTable)[256], const uint8_t *crc_DataPtr, uint32_t crc_Length, T crc_InitialValue, T crc_XorValue, bool reflectedOutput, bool reflectedInput)
{
    uint32_t ui32Counter;
    uint8_t temp;
    T crc = crc_InitialValue;

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

        if (1 == sizeof(T)) // CRC8
        {
            crc = crc ^ temp;
            crc = crcTable[crc];
        }
        else if (2 == sizeof(T)) // CRC16
        {
            crc = (crc << 8) ^ crcTable[(uint8_t)((crc >> 8) ^ temp)];
        }
        else // CRC32
        {
            crc = (crc << 8) ^ crcTable[(uint8_t)((crc >> 24) ^ temp)] ;
        }

        crc_DataPtr++;

    }

    crc ^= crc_XorValue;
    if (reflectedOutput)
    {
        crc = reflect(crc, sizeof(T) * 8);
    }
    return crc;
}

template <class T>
void TestCRC(T calculatedCrc, T expectedCrc)
{
    if (expectedCrc != calculatedCrc)
    {
        printf("Error for CRC 0x%x\n", calculatedCrc);
    }
    else
    {
        printf("CRC 0x%02x OK!\n", calculatedCrc);
    }
}

int main2()
{
    uint8_t crcTable8bit[256];
    uint16_t crcTable16bit[256];
    uint32_t crcTable32bit[256];

    uint8_t message1[] = { 0x00, 0x00, 0x00, 0x00 };
    uint8_t message2[] = { 0xF2, 0x01, 0x83 };
    uint8_t message3[] = { 0x0F, 0xAA, 0x00, 0x55 };
    uint8_t message4[] = { 0x00, 0xFF, 0x55, 0x11 };
    uint8_t message5[] = { 0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0XFF };
    uint8_t message6[] = { 0x92, 0x6B, 0x55 };
    uint8_t message7[] = { 0xFF, 0xFF, 0xFF, 0xFF };

    uint8_t polynominal8bit;
    uint8_t initValue;
    uint8_t xorValue;

    //CRC8 (SAE-J1850)  Specification of CRC Routines page 22
    polynominal8bit = 0x1D;
    initValue       = 0xFF;
    xorValue        = 0xFF;
    CrcTableGenerator<uint8_t>(polynominal8bit, crcTable8bit);

    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message1, sizeof(message1), initValue, xorValue, false, false), 0x59);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message2, sizeof(message2), initValue, xorValue, false, false), 0x37);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message3, sizeof(message3), initValue, xorValue, false, false), 0x79);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message4, sizeof(message4), initValue, xorValue, false, false), 0xB8);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message5, sizeof(message5), initValue, xorValue, false, false), 0xCB);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message6, sizeof(message6), initValue, xorValue, false, false), 0x8C);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message7, sizeof(message7), initValue, xorValue, false, false), 0x74);

    //CRC8H2F           Specification of CRC Routines page 23
    polynominal8bit = 0x2F;
    initValue       = 0xFF;
    xorValue        = 0xFF;
    CrcTableGenerator<uint8_t>(polynominal8bit, crcTable8bit);

    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message1, sizeof(message1), initValue, xorValue, false, false), 0x12);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message2, sizeof(message2), initValue, xorValue, false, false), 0xC2);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message3, sizeof(message3), initValue, xorValue, false, false), 0xC6);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message4, sizeof(message4), initValue, xorValue, false, false), 0x77);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message5, sizeof(message5), initValue, xorValue, false, false), 0x11);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message6, sizeof(message6), initValue, xorValue, false, false), 0x33);
    TestCRC<uint8_t>(CalculateCRC<uint8_t>(crcTable8bit, message7, sizeof(message7), initValue, xorValue, false, false), 0x6C);

    //CCITT-FALSE CRC16 Specification of CRC Routines page 23
    uint16_t polynominal16bit   = 0x1021;
    uint16_t initValue16bit     = 0xFFFF;
    uint16_t xorValue16bit      = 0x0000;
    CrcTableGenerator<uint16_t>(polynominal16bit, crcTable16bit);

    TestCRC<uint16_t>(CalculateCRC<uint16_t>(crcTable16bit, message1, sizeof(message1), initValue16bit, xorValue16bit, false, false), 0x84C0);
    TestCRC<uint16_t>(CalculateCRC<uint16_t>(crcTable16bit, message2, sizeof(message2), initValue16bit, xorValue16bit, false, false), 0xD374);
    TestCRC<uint16_t>(CalculateCRC<uint16_t>(crcTable16bit, message3, sizeof(message3), initValue16bit, xorValue16bit, false, false), 0x2023);
    TestCRC<uint16_t>(CalculateCRC<uint16_t>(crcTable16bit, message4, sizeof(message4), initValue16bit, xorValue16bit, false, false), 0xB8F9);
    TestCRC<uint16_t>(CalculateCRC<uint16_t>(crcTable16bit, message5, sizeof(message5), initValue16bit, xorValue16bit, false, false), 0xF53F);
    TestCRC<uint16_t>(CalculateCRC<uint16_t>(crcTable16bit, message6, sizeof(message6), initValue16bit, xorValue16bit, false, false), 0x0745);
    TestCRC<uint16_t>(CalculateCRC<uint16_t>(crcTable16bit, message7, sizeof(message7), initValue16bit, xorValue16bit, false, false), 0x1D0F);

    //Ethernet CRC Calculation - CRC32
    uint32_t polynominal32bit   = 0x04C11DB7;
    uint32_t initValue32bit     = 0xFFFFFFFF;
    uint32_t xorValue32bit      = 0xFFFFFFFF;
    CrcTableGenerator<uint32_t>(polynominal32bit, crcTable32bit);

    TestCRC<uint32_t>(CalculateCRC<uint32_t>(crcTable32bit, message1, sizeof(message1), initValue32bit, xorValue32bit, true, true), 0x2144DF1C);
    TestCRC<uint32_t>(CalculateCRC<uint32_t>(crcTable32bit, message2, sizeof(message2), initValue32bit, xorValue32bit, true, true), 0x24AB9D77);
    TestCRC<uint32_t>(CalculateCRC<uint32_t>(crcTable32bit, message3, sizeof(message3), initValue32bit, xorValue32bit, true, true), 0xB6C9B287);
    TestCRC<uint32_t>(CalculateCRC<uint32_t>(crcTable32bit, message4, sizeof(message4), initValue32bit, xorValue32bit, true, true), 0x32A06212);
    TestCRC<uint32_t>(CalculateCRC<uint32_t>(crcTable32bit, message5, sizeof(message5), initValue32bit, xorValue32bit, true, true), 0xB0AE863D);
    TestCRC<uint32_t>(CalculateCRC<uint32_t>(crcTable32bit, message6, sizeof(message6), initValue32bit, xorValue32bit, true, true), 0x9CDEA29B);
    TestCRC<uint32_t>(CalculateCRC<uint32_t>(crcTable32bit, message7, sizeof(message7), initValue32bit, xorValue32bit, true, true), 0xFFFFFFFF);

    return 0;
}
