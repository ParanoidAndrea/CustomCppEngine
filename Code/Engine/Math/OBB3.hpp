#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/AABB3.hpp"
struct OBB3
{
public:
	Vec3 m_center;
	Vec3 m_iBasisNormal;
	Vec3 m_jBasisNormal;
	Vec3 m_kBasisNormal;
	Vec3 m_halfDimensions;

public:
	~OBB3() {}
	OBB3() {}
	OBB3(OBB3 const& copyFrom);
	explicit OBB3(Vec3 const& center, Vec3 const& iBasisNormal, Vec3 const& jBasisNormal, Vec3 const& halfDimensions);
	void GetCornerPoints(Vec3* out_eightCornerWorldPositions) const;
	Vec3 GetLocalVectorForWorldVector(Vec3 const& worldVector) const;
	Vec3 GetLocalPosForWorldPos(Vec3 worldPos) const;
	Vec3 GetWorldPosForLocalPos(Vec3 localPos) const;
	void RotateAboutCenter(float rotationDeltaDegrees);
	AABB3 const GetLocalAABB3() const;
};