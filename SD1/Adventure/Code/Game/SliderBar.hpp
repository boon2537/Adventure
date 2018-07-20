#pragma once

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba.hpp"

class SliderBar
{
public:
	~SliderBar();
	SliderBar() {}
	SliderBar(const Vector2& offset, const AABB2& size, const Rgba& backColor, const Rgba& fillColor);

	void Render(const Vector2& center, const Vector2& anchor) const;

public:
	float m_value;
	Vector2 m_offset;
	AABB2 m_size;
	Rgba m_backColor;
	Rgba m_fillColor;
};
