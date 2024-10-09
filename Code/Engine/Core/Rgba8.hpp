#pragma once
#include "Engine/Math/Vec4.hpp"
struct Rgba8 {

	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;
	unsigned char a = 255;
public:
	
	Rgba8() {};
	explicit Rgba8(unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte);
	~Rgba8() {};
	//explicit Rgba8(float redFloat, float greenFloat, float blueFloat, float alphaFloat);
	//void operator= (Rgba8 const& copyFrom);
	void operator+= (Rgba8 const& rgb8AddTo);
	void operator-= (Rgba8 const& rgb8SubstractTo);
	bool operator!=(Rgba8 const& compare) const;
	bool operator==(Rgba8 const& compare) const;
	static const Rgba8 RED;	
	static const Rgba8 YELLOW;
	static const Rgba8 GREEN;
	static const Rgba8 WHITE;
	static const Rgba8 GREY;
	static const Rgba8 BLACK;
	static const Rgba8 BLUE;
	static const Rgba8 MAGENTA;
	bool SetFromText(const char* text);
	void GetAsFloats(float* colorAsFloats) const;
	Vec4 const GetAsFloats() const;
	static Rgba8 SetFromFloats(float redFloat, float greenFloat, float blueFloat, float alphaFloat);
	Rgba8 InterpolateColor(Rgba8 const& endColor, float fraction);
	Rgba8 InterpolateFromNewColor(Rgba8 const& startColor, Rgba8 const& endColor, float fraction);
	Rgba8 InterpolateAlpha(unsigned char startAlpha, unsigned char endAlpha, float fraction);
	Rgba8 InterpolateFromOldAlpha(unsigned char endAlpha, float fraction);
};
Rgba8 ClampedColor(Rgba8 const& color);
Rgba8 InterpolateFromNewColor(Rgba8 const& startColor, Rgba8 const& endColor, float fraction);