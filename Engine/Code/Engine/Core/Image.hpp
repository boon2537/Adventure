#pragma once

#include "Rgba.hpp"
#include <string>
#include <vector>
#include "Engine/Math/IntVector2.hpp"

class Image
{
public:
	explicit Image( const std::string& imageFilePath );
	Rgba	GetTexel( int x, int y ) const; 			// (0,0) is top-left
	void	SetTexel( int x, int y, const Rgba& color );
	IntVector2 GetDimensions();

private:
	IntVector2		m_dimensions;
	std::vector<Rgba>	m_texels; // ordered left-to-right, then down, from [0]=(0,0) at top-left
};
