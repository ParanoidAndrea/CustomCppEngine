#pragma once
#include "math.h"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/Plane3.hpp"
constexpr float PI = 3.14159265f;
struct Vec2;
struct Vec3;

enum class BillboardType
{
	NONE = -1,
	WORLD_UP_FACING,
	WORLD_UP_OPPOSING,
	FULL_FACING,
	FULL_OPPOSING,
	COUNT
};

Mat44 GetBillboardMatrix(BillboardType billboardType, Mat44 const& targetMatrix, Vec3 const& billboardPosition, Vec2 const& billboardScale = Vec2(1.f, 1.f));

float AbsFloat(float value);
float MaxFloat(float value1, float value2);
float MinFloat(float value1, float value2);
//Clamp and Lerp
int GetClamped(int value, int minValue, int maxValue);
float GetClamped(float value, float minValue, float maxValue);
float GetClampedZeroToOne(float value);
float Interpolate(float start, float end, float fractionTowardEnd);
Vec2 Interpolate(Vec2 startPos, Vec2 endPos, float fractionTowardEnd);
float GetFractionWithinRange(float value, float inEnd, float outStart, float outEnd);
float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd);
float RangeMapClamped(float inValue, float inStart, float inEnd, float outStart, float outEnd);
int   RoundDownToInt(float value);



// Angle utilities
float ConvertDegreesToRadians(float degrees);
float ConvertRadiansToDegrees(float radians);
float CosDegrees(float degrees);
float SinDegrees(float degrees);
float AcosDegrees(float degrees);
float AsinDegrees(float degrees);
float AtanDegrees(float degrees);
float Atan2Degrees(float y, float x);
float GetShortestAngularDispDegrees(float startDegrees, float endDegrees);
float GetTurnedTowardDegrees(float currentDegrees, float goalDegrees, float maxDeltaDegrees);
float GetAngleDegreesBetweenVectors2D(Vec2 const& a, Vec2 const& b);


//Dot and Cross
float DotProduct2D(Vec2 const& a, Vec2 const& b);
float DotProduct3D(Vec3 const& a, Vec3 const& b);
float DotProduct4D(Vec4 const& a, Vec4 const& b);
float CrossProduct2D(Vec2 const& a, Vec2 const& b);
Vec3 CrossProduct3D(Vec3 const& a, Vec3 const& b);

//Distance & projections utilities
float GetDistance2D( Vec2 const& positionA, Vec2 const& positionB );
float GetDistanceSquared2D( Vec2 const& positionA, Vec2 const& positionB );
float GetDistance3D( Vec3 const& positionA, Vec3 const& positionB );
float GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB);
float GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB);
float GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB);
int   GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB);
float GetProjectedLength2D(Vec2 const& vectorToProject, Vec2 const& vectorProTojectOnto); //normalize vectorProjectOnto
Vec2 const GetProjectedOnto2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto);
Vec3 const GetProjectedOnto3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto);

//Geometric query utilities
bool IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius);
bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box);
bool IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& orientedBox);
bool IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius);
bool IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius);
bool IsPointInsideSphere3D(Vec3 const& point, Vec3 const& sphereCenter, float sphereRadius);
bool IsPointInsideAABB3D(Vec3 const& point, AABB3 const& box);
bool IsPointInsideZCylinder3D(Vec3 const& point, Vec2 const& cylinderCenterXY, FloatRange cylinderMinMaxZ, float cylinderRadius);

bool DoDiscsOverLap(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB);
bool DoDiscAndAABB2DOverlap(Vec2 const& discCenter, float discRadius, AABB2 const& box);
bool DoDiscAndOBB2Overlap(Vec2 const& discCenter, float discRadius, OBB2 const& orientedBox);
bool DoDiscAndCapsuleOverlap(Vec2 const& discCenter, float discRadius, Vec2 const& boneStart, Vec2 const& boneEnd, float boneRadius);
bool DoSpheresOverLap(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB);
bool DoSpheresAndAABB3DOverlap(Vec3 const& sphereCenter, float sphereRadius, AABB3 const& box);
bool DoAABBsOverlap3D(AABB3 const& first, AABB3 const& second);
bool DoZCylindersOverlap3D(Vec2 const& cylinder1CenterXY, float cylinder1Radius, FloatRange cylinder1MinMaxZ, Vec2 const& cylinder2CenterXY, float cylinder2Radius, FloatRange cylinder2MinMaxZ);
bool DoZCylinderAndAABBOverlap3D(Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ, AABB3 const& box);
bool DoZCylinderAndSphereOverlap3D(Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ, Vec3 const& sphereCenter, float sphereRadius);
bool DoOBB3DAndPlane3DOverlap(OBB3 const& orientedBox, Plane3 const& plane);

Vec2 const GetNearestPointOnDisc2D(Vec2 const& referencePos, Vec2 const& discCenter, float discRaius);
Vec2 const GetNearestPointOnAABB2D(Vec2 const& referencePos, AABB2 const& box);
Vec2 const GetNearestPointOnInfiniteLine2D(Vec2 const& referencePos, Vec2 const& pointOnLine, Vec2 const& anotherPointOnLine);
Vec2 const GetNearestPointOnLineSegment2D(Vec2 const& referencePos, Vec2 const& lineStart, Vec2 const& lineEnd);
Vec2 const GetNearestPointOnCapsule2D(Vec2 const& referencePos, Vec2 const& boneStart, Vec2 const& boneEnd, float radius);
Vec2 const GetNearestPointOnOBB2D(Vec2 const& referencePos, OBB2 const& orientedBox);

Vec3 const GetNearestPointOnAABB3D(Vec3 const& referencePos, AABB3 const& box);
Vec3 const GetNearestPointOnSphere(Vec3 const& referencePos, Vec3 const& sphereCenter, float sphereRaius);
Vec3 const GetNearestPointOnZCylinder(Vec3 const& referencePos, Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange cylinderMinMaxZ);
Vec3 const GetNearestPointOnOBB3D(Vec3 const& referencePos, OBB3 const& orientedBox);

bool PushDiscOutOfFixedPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint);
bool PushDiscOutOfFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRaius, Vec2 const& fixedDiscCenter, float fixedDiscRadius);
bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRaius, Vec2& bCenter, float bRaius);
bool PushDiscOutOfFixedAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox);

bool PushDiscOutOfFixedPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint, Vec2& displacement);
bool PushDiscOutOfFixedDisc2D(Vec2& mobileDiscCenter, float mobileDiscRaius, Vec2 const& fixedDiscCenter, float fixedDiscRadius, Vec2& displacement);
bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRaius, Vec2& bCenter, float bRaius, Vec2& displacement);
bool PushDiscOutOfFixedAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox, Vec2& displacement);
bool PushDiscOutOfFixedCapsule2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& boneStart, Vec2 const& boneEnd, float boneRadius, Vec2& displacement);
bool PushDiscOutOfFixedOBB2D(Vec2& mobileDiscCenter, float discRadius, OBB2 const& orientedBox, Vec2& displacement);

bool PushAABB2OutOfFixedAABB2D(AABB2& mobileBox, AABB2 const& fixedBox);
//Transform utilities
void TransformPosition2D(Vec2& posToTransform, float uniformScale, float rotationDegrees, Vec2 const& translation);
void TransformPosition2D(Vec2& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);
void TransformPositionXY3D(Vec3& positionToTransform, float scaleXY, float rotationDegrees, Vec2 const& translation);
void TransformPositionXY3D(Vec3& positionToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation);

float NormalizeByte(unsigned char byteValue);
unsigned char DenormalizeByte(float zeroToOne);

float ComputeCubicBezier1D(float A, float B, float C, float D, float t);
float ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t);