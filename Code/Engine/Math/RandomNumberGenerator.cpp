#include "RandomNumberGenerator.hpp"
#include "ThirdParty/Squirrel/RawNoise.hpp"
int RandomNumberGenerator::RollRandomIntLessThan(int maxNotInclusive)
{
	unsigned int randomUInt = Get1dNoiseUint(m_position, m_seed);
	++m_position;
	return randomUInt% maxNotInclusive;
}

int RandomNumberGenerator::RollRandomIntInRange(int minInclusive, int maxInclusive)
{
	unsigned int randomUInt = Get1dNoiseUint(m_position, m_seed);
	++m_position;
	int range = 1 + maxInclusive - minInclusive;
	return minInclusive + (randomUInt % range);
}

float RandomNumberGenerator::RollRandomFloatZeroToOne()
{
	float randomFloat = Get1dNoiseZeroToOne(m_position ,m_seed);
		++m_position;
	return randomFloat;
}

// rand() from 0 to 32767 / RAND_MAX
float RandomNumberGenerator::RollRandomFloatInRange(float minInclusive, float maxInclusive)
{
	float range = maxInclusive - minInclusive;
	return minInclusive + (range * RollRandomFloatZeroToOne());
}

float RandomNumberGenerator::RollRandomFloatRange(FloatRange range)
{
	return range.m_min +(range.m_max - range.m_min) * RollRandomFloatZeroToOne();
}

Vec3 RandomNumberGenerator::RollRandomVector3DInRange(Vec3 mins, Vec3 maxs)
{
	Vec3 result;
	result.x = RollRandomFloatInRange(mins.x, maxs.x);
	result.y = RollRandomFloatInRange(mins.y, maxs.y);
	result.z = RollRandomFloatInRange(mins.z, maxs.z);
	return result;
}

Vec2 RandomNumberGenerator::RollRandomVector2DInBox(AABB2 const& box)
{
	Vec2 result;
	result.x = RollRandomFloatInRange(box.m_mins.x, box.m_maxs.x);
	result.y = RollRandomFloatInRange(box.m_mins.y, box.m_maxs.y);
	return result;
}

Rgba8 RandomNumberGenerator::RollRandomColor(Rgba8 const& color1, Rgba8 const& color2)
{
	float weight = RollRandomFloatZeroToOne();
    return InterpolateFromNewColor(color1, color2, weight);
}

bool RandomNumberGenerator::RollRandomBool()
{
	int number = RollRandomIntInRange(0, 1);
	if (number == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
