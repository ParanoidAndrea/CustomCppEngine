#include "EulerAngles.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
EulerAngles::EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees)
	:m_yawDegrees(yawDegrees),
	m_pitchDegrees(pitchDegrees),
	m_rollDegrees(rollDegrees)
{

}
void EulerAngles::GetAsVectors_IFwd_JLeft_KUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upwardKBasis)
{
	Mat44 eulerAsMatrix = GetAsMatrix_IFwd_JLeft_KUp();
	out_forwardIBasis = eulerAsMatrix.GetIBasis3D();
	out_leftJBasis = eulerAsMatrix.GetJBasis3D();
	out_upwardKBasis = eulerAsMatrix.GetKBasis3D();
	
}

Mat44 EulerAngles::GetAsMatrix_IFwd_JLeft_KUp() const 
{
	
// 	eulerAsMatrix.AppendZRotation(m_yawDegrees);
// 	eulerAsMatrix.AppendYRotation(m_pitchDegrees);
// 	eulerAsMatrix.AppendXRotation(m_rollDegrees);
	float cy = CosDegrees(m_yawDegrees);
	float sy = SinDegrees(m_yawDegrees);
	float cp = CosDegrees(m_pitchDegrees);
	float sp = SinDegrees(m_pitchDegrees);
	float cr = CosDegrees(m_rollDegrees);
	float sr = SinDegrees(m_rollDegrees);
	Mat44 eulerAsMatrix = Mat44(
		Vec4(cy * cp,                sy * cp,                -sp,     0.f),
		Vec4(cy * sp * sr - sy * cr, sy * sp * sr + cy * cr, cp * sr, 0.f),
		Vec4(cy * sp * cr + sy * sr, sy * sp * cr - cy * sr, cp * cr, 0.f),
		Vec4(0.f, 0.f, 0.f, 1.f)
	);
	return eulerAsMatrix;
}

Vec3 EulerAngles::GetForwardDir_XFwd_YLeft_ZUp() const
{
	float cy = CosDegrees(m_yawDegrees);
	float sy = SinDegrees(m_yawDegrees);
	float cp = CosDegrees(m_pitchDegrees);
	float sp = SinDegrees(m_pitchDegrees);

	return Vec3(cy * cp, sy * cp, -sp);
}

Vec3 EulerAngles::GetIForward() const
{
	return GetAsMatrix_IFwd_JLeft_KUp().GetIBasis3D();
}

Vec3 EulerAngles::GetJLeftNormal() const
{
	return GetAsMatrix_IFwd_JLeft_KUp().GetJBasis3D().GetNormalized();
}

Vec2 EulerAngles::GetForwardXY() const
{
	float cy = CosDegrees(m_yawDegrees);
	float sy = SinDegrees(m_yawDegrees);
	return Vec2(cy, sy);
}

bool EulerAngles::SetFromText(const char* text)
{
	Strings resultStrings = SplitStringOnDelimiter(text, ',');
	int numStrings = (int)resultStrings.size();
	if (numStrings != 3)
	{
		return false;
	}
	m_yawDegrees = static_cast<float>(atof(resultStrings[0].c_str()));
	m_pitchDegrees = static_cast<float>(atof(resultStrings[1].c_str()));
	m_rollDegrees = static_cast<float>(atof(resultStrings[2].c_str()));
	return true;
	
}

void EulerAngles::operator+=(EulerAngles const& anglesToAdd) 
{
	m_yawDegrees += anglesToAdd.m_yawDegrees;
	m_pitchDegrees += anglesToAdd.m_pitchDegrees;
	m_rollDegrees += anglesToAdd.m_rollDegrees;
}

void EulerAngles::operator-=(EulerAngles const& anglesToAdd)
{
	m_yawDegrees -= anglesToAdd.m_yawDegrees;
	m_pitchDegrees -= anglesToAdd.m_pitchDegrees;
	m_rollDegrees -= anglesToAdd.m_rollDegrees;
}

EulerAngles const EulerAngles::operator*(float uniformScale) const
{
	return EulerAngles(m_yawDegrees * uniformScale, m_pitchDegrees * uniformScale, m_rollDegrees * uniformScale);
}
