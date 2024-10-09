#include "AnalogJoystick.hpp"
#include "Engine/Math/MathUtils.hpp"
Vec2 AnalogJoystick::GetPosition() const
{
	return m_correctedPosition;
}

float AnalogJoystick::GetMagnitude() const
{
	return m_correctedPosition.GetLength();
}

float AnalogJoystick::GetOrientationDegrees() const
{
	return m_correctedPosition.GetOrientationDegrees();
}

Vec2 AnalogJoystick::GetRawUncorrectedPosition() const
{
	return m_uncorrectedPosition;
}

float AnalogJoystick::GetInnerDeadZoneFraction() const
{
	return m_innerDeadZoneFraction;
}

float AnalogJoystick::GetOuterDeadZoneFraction() const
{
	return m_outerDeadZoneFraction;
}

void AnalogJoystick::Reset()
{
	m_uncorrectedPosition = Vec2(0.f, 0.f);
	m_correctedPosition = Vec2(0.f, 0.f);
	SetDeadZoneThresholds(0.f, 1.f);
}

void AnalogJoystick::SetDeadZoneThresholds(float normalizedInnerDeadzoneThreshold, float normalizedOuterDeadzoneThreshold)
{
	m_innerDeadZoneFraction = normalizedInnerDeadzoneThreshold;
	m_outerDeadZoneFraction = normalizedOuterDeadzoneThreshold;
}

void AnalogJoystick::UpdatePosition(float rawNormalizedX, float rawNormalizedY)
{
	m_uncorrectedPosition.x = rawNormalizedX;
	m_uncorrectedPosition.y = rawNormalizedY;

	float uncorrectedMagnitude = m_uncorrectedPosition.GetLength();
	float uncorrectedDegrees = m_uncorrectedPosition.GetOrientationDegrees();
	float correctedMagnitude = RangeMapClamped(uncorrectedMagnitude, m_innerDeadZoneFraction, m_outerDeadZoneFraction,0.f,1.f);
	m_correctedPosition.SetPolarDegrees(uncorrectedDegrees, correctedMagnitude);
}
