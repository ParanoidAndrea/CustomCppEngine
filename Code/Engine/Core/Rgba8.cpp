#include "Rgba8.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "StringUtils.hpp"
Rgba8 const Rgba8::RED   (255, 0, 0, 255);
Rgba8 const Rgba8::YELLOW(255, 255, 0, 255);
Rgba8 const Rgba8::GREEN(0, 255, 0, 255);
Rgba8 const Rgba8::WHITE(255, 255, 255, 255);
Rgba8 const Rgba8::GREY (100, 100, 100, 200);
Rgba8 const Rgba8::BLACK (0, 0, 0, 255);
Rgba8 const Rgba8::BLUE (0, 0, 255, 255);
Rgba8 const Rgba8::MAGENTA(255, 0, 255, 255);
Rgba8::Rgba8(unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte):
	r(redByte), g(greenByte), b(blueByte), a(alphaByte)
{
}



void Rgba8::operator-=(Rgba8 const& rgb8SubstractTo)
{
	r = static_cast<unsigned char>(GetClamped((int)r - (int)rgb8SubstractTo.r, 0, 255));
	g = static_cast<unsigned char>(GetClamped((int)g - (int)rgb8SubstractTo.g, 0, 255));
	b = static_cast<unsigned char>(GetClamped((int)b - (int)rgb8SubstractTo.b, 0, 255));
	a = static_cast<unsigned char>(GetClamped((int)a - (int)rgb8SubstractTo.a, 0, 255));
}

void Rgba8::operator+=(Rgba8 const& rgb8AddTo) 
{
	r = static_cast<unsigned char>(GetClamped((int)r + (int)rgb8AddTo.r, 0, 255));
	g = static_cast<unsigned char>(GetClamped((int)g + (int)rgb8AddTo.g, 0, 255));
	b = static_cast<unsigned char>(GetClamped((int)b + (int)rgb8AddTo.b, 0, 255));
	a = static_cast<unsigned char>(GetClamped((int)a + (int)rgb8AddTo.a, 0, 255));
}

bool Rgba8::operator!=(Rgba8 const& compare) const
{
	return !(r == compare.r && g == compare.g && b == compare.b && a == compare.a);
}
bool Rgba8::operator==(Rgba8 const& compare) const
{
	return (r == compare.r && g == compare.g && b == compare.b && a == compare.a);
}
bool Rgba8::SetFromText(const char* text)
{
	Strings resultStrings = SplitStringOnDelimiter(text, ',');

	if (resultStrings.size() == 3 || resultStrings.size() == 4) 
	{
		r = static_cast<unsigned char>(atoi(resultStrings[0].c_str()));
		g = static_cast<unsigned char>(atoi(resultStrings[1].c_str()));
		b = static_cast<unsigned char>(atoi(resultStrings[2].c_str()));

		if (resultStrings.size() == 4) 
		{
			a = static_cast<unsigned char>(atoi(resultStrings[3].c_str()));
		}
		else 
		{
			a = 255;
		}
		return true;
	}
	else 
	{
		return false;
	}
}

void Rgba8::GetAsFloats(float* colorAsFloats) const
{
	colorAsFloats[0] = static_cast<float>(r) / 255.0f;
	colorAsFloats[1] = static_cast<float>(g) / 255.0f;
	colorAsFloats[2] = static_cast<float>(b) / 255.0f;
	colorAsFloats[3] = static_cast<float>(a) / 255.0f;
}


Vec4 const Rgba8::GetAsFloats() const
{
	return Vec4(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f);
}

Rgba8 Rgba8::SetFromFloats(float redFloat, float greenFloat, float blueFloat, float alphaFloat)
{
	unsigned char r = (unsigned char)(redFloat * 255.f);
	unsigned char g = (unsigned char)(greenFloat * 255.f);
	unsigned char b = (unsigned char)(blueFloat * 255.f);
	unsigned char a = (unsigned char)(alphaFloat * 255.f);
	return Rgba8(r, g, b, a);
}

Rgba8 Rgba8::InterpolateColor(Rgba8 const& endColor, float fraction)
{
	unsigned char red = static_cast<unsigned char>  (Interpolate((float)r, (float)endColor.r, fraction));
	unsigned char green = static_cast<unsigned char>(Interpolate((float)g, (float)endColor.g, fraction));
	unsigned char blue = static_cast<unsigned char> (Interpolate((float)b, (float)endColor.b, fraction));
	unsigned char alpha = static_cast<unsigned char>(Interpolate((float)a, (float)endColor.a, fraction));
	return Rgba8(red, green, blue, alpha);
}


Rgba8 Rgba8::InterpolateFromNewColor(Rgba8 const& newColor, Rgba8 const& endColor, float fraction)
{
	unsigned char red = static_cast<unsigned char>  (Interpolate((float)newColor.r, (float)endColor.r, fraction));
	unsigned char green = static_cast<unsigned char>(Interpolate((float)newColor.g, (float)endColor.g, fraction));
	unsigned char blue = static_cast<unsigned char> (Interpolate((float)newColor.b, (float)endColor.b, fraction));
	unsigned char alpha = static_cast<unsigned char>(Interpolate((float)newColor.a, (float)endColor.a, fraction));
	return Rgba8(red, green, blue, alpha);
}

Rgba8 Rgba8::InterpolateAlpha(unsigned char startAlpha, unsigned char endAlpha, float fraction)
{
	a = static_cast<unsigned char>(Interpolate(startAlpha, endAlpha, fraction));
	return Rgba8(r, g, b, a);
}
Rgba8 Rgba8::InterpolateFromOldAlpha(unsigned char endAlpha, float fraction)
{
	a = static_cast<unsigned char>(Interpolate(a, endAlpha, fraction));
	return Rgba8(r, g, b, a);
}

Rgba8 InterpolateFromNewColor(Rgba8 const& startColor, Rgba8 const& endColor, float fraction)
{
	unsigned char red = static_cast<unsigned char>  (GetClamped(Interpolate((float)startColor.r, (float)endColor.r, fraction),0.f,255.f));
	unsigned char green = static_cast<unsigned char>(GetClamped(Interpolate((float)startColor.g, (float)endColor.g, fraction),0.f,255.f));
	unsigned char blue = static_cast<unsigned char> (GetClamped(Interpolate((float)startColor.b, (float)endColor.b, fraction),0.f,255.f));
	unsigned char alpha = static_cast<unsigned char>(GetClamped(Interpolate((float)startColor.a, (float)endColor.a, fraction),0.f,255.f));
	return Rgba8(red, green, blue, alpha);
}

Rgba8 ClampedColor(Rgba8 const& color)
{
	Rgba8 clampedColor;
	clampedColor.r = static_cast<unsigned char>(GetClamped((int)color.r, 0, 255));
	clampedColor.g = static_cast<unsigned char>(GetClamped((int)color.g, 0, 255));
	clampedColor.b = static_cast<unsigned char>(GetClamped((int)color.b, 0, 255));
	clampedColor.a = static_cast<unsigned char>(GetClamped((int)color.a, 0, 255));
	return clampedColor;
}