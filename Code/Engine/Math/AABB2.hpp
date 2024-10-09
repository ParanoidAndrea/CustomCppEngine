#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"
//#include "math.h"
struct AABB2
{
public:
	Vec2 m_mins;
	Vec2 m_maxs;

public: 
	~AABB2() {}
	AABB2() {}
	AABB2(AABB2 const& copyFrom);
	explicit AABB2(float minX, float minY, float maxX, float maxY);
	explicit AABB2(Vec2 const& mins, Vec2 const& maxs);
	explicit AABB2(Vec2 const& center, float dimension);

	static const AABB2 ZERO_TO_ONE;
	//Accessors (const methods)
	bool IsPointInside(Vec2 const& point) const;
	float GetHeight() const;
	Vec2 const GetCenter() const;
	Vec2 const GetDimensions() const;
	Vec2 const GetNearestPoint(Vec2 const& referencePos) const;
	Vec2 const GetPointAtUV(Vec2 const& uv) const; //uv=(0,0) is at mins; uv=(1,1) is at maxs;
	Vec2 const GetUVForPoint(Vec2 const& point) const;//uv=(.5,.5) at center; u or v outside [0,1] extrapolated
	AABB2 const GetAABB2ForNormalizePoints(Vec2 const& minsNormalized, Vec2 const& maxsNormalized) const;
	// Mutators (non-const methods)
	void Translate(Vec2 const& translationToApply);
	void SetCenter(Vec2 const& newCenter);
	void SetDimensions(Vec2 const& newDimensions);
	void StretchToIncludePoint(Vec2 const& point);
};