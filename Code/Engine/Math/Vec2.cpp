#include "Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
//#include "Engine/Core/EngineCommon.hpp"


//-----------------------------------------------------------------------------------------------
Vec2::Vec2( const Vec2& copy )
	: x(copy.x)
	, y(copy.y)
{
}


//-----------------------------------------------------------------------------------------------
Vec2::Vec2( float initialX, float initialY )
	: x( initialX )
	, y( initialY )
{
}

const Vec2 Vec2::ZERO = Vec2(0.f, 0.f);
Vec2 const Vec2::MakeFromPolarRadians(float orientationRadians, float length)
{
	return Vec2(length *cosf(orientationRadians), length*sinf(orientationRadians));
}

Vec2 const Vec2::MakeFromPolarDegrees(float orientationDegrees, float length)
{
	return Vec2(length * CosDegrees(orientationDegrees), length * SinDegrees(orientationDegrees));
}

float Vec2::GetLength() const
{
	return sqrtf(x*x + y*y);
}

float Vec2::GetLengthSquared() const
{
	return x * x + y * y;
}

float Vec2::GetOrientationRadians() const
{
	return atan2f(y,x);
}

float Vec2::GetOrientationDegrees() const
{
	return Atan2Degrees(y,x);
}
Vec2 const Vec2::GetDotFromSlope(float k, float newX) const
{
	float newY = y - k * (x - newX);
	return Vec2(newX, newY);
}
Vec2 const Vec2::GetRotated90Degrees() const
{
	return Vec2(-y,x);
}

Vec2 const Vec2::GetRotatedMinus90Degrees() const
{
	return Vec2(y, -x);
}

Vec2 const Vec2::GetRotatedRadians(float deltaRadians) const
{
	float t_length = GetLength();
	float t_radian = GetOrientationRadians() + deltaRadians;
	return Vec2(t_length*cosf(t_radian), t_length*sinf(t_radian));
}

Vec2 const Vec2::GetRotatedDegrees(float deltaDegrees) const
{
	float t_length = GetLength();
	float t_degree = GetOrientationDegrees() + deltaDegrees;
	return Vec2(t_length * CosDegrees(t_degree), t_length * SinDegrees(t_degree));
}

Vec2 const Vec2::GetClamped(float maxLength) const
{
	float t_length = GetLength();
	float t_scale = maxLength / t_length;
	if (t_scale < 1.f && t_length > 0.f)
	{
		return Vec2(x * t_scale, y * t_scale);
	}
	else 
	{ 
		return Vec2(x, y); 
	}
}

Vec2 const Vec2::GetNormalized() const
{
	float t_length = GetLength();
	if (t_length > 0.f)
	{
		float t_scale = 1.f / t_length;
		return Vec2(x * t_scale, y * t_scale);
	}
	return Vec2(x, y);
}

Vec2 const Vec2::GetReflected(Vec2 const& impactSurfaceNormal) const
{
	Vec2 projectVector = impactSurfaceNormal * DotProduct2D(Vec2(x, y), impactSurfaceNormal);
	Vec2 reflectionVector = Vec2(x,y) - projectVector;
	return reflectionVector - projectVector;
}

void Vec2::Reflect(Vec2 const& impactSurfaceNormal)
{
	Vec2 projectVector = impactSurfaceNormal * DotProduct2D(Vec2(x, y), impactSurfaceNormal);
	Vec2 reflectionVector = Vec2(x, y) - projectVector - projectVector;
	x = reflectionVector.x;
	y = reflectionVector.y;
}

void Vec2::Bounce(Vec2 const& impactSurfaceNormal, float elasticity)
{
	Vec2 projectVector = impactSurfaceNormal * DotProduct2D(Vec2(x, y), impactSurfaceNormal);
	Vec2 reflectionVector = Vec2(x, y) - projectVector - projectVector*elasticity;
	x = reflectionVector.x;
	y = reflectionVector.y;
}

void Vec2::SetOrientationRadians(float newOrientationRadians)
{
	float t_length = GetLength();
	float t_radian = newOrientationRadians;
	x = t_length * cosf(t_radian);
	y = t_length * sinf(t_radian);
}

void Vec2::SetOrientationDegrees(float newOrientationDegrees)
{
	float t_length = GetLength();
	float t_degree = newOrientationDegrees;
	x = t_length * CosDegrees(t_degree);
	y = t_length * SinDegrees(t_degree);
}

void Vec2::SetPolarRadians(float newOrientationRadians, float newLength)
{
	float t_radian = newOrientationRadians;
	x = newLength * cosf(t_radian);
	y = newLength * sinf(t_radian);
}

void Vec2::SetPolarDegrees(float newOrientationDegrees, float newLength)
{
	float t_degree = newOrientationDegrees;
	x = newLength * CosDegrees(t_degree);
	y = newLength * SinDegrees(t_degree);
}

void Vec2::Rotate90Degrees()
{
	float t_x = x;
	float t_y = y;
	x = -t_y;
	y = t_x;
}

void Vec2::RotateMinus90Degrees()
{
	float t_x = x;
	float t_y = y;
	x = t_y;
	y = -t_x;
}

void Vec2::RotateRadians(float deltaRadians)
{
	float t_length = GetLength();
	float t_radian = GetOrientationRadians() + deltaRadians;
	x = t_length * cosf(t_radian);
	y = t_length * sinf(t_radian);
}

void Vec2::RotateDegrees(float deltaDegrees)
{
	float t_length = GetLength();
	float t_degree = GetOrientationDegrees() + deltaDegrees;
	x = t_length * CosDegrees(t_degree);
	y = t_length * SinDegrees(t_degree);
}

void Vec2::SetLength(float newLength)
{
	float t_length = GetLength();
	if (t_length > 0)
	{
		float t_scale = newLength / t_length;
		x *= t_scale;
		y *= t_scale;
	}
}

void Vec2::ClampLength(float maxLength)
{
	float t_length = GetLength();
	float t_scale = maxLength / t_length;
	if (t_scale < 1 && t_length>0) {
		x *= t_scale;
		y *= t_scale;
	}
}

void Vec2::Normalize()
{
	float t_length = GetLength();
	if (t_length > 0)
	{
		float t_scale = 1 / t_length;
		x *= t_scale;
		y *= t_scale;
	}
}

float Vec2::NormalizeAndGetPreviousLength()
{
	float t_length = GetLength();
	if (t_length>0)
	{
		float t_scale = 1 / t_length;
		x *= t_scale;
		y *= t_scale;
	}
	return t_length;
}




bool Vec2::SetFromText(const char* text)
{
	Strings resultStrings = SplitStringOnDelimiter(text, ',');
	int numStrings = (int)resultStrings.size();
	if (numStrings != 2)
	{
		return false;
	}
	x = static_cast<float>(atof(resultStrings[0].c_str()));
	y = static_cast<float>(atof(resultStrings[1].c_str()));
	return true;
}

//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator + ( const Vec2& vecToAdd ) const
{
	return Vec2( x+ vecToAdd.x, y + vecToAdd.y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-( const Vec2& vecToSubtract ) const
{
	return Vec2(x- vecToSubtract.x, y - vecToSubtract.y);
}


//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-() const
{
	return Vec2( -x, -y );
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*( float at ) const
{
	return Vec2( at * x, at * y);
}


//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*( const Vec2& vecToMultiply ) const
{
	return Vec2( vecToMultiply.x *x, vecToMultiply.y * y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator/( float inverseScale ) const
{
	return Vec2(x/inverseScale, y/inverseScale);
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator+=( const Vec2& vecToAdd )
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator-=( const Vec2& vecToSubtract )
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator*=( const float uniformScale )
{
	x *= uniformScale;
	y *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator/=( const float uniformDivisor )
{
	x /= uniformDivisor;
	y /= uniformDivisor;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator=( const Vec2& copyFrom )
{
	x = copyFrom.x;
	y = copyFrom.y;
}


//-----------------------------------------------------------------------------------------------
const Vec2 operator*( float uniformScale, const Vec2& vecToScale )
{
	return Vec2(uniformScale* vecToScale.x, uniformScale * vecToScale.y);
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator==( const Vec2& compare ) const
{
	if (x == compare.x && y == compare.y)
		return true;
	else return false;
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator!=( const Vec2& compare ) const
{
	if (x == compare.x && y == compare.y)
		return false;
	else return true;
	
}

