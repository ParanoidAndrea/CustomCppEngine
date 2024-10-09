#pragma once
#include "Engine/Render/SpriteSheet.hpp"
#include "Engine/Core/XmlUtils.hpp"
enum class SpriteAnimPlaybackType
{
	ONCE,
	LOOP,
	PINGPONG,
};
class SpriteAnimDefinition
{
public:

	SpriteAnimDefinition(const SpriteSheet& sheet, int startSpriteIndex, int endSpriteIndex,
		float durationSeconds, SpriteAnimPlaybackType playbackType = SpriteAnimPlaybackType::LOOP);
	SpriteAnimDefinition operator=(const SpriteAnimDefinition& other);
	bool operator==(SpriteAnimDefinition const& compare);
	const SpriteDefinition& GetSpriteDefAtTime(float seconds) const;
	void SetDurationTime(float durationSeconds);
	int GetDurationFrames() const;
	float GetDurationTime() const;
	bool IsAnimationFinished(float seconds) const;
	void SetAnimation(std::string const& name);
	std::string const GetAnimationName() const;

private:
	const SpriteSheet& m_spriteSheet;
	int	m_startSpriteIndex = -1;
	int	m_endSpriteIndex = -1;
	float m_durationSeconds = 1.f;
	double m_startTime = 0.0;
	std::string m_name = "UNKNOWN";
	SpriteAnimPlaybackType	m_playbackType = SpriteAnimPlaybackType::LOOP;
};
/*
* FinalColor = (SourceColor * SRC_FACTOR) + (DestColor * DST_FACTOR)
*/