 #include "Vertex_PCU.hpp"
#include "VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "EngineCommon.hpp"
void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float uniformScaleXY, float rotationDegreesAboutZ, Vec2 const& translationXY)
{
	for (int vertIndex = 0; vertIndex < numVerts; ++vertIndex)
	{
		Vec3& pos = verts[vertIndex].m_position;
		TransformPositionXY3D(pos, uniformScaleXY, rotationDegreesAboutZ, translationXY);
	}
}

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color)
{
	Vec2 capsuleDirection = boneEnd - boneStart;
	float capsuleLength = capsuleDirection.GetLength();
	capsuleDirection.Normalize();
	AddVertsForOBB2D(verts, OBB2((boneStart+boneEnd)*0.5f, capsuleDirection, Vec2(capsuleLength/2.f,radius)),color);
	//AddVertsForDisc2D(verts, boneStart, radius, color);
	//AddVertsForDisc2D(verts, boneEnd, radius, color);
	float orientationDegree = capsuleDirection.GetOrientationDegrees();
	AddVertsForSemiDisc2D(verts, boneStart, radius,color, orientationDegree+90.f);
	AddVertsForSemiDisc2D(verts, boneEnd, radius, color, orientationDegree-90.f);
}

void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color)
{
	constexpr int NUM_SIDES = 32;
	constexpr int NUM_TRIS = 2 * NUM_SIDES;
	constexpr int NUM_VERTS = 3 * NUM_TRIS;
	verts.reserve(verts.size() + NUM_VERTS);
	constexpr float DEGREES_PER_SIDE = 360.f / static_cast<float>(NUM_SIDES);
	for (int sideNum = 0; sideNum < NUM_SIDES; ++sideNum)
	{
		float startDegrees = DEGREES_PER_SIDE * static_cast<float>(sideNum);
		float endDegrees = DEGREES_PER_SIDE * static_cast<float>(sideNum + 1);


		Vec3 innerStartPos = Vec3(center.x, center.y, 0.f);
		Vec3 innerEndPos   = Vec3(center.x, center.y, 0.f);
		Vec3 outerStartPos = Vec3(center.x + radius * CosDegrees(startDegrees), center.y + radius * SinDegrees(startDegrees), 0.f);
		Vec3 outerEndPos   = Vec3(center.x + radius * CosDegrees(endDegrees),   center.y + radius * SinDegrees(endDegrees), 0.f);

		// Trapezoid is made of two triangles ABC and DEF
		// A is inner end; B is inner start; C is outer start
		// D is inner end; E is outer start; F is outer end;
		verts.push_back(Vertex_PCU(innerEndPos,   color, Vec2(0.f, 0.f)));
		verts.push_back(Vertex_PCU(innerStartPos, color, Vec2(0.f, 0.f)));
		verts.push_back(Vertex_PCU(outerStartPos, color, Vec2(0.f, 0.f)));
		verts.push_back(Vertex_PCU(innerEndPos,   color, Vec2(0.f, 0.f)));
		verts.push_back(Vertex_PCU(outerStartPos, color, Vec2(0.f, 0.f)));
		verts.push_back(Vertex_PCU(outerEndPos,   color, Vec2(0.f, 0.f)));
	}
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color)
{
	constexpr int NUM_TRIS = 2;
	constexpr int NUM_VERTS = 3 * NUM_TRIS;

	verts.reserve(verts.size() + NUM_VERTS);

	Vec3 BL = Vec3(bounds.m_mins.x, bounds.m_mins.y, 0.f);
	Vec3 BR = Vec3(bounds.m_maxs.x, bounds.m_mins.y, 0.f);
	Vec3 TR = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0.f);
	Vec3 TL = Vec3(bounds.m_mins.x, bounds.m_maxs.y, 0.f);

	verts.push_back(Vertex_PCU(BL, color, Vec2(0.f, 0.f)));
	verts.push_back(Vertex_PCU(BR, color, Vec2(1.f, 0.f)));
	verts.push_back(Vertex_PCU(TL, color, Vec2(0.f, 1.f)));

	verts.push_back(Vertex_PCU(BR, color, Vec2(1.f, 0.f)));
	verts.push_back(Vertex_PCU(TR, color, Vec2(1.f, 1.f)));
	verts.push_back(Vertex_PCU(TL, color, Vec2(0.f, 1.f)));
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color, Vec2 const& uvMins, Vec2 const& uvMaxs)
{
	constexpr int NUM_TRIS = 2;
	constexpr int NUM_VERTS = 3 * NUM_TRIS;

	verts.reserve(verts.size() + NUM_VERTS);

	Vec3 BL = Vec3(bounds.m_mins.x, bounds.m_mins.y, 0.f);
	Vec3 BR = Vec3(bounds.m_maxs.x, bounds.m_mins.y, 0.f);
	Vec3 TR = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0.f);
	Vec3 TL = Vec3(bounds.m_mins.x, bounds.m_maxs.y, 0.f);

	verts.push_back(Vertex_PCU(BL, color, uvMins));
	verts.push_back(Vertex_PCU(BR, color, Vec2(uvMaxs.x, uvMins.y)));
	verts.push_back(Vertex_PCU(TL, color, Vec2(uvMins.x, uvMaxs.y)));

	verts.push_back(Vertex_PCU(BR, color, Vec2(uvMaxs.x, uvMins.y)));
	verts.push_back(Vertex_PCU(TR, color, uvMaxs));
	verts.push_back(Vertex_PCU(TL, color, Vec2(uvMins.x, uvMaxs.y)));
}

void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& box, Rgba8 const& color)
{
	constexpr int NUM_TRIS = 2;
	constexpr int NUM_VERTS = 3 * NUM_TRIS;
	verts.reserve(verts.size() + NUM_VERTS);
	Vec2 cornerPoints[4];
	box.GetCornerPoints(cornerPoints);
	verts.push_back(Vertex_PCU(Vec3(cornerPoints[0].x, cornerPoints[0].y, 0.f), color, Vec2(0.f, 0.f)));
	verts.push_back(Vertex_PCU(Vec3(cornerPoints[1].x, cornerPoints[1].y, 0.f), color, Vec2(1.f, 0.f)));
	verts.push_back(Vertex_PCU(Vec3(cornerPoints[2].x, cornerPoints[2].y, 0.f), color, Vec2(0.f, 1.f)));

	verts.push_back(Vertex_PCU(Vec3(cornerPoints[1].x, cornerPoints[1].y, 0.f), color, Vec2(1.f, 0.f)));
	verts.push_back(Vertex_PCU(Vec3(cornerPoints[3].x, cornerPoints[3].y, 0.f), color, Vec2(1.f, 1.f)));
	verts.push_back(Vertex_PCU(Vec3(cornerPoints[2].x, cornerPoints[2].y, 0.f), color, Vec2(0.f, 1.f)));
}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& startPos, Vec2 const& endPos, float thickness, Rgba8 const& color)
{
	Vec2 Distance = endPos - startPos;
	Vec2 D = Distance.GetNormalized();
	float radius = thickness * 0.5f;
	Vec2 fwd = radius * D;
	Vec2 fwdleft = fwd.GetRotated90Degrees();
	Vec2 APos = endPos + fwd + fwdleft;
	Vec2 BPos = startPos - fwd + fwdleft;
	Vec2 CPos = startPos - fwd - fwdleft;
	Vec2 DPos = endPos + fwd - fwdleft;
	int vertsNum = (int)verts.size() + 6;
	verts.reserve(vertsNum);
	verts.push_back(Vertex_PCU(Vec3(APos.x, APos.y, 0.f), color, Vec2(1.f, 1.f)));
	verts.push_back(Vertex_PCU(Vec3(BPos.x, BPos.y, 0.f), color, Vec2(1.f, 0.f)));
	verts.push_back(Vertex_PCU(Vec3(CPos.x, CPos.y, 0.f), color, Vec2(0.f, 0.f)));
	verts.push_back(Vertex_PCU(Vec3(CPos.x, CPos.y, 0.f), color, Vec2(0.f, 0.f)));
	verts.push_back(Vertex_PCU(Vec3(DPos.x, DPos.y, 0.f), color, Vec2(0.f, 1.f)));
	verts.push_back(Vertex_PCU(Vec3(APos.x, APos.y, 0.f), color, Vec2(1.f, 1.f)));
}

void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness,float arrowRadius, Rgba8 const& color)
{
	Vec2 iForwardDir = (end - start).GetNormalized();
	Vec2 jForwardDir = iForwardDir.GetRotated90Degrees();
	Vec2 leftWingPos = end - (iForwardDir * arrowRadius) + (jForwardDir * arrowRadius);
	Vec2 rightWingPos = end - (iForwardDir * arrowRadius) - (jForwardDir * arrowRadius);


// 	verts.push_back(Vertex_PCU(Vec3(arrowheadTip.x, arrowheadTip.y, 0.f), color, Vec2(0.5f, 0.5f)));
// 	verts.push_back(Vertex_PCU(Vec3(arrowheadLeft.x, arrowheadLeft.y, 0.f), color, Vec2(0.f, 1.f)));
// 	verts.push_back(Vertex_PCU(Vec3(arrowheadRight.x, arrowheadRight.y, 0.f), color, Vec2(1.f, 1.f)));
	AddVertsForLineSegment2D(verts, end, leftWingPos, thickness, color);
	AddVertsForLineSegment2D(verts, end, rightWingPos, thickness, color);
	AddVertsForLineSegment2D(verts, start, end, thickness, color);
}

void AddVertsForSemiDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color, float orientationDegrees)
{
	constexpr int NUM_SIDES = 32;
	constexpr int NUM_TRIS = NUM_SIDES;
	constexpr int NUM_VERTS = 3 * NUM_TRIS;
	verts.reserve(verts.size() + NUM_VERTS);
	constexpr float DEGREES_PER_SIDE = 180.0f / static_cast<float>(NUM_SIDES);

	for (int sideNum = 0; sideNum < NUM_SIDES ; ++sideNum) {
		float startDegrees = DEGREES_PER_SIDE * static_cast<float>(sideNum);
		float endDegrees = DEGREES_PER_SIDE * static_cast<float>(sideNum + 1);
		startDegrees += orientationDegrees;
		endDegrees += orientationDegrees;

		Vec3 innerStartPos = Vec3(center.x, center.y, 0.f);
		Vec3 innerEndPos = Vec3(center.x, center.y, 0.f);
		Vec3 outerStartPos = Vec3(center.x + radius * CosDegrees(startDegrees), center.y + radius * SinDegrees(startDegrees), 0.f);
		Vec3 outerEndPos = Vec3(center.x + radius * CosDegrees(endDegrees), center.y + radius * SinDegrees(endDegrees), 0.f);

		verts.push_back(Vertex_PCU(innerEndPos, color, Vec2(0.f, 0.f)));
		verts.push_back(Vertex_PCU(innerStartPos, color, Vec2(0.f, 0.f)));
		verts.push_back(Vertex_PCU(outerStartPos, color, Vec2(0.f, 0.f)));
		verts.push_back(Vertex_PCU(innerEndPos, color, Vec2(0.f, 0.f)));
		verts.push_back(Vertex_PCU(outerStartPos, color, Vec2(0.f, 0.f)));
		verts.push_back(Vertex_PCU(outerEndPos, color, Vec2(0.f, 0.f)));
	}
}

void AddVertsForLaser2D(std::vector<Vertex_PCU>& verts, Vec2 const& startPos, Vec2 const& endPos, float thickness, Rgba8 const& startColor, Rgba8 const& endColor)
{
	Vec2 Distance = endPos - startPos;
	Vec2 D = Distance.GetNormalized();
	float radius = thickness * 0.5f;
	Vec2 fwd = radius * D;
	Vec2 fwdleft = fwd.GetRotated90Degrees();
	//Vec2 APos = endPos + fwd + fwdleft;
	Vec2 BPos = startPos - fwd + fwdleft;
	Vec2 CPos = startPos - fwd - fwdleft;
	//Vec2 DPos = endPos + fwd - fwdleft;
	int vertsNum = (int)verts.size() + 3;
	verts.reserve(vertsNum);
	verts.push_back(Vertex_PCU(Vec3(endPos.x, endPos.y, 0.f), endColor, Vec2(0.f, 0.f)));
	verts.push_back(Vertex_PCU(Vec3(BPos.x, BPos.y, 0.f), startColor, Vec2(0.f, 0.f)));
	verts.push_back(Vertex_PCU(Vec3(CPos.x, CPos.y, 0.f), startColor, Vec2(0.f, 0.f)));
// 	verts.push_back(Vertex_PCU(Vec3(CPos.x, CPos.y, 0.f), startColor, Vec2(0.f, 0.f)));
// 	verts.push_back(Vertex_PCU(Vec3(DPos.x, DPos.y, 0.f), startColor, Vec2(0.f, 1.f)));
	//verts.push_back(Vertex_PCU(Vec3(APos.x, APos.y, 0.f), endColor, Vec2(1.f, 1.f)));
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color, AABB2 const& UVs )
{
	constexpr int NUM_TRIS = 2;
	constexpr int NUM_VERTS = 3 * NUM_TRIS;

	verts.reserve(verts.size() + NUM_VERTS);

	verts.push_back(Vertex_PCU(bottomLeft, color, UVs.m_mins));
	verts.push_back(Vertex_PCU(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y)));
	verts.push_back(Vertex_PCU(topRight, color, UVs.m_maxs));

	verts.push_back(Vertex_PCU(topRight, color, UVs.m_maxs));
	verts.push_back(Vertex_PCU(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y)));
	verts.push_back(Vertex_PCU(bottomLeft, color, UVs.m_mins));

}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color, Vec2 const& uvBottomLeft , Vec2 const& uvBottomRight, Vec2 const& uvTopRight, Vec2 const& uvTopLeft)
{
	constexpr int NUM_TRIS = 2;
	constexpr int NUM_VERTS = 3 * NUM_TRIS;

	verts.reserve(verts.size() + NUM_VERTS);

	verts.push_back(Vertex_PCU(bottomLeft, color, uvBottomLeft));
	verts.push_back(Vertex_PCU(bottomRight, color, uvBottomRight));
	verts.push_back(Vertex_PCU(topRight, color, uvTopRight));

	verts.push_back(Vertex_PCU(topRight, color, uvTopRight));
	verts.push_back(Vertex_PCU(topLeft, color, uvTopLeft));
	verts.push_back(Vertex_PCU(bottomLeft, color, uvBottomLeft));
}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color, AABB2 const& UVs)
{
	unsigned int indexStart = (unsigned int)verts.size();

// 	verts.reserve(verts.size() + 4);
// 	indexes.reserve(indexes.size() + 6);
	indexes.push_back(indexStart);
	indexes.push_back(indexStart+1);
	indexes.push_back(indexStart+2);
	indexes.push_back(indexStart);
	indexes.push_back(indexStart+2);
	indexes.push_back(indexStart+3);

	Vec3 iBasis = (bottomRight - bottomLeft).GetNormalized();
	Vec3 jBasis = (topLeft - bottomLeft).GetNormalized();
	Vec3 kBasis = CrossProduct3D(iBasis, jBasis).GetNormalized();

	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins,                        Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs,                          Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y),     Vec3(), Vec3(), kBasis));

}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color, AABB2 const& UVs )
{
	verts.reserve(verts.size() + 4);
	Vec3 iBasis = (bottomRight - bottomLeft).GetNormalized();
	Vec3 jBasis = (topLeft - bottomLeft).GetNormalized();
	Vec3 kBasis = CrossProduct3D(iBasis, jBasis).GetNormalized();

	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3(), Vec3(), kBasis));
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color, AABB2 const& UVs)
{
	unsigned int indexStart = (unsigned int)verts.size();
// 	verts.reserve(verts.size() + 4);
// 	indexes.reserve(indexes.size() + 6);
	indexes.push_back(indexStart);
	indexes.push_back(indexStart + 1);
	indexes.push_back(indexStart + 2);
	indexes.push_back(indexStart);
	indexes.push_back(indexStart + 2);
	indexes.push_back(indexStart + 3);
	verts.push_back(Vertex_PCU(bottomLeft, color, UVs.m_mins));
	verts.push_back(Vertex_PCU(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y)));
	verts.push_back(Vertex_PCU(topRight, color, UVs.m_maxs));
	verts.push_back(Vertex_PCU(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y)));
}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Vec3 const& bottomLeftNormal, Vec3 const& bottomRightNormal, Vec3 const& topRightNormal, Vec3 const& topLeftNormal, Rgba8 const& color, AABB2 const& UVs)
{
	unsigned int indexStart = (unsigned int)verts.size();
	indexes.push_back(indexStart);
	indexes.push_back(indexStart + 1);
	indexes.push_back(indexStart + 2);
	indexes.push_back(indexStart);
	indexes.push_back(indexStart + 2);
	indexes.push_back(indexStart + 3);


	verts.push_back(Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3(), Vec3(), bottomLeftNormal));
	verts.push_back(Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3(), Vec3(), bottomRightNormal));
	verts.push_back(Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3(), Vec3(), topRightNormal));
	verts.push_back(Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), Vec3(), Vec3(),topLeftNormal));
}

void AddVertsForLineQuad3D(std::vector<Vertex_PCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color, AABB2 const& UVs, float thickness)
{
	AddVertsForCylinder3D(verts, bottomLeft, bottomRight, thickness, color, UVs.m_mins, UVs.m_maxs);
	AddVertsForCylinder3D(verts, bottomRight, topRight, thickness, color, UVs.m_mins, UVs.m_maxs);
	AddVertsForCylinder3D(verts, topRight, topLeft, thickness, color, UVs.m_mins, UVs.m_maxs);
	AddVertsForCylinder3D(verts, topLeft, bottomLeft, thickness, color, UVs.m_mins, UVs.m_maxs);
}

void AddVertsForRoundedQuad3D(std::vector<Vertex_PCUTBN>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight, Vec3 const& topLeft,   const Rgba8& color, AABB2 const& UVs)
{
	verts.reserve(verts.size() + 12);
	Vec3 iBasis = (bottomRight - bottomLeft).GetNormalized();
	Vec3 jBasis = (topLeft - bottomLeft).GetNormalized();
	Vec3 kBasis = CrossProduct3D(iBasis, jBasis).GetNormalized();
	Vec3 A = topLeft;
	Vec3 B = bottomLeft;
	Vec3 C =  (topRight + topLeft) * 0.5f;
	Vec3 D = (bottomRight + bottomLeft) * 0.5f;
	Vec3 E = topRight;
	Vec3 F = bottomRight;

	verts.push_back(Vertex_PCUTBN(B, color, UVs.m_mins, Vec3(), Vec3(), -iBasis));
	verts.push_back(Vertex_PCUTBN(D, color, Vec2((UVs.m_maxs.x + UVs.m_mins.x) * 0.5f, UVs.m_mins.y), Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(C, color, Vec2((UVs.m_maxs.x + UVs.m_mins.x) * 0.5f, UVs.m_maxs.y), Vec3(), Vec3(), kBasis));

	verts.push_back(Vertex_PCUTBN(C, color, Vec2((UVs.m_maxs.x + UVs.m_mins.x) * 0.5f, UVs.m_maxs.y), Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(A, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), Vec3(), Vec3(), -iBasis));
	verts.push_back(Vertex_PCUTBN(B, color, UVs.m_mins, Vec3(), Vec3(), -iBasis));

	verts.push_back(Vertex_PCUTBN(D, color, Vec2((UVs.m_maxs.x + UVs.m_mins.x) * 0.5f, UVs.m_mins.y), Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(F, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3(), Vec3(), iBasis));
	verts.push_back(Vertex_PCUTBN(E, color, UVs.m_maxs, Vec3(), Vec3(), iBasis));

	verts.push_back(Vertex_PCUTBN(E, color, UVs.m_maxs, Vec3(), Vec3(), iBasis));
	verts.push_back(Vertex_PCUTBN(C, color, Vec2((UVs.m_maxs.x + UVs.m_mins.x) * 0.5f, UVs.m_maxs.y), Vec3(), Vec3(), kBasis));
	verts.push_back(Vertex_PCUTBN(D, color, Vec2((UVs.m_maxs.x + UVs.m_mins.x) * 0.5f, UVs.m_mins.y), Vec3(), Vec3(), kBasis));
}

void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& bounds, Rgba8 const& color, Vec2 const& uvMins, Vec2 const& uvMaxs)
{
	Vec3 points[NUM_CUBEPOINTS];
	points[BLN] = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	points[BRN] = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	points[BRF] = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	points[BLF] = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);
	points[TLN] = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	points[TRN] = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);
	points[TRF] = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);
	points[TLF] = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_maxs.z);

	AddVertsForQuad3D(verts, points[BLN], points[BRN], points[TRN], points[TLN], color, AABB2(uvMins, uvMaxs));
	AddVertsForQuad3D(verts, points[BRN], points[BRF], points[TRF], points[TRN], color, AABB2(uvMins, uvMaxs));
	AddVertsForQuad3D(verts, points[BRF], points[BLF], points[TLF], points[TRF], color, AABB2(uvMins, uvMaxs));
	AddVertsForQuad3D(verts, points[BLF], points[BLN], points[TLN], points[TLF], color, AABB2(uvMins, uvMaxs));
	AddVertsForQuad3D(verts, points[TLN], points[TRN], points[TRF], points[TLF], color, AABB2(uvMins, uvMaxs));
	AddVertsForQuad3D(verts, points[BLF], points[BRF], points[BRN], points[BLN], color, AABB2(uvMins, uvMaxs));
	
}

void AddVertsForAABB3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, AABB3 const& bounds, Rgba8 const& color /*= Rgba8::WHITE*/, Vec2 const& uvMins /*= Vec2(0.f,0.f)*/, Vec2 const& uvMaxs /*= Vec2(1.f,1.f)*/)
{
	Vec3 points[NUM_CUBEPOINTS];
	points[BLN] = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	points[BRN] = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	points[BRF] = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	points[BLF] = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);
	points[TLN] = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	points[TRN] = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);
	points[TRF] = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);
	points[TLF] = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_maxs.z);

	AddVertsForQuad3D(verts,indexes, points[BLN], points[BRN], points[TRN], points[TLN], color, AABB2(uvMins, uvMaxs));
	AddVertsForQuad3D(verts,indexes, points[BRN], points[BRF], points[TRF], points[TRN], color, AABB2(uvMins, uvMaxs));
	AddVertsForQuad3D(verts,indexes, points[BRF], points[BLF], points[TLF], points[TRF], color, AABB2(uvMins, uvMaxs));
	AddVertsForQuad3D(verts,indexes, points[BLF], points[BLN], points[TLN], points[TLF], color, AABB2(uvMins, uvMaxs));
	AddVertsForQuad3D(verts,indexes, points[TLN], points[TRN], points[TRF], points[TLF], color, AABB2(uvMins, uvMaxs));
	AddVertsForQuad3D(verts,indexes, points[BLF], points[BRF], points[BRN], points[BLN], color, AABB2(uvMins, uvMaxs));
}

void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts, OBB3 const& orientedBox, Rgba8 const& color, Vec2 const& uvMins , Vec2 const& uvMaxs)
{
	AABB3 box = AABB3(-orientedBox.m_halfDimensions, orientedBox.m_halfDimensions);
	int vertsStartIndex = (int)verts.size();
// 	box.SetCenter(orientedBox.m_center);
// 	box.SetDimensions(orientedBox.m_halfDimensions * 2.f);
	AddVertsForAABB3D(verts, box, color, uvMins, uvMaxs);
	int vertsEndIndex = (int)verts.size();
	Mat44 transform;
	transform.SetTranslation3D(orientedBox.m_center);
	transform.SetIJK3D(orientedBox.m_iBasisNormal, orientedBox.m_jBasisNormal,orientedBox.m_kBasisNormal );
	TransformVertexArrayInRange3D(vertsStartIndex, vertsEndIndex, verts, transform);
}

void AddVertsForSphere3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float radius, Rgba8 const& color, Vec2 const& uvMins, Vec2 const& uvMaxs, int numSlices, int numStacks)
{
	int startVertIndex = (int)verts.size();
	int NUM_TRIS = 2 * numSlices * numStacks;
	int NUM_VERTS = 3 * NUM_TRIS;
	verts.reserve(verts.size() + NUM_VERTS);
	float pitchDeltaDegrees = 360.f / numSlices;
	float yawDeltaDegrees = 180.f / numStacks;

	float yawStartDegrees = -90.f;
	float yawEndDegrees = yawStartDegrees + yawDeltaDegrees;
	float pitchStartDegrees = 0.f;
	float pitchEndDegrees = pitchStartDegrees + pitchDeltaDegrees;

	for (int stackNum = 0; stackNum < numStacks; ++stackNum)
	{
		for (int sliceNum = 0; sliceNum < numSlices; ++sliceNum)
		{
			Vec3 bottomLeft  = Vec3::MakeFromPolarDegrees(yawStartDegrees, pitchStartDegrees, radius);
			Vec3 bottomRight = Vec3::MakeFromPolarDegrees(yawStartDegrees, pitchEndDegrees, radius);
			Vec3 topRight    = Vec3::MakeFromPolarDegrees(yawEndDegrees, pitchEndDegrees, radius);
			Vec3 topLeft     = Vec3::MakeFromPolarDegrees(yawEndDegrees, pitchStartDegrees, radius);

			Vec2 uvBottomLeft = Vec2((1.f - uvMins.x) *pitchStartDegrees / 360.f, (1.f - uvMins.y)*(0.5f - (-yawStartDegrees) / 180.f));
			Vec2 uvTopRight = Vec2(uvMaxs.x * pitchEndDegrees / 360.f, uvMaxs.y * (0.5f - (-yawEndDegrees) / 180.f));

			AddVertsForQuad3D(verts, bottomLeft, bottomRight, topRight, topLeft, color, AABB2(uvBottomLeft, uvTopRight));
			pitchStartDegrees +=pitchDeltaDegrees;
			pitchEndDegrees += pitchDeltaDegrees;
		}
		pitchStartDegrees = 0.f;
		pitchEndDegrees = pitchStartDegrees + pitchDeltaDegrees;
		yawStartDegrees += yawDeltaDegrees;
		yawEndDegrees += yawDeltaDegrees;
	}
	TransformVertexArrayInRange3D(startVertIndex, (int)verts.size(), verts, Mat44(center));
}

void AddVertsForSphere3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& center, float radius, Rgba8 const& color, Vec2 const& uvMins, Vec2 const& uvMaxs, int numSlices, int numStacks)
{
	 int startVertIndex = (int)verts.size();
	 int numVerts = (numSlices + 1) * (numStacks + 1);
	 verts.reserve(verts.size() + numVerts);

	 float pitchDeltaDegrees = 360.f / numSlices;
	 float yawDeltaDegrees = 180.f / numStacks;

	 // Generate vertices
	 for (int stackNum = 0; stackNum <= numStacks; ++stackNum)
	 {
		 float yawDegrees = -90.f + stackNum * yawDeltaDegrees;
		 for (int sliceNum = 0; sliceNum <= numSlices; ++sliceNum)
		 {
			 float pitchDegrees = sliceNum * pitchDeltaDegrees;

			 Vec3 position = Vec3::MakeFromPolarDegrees(yawDegrees, pitchDegrees, radius);
			 Vec2 uv = Vec2(
				 RangeMap(pitchDegrees, 0.f, 360.f, uvMins.x, uvMaxs.x),
				 RangeMap(yawDegrees, -90.f, 90.f, uvMaxs.y, uvMins.y)
			 );

			 Vertex_PCUTBN vert;
			 vert.m_position = position + center;
			 vert.m_color = color;
			 vert.m_uvTexCoords = uv;
			 vert.m_normal = (vert.m_position - center).GetNormalized();
			 verts.push_back(vert);
		 }
	 }
	 // Generate indexes
	 indexes.reserve(indexes.size() + numSlices * numStacks * 6);
	 for (int stackNum = 0; stackNum < numStacks; ++stackNum)
	 {
		 for (int sliceNum = 0; sliceNum < numSlices; ++sliceNum)
		 {
			 int bottomLeft = startVertIndex + stackNum * (numSlices + 1) + sliceNum;
			 int bottomRight = bottomLeft + 1;
			 int topLeft = bottomLeft + (numSlices + 1);
			 int topRight = topLeft + 1;

			 indexes.push_back(bottomLeft);
			 indexes.push_back(bottomRight);
			 indexes.push_back(topRight);

			 indexes.push_back(bottomLeft);
			 indexes.push_back(topRight);
			 indexes.push_back(topLeft);
		 }
	 }
}

void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color, Vec2 const& uvMins, Vec2 const& uvMaxs, int numSlices)
{
	UNUSED(uvMins);
	UNUSED(uvMaxs);
	int startVertIndex = (int)verts.size();
	//int NUM_TRIS = 4 * numSlices;
	//int NUM_VERTS = 3 * NUM_TRIS;
	//verts.reserve(verts.size() + NUM_VERTS);
	float height = (start - end).GetLength();
	float pitchDeltaDegrees = 360.f / numSlices;
	float pitchStartDegrees = 0.f;
	float pitchEndDegrees = pitchStartDegrees + pitchDeltaDegrees;

	for (int sliceNum = 0; sliceNum < numSlices; ++sliceNum)
	{
		Vec3 bottomLeft = Vec3::MakeFromPolarDegrees(0.f, pitchStartDegrees, radius);
		Vec3 bottomRight = Vec3::MakeFromPolarDegrees(0.f, pitchEndDegrees, radius);
		Vec3 topRight = Vec3::MakeFromPolarDegrees(0.f, pitchEndDegrees, radius);
		topRight += Vec3(0.f, 0.f, height);
		Vec3 topLeft = Vec3::MakeFromPolarDegrees(0.f, pitchStartDegrees, radius);
		topLeft += Vec3(0.f, 0.f, height);

 		Vec2 uvBottomLeft = Vec2((1.f - uvMins.x) * pitchStartDegrees / 360.f, 0.f);
 		Vec2 uvTopRight = Vec2(uvMaxs.x * pitchEndDegrees / 360.f, 1.f);

		AddVertsForQuad3D(verts, bottomLeft, bottomRight, topRight, topLeft, color, AABB2(uvBottomLeft, uvTopRight));

		verts.push_back(Vertex_PCU(bottomLeft, color, Vec2(0.5f - 0.5f * SinDegrees(pitchStartDegrees),0.5f - 0.5f * CosDegrees(pitchStartDegrees))));
		verts.push_back(Vertex_PCU(Vec3(), color, Vec2(0.5f,0.5f)));
		verts.push_back(Vertex_PCU(bottomRight, color, Vec2(0.5f - 0.5f * SinDegrees(pitchEndDegrees), 0.5f - 0.5f * CosDegrees(pitchEndDegrees))));
		
		verts.push_back(Vertex_PCU(topRight, color, Vec2(0.5f - 0.5f * CosDegrees(pitchEndDegrees), 0.5f - 0.5f * SinDegrees(pitchEndDegrees))));
		verts.push_back(Vertex_PCU(Vec3(0.f, 0.f, height), color, Vec2(0.5f,0.5f)));
		verts.push_back(Vertex_PCU(topLeft, color, Vec2(0.5f - 0.5f * CosDegrees(pitchStartDegrees), 0.5f - 0.5f * SinDegrees(pitchStartDegrees))));
		pitchStartDegrees += pitchDeltaDegrees;
		pitchEndDegrees += pitchDeltaDegrees;
	}

	Vec3 kBasis = (100.f*(end - start)).GetNormalized();
	Vec3 iBasis, jBasis;

	if (AbsFloat(DotProduct3D(kBasis, Vec3(0.f, 1.f, 0.f))) != 1.f)
	{
		iBasis = CrossProduct3D(Vec3(0.f, 1.f, 0.f), kBasis);
		jBasis = CrossProduct3D(kBasis, iBasis);
	}
	else
	{
		jBasis = CrossProduct3D(kBasis, Vec3(1.f, 0.f, 0.f));
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	iBasis = iBasis.GetNormalized();
	jBasis = jBasis.GetNormalized();
	TransformVertexArrayInRange3D(startVertIndex, (int)verts.size(), verts, Mat44(iBasis, jBasis, kBasis, start));
}

void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color, Vec2 const& uvMins, Vec2 const& uvMaxs, int numSlices)
{
	UNUSED(uvMins);
	UNUSED(uvMaxs);
	int startVertIndex = (int)verts.size();
	//int NUM_TRIS = 2 * numSlices;
	//int NUM_VERTS = 3 * NUM_TRIS;
	//verts.reserve(verts.size() + NUM_VERTS);
	std::vector<Vertex_PCU> coneVerts;
	float height = (start - end).GetLength();
	float pitchDeltaDegrees = 360.f / numSlices;
	float pitchStartDegrees = 0.f;
	float pitchEndDegrees = pitchStartDegrees + pitchDeltaDegrees;
	for (int sliceNum = 0; sliceNum < numSlices; ++sliceNum)
	{
		Vec3 bottomLeft = Vec3::MakeFromPolarDegrees(0.f, pitchStartDegrees, radius);
		Vec3 bottomRight = Vec3::MakeFromPolarDegrees(0.f, pitchEndDegrees, radius);
		verts.push_back(Vertex_PCU(bottomRight, color, Vec2()));
		verts.push_back(Vertex_PCU(Vec3(0.f, 0.f, height), color, Vec2()));
		verts.push_back(Vertex_PCU(bottomLeft, color, Vec2()));
		verts.push_back(Vertex_PCU(bottomLeft, color, Vec2()));
		verts.push_back(Vertex_PCU(Vec3(), color, Vec2()));
		verts.push_back(Vertex_PCU(bottomRight, color, Vec2()));
		pitchStartDegrees += pitchDeltaDegrees;
		pitchEndDegrees += pitchDeltaDegrees;
	}
	Vec3 kBasis = (100.f*(end - start)).GetNormalized();
	Vec3 iBasis, jBasis;
	if (AbsFloat(DotProduct3D(kBasis, Vec3(0.f, 1.f, 0.f))) != 1.f)
	{
		iBasis = CrossProduct3D(Vec3(0.f, 1.f, 0.f), kBasis);
		jBasis = CrossProduct3D(kBasis, iBasis);
	}
	else
	{
		jBasis = CrossProduct3D(kBasis, Vec3(1.f, 0.f, 0.f));
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	iBasis = iBasis.GetNormalized();
	jBasis = jBasis.GetNormalized();
	TransformVertexArrayInRange3D(startVertIndex, (int)verts.size(),verts, Mat44(iBasis, jBasis, kBasis, start));

}

void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, Vec3 const& startPos, Vec3 const& endPos, float radius, Rgba8 const& color, Vec2 const& uvMins, Vec2 const& uvMaxs, int numSlices)
{

// 	float halfLength = 0.6f * (endPos - startPos).GetLength(); // Assuming 0.6f is the length ratio for the cylinder part
// 	Vec3 kBasis = (endPos - startPos).GetNormalized();
// 	Vec3 iBasis, jBasis;
// 	if (AbsFloat(DotProduct3D(kBasis, Vec3(0.f, 1.f, 0.f))) != 1.f)
// 	{
// 		iBasis = CrossProduct3D(Vec3(0.f, 1.f, 0.f), kBasis);
// 		jBasis = CrossProduct3D(kBasis, iBasis);
// 	}
// 	else
// 	{
// 		jBasis = CrossProduct3D(kBasis, Vec3(1.f, 0.f, 0.f));
// 		iBasis = CrossProduct3D(jBasis, kBasis);
// 	}
// 	int startVertIndex = (int)verts.size();
// 	AddVertsForAABB3D(verts, AABB3(Vec3(-radius*0.5f,-radius*0.5f,0.f),Vec3(radius * 0.5f, radius * 0.5f,halfLength)), color, uvMins, uvMaxs);
// 	TransformVertexArrayInRange3D(startVertIndex, (int)verts.size(), verts, Mat44(iBasis, jBasis, kBasis, startPos));

	AddVertsForCylinder3D(verts, startPos, startPos +  0.8f * (endPos - startPos), radius * 0.7f, color, uvMins, uvMaxs, numSlices);
	AddVertsForCone3D(verts, startPos + 0.8f * (endPos - startPos), endPos, radius*1.5f, color, uvMins, uvMaxs, numSlices);
}

struct TriangleInVertexUtils
{
	TriangleInVertexUtils(){}
	unsigned int m_vertexIndexes[3] = {0,0,0};
};

void CalculateTangentSpaceBasisVectors(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, bool computeNormals , bool computeTangents)
{
	std::vector<TriangleInVertexUtils> triangles;
	triangles.reserve(indexes.size() / 3);
	for (int i = 0; i < (int)indexes.size(); i += 3) 
	{
		TriangleInVertexUtils triangle;
		triangle.m_vertexIndexes[0] = indexes[i];
		triangle.m_vertexIndexes[1] = indexes[i + 1];
		triangle.m_vertexIndexes[2] = indexes[i + 2];
		triangles.push_back(triangle);
	}

	for (auto& triangle : triangles)
	{
		Vec3 p0 = verts[triangle.m_vertexIndexes[0]].m_position;
		Vec3 p1 = verts[triangle.m_vertexIndexes[1]].m_position;
		Vec3 p2 = verts[triangle.m_vertexIndexes[2]].m_position;
		Vec3 e0 = p1 - p0;
		Vec3 e1 = p2 - p0;

		if (computeNormals) 
		{
			Vec3 faceNormal = CrossProduct3D(e0, e1).GetNormalized();
			verts[triangle.m_vertexIndexes[0]].m_normal += faceNormal;
			verts[triangle.m_vertexIndexes[1]].m_normal += faceNormal;
			verts[triangle.m_vertexIndexes[2]].m_normal += faceNormal;
		}

		if (computeTangents) 
		{
			float u0 = verts[triangle.m_vertexIndexes[0]].m_uvTexCoords.x;
			float u1 = verts[triangle.m_vertexIndexes[1]].m_uvTexCoords.x;
			float u2 = verts[triangle.m_vertexIndexes[2]].m_uvTexCoords.x;
			float v0 = verts[triangle.m_vertexIndexes[0]].m_uvTexCoords.y;
			float v1 = verts[triangle.m_vertexIndexes[1]].m_uvTexCoords.y;
			float v2 = verts[triangle.m_vertexIndexes[2]].m_uvTexCoords.y;
			float deltaU0 = u1 - u0;
			float deltaU1 = u2 - u0;
			float deltaV0 = v1 - v0;
			float deltaV1 = v2 - v0;
			float r = 1.f / ((deltaV1 * deltaU0) - (deltaU1 * deltaV0));
			
			Vec3 sharedTangent = r * ((deltaV1 * e0) - (deltaV0 * e1));
			sharedTangent.Normalize();
			Vec3 sharedBitangent = r * ((deltaU0 * e1) - (deltaU1 * e0));
			sharedBitangent.Normalize();
			verts[triangle.m_vertexIndexes[0]].m_tangent += sharedTangent;
			verts[triangle.m_vertexIndexes[1]].m_tangent += sharedTangent;
			verts[triangle.m_vertexIndexes[2]].m_tangent += sharedTangent;
			verts[triangle.m_vertexIndexes[0]].m_bitangent += sharedBitangent;
			verts[triangle.m_vertexIndexes[1]].m_bitangent += sharedBitangent;
			verts[triangle.m_vertexIndexes[2]].m_bitangent += sharedBitangent;
		}
	}

	for (auto& vert : verts) 
	{
		if (computeNormals) 
		{
			vert.m_normal.Normalize();
		}
		if (computeTangents)
		{
			vert.m_tangent.Normalize();
			vert.m_bitangent.Normalize();
			vert.m_normal.Normalize();
			vert.m_tangent = (vert.m_tangent - vert.m_normal * DotProduct3D(vert.m_normal, vert.m_tangent)).GetNormalized();
			vert.m_bitangent = CrossProduct3D(vert.m_normal, vert.m_tangent).GetNormalized();
		}
	}
}

void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, Mat44 const& tranform)
{
	for (int vertIndex = 0; vertIndex < (int)verts.size(); ++vertIndex)
	{
		Vec3& pos = verts[vertIndex].m_position;
		pos = tranform.TransformPosition3D(pos);
	}

}

void TransformVertexArray3D(std::vector<Vertex_PCUTBN>& verts, Mat44 const& tranform)
{
	for (int vertIndex = 0; vertIndex < (int)verts.size(); ++vertIndex)
	{
		Vec3& pos = verts[vertIndex].m_position;
		pos = tranform.TransformPosition3D(pos);
		Vec3& normal = verts[vertIndex].m_normal;
		normal = tranform.TransformVectorQuantity3D(normal).GetNormalized();
	}
}

void TransformVertexArrayInRange3D(int startIndex, int endIndex, std::vector<Vertex_PCU>& verts, Mat44 const& tranform)
{
	for (int vertIndex = startIndex; vertIndex < endIndex; ++vertIndex)
	{
		Vec3& pos = verts[vertIndex].m_position;
		pos = tranform.TransformPosition3D(pos);
	}
}

AABB2 GetVertexBounds2D(std::vector<Vertex_PCU> const& verts)
{
	float minX = verts[0].m_position.x;
	float minY = verts[0].m_position.y;
	float maxX = verts[0].m_position.x;
	float maxY = verts[0].m_position.y;
	for (int i = 1; i < (int)verts.size(); ++i)
	{
		if (verts[i].m_position.x < minX)
		{
			minX = verts[i].m_position.x;
		}
		if (verts[i].m_position.y < minY)
		{
			minY = verts[i].m_position.y;
		}
		if (verts[i].m_position.x > maxX)
		{
			maxX = verts[i].m_position.x;
		}
		if (verts[i].m_position.y > maxY)
		{
			maxY = verts[i].m_position.y;
		}
	}
	return AABB2(minX, minY,maxX,maxY);
}


