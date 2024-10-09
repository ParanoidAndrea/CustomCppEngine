#include "RaycastUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
RaycastResult2D RaycastVsDisc2D(Vec2 startPos, Vec2 ifwdNormal, float maxDist, Vec2 discCenter, float discRadius)
{

	RaycastResult2D result;
	Vec2 jFwdNormal = ifwdNormal.GetRotated90Degrees();
	Vec2 startToCenter = discCenter - startPos;
	float startToCenterjFwd = DotProduct2D(startToCenter, jFwdNormal);
	if (startToCenterjFwd >= discRadius)
	{
		return result;
	}
	if (startToCenterjFwd <= -discRadius)
	{
		return result;
	}

	float a = sqrtf(discRadius * discRadius - startToCenterjFwd * startToCenterjFwd);
	float startToCenteriFwd = DotProduct2D(startToCenter, ifwdNormal);
	result.m_impactDist = startToCenteriFwd - a;
	result.m_impactPos = startPos + (ifwdNormal * result.m_impactDist);
	result.m_impactNormal = (result.m_impactPos - discCenter).GetNormalized();

	if (IsPointInsideDisc2D(startPos, discCenter, discRadius))
	{
		result.m_impactDist = 0.f;
		result.m_impactPos = startPos;
		result.m_impactNormal = (result.m_impactPos - discCenter).GetNormalized();
		result.m_didImpact = true;
	}
	else if (result.m_impactDist >= maxDist || result.m_impactDist <= 0.f)
	{
		result.m_didImpact = false;
	}
	else
	{
		result.m_didImpact = true;
	}

	return result;
}

RaycastResult2D RaycastVsLineSegments2D(Vec2 rayStart, Vec2 iFwdNormal, float maxDist, Vec2 lineStart, Vec2 lineEnd)
{
	RaycastResult2D result;
	Vec2 jFwdNormal = iFwdNormal.GetRotated90Degrees();
	Vec2 rayStartToLineStart = lineStart - rayStart;
	Vec2 rayEndToLineStart = lineEnd - rayStart;

	float jRayStartToLineStart = DotProduct2D(rayStartToLineStart, jFwdNormal);
	float jRayEndToLineStart = DotProduct2D(rayEndToLineStart, jFwdNormal);
	if (jRayStartToLineStart * jRayEndToLineStart >= 0.f)
	{
		return result;
	}
	else if (jRayStartToLineStart == jRayEndToLineStart)
	{
		return result;
	}

	float t = jRayStartToLineStart / (jRayStartToLineStart - jRayEndToLineStart);
	result.m_impactPos = lineStart + t * (lineEnd - lineStart);
	Vec2 rayStartToImpactPoint = result.m_impactPos - rayStart;
	result.m_impactDist = DotProduct2D(rayStartToImpactPoint, iFwdNormal);
	if (result.m_impactDist >= maxDist || result.m_impactDist <= 0.f)
	{
		result.m_didImpact = false;
		return result;
	}
	else
	{
		result.m_didImpact = true;
	}
	Vec2 lineStartToEnd = lineEnd - lineStart;
	Vec2 impactNormal = lineStartToEnd.GetRotated90Degrees();
	if (jRayEndToLineStart < 0.f)
	{
		impactNormal *= -1.f;
	}
	result.m_impactNormal = impactNormal.GetNormalized();
	

	return result;
}


RaycastResult2D RaycastVsAABBs2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, AABB2 bound)
{
	RaycastResult2D result;
	Vec2 endPos = startPos + fwdNormal * maxDist;
	if (FloatRange(bound.m_mins.x, bound.m_maxs.x).IsOnRange(startPos.x) && FloatRange(bound.m_mins.y, bound.m_maxs.y).IsOnRange(startPos.y))
	{
		result.m_didImpact = true;
		result.m_impactPos = startPos;
		result.m_impactDist = 0.f;
		result.m_impactNormal = fwdNormal;
		return result;
	}

	float oneOverRangeX = 1.f / (endPos.x - startPos.x);
	float oneOverRangeY = 1.f / (endPos.y - startPos.y);
	float minX, minY, maxX, maxY;
	if (oneOverRangeX > 0.f)
	{
		minX = MaxFloat(bound.m_mins.x, startPos.x);
		maxX = MinFloat(bound.m_maxs.x, endPos.x);
	}
	else
	{
		minX = MinFloat(bound.m_maxs.x, startPos.x);
		maxX = MaxFloat(bound.m_mins.x, endPos.x);
	}
	if (oneOverRangeY > 0.f)
	{
		minY = MaxFloat(bound.m_mins.y, startPos.y);
		maxY = MinFloat(bound.m_maxs.y, endPos.y);
	}
	else
	{
		minY = MinFloat(bound.m_maxs.y, startPos.y);
		maxY = MaxFloat(bound.m_mins.y, endPos.y);
	}
	float tMinX = (minX- startPos.x) * oneOverRangeX;
	float tMaxX = (maxX- startPos.x) * oneOverRangeX;
	float tMinY = (minY - startPos.y) * oneOverRangeY;
	float tMaxY = (maxY - startPos.y) * oneOverRangeY;

	FloatRange tRangeX = FloatRange(tMinX,tMaxX);
	FloatRange tRangeY = FloatRange(tMinY, tMaxY);
	
	// Find the overlapping range
	if (tRangeX.IsOverlappingWith(tRangeY))
	{
		result.m_didImpact = true;
		//result.m_impactPos = startPos + Vec2((endPos-startPos).x*tRangeX.m_min, (endPos - startPos).y * tRangeX.m_min);
		float minOverlap = MaxFloat(tRangeX.m_min, tRangeY.m_min);
		if (tRangeX.m_min > tRangeY.m_min)
		{
			if (startPos.x < bound.m_mins.x)
			{
				result.m_impactNormal = Vec2(-1.f, 0.f);  // Impact on the left face
			}
			else
			{
				result.m_impactNormal = Vec2(1.f, 0.f);   // Impact on the right face
			}
		}
		else
		{
			if (startPos.y < bound.m_mins.y)
			{
				result.m_impactNormal = Vec2(0.f, -1.f);  // Impact on the bottom face
			}
			else
			{
				result.m_impactNormal = Vec2(0.f, 1.f);   // Impact on the top face
			}
		}
		// Calculate the impact position using the minimum overlap factor
		result.m_impactPos = startPos + (endPos - startPos) * minOverlap;
		result.m_impactDist = (result.m_impactPos-startPos).GetLength(); // Using tOverlap.min as a factor
	}

	return result;
}

RaycastResult3D RaycastVsAABB3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, AABB3 box)
{

	RaycastResult3D result;
	result.m_rayFwdNormal = rayForwardNormal;
	if (IsPointInsideAABB3D(rayStart, box))
	{
		result.m_didImpact = true;
		result.m_impactNormal = -1.f * rayForwardNormal;
		result.m_impactPos = rayStart;
		result.m_impactDist = 0.f;
		return result;
	}
	Vec3 rayEnd = rayStart + rayForwardNormal * rayLength;
	float oneRangeOverX = 1.f / (rayEnd.x - rayStart.x);
	float tMinX = (box.m_mins.x - rayStart.x) * oneRangeOverX;
	float tMaxX = (box.m_maxs.x - rayStart.x) * oneRangeOverX;
	FloatRange tRangeX;
	if (tMinX < tMaxX)
	{
		tRangeX = FloatRange(tMinX, tMaxX);
	}
	else
	{
		tRangeX = FloatRange(tMaxX, tMinX);
	}

	float oneRangeOverY = 1.f / (rayEnd.y - rayStart.y);
	float tMinY = (box.m_mins.y - rayStart.y) * oneRangeOverY;
	float tMaxY = (box.m_maxs.y - rayStart.y) * oneRangeOverY;
	FloatRange tRangeY;
	if (tMinY < tMaxY)
	{
		tRangeY = FloatRange(tMinY, tMaxY);
	}
	else
	{
		tRangeY = FloatRange(tMaxY, tMinY);
	}
	FloatRange tRangeXY = OverlappedFloatRange(tRangeX, tRangeY);
	if (tRangeXY == FloatRange())
	{
		return result;
	}

	float oneRangeOverZ = 1.f / (rayEnd.z - rayStart.z);
	float tMinZ = (box.m_mins.z - rayStart.z) * oneRangeOverZ;
	float tMaxZ = (box.m_maxs.z - rayStart.z) * oneRangeOverZ;
	FloatRange tRangeZ;
	if (tMinZ < tMaxZ)
	{
		tRangeZ = FloatRange(tMinZ, tMaxZ);
	}
	else
	{
		tRangeZ = FloatRange(tMaxZ, tMinZ);
	}

	FloatRange tRange = OverlappedFloatRange(tRangeXY, tRangeZ);
	if (tRange == FloatRange())
	{
		return result;
	}

	float t = tRange.m_min;
	if (t >= 0.f && t <= 1.f)
	{
		Vec3 intersectionPoint = rayStart + rayForwardNormal * t*rayLength;
		Vec2 intersectionPointXY = Vec2(intersectionPoint.x, intersectionPoint.y);
		result.m_didImpact = true;
		result.m_impactPos = intersectionPoint;
		result.m_impactDist = (intersectionPoint - rayStart).GetLength();
		AABB2 boxXY = AABB2(box.m_mins.x, box.m_mins.y, box.m_maxs.x, box.m_maxs.y);
			if (t == tMinZ)
			{
				result.m_impactNormal = Vec3(0.f, 0.f, -1.f);
			}
			else if (t == tMaxZ)
			{
				result.m_impactNormal = Vec3(0.f, 0.f, 1.f);
			}
			else if (t == tMinX)
			{
				result.m_impactNormal = Vec3(-1.f, 0.f, 0.f);
			}
			else if (t == tMaxX)
			{
				result.m_impactNormal = Vec3(1.f, 0.f, 0.f);
			}
			else if (t == tMinY)
			{
				result.m_impactNormal = Vec3(0.f, -1.f, 0.f);
			}
			else if (t == tMaxY)
			{
				result.m_impactNormal = Vec3(0.f, 1.f, 0.f);
			}
		
	}
	return result;
}

RaycastResult3D RaycastVsSphere3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, Vec3 sphereCenter, float sphereRadius)
{
	RaycastResult3D result;

	if (IsPointInsideSphere3D(rayStart, sphereCenter, sphereRadius))
	{
		result.m_impactDist = 0.f;
		result.m_impactPos = rayStart;
		result.m_impactNormal = -1.f* rayForwardNormal;
		result.m_didImpact = true;
		return result;
	}
	result.m_rayFwdNormal = rayForwardNormal;
	
	Vec3 startToCenter = sphereCenter - rayStart;
	float startToCenteri= DotProduct3D(rayForwardNormal, startToCenter);
	if (startToCenteri >= rayLength + sphereRadius)
	{
		return result;
	}

	Vec3 startToCenteriFwd = rayForwardNormal * startToCenteri;
	Vec3 startToCenterWithouti = startToCenter - startToCenteriFwd;
	float distanceSquareFromRay = DotProduct3D(startToCenterWithouti, startToCenterWithouti);
	if (distanceSquareFromRay >= sphereRadius * sphereRadius)
	{
		return result;
	}


	float a = sqrtf(sphereRadius * sphereRadius - distanceSquareFromRay);
	result.m_impactDist = startToCenteri - a;
	if (result.m_impactDist >= rayLength || result.m_impactDist <= 0.f)
	{
		return result;
	}
	else
	{
		result.m_didImpact = true;
	}
	result.m_impactPos = rayStart + (rayForwardNormal * result.m_impactDist);
	result.m_impactNormal = (result.m_impactPos - sphereCenter).GetNormalized();

	return result;
}

RaycastResult3D RaycastVsCylinderZ3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius)
{
	RaycastResult3D result;
	result.m_rayFwdNormal = rayForwardNormal;
	if (IsPointInsideZCylinder3D(rayStart, centerXY, minMaxZ, radius))
	{
		result.m_didImpact = true;
		result.m_impactNormal = -1.f * rayForwardNormal;
		result.m_impactPos = rayStart;
		result.m_impactDist = 0.f;
		return result;
	}
	Vec3 rayEnd = rayStart + rayLength * rayForwardNormal;
	Vec2 rayEndXY = Vec2(rayEnd.x, rayEnd.y);
	Vec2 rayStartXY = Vec2(rayStart.x, rayStart.y);
	Vec2 rayForwardNormalXY = (rayEndXY - rayStartXY).GetNormalized();
	float rayLengthXY = (rayEndXY - rayStartXY).GetLength();

		// Calculate the intersection with the cylinder's max and min z plane
	float tMaxZ = (minMaxZ.m_max - rayStart.z) / (rayEnd.z - rayStart.z);
	float tMinZ = (minMaxZ.m_min - rayStart.z) / (rayEnd.z - rayStart.z);
	float t = MinFloat(tMinZ, tMaxZ);

	Vec3 intersectionPoint = rayStart + rayForwardNormal * t * rayLength ;
	Vec2 intersectionPointXY = Vec2(intersectionPoint.x, intersectionPoint.y);
	if (IsPointInsideDisc2D(intersectionPointXY, centerXY, radius)&& t>=0.f && t<=1.f)
	{
		
		result.m_didImpact = true;
		result.m_impactPos = intersectionPoint;
		result.m_impactDist = (intersectionPoint - rayStart).GetLength();
		if (t == tMinZ)
		{
			result.m_impactNormal = Vec3(0.f, 0.f, -1.f);
		}
		else if (t == tMaxZ)
		{
			result.m_impactNormal = Vec3(0.f, 0.f, 1.f);
		}
		return result;
	}
	
	 RaycastResult2D t_result = RaycastVsDisc2D(rayStartXY, rayForwardNormalXY, rayLengthXY, centerXY, radius);
	 if (t_result.m_didImpact)
	 {

        float tX = (t_result.m_impactPos.x - rayStart.x) / rayForwardNormal.x;
	 	float tY = (t_result.m_impactPos.y - rayStart.y) / rayForwardNormal.y;
	 	t = MinFloat(tX, tY);
	 	intersectionPoint = Vec3(t_result.m_impactPos.x, t_result.m_impactPos.y, rayStart.z + rayForwardNormal.z * t);
	 	if (intersectionPoint.z >= minMaxZ.m_min && intersectionPoint.z <= minMaxZ.m_max)
	 	{
	 		result.m_didImpact = true;
	 		result.m_impactPos = intersectionPoint;
	 		result.m_impactDist = (intersectionPoint - rayStart).GetLength();
			Vec2 normal = (Vec2(intersectionPoint.x, intersectionPoint.y) - centerXY).GetNormalized();
	 		result.m_impactNormal = Vec3(normal.x, normal.y,0.f);
	 		return result;
	 	}
	 }
	 return result;
	
}

RaycastResult3D RaycastVsOBB3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, OBB3 const& orientedBox)
{
	RaycastResult3D result;
	result.m_rayFwdNormal = rayForwardNormal;

	// Convert the ray into the local space of the OBB
	Vec3 localRayStart = orientedBox.GetLocalPosForWorldPos(rayStart);
	Vec3 localRayEnd = orientedBox.GetLocalPosForWorldPos(rayStart + rayForwardNormal * rayLength);
	AABB3 localAABB = AABB3(-orientedBox.m_halfDimensions, orientedBox.m_halfDimensions);
	RaycastResult3D localResult = RaycastVsAABB3D(localRayStart, (localRayEnd - localRayStart).GetNormalized(), (localRayEnd - localRayStart).GetLength(), localAABB);
	
	if (localResult.m_didImpact)
	{
		// Transform the intersection point and normal back to world space
		result.m_didImpact = true;
		result.m_impactPos = orientedBox.GetWorldPosForLocalPos(localResult.m_impactPos);
		result.m_impactDist = (result.m_impactPos - rayStart).GetLength();
		result.m_impactNormal = (orientedBox.m_iBasisNormal * localResult.m_impactNormal.x +
			orientedBox.m_jBasisNormal * localResult.m_impactNormal.y +
			orientedBox.m_kBasisNormal * localResult.m_impactNormal.z).GetNormalized();
	}

	return result;
}

RaycastResult3D RaycastVsPlane3D(Vec3 rayStart, Vec3 rayForwardNormal, float rayLength, Plane3 const& plane)
{
	RaycastResult3D result;
	Vec3 rayEnd = rayStart + rayForwardNormal * rayLength;
	result.m_rayFwdNormal = rayForwardNormal;
	if (plane.IsPointInTheFrontSide(rayStart) == plane.IsPointInTheFrontSide(rayEnd))
	{
		return result;
	}
	float localStartAltitude = fabsf(plane.GetAltitudeOfPoint(rayStart));
	float localEndAltitude = fabsf(plane.GetAltitudeOfPoint(rayEnd));
	result.m_didImpact = true;
	result.m_impactPos = rayStart + rayForwardNormal * rayLength * (localStartAltitude / (localEndAltitude + localStartAltitude));
	result.m_impactDist = (result.m_impactPos - rayStart).GetLength();
	if (plane.IsPointInTheFrontSide(rayStart))
	{
		result.m_impactNormal = plane.m_normal;
	}
	else
	{
		result.m_impactNormal = plane.m_normal*-1.f;
	}
	return result;
}
