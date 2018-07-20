#include "Game/Camera.hpp"
#include "Game/GameCommon.hpp"

Camera::Camera(float size)
	: m_numTilesInViewVertically(size)
{
	m_aspectRatio = 16.f / 9;
	MoveCameraTo(Vector2(0, 0));
}

void Camera::MoveCameraTo(const Vector2 & pos)
{
	m_worldPos = pos;
	m_bounds.mins = Vector2(pos.x - (m_numTilesInViewVertically * m_aspectRatio * 0.5f), pos.y - (m_numTilesInViewVertically * 0.5f));
	m_bounds.maxs = Vector2(pos.x + (m_numTilesInViewVertically * m_aspectRatio * 0.5f), pos.y + (m_numTilesInViewVertically * 0.5f));	
}

void Camera::Update()
{
	g_theRenderer->SetOrtho(m_bounds.mins, m_bounds.maxs);
}

bool Camera::IsBoundsInView(const AABB2 & bounds)
{
	if (bounds.mins.x > m_bounds.maxs.x || bounds.maxs.x < m_bounds.mins.x || bounds.mins.y > m_bounds.maxs.y || bounds.maxs.y < m_bounds.mins.y)
		return false;

	return true;
}
