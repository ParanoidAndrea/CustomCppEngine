#pragma once
#include "Engine/Math/Vec2.hpp"

struct Plane2
{
public:
    Vec2 m_normal = Vec2(0.f, 1.f);
    float m_distFromOriginAlongNormal = 0.f;

    float GetAltitudeOfPoint(Vec2 const& refPos) const;
    bool IsPointInTheFrontSide(Vec2 const& rerPos) const;
};
