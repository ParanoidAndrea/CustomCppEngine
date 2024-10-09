#pragma once
struct IntRange
{
public:
	int m_min = 0;
	int m_max = 0;
	static const IntRange ZERO;
	static const IntRange ONE;
	static const IntRange ZERO_TO_ONE;
public:
	~IntRange() {}
	IntRange() {}
	IntRange(IntRange const& copyFrom);
	explicit IntRange(int initialMin, int initialMax);
	void	operator=(const IntRange& copyFrom);
	bool	operator==(const IntRange& compare) const;
	bool	operator!=(const IntRange& compare) const;
	bool IsOnRange(int range);
	bool IsOverlappingWith(IntRange rangeCompare);
};