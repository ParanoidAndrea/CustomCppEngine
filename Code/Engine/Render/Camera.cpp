#include "Camera.hpp"

#include "Engine/Core/ErrorWarningAssert.hpp"

Camera::Camera(Vec2 const& bottomLeft, Vec2 const& topRight)
	:m_orthographicBottomLeft(bottomLeft),
	m_orthographicTopRight(topRight)
{
}

void Camera::SetOrthoView(Vec2 const& bottomLeft, Vec2 const& topRight, float near, float far)
{
	m_mode = eMode_Orthographic;
	m_orthographicBottomLeft.x = bottomLeft.x;
	m_orthographicBottomLeft.y = bottomLeft.y;
	m_orthographicTopRight.x   = topRight.x;
	m_orthographicTopRight.y   = topRight.y;
	m_orthographicNear = near;
	m_orthographicFar  = far;
}

void Camera::SetPerspectiveView(float aspect, float fov, float near, float far)
{
	m_mode = eMode_Perspective;
	m_perspectiveAspect = aspect;
	m_perspectiveFOV = fov;
	m_perspectiveNear = near;
	m_perspectiveFar = far;

}

void Camera::SetCameraCenter(Vec2 const& center)
{
	float cameraWidth = m_orthographicTopRight.x - m_orthographicBottomLeft.x;
	float cameraHeight = m_orthographicTopRight.y - m_orthographicBottomLeft.y;
	m_orthographicTopRight.x = center.x + cameraWidth / 2;
	m_orthographicTopRight.y = center.y + cameraHeight / 2;
	m_orthographicBottomLeft.x = center.x - cameraWidth / 2;
	m_orthographicBottomLeft.y = center.y - cameraHeight / 2;
} 



void Camera::SetNormalizedViewport(AABB2 const& normalizedViewport)
{
	m_normalizedViewport = normalizedViewport;
}

void Camera::SetFOV(float fov)
{
	m_perspectiveFOV = fov;
}

AABB2 Camera::GetDXNormalizedViewport() const
{
// 	AABB2 screenBox;
// 	Vec2 dimension = Vec2(m_orthographicTopRight.x - m_orthographicBottomLeft.x, m_orthographicTopRight.y - m_orthographicBottomLeft.y);
// 	screenBox.m_mins = Vec2(m_normalizedViewport.m_mins.x * dimension.x, m_normalizedViewport.m_mins.y * dimension.y);
// 	screenBox.m_maxs = Vec2(m_normalizedViewport.m_maxs.x * dimension.x, m_normalizedViewport.m_maxs.y * dimension.y);

	AABB2 viewport;
	viewport.m_mins.x = m_normalizedViewport.m_mins.x;
	viewport.m_mins.y = 1.f - m_normalizedViewport.m_mins.y;
	viewport.m_maxs.x = m_normalizedViewport.m_maxs.x;
	viewport.m_maxs.y = 1.f - m_normalizedViewport.m_maxs.y;
	float t = viewport.m_mins.y;
	viewport.m_mins.y = viewport.m_maxs.y;
	viewport.m_maxs.y = t;
	return viewport;
}

Vec2 Camera::GetCameraCenter() const
{
	return Vec2((m_orthographicTopRight.x + m_orthographicBottomLeft.x) * 0.5f, (m_orthographicTopRight.y + m_orthographicBottomLeft.y) * 0.5f);
}

Vec2 Camera::GetOrthoBottomLeft() const
{
	return m_orthographicBottomLeft;
}

Vec2 Camera::GetOrthoTopRight() const
{
	return m_orthographicTopRight;
}

AABB2 Camera::GetCameraBounds() const
{
	return AABB2(m_orthographicBottomLeft, m_orthographicTopRight);
}

void Camera::Translate2D(Vec2 const& translation)
{
	m_orthographicBottomLeft += translation;
	m_orthographicTopRight += translation;
}

void Camera::SetTransform(Vec3 const& position, EulerAngles const& orientation)
{
	m_position = position;
	m_orientation = orientation;
}

Mat44 Camera::GetViewMatrix() const
{
	Mat44 viewMatrix = m_orientation.GetAsMatrix_IFwd_JLeft_KUp();
	viewMatrix.SetTranslation3D(m_position);
	viewMatrix = viewMatrix.GetOrthonormalInverse();
	return viewMatrix;
}

Mat44 Camera::GetOrthographicMatrix() const
{
	Mat44 ortho = Mat44::CreateOrthoProjection(m_orthographicBottomLeft.x, m_orthographicTopRight.x, m_orthographicBottomLeft.y, m_orthographicTopRight.y, m_orthographicNear, m_orthographicFar);
	return ortho;
}

Mat44 Camera::GetPerspectiveMatrix() const
{
	Mat44 perspective= Mat44::CreatePerspectiveProjection(m_perspectiveFOV, m_perspectiveAspect, m_perspectiveNear, m_perspectiveFar);
	return perspective;
}

Mat44 Camera::GetProjectionMatrix() const
{
	Mat44 projectionMatrix;
	if (m_mode == eMode_Orthographic)
	{
		projectionMatrix = GetOrthographicMatrix();
		projectionMatrix.Append(GetRenderMatrix());
		return projectionMatrix;
	}
	else if (m_mode == eMode_Perspective)
	{
		projectionMatrix = GetPerspectiveMatrix();
		projectionMatrix.Append(GetRenderMatrix());
		return projectionMatrix;
	}
	else
	{
		ERROR_AND_DIE("The projection mode of the camera is wrong.");
	}
}

void Camera::SetRenderBasis(Vec3 const& iBasis, Vec3 const& jBasis, Vec3 const& kBasis)
{
	m_renderIBasis = iBasis;
	m_renderJBasis = jBasis;
	m_renderKBasis = kBasis;
}

Mat44 Camera::GetRenderMatrix() const
{
	Mat44 render;
	render.SetIJK3D(m_renderIBasis, m_renderJBasis, m_renderKBasis);
	return render;
}

EulerAngles Camera::GetCameraOrientation() const
{
	return m_orientation;
}

Vec3 Camera::GetCameraPosition() const
{
	return m_position;
}
