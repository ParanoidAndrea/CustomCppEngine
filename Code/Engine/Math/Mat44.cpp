#include "Mat44.hpp"
#include <math.h>
#include "Engine/Math/MathUtils.hpp"
#include <cstring>
#include "Engine/Core/EngineCommon.hpp"
Mat44::Mat44() 
	:m_values{0.f}
{
	//memset(m_values, 0.f, sizeof(m_values));
	m_values[Ix] = 1.f;
	m_values[Jy] = 1.f;		
	m_values[Kz] = 1.f;
	m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translation2D)
	:m_values{ 0.f }
{
	//memset(m_values, 0.f, sizeof(m_values));
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Kz] = 1.f;
	m_values[Tx] = translation2D.x;
	m_values[Ty] = translation2D.y;
	m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translation3D)
	:m_values{ 0.f }
{
	//memset(m_values, 0.f, sizeof(m_values));
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Tx] = translation3D.x;
	m_values[Ty] = translation3D.y;
	m_values[Tz] = translation3D.z;
	m_values[Tw] = 1.f;
}

Mat44::Mat44(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;
	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;
	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;
	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}

Mat44::Mat44(float const* sixteenValuesBasisMajor)
{
	memcpy(m_values, sixteenValuesBasisMajor, sizeof(m_values));
}

Mat44::Mat44(Vec3 const& translation3D)
	:m_values{ 0.f }
{
	m_values[Ix] = 1.f;
	m_values[Jy] = 1.f;
	m_values[Kz] = 1.f;
	m_values[Tx] = translation3D.x;
	m_values[Ty] = translation3D.y;
	m_values[Tz] = translation3D.z;
	m_values[Tw] = 1.f;
}

Mat44 const Mat44::CreateTranslation2D(Vec2 const& translationXY)
{
	return Mat44(Vec2(1.f, 0.f), Vec2(0.f, 1.f), translationXY);
}

Mat44 const Mat44::CreateTranslation3D(Vec3 const& translationXYZ)
{
	return Mat44(Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f), translationXYZ);
}

Mat44 const Mat44::CreateUniformScale2D(float uniformScaleXY)
{
	return Mat44(Vec2(uniformScaleXY, 0.f), Vec2(0.f, uniformScaleXY), Vec2());
}

Mat44 const Mat44::CreateUniformScale3D(float uniformScaleXYZ)
{
	return Mat44(Vec3(uniformScaleXYZ, 0.f, 0.f), Vec3(0.f, uniformScaleXYZ, 0.f), Vec3(0.f, 0.f, uniformScaleXYZ), Vec3());
}

Mat44 const Mat44::CreateNonUniformScale2D(Vec2 const& nonUniformScaleXY)
{
	return Mat44(Vec2(nonUniformScaleXY.x, 0.f), Vec2(0.f, nonUniformScaleXY.y), Vec2());
}

Mat44 const Mat44::CreateNonUniformScale3D(Vec3 const& nonUniformScaleXYZ)
{
	return Mat44(Vec3(nonUniformScaleXYZ.x, 0.f, 0.f), Vec3(0.f, nonUniformScaleXYZ.y, 0.f), Vec3(0.f, 0.f, nonUniformScaleXYZ.z), Vec3());
}

Mat44 const Mat44::CreateZRotationDegrees(float rotationDegreesAboutZ)
{
	return Mat44(Vec3(CosDegrees(rotationDegreesAboutZ), SinDegrees(rotationDegreesAboutZ), 0.f), Vec3(-SinDegrees(rotationDegreesAboutZ), CosDegrees(rotationDegreesAboutZ), 0.f), Vec3(0.f, 0.f, 1.f), Vec3());
}

Mat44 const Mat44::CreateYRotationDegrees(float rotationDegreesAboutY)
{
	return Mat44(Vec3(CosDegrees(rotationDegreesAboutY), 0.f, -SinDegrees(rotationDegreesAboutY)), Vec3(0.f, 1.f, 0.f), Vec3(SinDegrees(rotationDegreesAboutY), 0.f, CosDegrees(rotationDegreesAboutY)), Vec3());
}

Mat44 const Mat44::CreateXRotationDegrees(float rotationDegreesAboutX)
{
	return Mat44(Vec3(1.f, 0.f, 0.f), Vec3(0.f, CosDegrees(rotationDegreesAboutX), SinDegrees(rotationDegreesAboutX)), Vec3(0.f, -SinDegrees(rotationDegreesAboutX), CosDegrees(rotationDegreesAboutX)), Vec3());
}

Mat44 const Mat44::CreateOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Mat44 ortho;
	ortho.m_values[Ix] = 2.f / (right - left);
	ortho.m_values[Jy] = 2.f / (top - bottom);
	ortho.m_values[Kz] = 1.f / (zFar - zNear);
	ortho.m_values[Tx] = (left + right) / (left - right);
	ortho.m_values[Ty] = (top + bottom) / (bottom - top);
	ortho.m_values[Tz] = zNear / (zNear - zFar);
	return ortho;
}

Mat44 const Mat44::CreatePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar)
{
	Mat44 perspective;
	perspective.m_values[Jy] = CosDegrees(fovYDegrees * 0.5f) / SinDegrees(fovYDegrees * 0.5f); // scaleY, equals 1 if vertical Field of View is 90
	perspective.m_values[Ix] = perspective.m_values[Jy] / aspect;//scaleX, equals scaleY is screen is square; equals 1 if square screen and FOV 90
	perspective.m_values[Kz] = zFar / (zFar - zNear);
	perspective.m_values[Kw] = 1.f; // This puts Z into the W component (in preparation for the hardware w-divide)
	perspective.m_values[Tz] = zNear * zFar / (zNear - zFar);
	perspective.m_values[Tw] = 0.f; // Otherwise we would be putting Z+1 (instead of Z) into the W component

	return perspective;
}

Vec2 const Mat44::TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const
{
	return Vec2(m_values[Ix] * vectorQuantityXY.x + m_values[Jx] * vectorQuantityXY.y,
		        m_values[Iy] * vectorQuantityXY.x + m_values[Jy] * vectorQuantityXY.y);
}

Vec3 const Mat44::TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const
{
	return Vec3(m_values[Ix] * vectorQuantityXYZ.x + m_values[Jx] * vectorQuantityXYZ.y + m_values[Kx] * vectorQuantityXYZ.z,
		        m_values[Iy] * vectorQuantityXYZ.x + m_values[Jy] * vectorQuantityXYZ.y + m_values[Ky] * vectorQuantityXYZ.z,
		        m_values[Iz] * vectorQuantityXYZ.x + m_values[Jz] * vectorQuantityXYZ.y + m_values[Kz] * vectorQuantityXYZ.z);
}

Vec2 const Mat44::TransformPosition2D(Vec2 const& positionXY) const
{
	return Vec2(m_values[Ix] * positionXY.x + m_values[Jx] * positionXY.y + m_values[Tx],
		        m_values[Iy] * positionXY.x + m_values[Jy] * positionXY.y + m_values[Ty]);
}	
Vec3 const Mat44::TransformPosition3D(Vec3 const& positionXYZ) const
{
	return Vec3(m_values[Ix] * positionXYZ.x + m_values[Jx] * positionXYZ.y + m_values[Kx] * positionXYZ.z + m_values[Tx],
			    m_values[Iy] * positionXYZ.x + m_values[Jy] * positionXYZ.y + m_values[Ky] * positionXYZ.z + m_values[Ty],
			    m_values[Iz] * positionXYZ.x + m_values[Jz] * positionXYZ.y + m_values[Kz] * positionXYZ.z + m_values[Tz]);
}

Vec4 const Mat44::TransformHomogeneous3D(Vec4 const& homogeneousPoint3D) const
{
	return Vec4(m_values[Ix] * homogeneousPoint3D.x + m_values[Jx] * homogeneousPoint3D.y + m_values[Kx] * homogeneousPoint3D.z + m_values[Tx] * homogeneousPoint3D.w,
		        m_values[Iy] * homogeneousPoint3D.x + m_values[Jy] * homogeneousPoint3D.y + m_values[Ky] * homogeneousPoint3D.z + m_values[Ty] * homogeneousPoint3D.w,
		        m_values[Iz] * homogeneousPoint3D.x + m_values[Jz] * homogeneousPoint3D.y + m_values[Kz] * homogeneousPoint3D.z + m_values[Tz] * homogeneousPoint3D.w,
		        m_values[Iw] * homogeneousPoint3D.x + m_values[Jw] * homogeneousPoint3D.y + m_values[Kw] * homogeneousPoint3D.z + m_values[Tw] * homogeneousPoint3D.w);
}

float* Mat44::GetAsFloatArray()
{
	return m_values;
}

float const* Mat44::GetAsFloatArray() const
{
	return m_values;
}

Vec2 const Mat44::GetIBasis2D() const
{
	return Vec2(m_values[Ix], m_values[Iy]);
}

Vec2 const Mat44::GetJBasis2D() const
{
	return Vec2(m_values[Jx],m_values[Jy]);
}

Vec2 const Mat44::GetTranslation2D() const
{
    return Vec2(m_values[Tx], m_values[Ty]);
}

Vec3 const Mat44::GetIBasis3D() const
{
	return Vec3(m_values[Ix], m_values[Iy], m_values[Iz]);
}

Vec3 const Mat44::GetJBasis3D() const
{
	return Vec3(m_values[Jx], m_values[Jy], m_values[Jz]);
}

Vec3 const Mat44::GetKBasis3D() const
{
	return Vec3(m_values[Kx], m_values[Ky], m_values[Kz]);
}

Vec3 const Mat44::GetTranslation3D() const
{
	return Vec3(m_values[Tx], m_values[Ty], m_values[Tz]);
}

Vec4 const Mat44::GetIBasis4D() const
{
	return Vec4(m_values[Ix], m_values[Iy], m_values[Iz],m_values[Iw]);
}

Vec4 const Mat44::GetJBasis4D() const
{
	return Vec4(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);
}

Vec4 const Mat44::GetKBasis4D() const
{
	return Vec4(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);
}

Vec4 const Mat44::GetTranslation4D() const
{
	return Vec4(m_values[Tx], m_values[Ty], m_values[Tz], m_values[Tw]);
}

Mat44 const Mat44::GetOrthonormalInverse() const
{
	Mat44 old(*this);
	Mat44 resultMatrix;
	resultMatrix.m_values[Ix] = old.m_values[Ix];
	resultMatrix.m_values[Iy] = old.m_values[Jx];
	resultMatrix.m_values[Iz] = old.m_values[Kx];
	resultMatrix.m_values[Iw] = 0.f;

	resultMatrix.m_values[Jx] = old.m_values[Iy];
	resultMatrix.m_values[Jy] = old.m_values[Jy];
	resultMatrix.m_values[Jz] = old.m_values[Ky];
	resultMatrix.m_values[Jw] = 0.f;

	resultMatrix.m_values[Kx] = old.m_values[Iz];
	resultMatrix.m_values[Ky] = old.m_values[Jz];
	resultMatrix.m_values[Kz] = old.m_values[Kz];
	resultMatrix.m_values[Kw] = 0.f;

	resultMatrix.m_values[Tx] = -old.m_values[Ix] * old.m_values[Tx] - old.m_values[Iy] * old.m_values[Ty] - old.m_values[Iz] * old.m_values[Tz] + old.m_values[Iw];
	resultMatrix.m_values[Ty] = -old.m_values[Jx] * old.m_values[Tx] - old.m_values[Jy] * old.m_values[Ty] - old.m_values[Jz] * old.m_values[Tz] + old.m_values[Jw];
	resultMatrix.m_values[Tz] = -old.m_values[Kx] * old.m_values[Tx] - old.m_values[Ky] * old.m_values[Ty] - old.m_values[Kz] * old.m_values[Tz] + old.m_values[Kw];
	resultMatrix.m_values[Tw] = 1.f;
	return resultMatrix;
}

void Mat44::SetTranslation2D(Vec2 const& translationXY)
{
	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

void Mat44::SetTranslation3D(Vec3 const& translationXYZ)
{
	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;
	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;
}

void Mat44::SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translationXY)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0.f;
	m_values[Iw] = 0.f;
	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0.f;
	m_values[Jw] = 0.f;
	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0.f;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;
	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;
	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;
}

void Mat44::SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0.f;
	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0.f;
	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0.f;
	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1.f;
}

void Mat44::SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Iy] = iBasis4D.y;
	m_values[Iz] = iBasis4D.z;
	m_values[Iw] = iBasis4D.w;
	m_values[Jx] = jBasis4D.x;
	m_values[Jy] = jBasis4D.y;
	m_values[Jz] = jBasis4D.z;
	m_values[Jw] = jBasis4D.w;
	m_values[Kx] = kBasis4D.x;
	m_values[Ky] = kBasis4D.y;
	m_values[Kz] = kBasis4D.z;
	m_values[Kw] = kBasis4D.w;
	m_values[Tx] = translation4D.x;
	m_values[Ty] = translation4D.y;
	m_values[Tz] = translation4D.z;
	m_values[Tw] = translation4D.w;
}

void Mat44::Append(Mat44 const& appendThis)
{
	Mat44 copyOfThis = *this; // Make a copy of my old values, so we don't pollute them as we go
	float const* left = &copyOfThis.m_values[0]; // column-notation nickname for the prior matrix, for brevity
	float const* right = &appendThis.m_values[0]; // column-notation nickname for the new matrix, for brevity

	m_values[Ix] = (left[Ix] * right[Ix]) + (left[Jx] * right[Iy]) + (left[Kx] * right[Iz]) + (left[Tx] * right[Iw]); //Dot(Lx, Ri)
	m_values[Iy] = (left[Iy] * right[Ix]) + (left[Jy] * right[Iy]) + (left[Ky] * right[Iz]) + (left[Ty] * right[Iw]); //Dot(Ly, Ri)
	m_values[Iz] = (left[Iz] * right[Ix]) + (left[Jz] * right[Iy]) + (left[Kz] * right[Iz]) + (left[Tz] * right[Iw]);
	m_values[Iw] = (left[Iw] * right[Ix]) + (left[Jw] * right[Iy]) + (left[Kw] * right[Iz]) + (left[Tw] * right[Iw]);

	m_values[Jx] = (left[Ix] * right[Jx]) + (left[Jx] * right[Jy]) + (left[Kx] * right[Jz]) + (left[Tx] * right[Jw]); //Dot(Lx, Rj)
	m_values[Jy] = (left[Iy] * right[Jx]) + (left[Jy] * right[Jy]) + (left[Ky] * right[Jz]) + (left[Ty] * right[Jw]);
	m_values[Jz] = (left[Iz] * right[Jx]) + (left[Jz] * right[Jy]) + (left[Kz] * right[Jz]) + (left[Tz] * right[Jw]);
	m_values[Jw] = (left[Iw] * right[Jx]) + (left[Jw] * right[Jy]) + (left[Kw] * right[Jz]) + (left[Tw] * right[Jw]);

	m_values[Kx] = (left[Ix] * right[Kx]) + (left[Jx] * right[Ky]) + (left[Kx] * right[Kz]) + (left[Tx] * right[Kw]);
	m_values[Ky] = (left[Iy] * right[Kx]) + (left[Jy] * right[Ky]) + (left[Ky] * right[Kz]) + (left[Ty] * right[Kw]);
	m_values[Kz] = (left[Iz] * right[Kx]) + (left[Jz] * right[Ky]) + (left[Kz] * right[Kz]) + (left[Tz] * right[Kw]);
	m_values[Kw] = (left[Iw] * right[Kx]) + (left[Jw] * right[Ky]) + (left[Kw] * right[Kz]) + (left[Tw] * right[Kw]);

	m_values[Tx] = (left[Ix] * right[Tx]) + (left[Jx] * right[Ty]) + (left[Kx] * right[Tz]) + (left[Tx] * right[Tw]); //Dot(Lw, Rt)
	m_values[Ty] = (left[Iy] * right[Tx]) + (left[Jy] * right[Ty]) + (left[Ky] * right[Tz]) + (left[Ty] * right[Tw]);
	m_values[Tz] = (left[Iz] * right[Tx]) + (left[Jz] * right[Ty]) + (left[Kz] * right[Tz]) + (left[Tz] * right[Tw]);
	m_values[Tw] = (left[Iw] * right[Tx]) + (left[Jw] * right[Ty]) + (left[Kw] * right[Tz]) + (left[Tw] * right[Tw]);
}

void Mat44::AppendZRotation(float degreesRotationAboutZ)
{
	Mat44 appendMatrix = appendMatrix.CreateZRotationDegrees(degreesRotationAboutZ);
	this->Append(appendMatrix);
}

void Mat44::AppendYRotation(float degreesRotationAboutY)
{
	Mat44 appendMatrix = appendMatrix.CreateYRotationDegrees(degreesRotationAboutY);
	this->Append(appendMatrix);
}

void Mat44::AppendXRotation(float degreesRotationAboutX)
{
	Mat44 appendMatrix = appendMatrix.CreateXRotationDegrees(degreesRotationAboutX);
	this->Append(appendMatrix);
}

void Mat44::AppendTranslation2D(Vec2 const& translationXY)
{
	Mat44 appendMatrix = appendMatrix.CreateTranslation2D(translationXY);
	this->Append(appendMatrix);
}

void Mat44::AppendTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 appendMatrix = appendMatrix.CreateTranslation3D(translationXYZ);
	this->Append(appendMatrix);
}

void Mat44::AppendScaleUniform2D(float uniformScaleXY)
{
	Mat44 appendMatrix = appendMatrix.CreateUniformScale2D(uniformScaleXY);
	this->Append(appendMatrix);
}

void Mat44::AppendScaleUniform3D(float uniformScaleXYZ)
{
	Mat44 appendMatrix = appendMatrix.CreateUniformScale3D(uniformScaleXYZ);
	this->Append(appendMatrix);
}

void Mat44::AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY)
{
	Mat44 appendMatrix = appendMatrix.CreateNonUniformScale2D(nonUniformScaleXY);
	this->Append(appendMatrix);
}

void Mat44::AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 appendMatrix = appendMatrix.CreateNonUniformScale3D(nonUniformScaleXYZ);
	this->Append(appendMatrix);
}

void Mat44::Transpose()
{
	Mat44 old(*this);
	m_values[Ix] = old.m_values[Ix];
	m_values[Iy] = old.m_values[Jx];
	m_values[Iz] = old.m_values[Kx];
	m_values[Iw] = old.m_values[Tx];

	m_values[Jx] = old.m_values[Iy];
	m_values[Jy] = old.m_values[Jy];
	m_values[Jz] = old.m_values[Ky];
	m_values[Jw] = old.m_values[Ty];

	m_values[Kx] = old.m_values[Iz];
	m_values[Ky] = old.m_values[Jz];
	m_values[Kz] = old.m_values[Kz];
	m_values[Kw] = old.m_values[Tz];

	m_values[Tx] = old.m_values[Iw];
	m_values[Ty] = old.m_values[Jw];
	m_values[Tz] = old.m_values[Kw];
	m_values[Tw] = old.m_values[Tw];
}

void Mat44::Orthonormalize_IFwd_JLeft_KUp()
{
	Vec3 iBasis = this->GetIBasis3D();
	Vec3 jBasis = this->GetJBasis3D();
	Vec3 kBasis = this->GetKBasis3D();

	Vec3 badKOnI = GetProjectedOnto3D(kBasis, iBasis);
	kBasis -= badKOnI;

    Vec3 badJOnI = GetProjectedOnto3D(jBasis, iBasis);	
	Vec3 badJOnK = GetProjectedOnto3D(jBasis, kBasis);
	jBasis -= badJOnI;
	jBasis -= badJOnK;

	iBasis = iBasis.GetNormalized();
	jBasis = jBasis.GetNormalized();
	kBasis = kBasis.GetNormalized();
	this->SetIJK3D(iBasis,jBasis,kBasis);
}

