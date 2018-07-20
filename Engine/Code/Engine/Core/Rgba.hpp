#pragma once

class Rgba
{
public:
	Rgba::Rgba(); // Default-constructs to opaque white (255,255,255,255)
	explicit Rgba::Rgba( unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte=255 );

	void Rgba::SetAsBytes( unsigned char redByte, unsigned char greenByte, unsigned char blueByte, unsigned char alphaByte=255 );
	void Rgba::SetAsFloats( float normalizedRed, float normalizedGreen, float normalizedBlue, float normalizedAlpha=1.0f );
	void Rgba::GetAsFloats( float& out_normalizedRed, float& out_normalizedGreen, float& out_normalizedBlue, float& out_normalizedAlpha ) const;
	void Rgba::ScaleRGB( float rgbScale ); // Scales (and clamps) RGB components, but not A
	void Rgba::ScaleAlpha( float alphaScale ); // Scales (and clamps) Alpha, RGB is untouched
	void Rgba::SetFromText( const char* text );

public:
	unsigned char r; // Red byte, 0-255
	unsigned char g; // Green byte, 0-255
	unsigned char b; // Blue byte, 0-255
	unsigned char a; // Alpha (opacity) byte, 0-255; 0 is transparent, 255 is opaque

	static const Rgba white;
	static const Rgba black;
	static const Rgba red;
	static const Rgba blue;
	static const Rgba green;
	static const Rgba cyan;
	static const Rgba magenta;
	static const Rgba yellow;
};

const Rgba Interpolate( const Rgba& start, const Rgba& end, float fractionTowardEnd );
