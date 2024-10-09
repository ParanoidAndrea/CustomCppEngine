#include "Curve.hpp"
CubicHermiteCurve2D::CubicHermiteCurve2D(Vec2 startPos, Vec2 startVel, Vec2 endPos, Vec2 endVel)
	:m_startPos(startPos),
	m_startVel(startVel),
	m_endPos(endPos),
	m_endVel(endVel)
{
}

CubicHermiteCurve2D::CubicHermiteCurve2D(CubicBezierCurve2D const& fromBezier)
{
	m_startPos = fromBezier.m_startPos;
	m_startVel = (fromBezier.m_guidePos1 - fromBezier.m_startPos) * 3.0f;
	m_endPos = fromBezier.m_endPos;
	m_endVel = (fromBezier.m_endPos - fromBezier.m_guidePos2) * 3.0f;
}

Vec2 CubicHermiteCurve2D::EvaluateAtParametric(float parametrixZeroToOne) const
{
	CubicBezierCurve2D curve = CubicBezierCurve2D(*this);
	return curve.EvaluateAtParametric(parametrixZeroToOne);
}

float CubicHermiteCurve2D::GetApproximateLength(int numSubdivisons)
{
	CubicBezierCurve2D curve = CubicBezierCurve2D(*this);
	return curve.GetApproximateLength(numSubdivisons);
}

Vec2 CubicHermiteCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdiv)
{
	CubicBezierCurve2D curve = CubicBezierCurve2D(*this);
	return curve.EvaluateAtApproximateDistance(distanceAlongCurve, numSubdiv);
}


CubicBezierCurve2D::CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos)
	:m_startPos(startPos),
	m_guidePos1(guidePos1),
	m_guidePos2(guidePos2),
	m_endPos(endPos)
{
}

CubicBezierCurve2D::CubicBezierCurve2D(CubicHermiteCurve2D const& fromHermite)
{
	m_startPos = fromHermite.m_startPos;
	m_guidePos1 = fromHermite.m_startPos + fromHermite.m_startVel / 3.f;
	m_guidePos2 = fromHermite.m_endPos - fromHermite.m_endVel / 3.0f;
	m_endPos = fromHermite.m_endPos;
}

Vec2 CubicBezierCurve2D::EvaluateAtParametric(float parametrixZeroToOne) const
{
	float x = ComputeCubicBezier1D(m_startPos.x, m_guidePos1.x, m_guidePos2.x, m_endPos.x, parametrixZeroToOne);
	float y = ComputeCubicBezier1D(m_startPos.y, m_guidePos1.y, m_guidePos2.y, m_endPos.y, parametrixZeroToOne);
	return Vec2(x, y);
}

float CubicBezierCurve2D::GetApproximateLength(int numSubdivisons)
{
	float length = 0.f;
	Vec2 previousPoint = m_startPos;
	for (int i = 1; i <= numSubdivisons; ++i) 
	{
		float t = (float)(i) / (float)numSubdivisons;
		Vec2 currentPoint = EvaluateAtParametric(t);
		length += (currentPoint - previousPoint).GetLength();
		previousPoint = currentPoint;
	}
	return length;
}

Vec2 CubicBezierCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubdiv)
{
	float deltaT = 1.f / (float)(numSubdiv); 
	float t = 0.f; 
	Vec2 previousPoint = m_startPos;
	Vec2 currentPoint;
	float remainingDistance = distanceAlongCurve; 

	// Loop through each segment
	for (int i = 1; i <= numSubdiv; ++i)
	{
		t += deltaT;
		currentPoint = EvaluateAtParametric(t);
		float segmentLength = (currentPoint - previousPoint).GetLength();
		if (segmentLength >= remainingDistance)
		{
			// Calculate the fraction of the final segment where the point is actually at
			float fraction = 1.0f - (segmentLength - remainingDistance) / segmentLength;

			// Interpolate between previous and current points to find the point at the fraction of the final segment
			return Interpolate(previousPoint, currentPoint, fraction);
		}
		remainingDistance -= segmentLength;
		previousPoint = currentPoint;
	}

	// If the remaining distance is not covered by the segments, return the last point
	return currentPoint;
}

