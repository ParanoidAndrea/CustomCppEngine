#pragma once
#include "Engine/Math/Vec3.hpp"
class AABB3
{
public:
	Vec3 m_mins;
	Vec3 m_maxs;
public:
	 AABB3() {}
	~AABB3() {}
	static const AABB3 ZERO_TO_ONE;
	AABB3(AABB3 const& copyFrom);
	explicit AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	explicit AABB3(Vec3 const& mins, Vec3 const& maxs);
	Vec3 const GetDimensions() const;
	Vec3 const GetCenter() const;
	Vec3 const GetNearestPoint(Vec3 const& referencePos) const;
	void SetCenter(Vec3 const& newCenter);
	void SetDimensions(Vec3 const& newDimensions);
};