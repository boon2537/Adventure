#pragma once

#include "Engine/Math/Vector2.hpp"

class Disc2
{
public:
	~Disc2() {}
	Disc2() {}
	Disc2( const Disc2& copyFrom );
	explicit Disc2( float initialX, float initialY, float initialRadius );
	explicit Disc2( const Vector2& initialCenter, float initialRadius );

	void StretchToIncludePoint( float x, float y ); // expand radius if (x,y) is outside
	void StretchToIncludePoint( const Vector2& point ); // expand radius if point outside
	void AddPaddingToRadius( float paddingRadius );
	void Translate( const Vector2& translation ); // move the center
	void Translate( float translationX, float translationY ); // move the center

	bool IsPointInside( float x, float y ) const; // is (x,y) within disc�s interior?
	bool IsPointInside( const Vector2& point ) const; // is �point� within disc�s interior?

	void operator+=( const Vector2& translation ); // move
	void operator-=( const Vector2& antiTranslation );
	Disc2 operator+( const Vector2& translation ) const; // create a moved copy
	Disc2 operator-( const Vector2& antiTranslation ) const;

public:
	Vector2 center; // like Vector2, this breaks the �no public members� and �m_� naming rules
	float	radius;
};

const Disc2 Interpolate( const Disc2& start, const Disc2& end, float fractionTowardEnd );