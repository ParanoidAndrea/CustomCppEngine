#include "AABB2.hpp"

AABB2::AABB2(AABB2 const& copyFrom)
	: m_mins(copyFrom.m_mins)
	, m_maxs(copyFrom.m_maxs)
{
}

AABB2::AABB2(float minX, float minY, float maxX, float maxY)
	:m_mins(Vec2(minX, minY))
	,m_maxs(Vec2(maxX, maxY))
{
}

AABB2::AABB2(Vec2 const& mins, Vec2 const& maxs)
	: m_mins(mins)
	, m_maxs(maxs)
{
}

AABB2::AABB2(Vec2 const& center, float dimension) // A Square
{
	m_maxs.x = center.x + dimension / 2.f;
	m_mins.x = center.x - dimension / 2.f;
	m_maxs.y = center.y + dimension / 2.f;
	m_mins.y = center.y - dimension / 2.f;
}

const AABB2 AABB2::ZERO_TO_ONE(Vec2(0.f, 0.f), Vec2(1.f, 1.f));
bool AABB2::IsPointInside(Vec2 const& point) const
{
	if (point.x > m_mins.x && point.x< m_maxs.x && point.y>m_mins.y && point.y < m_maxs.y)
		return true;
	else return false;
}

float AABB2::GetHeight() const
{
	return m_maxs.y - m_mins.y;
}

Vec2 const AABB2::GetCenter() const
{
	float cenX = (m_mins.x + m_maxs.x) / 2.f;
	float cenY = (m_mins.y + m_maxs.y) / 2.f;
	return Vec2(cenX, cenY);
}

Vec2 const AABB2::GetDimensions() const
{
	float xlength = m_maxs.x - m_mins.x;
	float ylength = m_maxs.y - m_mins.y;
	return Vec2(xlength, ylength);
}

Vec2 const AABB2::GetNearestPoint(Vec2 const& referencePos) const
{
	Vec2 nearestPos = referencePos;

	if (nearestPos.x < m_mins.x) 
		nearestPos.x = m_mins.x;
	else if (nearestPos.x > m_maxs.x) 
		nearestPos.x = m_maxs.x;

	if (nearestPos.y < m_mins.y) 
		nearestPos.y = m_mins.y;
	else if (nearestPos.y > m_maxs.y) 
		nearestPos.y = m_maxs.y;
	
	return nearestPos;
}

Vec2 const AABB2::GetPointAtUV(Vec2 const& uv) const
{
	Vec2 dimesion = GetDimensions();
	return Vec2(m_mins.x+ uv.x*dimesion.x, m_mins.y+uv.y*dimesion.y);
}

Vec2 const AABB2::GetUVForPoint(Vec2 const& point) const
{
	Vec2 dimesion = GetDimensions();
	return Vec2((point.x - m_mins.x) / dimesion.x, (point.y - m_mins.y) / dimesion.y);
}



AABB2 const AABB2::GetAABB2ForNormalizePoints(Vec2 const& minsNormalized, Vec2 const& maxsNormalized) const
{
	return AABB2(m_mins.x + minsNormalized.x * (m_maxs.x-m_mins.x), 
		m_mins.y + minsNormalized.y * (m_maxs.y - m_mins.y), 
		m_mins.x + maxsNormalized.x * (m_maxs.x - m_mins.x), 
		m_mins.y + maxsNormalized.y * (m_maxs.y - m_mins.y));
}

void AABB2::Translate(Vec2 const& translationToApply)
{
	Vec2 dimesion = GetDimensions();
	m_mins.x += translationToApply.x;
	m_mins.y += translationToApply.y;
	m_maxs.x = m_mins.x + dimesion.x;
	m_maxs.y = m_mins.y + dimesion.y;
}

void AABB2::SetCenter(Vec2 const& newCenter)
{
	Vec2 dimension = GetDimensions();
	m_maxs.x = newCenter.x + dimension.x / 2.f;
	m_mins.x = newCenter.x - dimension.x / 2.f;
	m_maxs.y = newCenter.y + dimension.y / 2.f;
	m_mins.y = newCenter.y - dimension.y / 2.f;
}

void AABB2::SetDimensions(Vec2 const& newDimension)
{
	Vec2 center = GetCenter();
	m_maxs.x = center.x + newDimension.x / 2.f;
	m_mins.x = center.x - newDimension.x / 2.f;
	m_maxs.y = center.y + newDimension.y / 2.f;
	m_mins.y = center.y - newDimension.y / 2.f;
}

void AABB2::StretchToIncludePoint(Vec2 const& point)
{
	if (point.x < m_mins.x)
		m_mins.x = point.x;
	else if (point.x > m_maxs.x)
		m_maxs.x=point.x;

	if (point.y < m_mins.y)
		m_mins.y = point.y;
	else if (point.y > m_maxs.y)
		m_maxs.y=point.y;
}


