#include "Game/SliderBar.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Math/MathUtils.hpp"

SliderBar::~SliderBar()
{
}

SliderBar::SliderBar(const Vector2 & offset, const AABB2 & size, const Rgba & backColor, const Rgba & fillColor)
	: m_offset(offset),
	m_size(size),
	m_backColor(backColor),
	m_fillColor(fillColor)
{
	m_value = 1.f;
}

void SliderBar::Render(const Vector2& center, const Vector2& anchor) const
{
	g_theRenderer->PushMatrix();
	g_theRenderer->TranslateInMatrix(center + m_offset - Vector2(m_size.GetDimensions().x * anchor.x, m_size.GetDimensions().y * anchor.y));
	g_theRenderer->DrawAABB(m_size, m_backColor);
	g_theRenderer->DrawAABB(AABB2(m_size.mins.x, m_size.mins.y, RangeMapFloat(m_value, 0.f, 1.f, m_size.mins.x, m_size.maxs.x), m_size.maxs.y), m_fillColor);
	g_theRenderer->PopMatrix();
}
