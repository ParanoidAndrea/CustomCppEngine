#pragma once
#include "Engine/Math/MathUtils.hpp"
#include <vector>
class CubicBezierCurve2D;
class CubicHermiteCurve2D
{
public:
	CubicHermiteCurve2D() = default;
	CubicHermiteCurve2D(Vec2 startPos, Vec2 startVel, Vec2 endPos, Vec2 endVel);
	explicit CubicHermiteCurve2D(CubicBezierCurve2D const& fromHermite);
	Vec2 EvaluateAtParametric(float parametrixZeroToOne) const;
	float GetApproximateLength(int numSubdivisons = 64);
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdiv);
public:
	Vec2 m_startPos;
	Vec2 m_startVel;
	Vec2 m_endPos;
	Vec2 m_endVel;
};

class CubicBezierCurve2D
{
public:
	CubicBezierCurve2D() = default;
	CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos);
	explicit CubicBezierCurve2D(CubicHermiteCurve2D const& fromHermite);
	Vec2 EvaluateAtParametric(float parametrixZeroToOne) const;
	float GetApproximateLength(int numSubdivisons = 64);
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdiv);
public:
	Vec2 m_startPos;
	Vec2 m_guidePos1;
	Vec2 m_guidePos2;
	Vec2 m_endPos;
};