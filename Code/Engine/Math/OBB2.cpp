#include "OBB2.hpp"
#include "Engine/Math/MathUtils.hpp"
#define UNUSED(x) (void)(x)
OBB2::OBB2(OBB2 const& copyFrom)
	:m_center(copyFrom.m_center),
	m_iBasisNormal(copyFrom.m_iBasisNormal),
	m_halfDimensions(copyFrom.m_halfDimensions)
{

}

OBB2::OBB2(Vec2 const& center, Vec2 const& iBasisNormal, Vec2 const& halfDimensions)
	:m_center(center),
	m_iBasisNormal(iBasisNormal),
	m_halfDimensions(halfDimensions)
{

}

void OBB2::GetCornerPoints(Vec2* out_fourCornerWorldPositions) const
{
	Vec2 jBasisNormal = m_iBasisNormal.GetRotated90Degrees();

	Vec2 halfExtentsX = m_iBasisNormal * m_halfDimensions.x;
	Vec2 halfExtentsY = jBasisNormal * m_halfDimensions.y;

	out_fourCornerWorldPositions[0] = m_center - halfExtentsX - halfExtentsY;
	out_fourCornerWorldPositions[1] = m_center + halfExtentsX - halfExtentsY;
	out_fourCornerWorldPositions[2] = m_center - halfExtentsX + halfExtentsY;
	out_fourCornerWorldPositions[3] = m_center + halfExtentsX + halfExtentsY;
}

Vec2 OBB2::GetLocalPosForWorldPos(Vec2 worldPos) const
{
	Vec2 jBasisNormal = m_iBasisNormal.GetRotated90Degrees();
	Vec2 centerToWorldPos = worldPos - m_center;
	return Vec2(DotProduct2D(centerToWorldPos,m_iBasisNormal), DotProduct2D(centerToWorldPos, jBasisNormal));
}

Vec2 OBB2::GetWorldPosForLocalPos(Vec2 localPos) const
{
	TransformPosition2D(localPos, m_iBasisNormal, m_iBasisNormal.GetRotated90Degrees(), m_center);
	return localPos;
}

void OBB2::RotateAboutCenter(float rotationDeltaDegrees)
{
	m_iBasisNormal.RotateDegrees(rotationDeltaDegrees);
}
