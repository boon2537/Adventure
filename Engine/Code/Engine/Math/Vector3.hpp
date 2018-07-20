#pragma once

//TODO: Just a data class for now, more funtions to be added
class Vector3
{
public:
	// Construction/Destruction
	~Vector3() {}											// destructor: do nothing (for speed)
	Vector3() {}											// default constructor: do nothing (for speed)
	Vector3( const Vector3& copyFrom );						// copy constructor (from another vec3)
	explicit Vector3( float initialX, float initialY, float initialZ );		// explicit constructor (from x, y, z)

															// Operators
	const Vector3 operator+( const Vector3& vecToAdd ) const;		// vec3 + vec3
	const Vector3 operator-( const Vector3& vecToSubtract ) const;	// vec3 - vec3
	const Vector3 operator*( float uniformScale ) const;			// vec3 * float
	const Vector3 operator/( float inverseScale ) const;			// vec3 / float
	void operator+=( const Vector3& vecToAdd );						// vec3 += vec3
	void operator-=( const Vector3& vecToSubtract );				// vec3 -= vec3
	void operator*=( const float uniformScale );					// vec3 *= float
	void operator/=( const float uniformDivisor );					// vec3 /= float
	void operator=( const Vector3& copyFrom );						// vec3 = vec3
	bool operator==( const Vector3& compare ) const;				// vec3 == vec3
	bool operator!=( const Vector3& compare ) const;				// vec3 != vec3

public:
	float x;
	float y;
	float z;
};