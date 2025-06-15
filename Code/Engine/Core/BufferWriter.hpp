#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include "Engine/Core/BufferUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Plane2.hpp"
#include "Engine/Math/AABB2.hpp"
class BufferWriter
{
public:
    BufferWriter(std::vector<unsigned char>& buffer, BufferEndianMode mode = BufferEndianMode::Native);

    void AppendByte(unsigned char byte);
    void AppendChar(char c);
    void AppendUShort(unsigned short ushort);
    void AppendShort(short s);
    void AppendUInt(unsigned int uint);
    void AppendInt(int i);
    void AppendUInt64(uint64_t ui64);
    void AppendInt64(int64_t i64);
    void AppendFloat(float f);
    void AppendDouble(double d);

    void AppendVec2(Vec2 const& vec2);
    void AppendPlane2(Plane2 const& plane2);
    void AppendAABB2(AABB2 const& box);

    void AppendStringZeroTerminated(const std::string& str);
    void AppendStringLengthPreceded(const std::string& str);

    void UpdateUInt32AtPosition(size_t position, unsigned int value);

    void SetEndianMode(BufferEndianMode mode);

    size_t GetCurrentWritePosition() const;
    uint32_t GetCurrentWritePosUInt() const;
private:
    std::vector<unsigned char>& m_buffer; 
    BufferEndianMode m_mode;
    bool m_isOppositeEndiannessFromNative;
};