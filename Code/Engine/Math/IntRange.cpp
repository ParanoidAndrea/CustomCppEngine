#include "IntRange.hpp"

IntRange::IntRange(IntRange const& copyFrom)
	:m_min(copyFrom.m_min),
	m_max(copyFrom.m_max)
{

}

IntRange::IntRange(int initialMin, int initialMax)
	:m_min(initialMin),
	m_max(initialMax)
{

}

bool IntRange::IsOnRange(int range)
{
	return (range >= m_min) && (range <= m_max);
}

bool IntRange::IsOverlappingWith(IntRange rangeCompare)
{
	if (rangeCompare.m_min > m_max || rangeCompare.m_max < m_min)
		return false;
	return true;
}

const IntRange IntRange::ZERO = IntRange(0, 0);
const IntRange IntRange::ONE = IntRange(1, 1);
const IntRange IntRange::ZERO_TO_ONE = IntRange(0, 1);

bool IntRange::operator!=(const IntRange& compare) const
{
	return (m_min != compare.m_min) || (m_max != compare.m_max);
}

bool IntRange::operator==(const IntRange& compare) const
{
	return (m_min == compare.m_min) && (m_max == compare.m_max);
}

void IntRange::operator=(const IntRange& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}
