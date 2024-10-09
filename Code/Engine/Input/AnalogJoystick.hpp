#pragma once
#include "Engine/Math/Vec2.hpp"
class AnalogJoystick
{
public:
	Vec2  GetPosition() const;
	float GetMagnitude() const;
	float GetOrientationDegrees() const;

	Vec2  GetRawUncorrectedPosition() const;
	float GetInnerDeadZoneFraction() const;
	float GetOuterDeadZoneFraction() const;

	void  Reset();
	void  SetDeadZoneThresholds(float normalizedInnerDeadzoneThreshold, float normalizedOuterDeadzoneThreshold);
	void  UpdatePosition(float rawNormalizedX, float rawNormalizedY);

protected:
	Vec2  m_uncorrectedPosition;                           //doesn't reset at zero
	Vec2  m_correctedPosition;                     //deadzone-corrected position
	float m_innerDeadZoneFraction = 0.f;        // if R < this%, R=0; "input range start" for corrective range map
	float m_outerDeadZoneFraction = 1.f;        // if R > this%, R=1; "input range end" for corrective range map

};