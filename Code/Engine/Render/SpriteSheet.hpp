#pragma once
#include "Engine/Render/Texture.hpp"
#include "Engine/Math/AABB2.hpp"
class SpriteSheet;
class SpriteDefinition
{
public:
	explicit SpriteDefinition(SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtMins, Vec2 const& uvAtMaxs);
	void GetUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs) const;
	AABB2 GetUVs() const;
	SpriteSheet const& GetSpriteSheet() const;
	Texture& GetTexture() const;
	float GetAspect() const;
protected:
	SpriteSheet const& m_spriteSheet;
	int m_spriteIndex = -1;
	Vec2 m_uvAtMins = Vec2(0.f, 0.f);
	Vec2 m_uvAtMaxs = Vec2(1.f, 1.f);

};
class SpriteSheet
{
public:
	explicit SpriteSheet(Texture& texture, IntVec2 const& simpleGridLayout);
	Texture& GetTexture() const;
	int GetNumSprites()const;
	bool  operator == (SpriteSheet const& compare) const;
	SpriteDefinition const& GetSpriteDef(int spriteIndex) const;
	void GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const;
	AABB2 GetSpriteUVs(int spriteIndex) const;
	AABB2 GetSpriteUVsFromCoords(IntVec2 spriteCoords) const;

 private:
 	Texture& m_texture;
	IntVec2 m_simpleGridLayout;
	std::vector<SpriteDefinition> m_spriteDefs;
};

