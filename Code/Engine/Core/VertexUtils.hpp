#pragma once
#include "Vertex_PCU.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/Mat44.hpp"
#include <vector>
enum CubePoints
{
	BLN,
	BRN,
	BRF,
	BLF,
	TLN,
	TRN,
	TRF,
	TLF,
	NUM_CUBEPOINTS
};
void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float uniformScaleXY,
	float rotationDegreesAboutZ, Vec2 const& translationXY);
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color);
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color);
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color);
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color, Vec2 const& uvMins, Vec2 const& uvMaxs);
void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& box, Rgba8 const& color);
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color);
void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, float arrowRadius, Rgba8 const& color);
void AddVertsForSemiDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color, float orientationDegrees);
void AddVertsForLaser2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& startColor, Rgba8 const& endColor);
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2(Vec2(), Vec2(1.f,1.f)));
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color = Rgba8::WHITE, Vec2 const& uvBottomLeft = Vec2(0.f, 0.f), Vec2 const& uvBottomRight = Vec2(0.f, 1.f), Vec2 const& uvTopRight = Vec2(1.f, 1.f), Vec2 const& uvTopLeft = Vec2(1.f, 0.f));
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2(Vec2(), Vec2(1.f, 1.f)));
void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2(Vec2(), Vec2(1.f, 1.f)));
void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Vec3 const& bottomLeftNormal, Vec3 const& bottomRightNormal, Vec3 const& topRightNormal, Vec3 const& topLeftNormal, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2(Vec2(), Vec2(1.f, 1.f)));
void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2(Vec2(), Vec2(1.f, 1.f)));
void AddVertsForLineQuad3D(std::vector<Vertex_PCU>& verts, Vec3 const& bottomLeft, Vec3 const& bottomRight, Vec3 const& topRight, Vec3 const& topLeft, Rgba8 const& color = Rgba8::WHITE, AABB2 const& UVs = AABB2(Vec2(), Vec2(1.f, 1.f)), float thickness = 0.01f);
void AddVertsForRoundedQuad3D(std::vector<Vertex_PCUTBN>& vertexes,    Vec3 const& bottomLeft, const Vec3& bottomRight,  Vec3 const& topRight, Vec3 const& topLeft, const Rgba8& color = Rgba8::WHITE, AABB2 const& UVs = AABB2(Vec2(), Vec2(1.f, 1.f)));



void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& bounds, Rgba8 const& color = Rgba8::WHITE, Vec2 const& uvMins = Vec2(0.f,0.f), Vec2 const& uvMaxs = Vec2(1.f,1.f));
void AddVertsForAABB3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, AABB3 const& bounds, Rgba8 const& color = Rgba8::WHITE, Vec2 const& uvMins = Vec2(0.f,0.f), Vec2 const& uvMaxs = Vec2(1.f,1.f));
void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts, OBB3 const& orientedBox, Rgba8 const& color = Rgba8::WHITE, Vec2 const& uvMins = Vec2(0.f, 0.f), Vec2 const& uvMaxs = Vec2(1.f, 1.f));

void AddVertsForSphere3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float radius, Rgba8 const& color = Rgba8::WHITE, Vec2 const& uvMins = Vec2(0.f, 0.f), Vec2 const& uvMaxs = Vec2(1.f, 1.f), int numSlices = 32, int numStacks = 16);
void AddVertsForSphere3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec3 const& center, float radius, Rgba8 const& color = Rgba8::WHITE, Vec2 const& uvMins = Vec2(0.f, 0.f), Vec2 const& uvMaxs = Vec2(1.f, 1.f), int numSlices = 32, int numStacks = 16);
void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, Mat44 const& tranform);
void TransformVertexArray3D(std::vector<Vertex_PCUTBN>& verts, Mat44 const& tranform);
void TransformVertexArrayInRange3D(int startIndex, int endIndex, std::vector<Vertex_PCU>& verts, Mat44 const& tranform);
AABB2 GetVertexBounds2D(std::vector<Vertex_PCU> const& verts);
void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color = Rgba8::WHITE, Vec2 const& uvMins = Vec2(0.f, 0.f), Vec2 const& uvMaxs = Vec2(1.f, 1.f), int numSlices = 8);
void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, Vec3 const& start, Vec3 const& end, float radius, Rgba8 const& color = Rgba8::WHITE, Vec2 const& uvMins = Vec2(0.f, 0.f), Vec2 const& uvMaxs = Vec2(1.f, 1.f), int numSlices = 8);
void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, Vec3 const& startPos, Vec3 const& endPos, float radius, Rgba8 const& color = Rgba8::WHITE, Vec2 const& uvMins = Vec2(0.f, 0.f), Vec2 const& uvMaxs = Vec2(1.f, 1.f), int numSlices = 8);
void CalculateTangentSpaceBasisVectors(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, bool computeNormals = true, bool computeTangents = true);