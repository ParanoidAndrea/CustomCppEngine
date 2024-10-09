#include "AABB3.hpp"

AABB3::AABB3(AABB3 const& copyFrom)
	:m_mins(copyFrom.m_mins),m_maxs(copyFrom.m_maxs)
{
	
}

AABB3::AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	: m_mins(Vec3(minX, minY, minZ)),
	m_maxs(Vec3(maxX, maxY, maxZ))
{
	
}

AABB3::AABB3(Vec3 const& mins, Vec3 const& maxs)
	:m_mins(mins),
	m_maxs(maxs)
{
}
const AABB3 AABB3::ZERO_TO_ONE(Vec3(0.f, 0.f,0.f), Vec3(1.f, 1.f,1.f));
Vec3 const AABB3::GetDimensions() const
{
	float deltaX = m_maxs.x - m_mins.x;
	float deltaY = m_maxs.y - m_mins.y;
	float deltaZ = m_maxs.z - m_mins.z;
	return Vec3(deltaX, deltaY, deltaZ);
}

Vec3 const AABB3::GetCenter() const
{
	float cenX = (m_mins.x + m_maxs.x) / 2.f;
	float cenY = (m_mins.y + m_maxs.y) / 2.f;
	float cenZ = (m_mins.z + m_maxs.z) / 2.f;
	return Vec3(cenX, cenY, cenZ);
}

Vec3 const AABB3::GetNearestPoint(Vec3 const& referencePos) const
{
	Vec3 nearestPos = referencePos;

	if (nearestPos.x < m_mins.x)
	{
		nearestPos.x = m_mins.x;
	}
	else if (nearestPos.x > m_maxs.x)
	{
		nearestPos.x = m_maxs.x;
	}

	if (nearestPos.y < m_mins.y)
	{
		nearestPos.y = m_mins.y;
	}
	else if (nearestPos.y > m_maxs.y)
	{
		nearestPos.y = m_maxs.y;
	}

	if (nearestPos.z < m_mins.z)
	{
		nearestPos.z = m_mins.z;
	}
	else if (nearestPos.z > m_maxs.z)
	{
		nearestPos.z = m_maxs.z;
	}

	return nearestPos;
	
}

void AABB3::SetCenter(Vec3 const& newCenter)
{
	Vec3 dimension = GetDimensions();
	m_maxs.x = newCenter.x + dimension.x / 2.f;
	m_mins.x = newCenter.x - dimension.x / 2.f;
	m_maxs.y = newCenter.y + dimension.y / 2.f;
	m_mins.y = newCenter.y - dimension.y / 2.f;
	m_maxs.z = newCenter.z + dimension.z / 2.f;
	m_mins.z = newCenter.z - dimension.z / 2.f;
}

void AABB3::SetDimensions(Vec3 const& newDimensions)
{
	Vec3 center = GetCenter();
	m_maxs.x = center.x + newDimensions.x / 2.f;
	m_mins.x = center.x - newDimensions.x / 2.f;
	m_maxs.y = center.y + newDimensions.y / 2.f;
	m_mins.y = center.y - newDimensions.y / 2.f;
	m_maxs.z = center.z + newDimensions.z / 2.f;
	m_mins.z = center.z - newDimensions.z / 2.f;
}
