#include "OBB3.hpp"
#include "Engine/Math/MathUtils.hpp"
OBB3::OBB3(OBB3 const& copyFrom)
{
	m_center = copyFrom.m_center;
	m_iBasisNormal = copyFrom.m_iBasisNormal;
	m_jBasisNormal = copyFrom.m_jBasisNormal;
	m_kBasisNormal = copyFrom.m_kBasisNormal;
	m_halfDimensions = copyFrom.m_halfDimensions;
}

OBB3::OBB3(Vec3 const& center, Vec3 const& iBasisNormal,Vec3 const& jBasisNormal, Vec3 const& halfDimensions)
	:m_center(center),
	m_iBasisNormal(iBasisNormal),
	m_jBasisNormal(jBasisNormal),
	m_halfDimensions(halfDimensions)
{
	m_kBasisNormal = CrossProduct3D(m_iBasisNormal, m_jBasisNormal).GetNormalized();
}

void OBB3::GetCornerPoints(Vec3* out_eightCornerWorldPositions) const
{
	//Vec3 kBasisNormal = CrossProduct3D(m_iBasisNormal, m_jBasisNormal).GetNormalized();

	Vec3 halfExtentsX = m_iBasisNormal * m_halfDimensions.x;
	Vec3 halfExtentsY = m_jBasisNormal * m_halfDimensions.y;
	Vec3 halfExtentsZ = m_kBasisNormal * m_halfDimensions.z;

	out_eightCornerWorldPositions[0] = m_center - halfExtentsX - halfExtentsY - halfExtentsZ;
	out_eightCornerWorldPositions[1] = m_center + halfExtentsX - halfExtentsY - halfExtentsZ;
	out_eightCornerWorldPositions[2] = m_center - halfExtentsX + halfExtentsY - halfExtentsZ;
	out_eightCornerWorldPositions[3] = m_center + halfExtentsX + halfExtentsY - halfExtentsZ;
	out_eightCornerWorldPositions[4] = m_center - halfExtentsX - halfExtentsY + halfExtentsZ;
	out_eightCornerWorldPositions[5] = m_center + halfExtentsX - halfExtentsY + halfExtentsZ;
	out_eightCornerWorldPositions[6] = m_center - halfExtentsX + halfExtentsY + halfExtentsZ;
	out_eightCornerWorldPositions[7] = m_center + halfExtentsX + halfExtentsY + halfExtentsZ;
}

Vec3 OBB3::GetLocalVectorForWorldVector(Vec3 const& worldVector) const
{
	Mat44 worldMatrix = Mat44();
	worldMatrix.SetIJK3D(m_iBasisNormal, m_jBasisNormal, m_kBasisNormal);
	Mat44 localMatrix = worldMatrix.GetOrthonormalInverse();
	return worldMatrix.TransformVectorQuantity3D(worldVector);
}

Vec3 OBB3::GetLocalPosForWorldPos(Vec3 worldPos) const
{
	Vec3 localPos = worldPos - m_center;
// 	Mat44 localMatrix = Mat44(m_iBasisNormal, m_jBasisNormal, m_kBasisNormal, Vec3());
// 	Mat44 worldMatrix = localMatrix.GetOrthonormalInverse();
// 	Mat44 localMatrix = Mat44();
// 	localMatrix.SetIJK3D(m_iBasisNormal, m_jBasisNormal, m_kBasisNormal);
// 	Mat44 worldMatrix = localMatrix.GetOrthonormalInverse();
	//return worldMatrix
 	float localX = DotProduct3D(localPos, m_iBasisNormal);
 	float localY = DotProduct3D(localPos, m_jBasisNormal);
 	float localZ = DotProduct3D(localPos, m_kBasisNormal);
 
 	 return Vec3(localX, localY, localZ);
}

Vec3 OBB3::GetWorldPosForLocalPos(Vec3 localPos) const
{
	Vec3 worldPos = m_center 
		+ localPos.x * m_iBasisNormal
		+ localPos.y * m_jBasisNormal
		+ localPos.z * m_kBasisNormal;

	return worldPos;
}

void OBB3::RotateAboutCenter(float rotationDeltaDegrees)
{
	m_iBasisNormal = m_iBasisNormal.GetRotatedAboutZDegrees(rotationDeltaDegrees);
	m_jBasisNormal = m_jBasisNormal.GetRotatedAboutZDegrees(rotationDeltaDegrees);
}

AABB3 const OBB3::GetLocalAABB3() const
{

	Vec3 cornerPoints[8];
	GetCornerPoints(cornerPoints);
	Vec3 localMin = GetLocalPosForWorldPos(cornerPoints[0]);
	Vec3 localMax = GetLocalPosForWorldPos(cornerPoints[7]);
	return AABB3(localMin, localMax);
	
}
