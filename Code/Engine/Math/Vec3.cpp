#include "Vec3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
//#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec3.hpp"
//-----------------------------------------------------------------------------------------------
Vec3::Vec3(float initialX, float initialY, float initialZ)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
{
}

Vec3::Vec3(Vec2 const& vect2)
	:x(vect2.x)
	,y(vect2.y)
	,z(0.f)
{

}

Vec3::Vec3(Vec2 const& vect2, float initialZ)
	:x(vect2.x)
	, y(vect2.y)
	, z(initialZ)
{

}
const Vec3 Vec3::ZERO = Vec3(0.f, 0.f, 0.f);
Vec3::Vec3(IntVec3 const& copyFrom)
	:x((float)copyFrom.x),y((float)copyFrom.y),z((float)copyFrom.z)
{
}

Vec3 const Vec3::Min(const Vec3& a, const Vec3& b) 
{
	return Vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Vec3 const Vec3::Max(const Vec3& a, const Vec3& b) 
{
	return Vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}
float Vec3::GetLength() const
{
	return sqrtf(x * x + y * y + z * z);
}

float Vec3::GetLengthXY() const
{
	return sqrtf(x * x + y * y);
}

float Vec3::GetLengthSquared() const
{
	return x * x + y * y + z * z;
}

float Vec3::GetLengthXYSquared() const
{
	return x * x + y * y;
}

float Vec3::GetAngleAboutZRadians() const
{
	return atan2f(y, x);
}

float Vec3::GetAngleAboutZDegrees() const
{
	return Atan2Degrees(y, x);
}

Vec3 const Vec3::GetRotatedAboutZRadians(float deltaRadians) const
{
	float t_length = GetLengthXY();
	float t_radian = GetAngleAboutZRadians() + deltaRadians;
	return Vec3(t_length * cosf(t_radian), t_length * sinf(t_radian), z);
}

Vec3 const Vec3::GetRotatedAboutZDegrees(float deltaDegrees) const
{
	float t_length = GetLengthXY();
	float t_degree = GetAngleAboutZDegrees() + deltaDegrees;
	return Vec3(t_length * CosDegrees(t_degree), t_length * SinDegrees(t_degree), z);
}

Vec3 const Vec3::GetClamped(float maxLength) const
{
	float t_length = GetLength();
	float t_scale = maxLength / t_length;
	if (t_scale < 1.f && t_length > 0.f)
	{
		return Vec3(x * t_scale, y * t_scale, z * t_scale);
	}
	return Vec3(x, y, z);
}

Vec3 const Vec3::GetNormalized() const
{
	float t_length = GetLength();
	float t_scale = 1.f / t_length;
	if (t_length > 0.f)
	{
		return Vec3(x * t_scale, y * t_scale, z * t_scale);
	}
	return Vec3(x,y,z);
}

void Vec3::Normalize()
{
	float t_length = GetLength();
	float t_scale = 1.f / t_length;
	if (t_length > 0.f)
	{
		x = x * t_scale;
		y = y * t_scale;
		z = z * t_scale;
	}
}

Vec3 const Vec3::MakeFromPolarRadians(float latitudeRadians, float longitudeRadians, float length)
{
	return length * Vec3(cosf(longitudeRadians) * cosf(latitudeRadians), sinf(longitudeRadians) * cosf(latitudeRadians), sinf(latitudeRadians));
}

Vec3 const Vec3::MakeFromPolarDegrees(float latitudeDegrees, float longitudeDegrees, float length)
{
	return length * Vec3(CosDegrees(longitudeDegrees) * CosDegrees(latitudeDegrees), SinDegrees(longitudeDegrees) * CosDegrees(latitudeDegrees), SinDegrees(latitudeDegrees));
}

bool Vec3::SetFromText(const char* text)
{
	Strings resultStrings = SplitStringOnDelimiter(text, ',');
	int numStrings = (int)resultStrings.size();
	if (numStrings != 3)
	{
		return false;
	}
	x = static_cast<float>(atof(resultStrings[0].c_str()));
	y = static_cast<float>(atof(resultStrings[1].c_str()));
	z = static_cast<float>(atof(resultStrings[2].c_str()));
	return true;
	
}

EulerAngles const Vec3::GetEulerAngle() const
{
	return EulerAngles(Atan2Degrees(y, x), AsinDegrees(-z), 0.f);
}

//-----------------------------------------------------------------------------------------------
const Vec3 Vec3::operator + (const Vec3& vecToAdd) const
{
	return Vec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}


//-----------------------------------------------------------------------------------------------
const Vec3 Vec3::operator-(const Vec3& vecToSubtract) const
{
	return Vec3(x - vecToSubtract.x, y - vecToSubtract.y, z-vecToSubtract.z);
}


//------------------------------------------------------------------------------------------------
const Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y,-z);
}


//-----------------------------------------------------------------------------------------------
Vec3 const Vec3::operator*(float at) const
{
	return Vec3(at * x, at * y, at * z);
}


//------------------------------------------------------------------------------------------------
 Vec3 const Vec3::operator*(const Vec3& vecToMultiply) const
{
	return Vec3(vecToMultiply.x * x, vecToMultiply.y * y, vecToMultiply.z * z);
}


//-----------------------------------------------------------------------------------------------
Vec3 const Vec3::operator/(float inverseScale) const
{
	return Vec3(x / inverseScale, y / inverseScale, z / inverseScale);
}


Vec3 const Vec3::operator/(Vec3 inverseVector) const
{
	return Vec3(x / inverseVector.x, y / inverseVector.y, z / inverseVector.z);
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator+=(const Vec3& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator-=(const Vec3& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
}


//-----------------------------------------------------------------------------------------------
void Vec3::operator=(const Vec3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}


//-----------------------------------------------------------------------------------------------
const Vec3 operator*(float uniformScale, const Vec3& vecToScale)
{
	return Vec3(uniformScale * vecToScale.x, uniformScale * vecToScale.y, uniformScale * vecToScale.z);
}


//-----------------------------------------------------------------------------------------------
bool Vec3::operator==(const Vec3& compare) const
{
	if (x == compare.x && y == compare.y && z==compare.z)
		return true;
	else return false;
}


//-----------------------------------------------------------------------------------------------
bool Vec3::operator!=(const Vec3& compare) const
{
	if (x == compare.x && y == compare.y && z==compare.z)
		return false;
	else return true;

}