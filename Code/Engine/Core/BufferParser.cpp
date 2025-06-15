#include "Engine/Core/BufferParser.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#define UNUSED(x) (void)(x)

BufferParser::BufferParser(unsigned char const* bufferData, size_t bufferSize, BufferEndianMode mode)
    :m_mode(mode)
    ,m_bufferSize(bufferSize)
    ,m_scanStartPos(bufferData)
    ,m_scanEndPos(bufferData + (bufferSize))
    ,m_scanPosition(bufferData)
{
    m_dataCopy.resize(bufferSize);
    std::memcpy(m_dataCopy.data(), bufferData, bufferSize);
    if (mode == BufferEndianMode::Native)
    {
        m_mode = GetPlatformNativeEndian();
    }

    bool platformIsLittleEndian = IsPlatformLittleEndian();
    m_isOppositeEndiannessFromNative = (m_mode == BufferEndianMode::Little && !platformIsLittleEndian) 
        || (m_mode == BufferEndianMode::Big && platformIsLittleEndian);

}

BufferParser::BufferParser(std::vector<unsigned char> const& buffer, BufferEndianMode mode)
    : m_mode(mode)
    , m_dataCopy(buffer)
    , m_bufferSize(buffer.size())
    , m_scanStartPos(buffer.data())
    , m_scanEndPos(buffer.data() + (buffer.size()))
    , m_scanPosition(buffer.data())
{

}

unsigned char const* BufferParser::ParseBytes(size_t byteSize)
{
    GuaranteeBufferDataAvailable(byteSize);
    unsigned char const* result = m_scanPosition;
    m_scanPosition += byteSize;
    return result;
}

unsigned char BufferParser::ParseByte()
{
    GuaranteeBufferDataAvailable(1);
    unsigned char value = *m_scanPosition;
    m_scanPosition++;
    return value;
}

char BufferParser::ParseChar()
{
    return static_cast<char>(ParseByte());
}

unsigned short BufferParser::ParseUShort()
{
    GuaranteeBufferDataAvailable(2);

    unsigned short finalValue;
    unsigned char* bytesArray = reinterpret_cast<unsigned char*>(&finalValue);

    bytesArray[0] = ParseByte();
    bytesArray[1] = ParseByte();

    if (m_isOppositeEndiannessFromNative)
    {
        Reverse2BytesInPlace(bytesArray);
    }

    return finalValue;
}

short BufferParser::ParseShort()
{
    return static_cast<short>(ParseUShort());
}

unsigned int BufferParser::ParseUInt()
{
    GuaranteeBufferDataAvailable(4);

    unsigned int finalValue;
    unsigned char* bytesArray = reinterpret_cast<unsigned char*>(&finalValue);

    bytesArray[0] = ParseByte();
    bytesArray[1] = ParseByte();
    bytesArray[2] = ParseByte();
    bytesArray[3] = ParseByte();

    if (m_isOppositeEndiannessFromNative)
    {
        Reverse4BytesInPlace(&finalValue);
    }

    return finalValue;
}

int BufferParser::ParseInt()
{
    return static_cast<int>(ParseUInt());
}


uint64_t BufferParser::ParseUInt64()
{
    GuaranteeBufferDataAvailable(8);

    uint64_t finalValue;
    unsigned char* bytesArray = reinterpret_cast<unsigned char*>(&finalValue);

    bytesArray[0] = ParseByte();
    bytesArray[1] = ParseByte();
    bytesArray[2] = ParseByte();
    bytesArray[3] = ParseByte();
    bytesArray[4] = ParseByte();
    bytesArray[5] = ParseByte();
    bytesArray[6] = ParseByte();
    bytesArray[7] = ParseByte();

    if (m_isOppositeEndiannessFromNative)
    {
        Reverse8BytesInPlace(&finalValue);
    }

    return finalValue;
}

int64_t BufferParser::ParseInt64()
{
    return static_cast<int64_t>(ParseUInt64());
}



float BufferParser::ParseFloat()
{
    GuaranteeBufferDataAvailable(4); 
    float finalValue;
    unsigned char* floatAsArrayOfBytes = reinterpret_cast<unsigned char*> (&finalValue);
    floatAsArrayOfBytes[0] = ParseByte();
    floatAsArrayOfBytes[1] = ParseByte();
    floatAsArrayOfBytes[2] = ParseByte();
    floatAsArrayOfBytes[3] = ParseByte();

    if (m_isOppositeEndiannessFromNative)
    {
        Reverse4BytesInPlace(&finalValue);
    }

    return finalValue;
}

double BufferParser::ParseDouble()
{
    GuaranteeBufferDataAvailable(8);

    double finalValue;
    unsigned char* bytesArray = reinterpret_cast<unsigned char*>(&finalValue);

    bytesArray[0] = ParseByte();
    bytesArray[1] = ParseByte();
    bytesArray[2] = ParseByte();
    bytesArray[3] = ParseByte();
    bytesArray[4] = ParseByte();
    bytesArray[5] = ParseByte();
    bytesArray[6] = ParseByte();
    bytesArray[7] = ParseByte();

    if (m_isOppositeEndiannessFromNative)
    {
        Reverse8BytesInPlace(&finalValue);
    }

    return finalValue;
}

void BufferParser::ParseStringOfLength(std::string& out_string, unsigned int stringLength)
{
    GuaranteeBufferDataAvailable(stringLength);
    out_string.reserve(stringLength);
    out_string.assign((char const*)m_scanPosition, stringLength);
    m_scanPosition += stringLength;
}

std::string BufferParser::ParseStringZeroTerminated()
{
    std::string result;
    ParseStringZeroTerminated(result);
    return result;
}

std::string BufferParser::ParseStringLengthPreceded()
{
    unsigned int length = ParseUInt();
    std::string result;
    ParseStringOfLength(result, length);
    return result;
}

void BufferParser::ParseStringZeroTerminated(std::string& out_string)
{
    out_string.clear();

    while (m_scanPosition < m_scanEndPos && *m_scanPosition != 0)
    {
        out_string.push_back(static_cast<char>(*m_scanPosition));
        m_scanPosition++;
    }

    // Skip the terminating zero byte if we're not at the end
    if (m_scanPosition < m_scanEndPos)
    {
        m_scanPosition++;
    }
}

void BufferParser::JumpToPosition(size_t position)
{
    if (position >= m_bufferSize)
    {
        ERROR_RECOVERABLE("Position is beyond buffer data end.");
    }
    else
    {
        m_scanPosition = m_scanStartPos + position;
    }
}

void BufferParser::GuaranteeBufferDataAvailable(size_t bytesNeeded)
{
    if (m_scanPosition + bytesNeeded > m_scanEndPos )
    {
        ERROR_AND_DIE("Attempted to read beyond buffer data end.");
    }
}

void BufferParser::SetEndianMode(BufferEndianMode mode)
{
    if (mode == BufferEndianMode::Native)
    {
        m_mode = GetPlatformNativeEndian();
    }
    else
    {
        m_mode = mode;
    }
    bool platformIsLittleEndian = IsPlatformLittleEndian();
    m_isOppositeEndiannessFromNative = (m_mode == BufferEndianMode::Little && !platformIsLittleEndian) 
        || (m_mode == BufferEndianMode::Big && platformIsLittleEndian);
}

bool BufferParser::IsAtEnd() const
{
    return m_scanPosition >= m_scanEndPos;
}

Vec2 BufferParser::ParseVec2()
{
    Vec2 result;
    result.x = ParseFloat();
    result.y = ParseFloat();
    return result;
}

Vec3 BufferParser::ParseVec3()
{
    Vec3 result;
    result.x = ParseFloat();
    result.y = ParseFloat();
    result.z = ParseFloat();
    return result;
}

IntVec2 BufferParser::ParseIntVec2()
{
    IntVec2 result;
    result.x = ParseInt();
    result.y = ParseInt();
    return result;
}

AABB2 BufferParser::ParseAABB2()
{
    AABB2 result;
    result.m_mins = ParseVec2();
    result.m_maxs = ParseVec2();
    return result;
}

Plane2 BufferParser::ParsePlane2()
{
    Plane2 result;
    result.m_normal = ParseVec2();
    result.m_distFromOriginAlongNormal = ParseFloat();
    return result;
}
