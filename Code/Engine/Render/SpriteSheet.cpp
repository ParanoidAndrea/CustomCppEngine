#include "SpriteSheet.hpp"

SpriteSheet::SpriteSheet(Texture& texture, IntVec2 const& simpleGridLayout)
	:m_texture(texture),
	m_simpleGridLayout(simpleGridLayout)
{
	int number = simpleGridLayout.x * simpleGridLayout.y;
	float width = (float)simpleGridLayout.x;
	float height = (float)simpleGridLayout.y;
	m_spriteDefs.reserve(number);
	IntVec2 spriteSheetDimensions = m_texture.GetDimensions();
	Vec2 uvCorrectionOffset = Vec2(1.f / ((float)spriteSheetDimensions.x * 8.f), 1.f / ((float)spriteSheetDimensions.y * 8.f));
	for (int y = simpleGridLayout.y; y > 0; y--)
		for (int x = 0; x < simpleGridLayout.x; x++)
		{
			int index = x + (simpleGridLayout.y - y) * simpleGridLayout.x;
			Vec2 uvAtMins = Vec2((float)x / width, ((float)y - 1.f) / height);
			                     uvAtMins += uvCorrectionOffset;
			Vec2 uvAtMaxs = Vec2(((float)x + 1.f) / width,(float)y / height);
			uvAtMaxs -= uvCorrectionOffset;
			m_spriteDefs.push_back(SpriteDefinition(*this, index, uvAtMins,uvAtMaxs));
		}
}


Texture& SpriteSheet::GetTexture() const
{
	return m_texture;
}

int SpriteSheet::GetNumSprites() const
{
	return (int)m_spriteDefs.size();
}

bool  SpriteSheet::operator==(SpriteSheet const& compare) const
{
	if (m_texture.GetImageFilePath().compare(compare.GetTexture().GetImageFilePath()) == 0 )
		return true;
	else
		return false;
}

SpriteDefinition const& SpriteSheet::GetSpriteDef(int spriteIndex) const
{
	return m_spriteDefs[spriteIndex];
}

void SpriteSheet::GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const
{
	m_spriteDefs[spriteIndex].GetUVs(out_uvAtMins, out_uvAtMaxs);
}

AABB2 SpriteSheet::GetSpriteUVs(int spriteIndex) const
{
	return m_spriteDefs[spriteIndex].GetUVs();
}


AABB2 SpriteSheet::GetSpriteUVsFromCoords(IntVec2 spriteCoords) const
{
	int linearIndex = spriteCoords.y * m_simpleGridLayout.x + spriteCoords.x;
	return m_spriteDefs[linearIndex].GetUVs();
}

SpriteDefinition::SpriteDefinition(SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtMins, Vec2 const& uvAtMaxs)
	:m_spriteSheet(spriteSheet),
	m_spriteIndex(spriteIndex),
	m_uvAtMins(uvAtMins),
	m_uvAtMaxs(uvAtMaxs)
{

}

void SpriteDefinition::GetUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs) const
{
	out_uvAtMins = m_uvAtMins;
	out_uvAtMaxs = m_uvAtMaxs;
}

AABB2 SpriteDefinition::GetUVs() const
{
	return AABB2(m_uvAtMins, m_uvAtMaxs);
}

SpriteSheet const& SpriteDefinition::GetSpriteSheet() const
{
	return m_spriteSheet;
}

Texture& SpriteDefinition::GetTexture() const
{
	return m_spriteSheet.GetTexture();
}

float SpriteDefinition::GetAspect() const
{
	float width = m_uvAtMaxs.x - m_uvAtMins.x;
	float height = m_uvAtMaxs.y - m_uvAtMins.y;
	if (height != 0.f)
	{
		return width / height;
	}
	return 1.f;
}

