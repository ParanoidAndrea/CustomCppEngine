#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/BufferUtils.hpp"
#include "Engine/Math/Plane2.hpp"
class BufferParser
{
public:
    BufferParser(unsigned char const* bufferData, size_t bufferSize, BufferEndianMode mode = BufferEndianMode::Native);
    BufferParser( std::vector<unsigned char> const& buffer, BufferEndianMode mode = BufferEndianMode::Native);
    unsigned char const* ParseBytes(size_t byteSize); // returns pointer to the data and advances position
    unsigned char ParseByte();
    char ParseChar();
    unsigned short ParseUShort();
    short ParseShort();
    unsigned int ParseUInt();
    int ParseInt();
    uint64_t ParseUInt64();
    int64_t ParseInt64();
    float ParseFloat();
    double ParseDouble();

    void ParseStringZeroTerminated(std::string& out_string);
    void ParseStringOfLength(std::string& out_string, unsigned int stringLength);
    std::string ParseStringZeroTerminated();
    std::string ParseStringLengthPreceded();

    void JumpToPosition(size_t position);
    void GuaranteeBufferDataAvailable(size_t bytesNeeded);
    void SetEndianMode(BufferEndianMode mode);
    bool IsAtEnd() const;

    Vec2 ParseVec2();
    Vec3 ParseVec3();
    IntVec2 ParseIntVec2();
    AABB2 ParseAABB2();
    Plane2 ParsePlane2();
private:

    std::vector<unsigned char>  m_dataCopy;
    unsigned char const* m_scanPosition = nullptr;
    unsigned char const* m_scanEndPos = nullptr;
    unsigned char const* m_scanStartPos = nullptr;
    size_t m_bufferSize;
    BufferEndianMode m_mode = BufferEndianMode::Native;
    bool m_isOppositeEndiannessFromNative = false;
};
