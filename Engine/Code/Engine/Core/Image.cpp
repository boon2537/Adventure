#include "Image.hpp"

#include "Engine/ThirdParty/stb_image.h"

Image::Image(const std::string & imageFilePath)
{
	int numComponents = 0; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
	int numComponentsRequested = 0; // don't care; we support 3 (RGB) or 4 (RGBA)
	unsigned char* imageData = stbi_load(imageFilePath.c_str(), &m_dimensions.x, &m_dimensions.y, &numComponents, numComponentsRequested);
	unsigned char* beginData = imageData;

	for (int y = 0; y < m_dimensions.y; y++)
	{
		for (int x = 0; x < m_dimensions.x; x++)
		{
			m_texels.push_back(Rgba::magenta);

			unsigned char r = *imageData;
			imageData++;
			unsigned char g = *imageData;
			imageData++;
			unsigned char b = *imageData;
			imageData++;
			unsigned char a = 255;
			if (numComponents == 4)
			{
				a = *imageData;
				imageData++;
			}
			
			SetTexel(x, y, Rgba(r, g, b, a));
		}
	}

	stbi_image_free(beginData);
}

Rgba Image::GetTexel(int x, int y) const
{
	int index = x + (m_dimensions.x * y);
	return m_texels[index];
}

void Image::SetTexel(int x, int y, const Rgba & color)
{
	int index = x + (m_dimensions.x * y);
	m_texels[index] = color;
}

IntVector2 Image::GetDimensions()
{
	return m_dimensions;
}
