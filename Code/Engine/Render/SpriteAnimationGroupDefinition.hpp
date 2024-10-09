#pragma once
#include "Engine/Render/SpriteAnimDefinition.hpp"
#include <map>
class SpriteAnimationGroupDefinition
{
public:
	SpriteAnimationGroupDefinition(std::string name, bool scaleBySpeed, float secondsPerFrame, SpriteAnimPlaybackType m_playBackMode, std::vector<Vec3> m_directions, std::vector<SpriteAnimDefinition*> m_animations);
	Vec3 const GetDirectionofTheAnimation(SpriteAnimDefinition const& animationName );
	SpriteAnimDefinition* const GetAnimationBasedOnDirection(Vec3 const& direction) ;
public:
	Vec2 m_size;
	std::string m_name = "UNKNOWN";
	bool m_scaleBySpeed = false;
	float m_secondsPerFrame = 0.f;
	SpriteAnimPlaybackType m_playBackMode = SpriteAnimPlaybackType::LOOP;
	std::vector<Vec3> m_directions;
 	std::vector<SpriteAnimDefinition*> m_animations;
};