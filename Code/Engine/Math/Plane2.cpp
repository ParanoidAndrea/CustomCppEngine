#include "Engine/Math/Plane2.hpp"
#include "Engine/Math/MathUtils.hpp"

float Plane2::GetAltitudeOfPoint(Vec2 const& refPos) const
{
    return DotProduct2D(refPos, m_normal) - m_distFromOriginAlongNormal;
}

bool Plane2::IsPointInTheFrontSide(Vec2 const& rerPos) const
{
    if (DotProduct2D(rerPos, m_normal) >= m_distFromOriginAlongNormal)
    {
        return true;
    }
    return false;
}
