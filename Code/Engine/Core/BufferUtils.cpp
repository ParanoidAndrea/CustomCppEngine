#include "Engine/Core/BufferUtils.hpp"
//#define UNUSED(x) (void)(x)

bool IsPlatformLittleEndian()
{
    uint16_t value = 0x0001;
    return *(reinterpret_cast<uint8_t*>(&value)) == 0x01;
}

BufferEndianMode GetPlatformNativeEndian()
{
    return IsPlatformLittleEndian() ? BufferEndianMode::Little : BufferEndianMode::Big;
}

void Reverse2BytesInPlace(void* ptrTo16BitWord)
{
    unsigned short* asUint16Ptr = reinterpret_cast<unsigned short*>(ptrTo16BitWord);
    unsigned short originalUint16 = *asUint16Ptr;
    unsigned short reversedUint16 = ((originalUint16 & 0x00ff) << 8) |
        ((originalUint16 & 0xff00) >> 8);
    *asUint16Ptr = reversedUint16;
}

void Reverse4BytesInPlace(void* ptrTo32BitDword)
{
    unsigned int* asUint32Ptr = reinterpret_cast<unsigned int*>(ptrTo32BitDword);
    unsigned int originalUint32 = *asUint32Ptr;
    unsigned int reversedUint32 = ((originalUint32 & 0x000000ff) << 24) |
        ((originalUint32 & 0x0000ff00) << 8) |
        ((originalUint32 & 0x00ff0000) >> 8) |
        ((originalUint32 & 0xff000000) >> 24);
    *asUint32Ptr = reversedUint32;
}

void Reverse8BytesInPlace(void* ptrTo64BitQword)
{
    uint64_t* asUint64Ptr = reinterpret_cast<uint64_t*>(ptrTo64BitQword);
    uint64_t originalUint64 = *asUint64Ptr;
    uint64_t reversedUint64 = ((originalUint64 & 0x00000000000000ffULL) << 56) |
        ((originalUint64 & 0x000000000000ff00ULL) << 40) |
        ((originalUint64 & 0x0000000000ff0000ULL) << 24) |
        ((originalUint64 & 0x00000000ff000000ULL) << 8) |
        ((originalUint64 & 0x000000ff00000000ULL) >> 8) |
        ((originalUint64 & 0x0000ff0000000000ULL) >> 24) |
        ((originalUint64 & 0x00ff000000000000ULL) >> 40) |
        ((originalUint64 & 0xff00000000000000ULL) >> 56);
    *asUint64Ptr = reversedUint64;
}



