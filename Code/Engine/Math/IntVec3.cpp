#include "IntVec3.hpp"

IntVec3::IntVec3(IntVec3 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

IntVec3::IntVec3(int initialX, int initialY, int initialZ)
	:x(initialX),
	y(initialY),
	z(initialZ)
{
}


int IntVec3::GetLengthSquared() const
{
	return x * x + y * y + z * z;
}

bool IntVec3::operator!=(IntVec3 const& compareFrom)
{
	if (x == compareFrom.x && y == compareFrom.y && z == compareFrom.z)
		return false;
	return true;
}

IntVec3 IntVec3::operator-(IntVec3 const& vecToMinus) const
{
	return IntVec3(x - vecToMinus.x, y - vecToMinus.y, z - vecToMinus.z);
}

bool IntVec3::operator==(IntVec3 const& compareFrom)
{
	if (x == compareFrom.x && y == compareFrom.y && z == compareFrom.z)
		return true;
	return false;
}

IntVec3 IntVec3::operator +(IntVec3 const& vecToAdd) const
{
	return IntVec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}
