#include "SpriteAnimDefinition.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Math/MathUtils.hpp"
SpriteAnimDefinition::SpriteAnimDefinition(const SpriteSheet& sheet, int startSpriteIndex, int endSpriteIndex, float durationSeconds, SpriteAnimPlaybackType playbackType)
	:m_spriteSheet(sheet),
	m_startSpriteIndex(startSpriteIndex), 
	m_endSpriteIndex(endSpriteIndex),
    m_durationSeconds(durationSeconds), 
	m_playbackType(playbackType)
{
	m_startTime = GetCurrentTimeSeconds();
}



// SpriteAnimDefinition::SpriteAnimDefinition(SpriteAnimDefinition const& copy)
// 	:m_spriteSheet(copy.m_spriteSheet),
// 	m_startSpriteIndex(copy.m_startSpriteIndex),
// 	m_endSpriteIndex(copy.m_endSpriteIndex),
// 	m_durationSeconds(copy.m_durationSeconds),
// 	m_playbackType(copy.m_playbackType),
// 	m_startTime(copy.m_startTime)
// {
// 
// }

SpriteAnimDefinition SpriteAnimDefinition::operator=(const SpriteAnimDefinition& other)
{
	return SpriteAnimDefinition(other.m_spriteSheet, other.m_startSpriteIndex, other.m_endSpriteIndex, other.m_durationSeconds, other.m_playbackType);
}

bool SpriteAnimDefinition::operator==(SpriteAnimDefinition const& compare)
{
	if (m_spriteSheet == compare.m_spriteSheet &&
		m_startSpriteIndex == compare.m_startSpriteIndex &&
		m_endSpriteIndex == compare.m_endSpriteIndex &&
		m_playbackType == compare.m_playbackType &&
		m_startTime == compare.m_startTime)
		return true;
	else 
        return false;
}

const SpriteDefinition& SpriteAnimDefinition::GetSpriteDefAtTime(float seconds) const
{

	int numFrames = m_endSpriteIndex - m_startSpriteIndex + 1;
	float totalTime = m_durationSeconds;
	float elapsedTime = seconds;

	switch (m_playbackType)
	{
	case SpriteAnimPlaybackType::ONCE:
		elapsedTime = MinFloat(elapsedTime, totalTime);
		break;
	case SpriteAnimPlaybackType::LOOP:
		elapsedTime = fmodf(elapsedTime, totalTime);
		break;
	case SpriteAnimPlaybackType::PINGPONG:
		elapsedTime = fmodf(elapsedTime, totalTime * 2.f);
		if (elapsedTime > totalTime)
		{
			elapsedTime = totalTime * 2.f - elapsedTime;
		}
		break;
	}

	float fraction = elapsedTime/ totalTime ;
	int frameIndex = static_cast<int>(fraction * numFrames) + m_startSpriteIndex;
	frameIndex = GetClamped(frameIndex, m_startSpriteIndex, m_endSpriteIndex);

	return m_spriteSheet.GetSpriteDef(frameIndex);
}

void SpriteAnimDefinition::SetDurationTime(float durationSeconds)
{
	m_durationSeconds = durationSeconds;
}

int SpriteAnimDefinition::GetDurationFrames() const
{
	return m_endSpriteIndex - m_startSpriteIndex + 1;
}

float SpriteAnimDefinition::GetDurationTime() const
{
	return m_durationSeconds;
}

bool SpriteAnimDefinition::IsAnimationFinished(float seconds) const
{
	//int numFrames = m_endSpriteIndex - m_startSpriteIndex + 1;
	float totalTime = m_durationSeconds;
	float elapsedTime = seconds;

	if (elapsedTime >= totalTime)
	{
		return true;
	}

	
	return false;
}

void SpriteAnimDefinition::SetAnimation(std::string const& name)
{
	m_name = name;
}

std::string const SpriteAnimDefinition::GetAnimationName() const
{
	return m_name;
}
