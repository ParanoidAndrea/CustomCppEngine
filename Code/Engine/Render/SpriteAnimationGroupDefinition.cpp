#include "SpriteAnimationGroupDefinition.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"
SpriteAnimationGroupDefinition::SpriteAnimationGroupDefinition(std::string name, bool scaleBySpeed, float secondsPerFrame, SpriteAnimPlaybackType playBackMode, std::vector<Vec3> directions, std::vector<SpriteAnimDefinition*> animations)
	:m_animations(animations),
	m_name(name),
	m_scaleBySpeed(scaleBySpeed),
	m_secondsPerFrame(secondsPerFrame),
	m_playBackMode(playBackMode)
{
	m_directions.resize(directions.size());
	for (int i = 0; i < (int)directions.size(); ++i)
	{
		m_directions[i] = directions[i].GetNormalized();
	}
}

Vec3 const SpriteAnimationGroupDefinition::GetDirectionofTheAnimation(SpriteAnimDefinition const& animation)
{
	for (int i = 0; i < (int)m_animations.size(); ++i)
	{
		if (*m_animations[i] == animation)
		{
			return m_directions[i];
		}
	}
	return Vec3();
}

SpriteAnimDefinition* const SpriteAnimationGroupDefinition::GetAnimationBasedOnDirection(Vec3 const& direction)
{
	//Vec3 normalDir = direction.GetNormalized();
	float dot = -99999.f;
	int index = -1;
	for (int i = 0; i < (int)m_directions.size(); ++i)
	{
		float iDot = DotProduct3D(m_directions[i], direction );
		if (iDot > dot)
		{
			dot = iDot;
			index = i;
		}
	}
	if (index == -1)
	{
		return nullptr;
	}
	return m_animations[index];
}
