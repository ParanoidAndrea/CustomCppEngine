#pragma once
#include <string>
enum class BufferEndianMode
{
    Native = 0,
    Little,
    Big
};

bool IsPlatformLittleEndian();
BufferEndianMode GetPlatformNativeEndian();
void Reverse2BytesInPlace(void* ptrTo16BitWord);
void Reverse4BytesInPlace(void* ptrTo32BitDword);
void Reverse8BytesInPlace(void* ptrTo64BitQword);