#pragma once

class Vector2;
class AABB2;
class Disc2;

class MathUtils
{
public:
	static const float PI;
};

void InitRandomSeed();
int GetRandomIntLessThan(int maxExclusive);
int GetRandomIntInRange(int minInclusive, int maxInclusive);
float GetRandomFloatZeroToOne();
float GetRandomFloatInRange(float min, float max);
bool CheckRandomChance(float chanceForSuccess);

float GetDistance( const Vector2& a, const Vector2& b );
float GetDistanceSquared( const Vector2& a, const Vector2& b );
float ConvertRadiansToDegrees( float radians );
float ConvertDegreesToRadians( float degrees );
float CosDegrees( float degrees );
float SinDegrees( float degrees );
float Atan2Degress(float y, float x);
float Acos2Degree( float value);
bool DoAABBsOverlap( const AABB2& a, const AABB2& b );
bool DoDiscsOverlap( const Disc2& a, const Disc2& b );
bool DoDiscsOverlap( const Vector2& aCenter, const float aRadius, const Vector2& bCenter, const float bRadius );
bool DoDiscToPointOverlap(const Vector2& center, const float radius, const Vector2& point);
Vector2 GetDiscPushOutOfPointFactor(const Vector2 & center, float radius, const Vector2 & point);
float GetDiscOverlapMagnitude( const Vector2& aCenter, const float aRadius, const Vector2& bCenter, const float bRadius );

float MinFloat(float inValue, float minInclusive);
float MaxFloat(float inValue, float maxInclusive);
float RangeMapFloat(float inValue, float inStart, float inEnd, float outStart, float outEnd);

int RoundToNearestInt(float inValue); // 0.5 rounds up to 1; -0.5 rounds up to 0
int ClampInt(int inValue, int min, int max);
float ClampFloat(float inValue, float minInclusive, float maxInclusive);
float ClampFloatZeroToOne(float inValue);
float ClampFloatNegativeOneToOne(float inValue);

// Finds the % (as a fraction) of inValue in [rangeStart,rangeEnd].
// For example, 3 is 25% (0.25) of the way through the range [2,6].
float GetFractionInRange(float inValue, float rangeStart, float rangeEnd);

// Finds the value at a certain % (fraction) in [rangeStart,rangeEnd].
// For example, at 75% (0.75) of the way through [2,6] is 5.
float Interpolate(float start, float end, float fractionTowardEnd);
int Interpolate( int start, int end, float fractionTowardEnd );
unsigned char Interpolate( unsigned char start, unsigned char end, float fractionTowardEnd );

// For a value in [inStart,inEnd], finds the corresponding value in [outStart,outEnd].
// Note: must correctly interpolate and extrapolate, and handle all special cases.
float RangeMapFloat(float inValue, float inStart, float inEnd, float outStart, float outEnd);

// Finds the �angular displacement� (or signed angular distance) from startDegrees to endDegrees.
// This function always takes the smartest/shortest way around, so going from 5 to 355 yields -10.
float GetAngularDisplacement(float startDegrees, float endDegrees);

// I face currentDegrees and want to turn �toward� goalDegrees, by up to maxTurnDegrees.
// Note: must always take the shortest/smartest way around, and will never overshoot.
float TurnToward(float currentDegrees, float goalDegrees, float maxTurnDegrees);

float DotProduct(const Vector2& a, const Vector2& b);

bool AreBitsSet(unsigned char bitFlags8, unsigned char flagsToCheck);
bool AreBitsSet(unsigned int bitFlags32, unsigned int flagsToCheck);
void SetBits(unsigned char& bitFlags8, unsigned char flagsToSet);
void SetBits(unsigned int& bitFlags32, unsigned int flagsToSet);
void ClearBits(unsigned char& bitFlags8, unsigned char flagToClear);
void ClearBits(unsigned int& bitFlags32, unsigned int flagToClear);

float	SmoothStart2( float t ); // 2nd-degree smooth start (a.k.a. �quadratic ease in�)
float	SmoothStart3( float t ); // 3rd-degree smooth start (a.k.a. �cubic ease in�)
float	SmoothStart4( float t ); // 4th-degree smooth start (a.k.a. �quartic ease in�)
float	SmoothStop2( float t ); // 2nd-degree smooth start (a.k.a. �quadratic ease out�)
float	SmoothStop3( float t ); // 3rd-degree smooth start (a.k.a. �cubic ease out�)
float	SmoothStop4( float t ); // 4th-degree smooth start (a.k.a. �quartic ease out�)
float	SmoothStep3( float t ); // 3rd-degree smooth start/stop (a.k.a. �smoothstep�)
