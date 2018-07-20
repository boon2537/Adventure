#pragma once

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/AABB2.hpp"

class Camera
{
public:
	~Camera() {}
	Camera(float size);

	void MoveCameraTo(const Vector2 &pos);
	void Update();
	bool IsBoundsInView(const AABB2& bounds);

public:
	Vector2 m_worldPos;
	AABB2 m_bounds;
	float m_orientationDegrees;
	float m_numTilesInViewVertically;
	float m_aspectRatio;
};