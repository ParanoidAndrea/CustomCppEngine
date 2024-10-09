#include "IntVec2.hpp"
#include "Engine/Core/StringUtils.hpp"
IntVec2::IntVec2(int initialX, int initialY)
	: x(initialX)
	, y(initialY)
{
}

IntVec2::IntVec2(IntVec2 const& copyFrom)
	: x(copyFrom.x)
	, y(copyFrom.y)
{
}
const IntVec2 IntVec2::ZERO = IntVec2(0, 0);
void IntVec2::operator=(IntVec2 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

float IntVec2::GetLength() const
{
	return sqrtf((float)(x*x)+(float)(y*y));
}
int IntVec2::GetTaxicabLength() const
{
	if (x >= 0 && y >= 0)
		return x + y;
	else if (x < 0 && y >= 0)
		return -x + y;
	else if (x >= 0 && y < 0)
		return x - y;
	else return -x - y;
}

float IntVec2::GetOrientationRadians() const
{
	return atan2f((float)y,(float)x);
}

float IntVec2::GetOrientationDegrees() const
{
	float radians = atan2f((float)y, (float)x);
	return radians * 180.f / 3.14159265f;
}

IntVec2 const IntVec2::GetRotated90Degrees() const
{
	return IntVec2(-y, x);
}

IntVec2 const IntVec2::GetRotatedMinus90Degrees() const
{
	return IntVec2(y, -x);
}

bool IntVec2::SetFromText(char const* text)
{
  	std::vector<std::string> resultStrings;
	resultStrings = SplitStringOnDelimiter(text, ',');
  	int numStrings = (int)resultStrings.size();
  	if (numStrings != 2)
  	{
  		return false;
  	}
	x = atoi(resultStrings[0].c_str());
	y = atoi(resultStrings[1].c_str());
	return true;
}

void IntVec2::Rotate90Degrees()
{
	int t_x = x;
	int t_y = y;
	x = -t_y;
	y = t_x;
}

void IntVec2::RotateMinus90Degrees()
{
	int t_x = x;
	int t_y = y;
	x = t_y;
	y = -t_x;
}

bool IntVec2::operator<(const IntVec2& compareForm) const
{
	if (y < compareForm.y)
	{
		return true;
	}
	else if (y > compareForm.y)
	{
		return false;
	}
	else
	{
		return x < compareForm.x;
	}
}

bool IntVec2::operator==(IntVec2 const& compareFrom) const
{
	if (x == compareFrom.x && y == compareFrom.y)
		return true;
	return false;
}

bool IntVec2::operator!=(IntVec2 const& compareFrom)
{
	if (x == compareFrom.x && y == compareFrom.y)
		return false;
	return true;
}

IntVec2 IntVec2::operator-(IntVec2 const& vecToMinus) const
{
	return IntVec2(x - vecToMinus.x, y - vecToMinus.y);
}

bool IntVec2::operator==(IntVec2 const& compareFrom)
{
	if (x == compareFrom.x && y == compareFrom.y)
		return true;
	return false;
}

 IntVec2 IntVec2::operator +(IntVec2 const& vecToAdd) const
{
	return IntVec2(x + vecToAdd.x, y + vecToAdd.y);
}





