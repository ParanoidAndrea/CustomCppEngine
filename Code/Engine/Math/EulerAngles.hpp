#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Mat44.hpp"
struct EulerAngles
{
public:
	EulerAngles() = default;
    EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees);
	void GetAsVectors_IFwd_JLeft_KUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upwardKBasis);
	Mat44 GetAsMatrix_IFwd_JLeft_KUp() const;
	Vec3 GetForwardDir_XFwd_YLeft_ZUp() const;
	Vec3 GetIForward() const;
	Vec3 GetJLeftNormal() const;
	Vec2 GetForwardXY() const;
	bool SetFromText(const char* text);
	void  operator+= (EulerAngles const& anglesToAdd);
	void  operator-= (EulerAngles const& anglesToAdd);
	EulerAngles const operator*(float uniformScale) const;
public:
	float m_yawDegrees = 0.f;
	float m_pitchDegrees = 0.f;
	float m_rollDegrees = 0.f;

};

