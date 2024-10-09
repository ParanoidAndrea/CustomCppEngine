#pragma once
struct FloatRange
{
public:
	float m_min = 0.f;
	float m_max = 0.f;
	static const FloatRange ZERO;
	static const FloatRange ONE;
	static const FloatRange ZERO_TO_ONE;
public:
	~FloatRange(){}
	FloatRange() {}
	FloatRange(FloatRange const& copyFrom);
	explicit FloatRange(float initialMin, float initialMax);
	void	operator=(const FloatRange& copyFrom);
	bool	operator==(const FloatRange& compare) const;		
	bool	operator!=(const FloatRange& compare) const;
	bool IsOnRange(float value) const;
	bool IsOverlappingWith(FloatRange rangeCompare) const;
	bool SetFromText(char const* text);
};
FloatRange const OverlappedFloatRange(FloatRange const& range1, FloatRange const& range2);