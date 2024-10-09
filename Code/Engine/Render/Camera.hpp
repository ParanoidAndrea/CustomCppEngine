#pragma once
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/EulerAngles.hpp"
#pragma  warning(disable : 26812) // prefer enum class to enum (yes, but not ALWAYS)


class Camera {
public:
	enum Mode
	{
		eMode_Orthographic,
		eMode_Perspective,
		eMode_Count
	};
public:
	Camera() {};
	explicit Camera(Vec2 const& bottomLeft, Vec2 const& topRight);

	void SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight, float near = 0.f, float far = 1.f);
	void SetPerspectiveView(float aspect, float fov, float near, float far);
	void SetCameraCenter(Vec2 const& center);
	void SetNormalizedViewport(AABB2 const& normalizedViewport);
	void SetFOV(float fov);
	AABB2 GetDXNormalizedViewport() const;
	Vec2 GetCameraCenter() const;
	Vec2 GetOrthoBottomLeft() const;
	Vec2 GetOrthoTopRight() const;
	AABB2 GetCameraBounds() const;
	void Translate2D(Vec2 const& translation);
	void SetTransform(Vec3 const& position, EulerAngles const& orientation);
	Mat44 GetViewMatrix() const;

	Mat44 GetOrthographicMatrix() const;
	Mat44 GetPerspectiveMatrix() const;
	Mat44 GetProjectionMatrix() const;

	void SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis);
	Mat44 GetRenderMatrix() const;
	EulerAngles GetCameraOrientation() const;
	Vec3 GetCameraPosition() const;
private:
	Mode m_mode = eMode_Orthographic;
	Vec2 m_orthographicBottomLeft;
	Vec2 m_orthographicTopRight;
	float m_orthographicNear = 0.f;
	float m_orthographicFar = 1.f;

	float m_perspectiveAspect = 1.f;
	float m_perspectiveFOV = 90.f;
	float m_perspectiveNear = 0.f;
	float m_perspectiveFar = 1.f;
	EulerAngles m_orientation;
	Vec3 m_position;
	AABB2 m_normalizedViewport = AABB2::ZERO_TO_ONE;
	AABB2 m_viewportBox;
	Vec3 m_renderIBasis = Vec3(1.f, 0.f, 0.f);
	Vec3 m_renderJBasis = Vec3(0.f, 1.f, 0.f);
	Vec3 m_renderKBasis = Vec3(0.f, 0.f, 1.f);
};