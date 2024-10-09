#include "Easing.hpp"

float EaseInQuadratic(float t)
{
	return t * t;
}

float EaseInCubic(float t)
{
	return t * t * t;
}

float EaseInQuartic(float t)
{
	return (t * t) * (t * t);
}

float EaseInQuintic(float t)
{
	return (t * t) * (t * t) * t;
}

float EaseIn6thOrder(float t)
{
	return (t * t) * (t * t) * (t * t);
}

float EaseOutQuadratic(float t)
{
	float u = 1.f - t;
	return 1.f - (u * u);
}

float EaseOutCubic(float t)
{
	float u = 1.f - t;
	return 1.f - (u * u * u);
}

float EaseOutQuartic(float t)
{
	float u = 1.f - t;
	return 1.f - ((u * u) * (u * u));
}

float EaseOutQuintic(float t)
{
	float u = 1.f - t;
	return 1.f - ((u * u) * (u * u) * u);
}

float EaseOut6thOrder(float t)
{
	float u = 1.f - t;
	return 1.f - ((u * u) * (u * u) * (u * u));
}

float SmoothStart2(float t)
{
	return t * t;
}

float SmoothStart3(float t)
{
	return t * t * t;
}

float SmoothStart4(float t)
{
	return (t * t) * (t * t);
}

float SmoothStart5(float t)
{
	return (t * t) * (t * t) * t;
}

float SmoothStart6(float t)
{
	return (t * t) * (t * t) * (t * t);
}

float SmoothStop2(float t)
{
	float u = 1.f - t;
	return 1.f - (u * u);
}

float SmoothStop3(float t)
{
	float u = 1.f - t;
	return 1.f - (u * u * u);
}

float SmoothStop4(float t)
{
	float u = 1.f - t;
	return 1.f - ((u * u) * (u * u));
}

float SmoothStop5(float t)
{
	float u = 1.f - t;
	return 1.f - ((u * u) * (u * u) * u);
}

float SmoothStop6(float t)
{
	float u = 1.f - t;
	return 1.f - ((u * u) * (u * u) * (u * u));
}

float SmoothStep3(float t)
{
	//return ComputeQuinticBezier1D(0.f, 0.f, 0.f, 1.f, 1.f, 1.f, t);
	//float A = EaseInQuadratic(t);
	//float B = EaseOutQuadratic(t);
	//return A * (1 - t) + B * t;
	return 3.f * t * t - 2.f * (t * t) * t;
}

float SmoothStep5(float t)
{
	return ComputeQuinticBezier1D(0.f, 0.f, 0.f, 1.f, 1.f, 1.f, t);
// 	float t2 = t * t;
// 	float t3 = t2 * t;
// 	return t3 * (6 * t2 - 15 * t + 10);
}

float Hesitate3(float t)
{
	return ComputeCubicBezier1D(0.f, 1.f, 0.f, 1.f, t);
}

float Hesitate5(float t)
{
	return ComputeQuinticBezier1D(0.f, 1.f, 0.f, 1.f, 0.f, 1.f, t);
}

float MyCustomFunkyEasing(float t)
{
	if (t <= 0.5f)
	{
		return ComputeQuinticBezier1D(1.f, 2 * t, 1.f, t, 1.f, 0.5f * t, t);
	}
	else if (t<=0.99f)
	{
		float u = 1 - t;
		return ComputeQuinticBezier1D(1.f, 2 * u, 1.f, u, 1.f, 0.5f * u, u);
	}
	else
	{
		return 0.f;
	}
}
