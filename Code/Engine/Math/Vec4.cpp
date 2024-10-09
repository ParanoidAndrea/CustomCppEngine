#include "Vec4.hpp"


Vec4::Vec4(float initialX, float initialY, float initialZ, float initialW)
	:x(initialX),y(initialY),z(initialZ),w(initialW)
{

}

Vec4::Vec4(Vec3 const& initialVec3, float initialW)
	:x(initialVec3.x),y(initialVec3.y), z(initialVec3.z),w(initialW)
{

}

Vec4 const Vec4::operator+(Vec4 const& vecToAdd) const
{
	return Vec4(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z, w + vecToAdd.w);
}

Vec4 const Vec4::operator-(Vec4 const& vecToSubtract) const
{
	return Vec4(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z, w - vecToSubtract.w);
}

Vec4 const Vec4::operator-() const
{
	return Vec4(-x,-y,-z,-w);
}

Vec4 const Vec4::operator*(float uniformScale) const
{
	return Vec4(uniformScale * x, uniformScale * y, uniformScale * z, uniformScale * w);
}

Vec4 const Vec4::operator*(const Vec4& vecToMultiply) const
{
	return Vec4(vecToMultiply.x * x, vecToMultiply.y * y, vecToMultiply.z * z, vecToMultiply.w * w);
}

Vec4 const Vec4::operator/(float inverseScale) const
{
	return Vec4(x / inverseScale, y / inverseScale, z / inverseScale, w / inverseScale);
}


void Vec4::operator+=(Vec4 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
	w += vecToAdd.w;
}

void Vec4::operator-=(Vec4 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
	w -= vecToSubtract.w;
}

void Vec4::operator*=(float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
	w *= uniformScale;
}

void Vec4::operator/=(float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
	w /= uniformDivisor;
}


void Vec4::operator=(Vec4 const& copyfrom)
{
	x = copyfrom.x;
	y = copyfrom.y;
	z = copyfrom.z;
	w = copyfrom.w;
}

bool Vec4::operator!=(Vec4 const& compare) const
{
	if (x == compare.x && y == compare.y && z == compare.z && w == compare.w)
		return false;
	return true;
}

bool Vec4::operator==(Vec4 const& compare) const
{
	if (x == compare.x && y == compare.y && z == compare.z && w == compare.w)
		return true;
	return false;
}

Vec4 const operator*(float uniformScale, Vec4 const& vecToScale)
{
	return Vec4(uniformScale * vecToScale.x, uniformScale * vecToScale.y, uniformScale * vecToScale.z, uniformScale * vecToScale.w );
}
