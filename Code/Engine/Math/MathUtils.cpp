#include "MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
Mat44 GetBillboardMatrix(BillboardType billboardType, Mat44 const& targetMatrix, Vec3 const& billboardPosition, Vec2 const& billboardScale)
{
	Vec3 iBasis, jBasis, kBasis;
	switch (billboardType)
	{
	case BillboardType::FULL_FACING:
		iBasis = (targetMatrix.GetTranslation3D() - billboardPosition).GetNormalized();
		if (abs(DotProduct3D(iBasis, Vec3(0.f, 0.f, 1.f))) != 1.f)
		{
			jBasis = CrossProduct3D(Vec3(0.f,0.f,1.f),iBasis);
			kBasis = CrossProduct3D(iBasis, jBasis);
		}
		else
		{
			kBasis = CrossProduct3D(iBasis, Vec3(0.f,1.f,0.f));
			jBasis = CrossProduct3D(kBasis, iBasis);
		}
		return Mat44(iBasis * billboardScale.x, jBasis * billboardScale.y, kBasis, billboardPosition);
	case BillboardType::FULL_OPPOSING:
		iBasis = -targetMatrix.GetIBasis3D().GetNormalized();
		jBasis = -targetMatrix.GetJBasis3D().GetNormalized();
		kBasis = targetMatrix.GetKBasis3D().GetNormalized();
		return Mat44(iBasis * billboardScale.x, jBasis * billboardScale.y, kBasis, billboardPosition);
	case BillboardType::WORLD_UP_FACING:
		kBasis = Vec3(0.f, 0.f, 1.f);
		iBasis = (targetMatrix.GetTranslation3D() - billboardPosition).GetNormalized();
		iBasis.z = 0.f;
		iBasis = iBasis.GetNormalized();
		jBasis = Vec3(-iBasis.y, iBasis.x, 0.f);
		return Mat44(iBasis * billboardScale.x, jBasis * billboardScale.y, kBasis, billboardPosition);
	case BillboardType::WORLD_UP_OPPOSING:
		if (abs(DotProduct3D(targetMatrix.GetIBasis3D(), Vec3(0.f, 0.f, 1.f))) == 1.f)
		{
			return Mat44(billboardPosition);
		}
		kBasis = Vec3(0.f, 0.f, 1.f);
		iBasis = -targetMatrix.GetIBasis3D().GetNormalized();
		iBasis.z = 0.f;
		iBasis = iBasis.GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
		return Mat44(iBasis * billboardScale.x, jBasis * billboardScale.y, kBasis, billboardPosition);
	}
	

	return Mat44(billboardPosition);
}

float AbsFloat(float value)
{
	return fabsf(value);
}

float MaxFloat(float value1, float value2)
{
	return std::max(value1, value2);
}

float MinFloat(float value1, float value2)
{
	return std::min(value1, value2);
}

float GetClamped(float value, float minValue, float maxValue)
{
	if (value > maxValue)
	{
		value = maxValue;
	}
	if (value < minValue)
	{
		value = minValue;
	}
	return value;
}

int GetClamped(int value, int minValue, int maxValue)
{
	if (value > maxValue)
	{
		value = maxValue;
	}
	if (value < minValue)
	{
		value = minValue;
	}
	return value;
}

float GetClampedZeroToOne(float value)
{
	if (value > 1.f)
	{
		value = 1.f;
	}
	if (value < 0.f)
	{
		value = 0.f;
	}
	return value;
}

float Interpolate(float start, float end, float fractionTowardEnd)
{
	float range = end - start;
	float disWithinRange = fractionTowardEnd * range;
	return start+disWithinRange;
}

Vec2 Interpolate(Vec2 startPos, Vec2 endPos, float fractionTowardEnd)
{
	return startPos + (endPos - startPos) * fractionTowardEnd;
}

float GetFractionWithinRange(float value, float inEnd, float outStart, float outEnd)
{
	float range = outEnd - outStart;
	float inStart = inEnd - range;
	return (value - inStart)/range;
}

float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	float fraction = (inValue - inStart) / (inEnd - inStart);
	float range = outEnd - outStart;
	return range * fraction + outStart;
}

float RangeMapClamped(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	inValue = GetClamped(inValue, inStart, inEnd);
	return RangeMap(inValue, inStart, inEnd, outStart, outEnd);
}

int RoundDownToInt(float value)
{
	int result = 0;
	if (value>0.f)
	{
	    result = (int)value;
	}
	else
	{
		result = (int)value-1;
	}
	return result;
}



float ConvertDegreesToRadians(float degrees)
{
	float radians = degrees * PI / 180.f;
	return radians;
}

float ConvertRadiansToDegrees(float radians)
{
	float degrees = radians * 180.f / PI;
	return degrees;
}

float CosDegrees(float degrees)
{
	float t_cos=cosf(ConvertDegreesToRadians(degrees));
	return t_cos;
}

float SinDegrees(float degrees)
{
	float t_sin = sinf(ConvertDegreesToRadians(degrees));
	return t_sin;
}

float AcosDegrees(float degrees)
{
	return acosf(ConvertDegreesToRadians(degrees));
}

float AsinDegrees(float degrees)
{
	return asinf(ConvertDegreesToRadians(degrees));
}

float AtanDegrees(float degrees)
{
	return atanf(ConvertDegreesToRadians(degrees));
}

float Atan2Degrees(float y, float x)
{
	float t_atan2 = ConvertRadiansToDegrees(atan2f(y, x));
	return t_atan2;
}


float GetShortestAngularDispDegrees(float startDegrees, float endDegrees)
{
	float spinDegree= endDegrees - startDegrees;
	while (spinDegree > 180.f)
	{
		spinDegree -= 360.f;
	}
	while (spinDegree < -180.f)
	{
		spinDegree += 360.f;
	}
	return spinDegree;
}

float GetTurnedTowardDegrees(float currentDegrees, float goalDegrees, float maxDeltaDegrees)
{
	float shortestDegrees = GetShortestAngularDispDegrees(currentDegrees, goalDegrees);
	//currentDegrees = fmod(currentDegrees, 360.f);
	return currentDegrees + GetClamped(shortestDegrees, -maxDeltaDegrees, maxDeltaDegrees);

}

float GetAngleDegreesBetweenVectors2D(Vec2 const& a, Vec2 const& b)
{
	float aLength = a.GetLength();
	float bLength = b.GetLength();
	float dotProductAandB = DotProduct2D(a, b);
	float cosThetaRadians = dotProductAandB / (aLength * bLength);
	return ConvertRadiansToDegrees(acosf(cosThetaRadians));
}


float DotProduct2D(Vec2 const& a, Vec2 const& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float DotProduct3D(Vec3 const& a, Vec3 const& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float DotProduct4D(Vec4 const& a, Vec4 const& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float CrossProduct2D(Vec2 const& a, Vec2 const& b)
{
	return a.x*b.y - b.x*a.y;
}

Vec3 CrossProduct3D(Vec3 const& a, Vec3 const& b)
{
	return Vec3(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
}

float GetDistanceSquared2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float t_xpower2 = (positionA.x - positionB.x) * (positionA.x - positionB.x);
	float t_ypower2 = (positionA.y - positionB.y) * (positionA.y - positionB.y);
	return t_ypower2 + t_xpower2;
}

float GetDistance2D(Vec2 const& positionA, Vec2 const& positionB)
{
	return sqrtf(GetDistanceSquared2D(positionA,positionB));
}

float GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float t_xpower2 = (positionA.x - positionB.x)* (positionA.x - positionB.x);
	float t_ypower2 = (positionA.y - positionB.y)* (positionA.y - positionB.y);
	float t_zpower2 = (positionA.z - positionB.z)* (positionA.z - positionB.z);
	return t_xpower2 + t_ypower2 + t_zpower2;
}

float GetDistance3D(Vec3 const& positionA, Vec3 const& positionB)
{
	return sqrtf(GetDistanceSquared3D(positionA, positionB));
}

float GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float t_xpower2 = (positionA.x - positionB.x)* (positionA.x - positionB.x);
	float t_ypower2 = (positionA.y - positionB.y)* (positionA.y - positionB.y);
	return t_ypower2 + t_xpower2;
}

int GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB)
{
	int taxicabDistance = 0;
	if (pointA.x > pointB.x)
	{
		taxicabDistance += (pointA.x - pointB.x);
	}
	else
	{
		taxicabDistance += (pointB.x - pointA.x);
	}
	if (pointA.y > pointB.y)
	{
		taxicabDistance += (pointA.y - pointB.y);
	}
	else
	{
		taxicabDistance += (pointB.y - pointA.y);
	}

	return taxicabDistance;
		
}

float GetProjectedLength2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto)
{
	Vec2 nNormalized = vectorToProjectOnto.GetNormalized();
	float length = DotProduct2D(vectorToProject, nNormalized);
	return length;
}

Vec2 const GetProjectedOnto2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto)
{
	Vec2 nNormalized = vectorToProjectOnto.GetNormalized();
	float length = DotProduct2D(vectorToProject, nNormalized);
	return nNormalized * length;
}

Vec3 const GetProjectedOnto3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto)
{
	Vec3 nNormalized = vectorToProjectOnto.GetNormalized();
	float length = DotProduct3D(vectorToProject, nNormalized);
	return nNormalized * length;
}

bool IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius)
{
	Vec2 pointToCenter = point - discCenter;
	float distanceSquare = pointToCenter.x * pointToCenter.x + pointToCenter.y * pointToCenter.y;
	if (distanceSquare >= discRadius * discRadius)
	{
		return false;
	}
	return true;
}

bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box)
{
	return (point.x > box.m_mins.x && point.x < box.m_maxs.x && point.y > box.m_mins.y && point.y < box.m_maxs.y);
}

bool IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 boneDirection = boneEnd - boneStart;
	Vec2 boneStartToPoint = point - boneStart;
	Vec2 boneEndToPoint = point - boneEnd;

	if (DotProduct2D(boneStartToPoint, boneDirection) <= 0)
	{
		float pointToStartDistance = boneStartToPoint.GetLength();
		if (pointToStartDistance < radius)
			return true;
		else
			return false;
	}

	if (DotProduct2D(boneEndToPoint, boneDirection) >= 0)
	{
		float pointToEndDistance = boneEndToPoint.GetLength();
		if (pointToEndDistance < radius)
			return true;
		else
			return false;
	}

	Vec2 startToPointProjection = GetProjectedOnto2D(boneStartToPoint, boneDirection)+boneStart;
	Vec2 pointToProjection = startToPointProjection - point;
	float distance = pointToProjection.GetLength();
	if (distance < radius)
		return true;
	else 
		return false;
}

bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& orientedBox)
{
	Vec2 jBasisNormal = orientedBox.m_iBasisNormal.GetRotated90Degrees();
	Vec2 centerToPoint = point - orientedBox.m_center;
	float localPointiBasis = DotProduct2D(centerToPoint, orientedBox.m_iBasisNormal);
	float localPointjBasis = DotProduct2D(centerToPoint, jBasisNormal);
	if (localPointiBasis >= orientedBox.m_halfDimensions.x || localPointiBasis <= 
		-orientedBox.m_halfDimensions.x)
		return false;

	if (localPointjBasis >= orientedBox.m_halfDimensions.y || localPointjBasis <= -orientedBox.m_halfDimensions.y)
		return false;

	return true;
}

bool IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius)
{
	if (!IsPointInsideDisc2D(point, sectorTip, sectorRadius))
		return false;
	else
	{
		Vec2 tipToPoint = point - sectorTip;
		Vec2 sectorForwardNormal = Vec2(CosDegrees(sectorForwardDegrees), SinDegrees(sectorForwardDegrees));
		if (GetAngleDegreesBetweenVectors2D(tipToPoint,sectorForwardNormal)  > sectorApertureDegrees/2.f)
			return false;
		else 
			return true;
	}
}

bool IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius)
{
	if (!IsPointInsideDisc2D(point, sectorTip, sectorRadius))
		return false;
	else
	{
		Vec2 tipToPoint = point - sectorTip;
		if (GetAngleDegreesBetweenVectors2D(tipToPoint, sectorForwardNormal) > sectorApertureDegrees / 2.f)
			return false;
		else
			return true;
	}
}

bool IsPointInsideSphere3D(Vec3 const& point, Vec3 const& sphereCenter, float sphereRadius)
{
	float distanceSquare = GetDistanceSquared3D(point, sphereCenter);
	if (distanceSquare >= sphereRadius * sphereRadius)
	{
		return false;
	}
	return true;
}

bool IsPointInsideAABB3D(Vec3 const& point, AABB3 const& box)
{
	return (point.x > box.m_mins.x && point.x < box.m_maxs.x&& point.y > box.m_mins.y && point.y < box.m_maxs.y&& point.z > box.m_mins.z && point.z < box.m_maxs.z);
}

bool IsPointInsideZCylinder3D(Vec3 const& point, Vec2 const& cylinderCenterXY, FloatRange cylinderMinMaxZ, float cylinderRadius)
{
	float distanceSpuareXY = GetDistanceSquared2D(cylinderCenterXY, Vec2(point.x, point.y));
	if (distanceSpuareXY >= cylinderRadius * cylinderRadius)
	{
		return false;
	}
	else if (!cylinderMinMaxZ.IsOnRange(point.z))
	{
		return false;
	}
	return true;
}

float GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB)
{
	return sqrtf(GetDistanceXYSquared3D(positionA, positionB));
}

bool DoDiscsOverLap(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB)
{
	float t_distanceSquared = GetDistanceSquared2D(centerA, centerB);
	return (t_distanceSquared <= (radiusA + radiusB) * (radiusA + radiusB));
}

bool DoDiscAndAABB2DOverlap(Vec2 const& discCenter, float discRadius, AABB2 const& box)
{
	Vec2 nearestPos = GetNearestPointOnAABB2D(discCenter, box);
	if (GetDistanceSquared2D(discCenter, nearestPos) <= discRadius * discRadius)
	{
		return true;
	}
	return false;
}

bool DoDiscAndOBB2Overlap(Vec2 const& discCenter, float discRadius, OBB2 const& orientedBox)
{
	Vec2 nearestPos = GetNearestPointOnOBB2D(discCenter, orientedBox);
	if (GetDistanceSquared2D(discCenter, nearestPos) <= discRadius * discRadius)
	{
		return true;
	}
	return false;
}

bool DoDiscAndCapsuleOverlap(Vec2 const& discCenter, float discRadius, Vec2 const& boneStart, Vec2 const& boneEnd, float boneRadius)
{
	Vec2 nearestPos = GetNearestPointOnCapsule2D(discCenter, boneStart, boneEnd, boneRadius);
	if (GetDistanceSquared2D(discCenter, nearestPos) <= discRadius * discRadius)
	{
		return true;
	}
	return false;
}

bool DoSpheresOverLap(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB)
{
	float t_distanceSquared = GetDistanceSquared3D(centerA,centerB);
	return (t_distanceSquared <= (radiusA + radiusB)* (radiusA + radiusB));
}

bool DoSpheresAndAABB3DOverlap(Vec3 const& sphereCenter, float sphereRadius, AABB3 const& box)
{
	Vec3 nearestPos = GetNearestPointOnAABB3D(sphereCenter, box);
	if (GetDistanceSquared3D(nearestPos, sphereCenter) <= sphereRadius * sphereRadius)
	{
		return true;
	}
	return false;
}

bool DoAABBsOverlap3D(AABB3 const& first, AABB3 const& second)
{
	return (first.m_maxs.x >= second.m_mins.x) && (first.m_mins.x <= second.m_maxs.x) && (first.m_maxs.y >= second.m_mins.y) && (first.m_mins.y <= second.m_maxs.y) && (first.m_maxs.z >= second.m_mins.z) && (first.m_mins.z <= second.m_maxs.z);
}

bool DoZCylindersOverlap3D(Vec2 const& cylinder1CenterXY, float cylinder1Radius, FloatRange cylinder1MinMaxZ, Vec2 const& cylinder2CenterXY, float cylinder2Radius, FloatRange cylinder2MinMaxZ)
{
	if (!DoDiscsOverLap(cylinder1CenterXY, cylinder1Radius, cylinder2CenterXY, cylinder2Radius))
	{
		return false;
	}
	if (cylinder1MinMaxZ.IsOverlappingWith(cylinder2MinMaxZ))
	{
		return true;
	}
	return false;
}

bool DoZCylinderAndAABBOverlap3D(Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ, AABB3 const& box)
{
	AABB2 boxXY = AABB2(box.m_mins.x, box.m_mins.y, box.m_maxs.x, box.m_maxs.y);
	if (!DoDiscAndAABB2DOverlap(cylinderCenterXY, cylinderRadius, boxXY))
	{
		return false;
	}
	FloatRange boxMinZMaxZ = FloatRange(box.m_mins.z, box.m_maxs.z);
	if (cylinderMinMaxZ.IsOverlappingWith(boxMinZMaxZ))
	{
		return true;
	}
	return false;
}

bool DoZCylinderAndSphereOverlap3D(Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ, Vec3 const& sphereCenter, float sphereRadius)
{
	Vec3 nearestPoint = GetNearestPointOnZCylinder(sphereCenter, cylinderCenterXY, cylinderRadius, cylinderMinMaxZ);
	if (GetDistanceSquared3D(nearestPoint, sphereCenter) <= sphereRadius * sphereRadius)
	{
		return true;
	}
	return false;
}

bool DoOBB3DAndPlane3DOverlap(OBB3 const& orientedBox, Plane3 const& plane)
{
	Vec3 cornerPoints[8];
	orientedBox.GetCornerPoints(cornerPoints);

	for (int i = 0; i < 8; ++i)
	{
		for (int j = i+1 ; j < 8; ++j)
		{
			if (plane.IsPointInTheFrontSide(cornerPoints[i]) != plane.IsPointInTheFrontSide(cornerPoints[j]))
			{
				return true;
			}
		}
	}
	return false;
}

Vec2 const GetNearestPointOnDisc2D(Vec2 const& referencePosition, Vec2 const& discCenter, float discRaius)
{
	Vec2 displacement = referencePosition - discCenter;
	displacement = displacement.GetClamped(discRaius);
	return discCenter + displacement;
}

Vec3 const GetNearestPointOnSphere(Vec3 const& referencePos, Vec3 const& sphereCenter, float sphereRaius)
{
	Vec3 displacement = referencePos - sphereCenter;
	displacement = displacement.GetClamped(sphereRaius);
	return sphereCenter + displacement;
}

Vec3 const GetNearestPointOnZCylinder(Vec3 const& referencePos, Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ)
{
	Vec2 referencePosXY = Vec2(referencePos.x, referencePos.y);
	Vec2 nearestPointXY = GetNearestPointOnDisc2D(referencePosXY, cylinderCenterXY, cylinderRadius);
	if (cylinderMinMaxZ.IsOnRange(referencePos.z))
	{
		return Vec3(nearestPointXY.x, nearestPointXY.y, referencePos.z);
	}
	else if (referencePos.z > cylinderMinMaxZ.m_max)
	{
		return Vec3(nearestPointXY.x, nearestPointXY.y, cylinderMinMaxZ.m_max);
	}
	else
	{
		return Vec3(nearestPointXY.x, nearestPointXY.y, cylinderMinMaxZ.m_min);
	}

}



Vec2 const GetNearestPointOnAABB2D(Vec2 const& referencePos, AABB2 const& box)
{
	return box.GetNearestPoint(referencePos);
}

Vec2 const GetNearestPointOnInfiniteLine2D(Vec2 const& referencePos, Vec2 const& pointOnLine, Vec2 const& anotherPointOnLine)
{
	Vec2 lineDirection = anotherPointOnLine - pointOnLine;
	Vec2 lineStartToPoint = referencePos - pointOnLine;
	Vec2 lineEndToPoint = referencePos - anotherPointOnLine;

	if (DotProduct2D(lineStartToPoint, lineDirection) <= 0)
	{
		return pointOnLine;
	}

	if (DotProduct2D(lineEndToPoint, lineDirection) >= 0)
	{
		return anotherPointOnLine;
	}

	return GetProjectedOnto2D(lineStartToPoint, lineDirection) + pointOnLine;
}

Vec2 const GetNearestPointOnLineSegment2D(Vec2 const& referencePos, Vec2 const& lineStart, Vec2 const& lineEnd)
{
	Vec2 lineDirection = lineEnd - lineStart;
	Vec2 lineStartToPoint = referencePos - lineStart;
	Vec2 lineEndToPoint = referencePos - lineEnd;

	if (DotProduct2D(lineStartToPoint, lineDirection) <= 0)
	{
		return lineStart;
	}

	if (DotProduct2D(lineEndToPoint, lineDirection) >= 0)
	{
		return lineEnd;
	}

	return GetProjectedOnto2D(lineStartToPoint, lineDirection) + lineStart;

}

Vec2 const GetNearestPointOnCapsule2D(Vec2 const& referencePos, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 boneDirection = boneEnd - boneStart;
	Vec2 boneStartToPoint = referencePos - boneStart;
	Vec2 boneEndToPoint = referencePos - boneEnd;

	if (DotProduct2D(boneStartToPoint, boneDirection) <= 0)
	{
		float pointToStartDistance = boneStartToPoint.GetLength();
		if (pointToStartDistance < radius)
			return referencePos;
		else
		{
			boneStartToPoint.SetLength(radius);
			return boneStart+boneStartToPoint;
		}
	}

	if (DotProduct2D(boneEndToPoint, boneDirection) >= 0)
	{
		float pointToEndDistance = boneEndToPoint.GetLength();
		if (pointToEndDistance < radius)
			return referencePos;
		else
		{
			boneEndToPoint.SetLength(radius);
			return boneEnd+boneEndToPoint;
		}
	}

	Vec2 startToPointProjection = GetProjectedOnto2D(boneStartToPoint, boneDirection) + boneStart;
	Vec2 pointToProjection = startToPointProjection - referencePos;
	float distance = pointToProjection.GetLength();
	if (distance < radius)
		return referencePos;
	else
	{
		return startToPointProjection-pointToProjection.GetNormalized()*radius;
	}
}

Vec2 const GetNearestPointOnOBB2D(Vec2 const& referencePos, OBB2 const& orientedBox)
{
	Vec2 jBasisNormal = orientedBox.m_iBasisNormal.GetRotated90Degrees();
	Vec2 centerToPoint = referencePos - orientedBox.m_center;
	float localPointiBasis = DotProduct2D(centerToPoint, orientedBox.m_iBasisNormal);
	float localPointjBasis = DotProduct2D(centerToPoint, jBasisNormal);
	float localNearestPointiBasis = GetClamped(localPointiBasis, -orientedBox.m_halfDimensions.x, orientedBox.m_halfDimensions.x);
	float localNearestPointjBasis = GetClamped(localPointjBasis, -orientedBox.m_halfDimensions.y, orientedBox.m_halfDimensions.y);
	return orientedBox.m_center + orientedBox.m_iBasisNormal * localNearestPointiBasis + jBasisNormal * localNearestPointjBasis;
}

Vec3 const GetNearestPointOnOBB3D(Vec3 const& referencePos, OBB3 const& orientedBox)
{
	//Vec3 kBasisNormal = CrossProduct3D(orientedBox.m_iBasisNormal, orientedBox.m_jBasisNormal).GetNormalized();
	Vec3 centerToPoint = referencePos - orientedBox.m_center;
	float localPointiBasis = DotProduct3D(centerToPoint, orientedBox.m_iBasisNormal);
	float localPointjBasis = DotProduct3D(centerToPoint, orientedBox.m_jBasisNormal);
	float localPointkBasis = DotProduct3D(centerToPoint, orientedBox.m_kBasisNormal);

	float localNearestPointiBasis = GetClamped(localPointiBasis, -orientedBox.m_halfDimensions.x, orientedBox.m_halfDimensions.x);
	float localNearestPointjBasis = GetClamped(localPointjBasis, -orientedBox.m_halfDimensions.y, orientedBox.m_halfDimensions.y);
	float localNearestPointkBasis = GetClamped(localPointkBasis, -orientedBox.m_halfDimensions.z, orientedBox.m_halfDimensions.z);

	return orientedBox.m_center + orientedBox.m_iBasisNormal * localNearestPointiBasis +
		orientedBox.m_jBasisNormal * localNearestPointjBasis + orientedBox.m_kBasisNormal * localNearestPointkBasis;
}

Vec3 const GetNearestPointOnAABB3D(Vec3 const& referencePos, AABB3 const& box)
{
	return box.GetNearestPoint(referencePos);
}

bool PushDiscOutOfFixedPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint)
{
	if (!IsPointInsideDisc2D(fixedPoint, mobileDiscCenter, discRadius))
		return false;
	Vec2 displacement = mobileDiscCenter - fixedPoint;
	float overlapDepth = discRadius - displacement.GetLength();
	displacement.SetLength(overlapDepth);
	mobileDiscCenter = mobileDiscCenter + displacement;
	return true;
}

bool PushDiscOutOfFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRaius, Vec2 const& fixedDiscCenter, float fixedDiscRadius)
{
	if (!DoDiscsOverLap(fixedDiscCenter, fixedDiscRadius, mobileDiscCenter, mobileDiscRaius))
		return false;
	Vec2 displacement = mobileDiscCenter - fixedDiscCenter;
	float overlapDepth = (mobileDiscRaius + fixedDiscRadius) - displacement.GetLength();
	displacement.SetLength(overlapDepth);
	mobileDiscCenter = mobileDiscCenter + displacement;
	return true;
}

bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRaius, Vec2& bCenter, float bRaius)
{
	if (!DoDiscsOverLap(aCenter, aRaius, bCenter, bRaius))
		return false;
	Vec2 displacement = aCenter - bCenter;
	float overlapDepth = (aRaius + bRaius) - displacement.GetLength();
	displacement.SetLength(overlapDepth/2);
	aCenter = aCenter + displacement;
	bCenter = bCenter - displacement;
	return true;
}

bool PushDiscOutOfFixedAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox)
{
	Vec2 nearestPoint = fixedBox.GetNearestPoint(mobileDiscCenter);
	return PushDiscOutOfFixedPoint2D(mobileDiscCenter, discRadius, nearestPoint);
}

bool PushDiscOutOfFixedPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint, Vec2& displacement)
{
	displacement = mobileDiscCenter - fixedPoint;
	float overlapDepth = discRadius - displacement.GetLength();
	displacement.SetLength(overlapDepth);
	mobileDiscCenter = mobileDiscCenter + displacement;
	return true;
}

bool PushDiscOutOfFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRaius, Vec2 const& fixedDiscCenter, float fixedDiscRadius, Vec2& displacement)
{
	displacement = mobileDiscCenter - fixedDiscCenter;
	float overlapDepth = (mobileDiscRaius + fixedDiscRadius) - displacement.GetLength();
	displacement.SetLength(overlapDepth);
	mobileDiscCenter = mobileDiscCenter + displacement;
	return true;
}

bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRaius, Vec2& bCenter, float bRaius, Vec2& displacement)
{

	displacement = aCenter - bCenter;
	float overlapDepth = (aRaius + bRaius) - displacement.GetLength();
	displacement.SetLength(overlapDepth / 2);
	aCenter = aCenter + displacement;
	bCenter = bCenter - displacement;
	return true;
}

bool PushDiscOutOfFixedAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox, Vec2& displacement)
{
	Vec2 nearestPoint = fixedBox.GetNearestPoint(mobileDiscCenter);
	return PushDiscOutOfFixedPoint2D(mobileDiscCenter, discRadius, nearestPoint,displacement);
}

bool PushDiscOutOfFixedCapsule2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& boneStart, Vec2 const& boneEnd, float boneRadius, Vec2& displacement)
{
	Vec2 nearestPoint = GetNearestPointOnCapsule2D(mobileDiscCenter, boneStart, boneEnd, boneRadius);
	return PushDiscOutOfFixedPoint2D(mobileDiscCenter, discRadius, nearestPoint, displacement);
}

bool PushDiscOutOfFixedOBB2D(Vec2& mobileDiscCenter, float discRadius, OBB2 const& orientedBox, Vec2& displacement)
{
	Vec2 nearestPoint = GetNearestPointOnOBB2D(mobileDiscCenter, orientedBox);
	return PushDiscOutOfFixedPoint2D(mobileDiscCenter, discRadius, nearestPoint, displacement);
}


bool PushAABB2OutOfFixedAABB2D(AABB2& mobileBox, AABB2 const& fixedBox)
{
	Vec2 mobileCenter = mobileBox.GetCenter();
	Vec2 fixedCenter = fixedBox.GetCenter();

	Vec2 displacement = mobileCenter - fixedCenter;

	Vec2 halfExtentMobile = 0.5f * mobileBox.GetDimensions();
	Vec2 halfExtentFixed = 0.5f * fixedBox.GetDimensions();

	float overlapX = halfExtentMobile.x + halfExtentFixed.x - AbsFloat(displacement.x);
	float overlapY = halfExtentMobile.y + halfExtentFixed.y - AbsFloat(displacement.y);

	// If there's no overlap, return false
	if (overlapX <= 0 || overlapY <= 0)
		return false;

	// Determine which direction to push the mobile AABB2
	if (overlapX < overlapY)
	{
		if (displacement.x < 0)
		{
			mobileBox.Translate(Vec2(overlapX, 0));
		}
		else
		{
			mobileBox.Translate(Vec2(-overlapX, 0));
		}
	}
	else 
	{
		if (displacement.y < 0)
			mobileBox.Translate(Vec2(0, overlapY));
		else
			mobileBox.Translate(Vec2(0, -overlapY));
	}

	return true;
	
}

void TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotationDegrees, Vec2 const& translation)
{
	posToTransform.x *= uniformScale;
	posToTransform.y *= uniformScale;
	float t_length = posToTransform.GetLength();
	float t_degree = Atan2Degrees(posToTransform.y, posToTransform.x)+rotationDegrees;
	posToTransform.x = t_length * CosDegrees(t_degree);
	posToTransform.y = t_length * SinDegrees(t_degree);
	posToTransform.x += translation.x;
	posToTransform.y += translation.y;
}
 
void TransformPosition2D(Vec2& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	posToTransform = translation + posToTransform.x * iBasis + posToTransform.y * jBasis;
}

void TransformPositionXY3D(Vec3& positionToTransform, float scaleXY, float rotationDegrees, Vec2 const& translation)
{
	positionToTransform.x *= scaleXY;
	positionToTransform.y *= scaleXY;
	float t_length = positionToTransform.GetLengthXY();
	float t_degree = Atan2Degrees(positionToTransform.y, positionToTransform.x) + rotationDegrees;
	positionToTransform.x = t_length * CosDegrees(t_degree);
	positionToTransform.y = t_length * SinDegrees(t_degree);
	positionToTransform.x += translation.x;
	positionToTransform.y += translation.y;
}
 
void TransformPositionXY3D(Vec3& positionToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	Vec2 iBasisTransform = positionToTransform.x * iBasis;
	Vec2 jBasisTransform = positionToTransform.y * jBasis;
	positionToTransform = Vec3(translation.x, translation.y, positionToTransform.z) + Vec3(iBasisTransform.x, iBasisTransform.y, 0.f) + Vec3(jBasisTransform.x, jBasisTransform.y, 0.f);
}

float NormalizeByte(unsigned char byteValue)
{
	float oneNormalizeFloat = 1.f / 255.f;
	if (byteValue > 255)
	{
		return 1.f;

	}
	if (byteValue < 0)
	{
		return 0.f;
	}

	return GetClamped(byteValue*oneNormalizeFloat,0.f,1.f);
}

unsigned char DenormalizeByte(float zeroToOne)
{
	if (zeroToOne == 1.f)
	{
		return 255;
	}
	zeroToOne = GetClamped(zeroToOne, 0.f, 1.f);

	return (unsigned char)(zeroToOne *256.f);
}

float ComputeCubicBezier1D(float A, float B, float C, float D, float t)
{
	// First level of interpolation
	float P0 = Interpolate(A, B, t);
	float P1 = Interpolate(B, C, t);
	float P2 = Interpolate(C, D, t);

	// Second level of interpolation
	float Q0 = Interpolate(P0, P1, t);
	float Q1 = Interpolate(P1, P2, t);

	// Final interpolation
	return Interpolate(Q0, Q1, t);
}

float ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t)
{
	// First level of interpolation
	float p0 = Interpolate(A, B, t);
	float p1 = Interpolate(B, C, t);
	float p2 = Interpolate(C, D, t);
	float p3 = Interpolate(D, E, t);
	float p4 = Interpolate(E, F, t);

	// Second level of interpolation
	float q0 = Interpolate(p0, p1, t);
	float q1 = Interpolate(p1, p2, t);
	float q2 = Interpolate(p2, p3, t);
	float q3 = Interpolate(p3, p4, t);

	// Third level of interpolation
	float r0 = Interpolate(q0, q1, t);
	float r1 = Interpolate(q1, q2, t);
	float r2 = Interpolate(q2, q3, t);

	// Final interpolation
	return Interpolate(Interpolate(r0, r1, t), Interpolate(r1, r2, t), t);
}
