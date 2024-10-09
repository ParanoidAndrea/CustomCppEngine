#pragma once
#include "math.h"
struct  IntVec2
{
public:
	int x = 0;
	int y = 0;
	static const IntVec2 ZERO;
public:
	~IntVec2() {}
	IntVec2() {}
	IntVec2(IntVec2 const& copyFrom);
	explicit IntVec2(int initialX, int initialY);

	// Accessors
	float GetLength() const;
	int GetTaxicabLength() const;
	float GetOrientationRadians() const;
	float GetOrientationDegrees() const;
	IntVec2 const GetRotated90Degrees() const;
	IntVec2 const GetRotatedMinus90Degrees() const;
	bool SetFromText(char const* text);

	// Mutators(non-const methods)
	void Rotate90Degrees();
	void RotateMinus90Degrees();
	//Operators(self-mutating/non-const)
	bool operator<(const IntVec2& other) const;
	void operator= (IntVec2 const& copyFrom);
	bool operator== (IntVec2 const& compareFrom);
	bool operator== (IntVec2 const& compareFrom) const;
	bool operator!= (IntVec2 const& compareFrom);
	 IntVec2 operator+ (IntVec2 const& vecToAdd)const;
	 IntVec2 operator- (IntVec2 const& vecToMinus)const;
};