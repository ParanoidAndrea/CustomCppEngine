#pragma once
#include "Engine/Math/Vec3.hpp"

struct Plane3
{
public:
	Vec3 m_normal = Vec3(0.f, 0.f, 1.f);
	float m_distFromOriginAlongNormal = 0.f;
	
	float GetAltitudeOfPoint(Vec3 const& refPos) const; 
	bool IsPointInTheFrontSide(Vec3 const& rerPos) const;
};
