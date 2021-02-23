#include "bitwise.h"

uint8_t bitReverse(uint8_t d)
{
#ifndef BM_NO_CACHE_REVERSE

    static const uint8_t bitReverseLookup[256] ={
        0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
        0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
        0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
        0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
        0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
        0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
        0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
        0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
        0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
        0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
        0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
        0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
        0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
        0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
        0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
        0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
    };
    return bitReverseLookup[d];

#else

    d = (d & 0xF0) >> 4 | (d & 0x0F) << 4;
    d = (d & 0xCC) >> 2 | (d & 0x33) << 2;
    d = (d & 0xAA) >> 1 | (d & 0x55) << 1;
    return d;

#endif // !BM_NO_CACHE_REVERSE
}


const char* bitStringA(uint8_t d)
{
#ifndef BM_NO_CACHE_CHAR

    static const char str[256][9] ={
        "00000000", "00000001", "00000010", "00000011", "00000100", "00000101", "00000110", "00000111",
        "00001000", "00001001", "00001010", "00001011", "00001100", "00001101", "00001110", "00001111",
        "00010000", "00010001", "00010010", "00010011", "00010100", "00010101", "00010110", "00010111",
        "00011000", "00011001", "00011010", "00011011", "00011100", "00011101", "00011110", "00011111",
        "00100000", "00100001", "00100010", "00100011", "00100100", "00100101", "00100110", "00100111",
        "00101000", "00101001", "00101010", "00101011", "00101100", "00101101", "00101110", "00101111",
        "00110000", "00110001", "00110010", "00110011", "00110100", "00110101", "00110110", "00110111",
        "00111000", "00111001", "00111010", "00111011", "00111100", "00111101", "00111110", "00111111",
        "01000000", "01000001", "01000010", "01000011", "01000100", "01000101", "01000110", "01000111",
        "01001000", "01001001", "01001010", "01001011", "01001100", "01001101", "01001110", "01001111",
        "01010000", "01010001", "01010010", "01010011", "01010100", "01010101", "01010110", "01010111",
        "01011000", "01011001", "01011010", "01011011", "01011100", "01011101", "01011110", "01011111",
        "01100000", "01100001", "01100010", "01100011", "01100100", "01100101", "01100110", "01100111",
        "01101000", "01101001", "01101010", "01101011", "01101100", "01101101", "01101110", "01101111",
        "01110000", "01110001", "01110010", "01110011", "01110100", "01110101", "01110110", "01110111",
        "01111000", "01111001", "01111010", "01111011", "01111100", "01111101", "01111110", "01111111",
        "10000000", "10000001", "10000010", "10000011", "10000100", "10000101", "10000110", "10000111",
        "10001000", "10001001", "10001010", "10001011", "10001100", "10001101", "10001110", "10001111",
        "10010000", "10010001", "10010010", "10010011", "10010100", "10010101", "10010110", "10010111",
        "10011000", "10011001", "10011010", "10011011", "10011100", "10011101", "10011110", "10011111",
        "10100000", "10100001", "10100010", "10100011", "10100100", "10100101", "10100110", "10100111",
        "10101000", "10101001", "10101010", "10101011", "10101100", "10101101", "10101110", "10101111",
        "10110000", "10110001", "10110010", "10110011", "10110100", "10110101", "10110110", "10110111",
        "10111000", "10111001", "10111010", "10111011", "10111100", "10111101", "10111110", "10111111",
        "11000000", "11000001", "11000010", "11000011", "11000100", "11000101", "11000110", "11000111",
        "11001000", "11001001", "11001010", "11001011", "11001100", "11001101", "11001110", "11001111",
        "11010000", "11010001", "11010010", "11010011", "11010100", "11010101", "11010110", "11010111",
        "11011000", "11011001", "11011010", "11011011", "11011100", "11011101", "11011110", "11011111",
        "11100000", "11100001", "11100010", "11100011", "11100100", "11100101", "11100110", "11100111",
        "11101000", "11101001", "11101010", "11101011", "11101100", "11101101", "11101110", "11101111",
        "11110000", "11110001", "11110010", "11110011", "11110100", "11110101", "11110110", "11110111",
        "11111000", "11111001", "11111010", "11111011", "11111100", "11111101", "11111110", "11111111"
    };
    return str[d];

#else 

    static char buf[9] ={ 0 };
    buf[0] = d & 0x80 ? '1' : '0';
    buf[1] = d & 0x40 ? '1' : '0';
    buf[2] = d & 0x20 ? '1' : '0';
    buf[3] = d & 0x10 ? '1' : '0';
    buf[4] = d & 0x08 ? '1' : '0';
    buf[5] = d & 0x04 ? '1' : '0';
    buf[6] = d & 0x02 ? '1' : '0';
    buf[7] = d & 0x01 ? '1' : '0';
    return buf;

#endif // !BM_NO_CACHE_CHAR
}


const wchar_t* bitStringW(uint8_t d)
{
#ifndef BM_NO_CACHE_WCHAR

    static const wchar_t str[256][9] ={
        L"00000000", L"00000001", L"00000010", L"00000011", L"00000100", L"00000101", L"00000110", L"00000111",
        L"00001000", L"00001001", L"00001010", L"00001011", L"00001100", L"00001101", L"00001110", L"00001111",
        L"00010000", L"00010001", L"00010010", L"00010011", L"00010100", L"00010101", L"00010110", L"00010111",
        L"00011000", L"00011001", L"00011010", L"00011011", L"00011100", L"00011101", L"00011110", L"00011111",
        L"00100000", L"00100001", L"00100010", L"00100011", L"00100100", L"00100101", L"00100110", L"00100111",
        L"00101000", L"00101001", L"00101010", L"00101011", L"00101100", L"00101101", L"00101110", L"00101111",
        L"00110000", L"00110001", L"00110010", L"00110011", L"00110100", L"00110101", L"00110110", L"00110111",
        L"00111000", L"00111001", L"00111010", L"00111011", L"00111100", L"00111101", L"00111110", L"00111111",
        L"01000000", L"01000001", L"01000010", L"01000011", L"01000100", L"01000101", L"01000110", L"01000111",
        L"01001000", L"01001001", L"01001010", L"01001011", L"01001100", L"01001101", L"01001110", L"01001111",
        L"01010000", L"01010001", L"01010010", L"01010011", L"01010100", L"01010101", L"01010110", L"01010111",
        L"01011000", L"01011001", L"01011010", L"01011011", L"01011100", L"01011101", L"01011110", L"01011111",
        L"01100000", L"01100001", L"01100010", L"01100011", L"01100100", L"01100101", L"01100110", L"01100111",
        L"01101000", L"01101001", L"01101010", L"01101011", L"01101100", L"01101101", L"01101110", L"01101111",
        L"01110000", L"01110001", L"01110010", L"01110011", L"01110100", L"01110101", L"01110110", L"01110111",
        L"01111000", L"01111001", L"01111010", L"01111011", L"01111100", L"01111101", L"01111110", L"01111111",
        L"10000000", L"10000001", L"10000010", L"10000011", L"10000100", L"10000101", L"10000110", L"10000111",
        L"10001000", L"10001001", L"10001010", L"10001011", L"10001100", L"10001101", L"10001110", L"10001111",
        L"10010000", L"10010001", L"10010010", L"10010011", L"10010100", L"10010101", L"10010110", L"10010111",
        L"10011000", L"10011001", L"10011010", L"10011011", L"10011100", L"10011101", L"10011110", L"10011111",
        L"10100000", L"10100001", L"10100010", L"10100011", L"10100100", L"10100101", L"10100110", L"10100111",
        L"10101000", L"10101001", L"10101010", L"10101011", L"10101100", L"10101101", L"10101110", L"10101111",
        L"10110000", L"10110001", L"10110010", L"10110011", L"10110100", L"10110101", L"10110110", L"10110111",
        L"10111000", L"10111001", L"10111010", L"10111011", L"10111100", L"10111101", L"10111110", L"10111111",
        L"11000000", L"11000001", L"11000010", L"11000011", L"11000100", L"11000101", L"11000110", L"11000111",
        L"11001000", L"11001001", L"11001010", L"11001011", L"11001100", L"11001101", L"11001110", L"11001111",
        L"11010000", L"11010001", L"11010010", L"11010011", L"11010100", L"11010101", L"11010110", L"11010111",
        L"11011000", L"11011001", L"11011010", L"11011011", L"11011100", L"11011101", L"11011110", L"11011111",
        L"11100000", L"11100001", L"11100010", L"11100011", L"11100100", L"11100101", L"11100110", L"11100111",
        L"11101000", L"11101001", L"11101010", L"11101011", L"11101100", L"11101101", L"11101110", L"11101111",
        L"11110000", L"11110001", L"11110010", L"11110011", L"11110100", L"11110101", L"11110110", L"11110111",
        L"11111000", L"11111001", L"11111010", L"11111011", L"11111100", L"11111101", L"11111110", L"11111111"
    };
    return str[d];

#else

    static wchar_t buf[9] ={ 0 };
    buf[0] = d & 0x80U ? L'1' : L'0';
    buf[1] = d & 0x40U ? L'1' : L'0';
    buf[2] = d & 0x20U ? L'1' : L'0';
    buf[3] = d & 0x10U ? L'1' : L'0';
    buf[4] = d & 0x08U ? L'1' : L'0';
    buf[5] = d & 0x04U ? L'1' : L'0';
    buf[6] = d & 0x02U ? L'1' : L'0';
    buf[7] = d & 0x01U ? L'1' : L'0';
    return buf;

#endif // !BM_NO_CACHE_WCAHR
}