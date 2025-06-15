#include "Engine/Core/BufferWriter.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
BufferWriter::BufferWriter(std::vector<unsigned char>& buffer, BufferEndianMode mode)
    : m_buffer(buffer)
    , m_mode(mode)
{
    BufferEndianMode platformEndian = GetPlatformNativeEndian();
    if (m_mode == BufferEndianMode::Native)
    {
        m_mode = platformEndian;
        m_isOppositeEndiannessFromNative = false;
    }
    else
    {
        m_isOppositeEndiannessFromNative = (m_mode != platformEndian);
    }
}

void BufferWriter::AppendByte(unsigned char byte)
{
    m_buffer.push_back(byte);
}

void BufferWriter::AppendChar(char c)
{
    m_buffer.push_back(static_cast<unsigned char>(c));
}

void BufferWriter::AppendUShort(unsigned short ushort)
{
    unsigned short* addressOfShort = &ushort;
    unsigned char* addressOfShortAsByteArray = reinterpret_cast<unsigned char*>(addressOfShort);
    if (m_isOppositeEndiannessFromNative)
    {
        Reverse2BytesInPlace(addressOfShortAsByteArray);
    }
    AppendByte(addressOfShortAsByteArray[0]);
    AppendByte(addressOfShortAsByteArray[1]);
}

void BufferWriter::AppendShort(short s)
{
    short* addressOfShort = &s;
    unsigned char* addressOfShortAsByteArray = reinterpret_cast<unsigned char*>(addressOfShort);
    if (m_isOppositeEndiannessFromNative)
    {
        Reverse2BytesInPlace(addressOfShortAsByteArray);
    }
    AppendByte(addressOfShortAsByteArray[0]);
    AppendByte(addressOfShortAsByteArray[1]);
}

void BufferWriter::AppendUInt(unsigned int uint)
{
    unsigned int* addressOfUInt = &uint;
    unsigned char* addressOfUIntAsByteArray = reinterpret_cast<unsigned char*>(addressOfUInt);
    if (m_isOppositeEndiannessFromNative)
    {
        Reverse4BytesInPlace(addressOfUIntAsByteArray);
    }
    AppendByte(addressOfUIntAsByteArray[0]);
    AppendByte(addressOfUIntAsByteArray[1]);
    AppendByte(addressOfUIntAsByteArray[2]);
    AppendByte(addressOfUIntAsByteArray[3]);
}

void BufferWriter::AppendInt(int i)
{
    int* addressOfInt = &i;
    unsigned char* addressOfIntAsByteArray = reinterpret_cast<unsigned char*>(addressOfInt);
    if (m_isOppositeEndiannessFromNative)
    {
        Reverse4BytesInPlace(addressOfIntAsByteArray);
    }
    AppendByte(addressOfIntAsByteArray[0]);
    AppendByte(addressOfIntAsByteArray[1]);
    AppendByte(addressOfIntAsByteArray[2]);
    AppendByte(addressOfIntAsByteArray[3]);
}

void BufferWriter::AppendUInt64(uint64_t ui64)
{
    uint64_t* addressOfUInt64 = &ui64;
    unsigned char* addressOfUInt64AsByteArray = reinterpret_cast<unsigned char*>(addressOfUInt64);
    if (m_isOppositeEndiannessFromNative)
    {
        Reverse8BytesInPlace(addressOfUInt64AsByteArray);
    }
    AppendByte( addressOfUInt64AsByteArray[0]);
    AppendByte( addressOfUInt64AsByteArray[1]);
    AppendByte( addressOfUInt64AsByteArray[2]);
    AppendByte( addressOfUInt64AsByteArray[3]);
    AppendByte( addressOfUInt64AsByteArray[4]);
    AppendByte( addressOfUInt64AsByteArray[5]);
    AppendByte( addressOfUInt64AsByteArray[6]);
    AppendByte( addressOfUInt64AsByteArray[7]);
}

void BufferWriter::AppendInt64(int64_t i64)
{
    int64_t* addressOfInt64 = &i64;
    unsigned char* addressOfInt64AsByteArray = reinterpret_cast<unsigned char*>(addressOfInt64);
    if (m_isOppositeEndiannessFromNative)
    {
        Reverse8BytesInPlace(addressOfInt64AsByteArray);
    }
    AppendByte(addressOfInt64AsByteArray[0]);
    AppendByte(addressOfInt64AsByteArray[1]);
    AppendByte(addressOfInt64AsByteArray[2]);
    AppendByte(addressOfInt64AsByteArray[3]);
    AppendByte(addressOfInt64AsByteArray[4]);
    AppendByte(addressOfInt64AsByteArray[5]);
    AppendByte(addressOfInt64AsByteArray[6]);
    AppendByte(addressOfInt64AsByteArray[7]);
}

void BufferWriter::AppendFloat(float f)
{
    float* addressOfFloat = &f;
    unsigned char* addressOfFloatAsByteArray = reinterpret_cast<unsigned char*> (addressOfFloat);
    if (m_isOppositeEndiannessFromNative)
    {
        Reverse4BytesInPlace(addressOfFloatAsByteArray);
    }
    AppendByte(addressOfFloatAsByteArray[0]);
    AppendByte(addressOfFloatAsByteArray[1]);
    AppendByte(addressOfFloatAsByteArray[2]);
    AppendByte(addressOfFloatAsByteArray[3]);
}

void BufferWriter::AppendDouble(double d)
{
    double* addressOfDouble = &d;
    unsigned char* addressOfDoubleAsByteArray = reinterpret_cast<unsigned char*>(addressOfDouble);
    if (m_isOppositeEndiannessFromNative)
    {
        Reverse8BytesInPlace(addressOfDoubleAsByteArray);
    }
    AppendByte(addressOfDoubleAsByteArray[0]);
    AppendByte(addressOfDoubleAsByteArray[1]);
    AppendByte(addressOfDoubleAsByteArray[2]);
    AppendByte(addressOfDoubleAsByteArray[3]);
    AppendByte(addressOfDoubleAsByteArray[4]);
    AppendByte(addressOfDoubleAsByteArray[5]);
    AppendByte(addressOfDoubleAsByteArray[6]);
    AppendByte(addressOfDoubleAsByteArray[7]);
}                       

void BufferWriter::AppendVec2(Vec2 const& vec2)
{
    AppendFloat(vec2.x);
    AppendFloat(vec2.y);
}

void BufferWriter::AppendPlane2(Plane2 const& plane)
{
    AppendVec2(plane.m_normal);
    AppendFloat(plane.m_distFromOriginAlongNormal);
}

void BufferWriter::AppendAABB2(AABB2 const& box)
{
    AppendVec2(box.m_mins);
    AppendVec2(box.m_maxs);
}

void BufferWriter::AppendStringZeroTerminated(const std::string& str)
{
    for (char c : str)
    {
        AppendChar(c);
    }
    AppendByte(0);
}

void BufferWriter::AppendStringLengthPreceded(const std::string& str)
{
    AppendUInt(static_cast<unsigned int>(str.length()));
    for (char c : str)
    {
        AppendChar(c);
    }
}

void BufferWriter::UpdateUInt32AtPosition(size_t position, unsigned int value)
{
    if (position + sizeof(unsigned int) > m_buffer.size())
    {
        ERROR_RECOVERABLE("Error: update unsigned int position is out of bounds");
        return;
    }

    unsigned int* addressOfUInt = &value;
    unsigned char* addressOfUIntAsByteArray = reinterpret_cast<unsigned char*>(addressOfUInt);

    if (m_isOppositeEndiannessFromNative)
    {
        Reverse4BytesInPlace(addressOfUIntAsByteArray);
    }

    m_buffer[position + 0] = addressOfUIntAsByteArray[0];
    m_buffer[position + 1] = addressOfUIntAsByteArray[1];
    m_buffer[position + 2] = addressOfUIntAsByteArray[2];
    m_buffer[position + 3] = addressOfUIntAsByteArray[3];
}

void BufferWriter::SetEndianMode(BufferEndianMode mode)
{
    m_mode = mode;
    BufferEndianMode platformEndian = GetPlatformNativeEndian();
    if (m_mode == BufferEndianMode::Native)
    {
        m_mode = platformEndian;
        m_isOppositeEndiannessFromNative = false;
    }
    else
    {
        m_isOppositeEndiannessFromNative = (m_mode != platformEndian);
    }
}

size_t BufferWriter::GetCurrentWritePosition() const
{ 
    return m_buffer.size(); 
}

uint32_t BufferWriter::GetCurrentWritePosUInt() const
{
    return (uint32_t)m_buffer.size(); 
}
