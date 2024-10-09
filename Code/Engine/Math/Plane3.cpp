#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/MathUtils.hpp"
float Plane3::GetAltitudeOfPoint(Vec3 const& refPos) const
{
	return DotProduct3D(refPos, m_normal) - m_distFromOriginAlongNormal;
}

bool Plane3::IsPointInTheFrontSide(Vec3 const& rerPos) const
{
	if (DotProduct3D(rerPos, m_normal) >= m_distFromOriginAlongNormal)
	{
		return true;
	}
	return false;
}

