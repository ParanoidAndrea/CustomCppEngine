#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Core/StringUtils.hpp"
FloatRange::FloatRange(FloatRange const& copyFrom)
	:m_min(copyFrom.m_min),
	m_max(copyFrom.m_max)
{

}

FloatRange::FloatRange(float initialMin, float initialMax)
	:m_min(initialMin),
	m_max(initialMax)
{

}


bool FloatRange::IsOnRange(float value) const
{
	return (value >= m_min) && (value <= m_max);
}


bool FloatRange::IsOverlappingWith(FloatRange rangeCompare) const
{
	if (rangeCompare.m_min > m_max || rangeCompare.m_max < m_min)
		return false;
	return true;
}

bool FloatRange::SetFromText(char const* text)
{
	std::vector<std::string> resultStrings;
	resultStrings = SplitStringOnDelimiter(text, '~');
	int numStrings = (int)resultStrings.size();
	if (numStrings != 2)
	{
		return false;
	}
	m_min = (float)atof(resultStrings[0].c_str());
	m_max = (float)atof(resultStrings[1].c_str());
	return true;
}

const FloatRange FloatRange::ZERO = FloatRange(0.f, 0.f);
const FloatRange FloatRange::ONE = FloatRange(1.f, 1.f);
const FloatRange FloatRange::ZERO_TO_ONE = FloatRange(0.f, 1.f);

bool FloatRange::operator!=(const FloatRange& compare) const
{
	return (m_min != compare.m_min) || (m_max != compare.m_max);
}

bool FloatRange::operator==(const FloatRange& compare) const
{
	return (m_min == compare.m_min) && (m_max == compare.m_max);
}

void FloatRange::operator=(const FloatRange& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}
FloatRange const OverlappedFloatRange(FloatRange const& range1, FloatRange const& range2)
{
	if (!range1.IsOverlappingWith(range2))
	{
		return FloatRange();
	}
	FloatRange overlappedRange;
	if (range1.m_min > range2.m_min)
	{
		overlappedRange.m_min = range1.m_min;
	}
	else
	{
		overlappedRange.m_min = range2.m_min;
	}

	if (range1.m_max < range2.m_max)
	{
		overlappedRange.m_max = range1.m_max;
	}
	else
	{
		overlappedRange.m_max = range2.m_max;
	}
	return overlappedRange;
}
