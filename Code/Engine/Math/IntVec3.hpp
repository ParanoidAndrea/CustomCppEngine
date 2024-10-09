#pragma once
struct  IntVec3
{
public:
	int x = 0;
	int y = 0;
	int z = 0;
	static const IntVec3 ZERO;
public:
	~IntVec3() {}
	IntVec3() {}
	IntVec3(IntVec3 const& copyFrom);
	explicit IntVec3(int initialX, int initialY, int initialZ);
	int GetLengthSquared() const;
	bool    operator== (IntVec3 const& compareFrom);
	bool    operator!= (IntVec3 const& compareFrom);
	IntVec3 operator+  (IntVec3 const& vecToAdd)const;
	IntVec3 operator-  (IntVec3 const& vecToMinus)const;
};