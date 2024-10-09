#include "HeatMap.hpp"
#include "VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#define UNUSED(x) (void)(x)
TileHeatMap::TileHeatMap(IntVec2 const& dimensions)
	:m_dimensions(dimensions)
{
	int tilesNumber = m_dimensions.x * m_dimensions.y;
	m_values.resize(tilesNumber);
	for (int i = 0; i < (int)m_values.size(); i++)
	{
		m_values[i] = 0.f;
	}
}

float TileHeatMap::GetHighestHeat()
{
	float highestHeat = 0.f;
	for (int i = 0; i < (int)m_values.size(); i++)
	{
		if (m_values[i] > highestHeat)
		{
			highestHeat = m_values[i];
		}
	}
	return highestHeat;
}

void TileHeatMap::AddVertsForDebugDraw(std::vector<Vertex_PCU>& verts, AABB2 bounds, FloatRange valueRange, Rgba8 lowColor, Rgba8 highColor, float specialValue, Rgba8 specialColor)
{
	UNUSED(bounds);
	for (int y = 0; y < m_dimensions.y; y++)
	{
		for (int x = 0; x < m_dimensions.x; x++)
		{
			int tileIndex = x + y * m_dimensions.x;
			Vec2 bottomLeft = Vec2(static_cast<float>(x), static_cast<float>(y));
			Vec2 topRight = bottomLeft + Vec2(1.f, 1.f);
			if (m_values[tileIndex] == specialValue)
			{
				AddVertsForAABB2D(verts, AABB2(bottomLeft,topRight), specialColor);
			}
			else
			{
				float colorFriction = RangeMapClamped(m_values[tileIndex], valueRange.m_min, valueRange.m_max, 0.f, 1.f);
				Rgba8 interpolatedColor = InterpolateFromNewColor(lowColor, highColor, colorFriction);
				AddVertsForAABB2D(verts, AABB2(bottomLeft, topRight), interpolatedColor);
			}
		}
	}

	
}

void TileHeatMap::SetAllVallues(float value)
{
	for (int i = 0; i < (int)m_values.size(); i++)
	{
		m_values[i] = value;
	}
}

float TileHeatMap::GetValueToTileCoords(IntVec2 tileCoords) const
{
	int tileIndex = tileCoords.x + tileCoords.y * m_dimensions.x;
	return m_values[tileIndex];
}

void TileHeatMap::SetValueToTileCoords(IntVec2 tileCoords, float value)
{
	int tileIndex = tileCoords.x + tileCoords.y * m_dimensions.x;
	m_values[tileIndex] = value;
}

void TileHeatMap::AddValueToTileCoords(IntVec2 tileCoords, float value)
{
	int tileIndex = tileCoords.x + tileCoords.y * m_dimensions.x;
	if (m_values[tileIndex])
	{
		m_values[tileIndex] = value;
	}
	else
	{
		m_values[tileIndex] += value;
	}
}


