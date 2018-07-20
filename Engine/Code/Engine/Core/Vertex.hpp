#pragma once

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Core/Rgba.hpp"

struct Vertex3D_PCU
{
public:
	Vertex3D_PCU() {}
	explicit Vertex3D_PCU(Vector3 initialPosition, Rgba initialColor = Rgba::white, Vector2 initialUVs = Vector2(0, 0))
		: m_position(initialPosition)
		, m_color(initialColor)
		, m_UVs(initialUVs) {}
	explicit Vertex3D_PCU(Vector2 initialPosition, Rgba initialColor = Rgba::white, Vector2 initialUVs = Vector2(0, 0))
		: m_position(Vector3(initialPosition.x, initialPosition.y, 0))
		, m_color(initialColor)
		, m_UVs(initialUVs) {}

public:
	Vector3 m_position;
	Rgba m_color;
	Vector2 m_UVs;
};
