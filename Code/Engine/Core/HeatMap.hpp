#pragma once
#include "Engine/Math/IntVec2.hpp"
#include <vector>
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/RaycastUtils.hpp"
class TileHeatMap 
{
public:
	TileHeatMap(IntVec2 const& dimensions);
	float GetHighestHeat();
	void AddVertsForDebugDraw(std::vector<Vertex_PCU>& verts, AABB2 bounds, FloatRange valueRange = FloatRange(0.f, 1.f), Rgba8 lowColor = Rgba8(0, 0, 0, 100), Rgba8 highColor = Rgba8(255, 255, 255, 100), float specialValue = 999999.f, Rgba8 specialColor = Rgba8(255, 0, 255, 255));
	void SetAllVallues(float value);
	float GetValueToTileCoords(IntVec2 tileCoords) const;
	void SetValueToTileCoords(IntVec2 tileCoords, float value);
	void AddValueToTileCoords(IntVec2 tileCoords, float value);

//	void GenerateDistanceField(IntVec2 startCoords, float maxHeat, TileHeatMap const& solidMap);
//	RaycastResult2D Raycast(Vec2 startPos, Vec2 rayDir, float rayMaxDist) const;
	IntVec2 GetDimensions()const { return m_dimensions; }
private:
	IntVec2 m_dimensions;
	std::vector<float> m_values;
}; 