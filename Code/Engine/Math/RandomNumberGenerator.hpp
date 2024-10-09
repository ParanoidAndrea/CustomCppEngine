#pragma once
#include "stdlib.h"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.hpp"
class RandomNumberGenerator
{
public:
	RandomNumberGenerator(unsigned int seed = 0)
		:m_seed(seed) {
		m_position = 0;
	}
public:
	void SetSeed(unsigned int newSeed) { m_seed = newSeed; m_position = 0; }
	int RollRandomIntLessThan(int maxNotInclusive);
	int RollRandomIntInRange(int minInclusive, int maxInclusive);
	float RollRandomFloatZeroToOne();
	float RollRandomFloatInRange(float minInclusive, float maxInclusive);
	float RollRandomFloatRange(FloatRange range);
	Vec3 RollRandomVector3DInRange(Vec3 mins, Vec3 maxs);
	Vec2 RollRandomVector2DInBox(AABB2 const& box);
	bool RollRandomBool();
private:
	// will replace rand() with noise
	unsigned int m_seed = 0;
	int m_position = 0; 
};

// rand() - random integer from 0 to 32767
