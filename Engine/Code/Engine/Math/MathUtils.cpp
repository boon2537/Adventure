#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Disc2.hpp"
#include <math.h> 
#include <stdlib.h>
#include <time.h> 

const float MathUtils::PI = 3.14159265359f;

void InitRandomSeed()
{
	unsigned int temp = (unsigned int) time(NULL);
	srand(temp);
}

int GetRandomIntLessThan(int maxExclusive)
{
	return rand() % maxExclusive;
}

int GetRandomIntInRange(int minInclusive, int maxInclusive)
{
	int range = (maxInclusive - minInclusive) + 1;
	return minInclusive + GetRandomIntLessThan(range);
}

float GetRandomFloatZeroToOne()
{
	return (float) rand() / (float) RAND_MAX;
}

float GetRandomFloatInRange(float min, float max)
{
	return min + ((float) rand() / ((float) RAND_MAX / (max - min)));
}

bool CheckRandomChance(float chanceForSuccess)
{
	float clampedChance = ClampFloatZeroToOne(chanceForSuccess);
	if (clampedChance == 1)
		return true;

	return GetRandomFloatInRange(0, 1) < clampedChance;
}

float GetDistance(const Vector2 & a, const Vector2 & b)
{
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	return sqrtf((dx * dx) + (dy * dy));
}

float GetDistanceSquared(const Vector2 & a, const Vector2 & b)
{
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	return (dx * dx) + (dy * dy);
}

float ConvertRadiansToDegrees(float radians)
{
	return radians * (180.f / MathUtils::PI);
}

float ConvertDegreesToRadians(float degrees)
{
	return degrees * (MathUtils::PI / 180.f);
}

float CosDegrees(float degrees)
{
	float radians = ConvertDegreesToRadians(degrees);
	return cosf(radians);
}

float SinDegrees(float degrees)
{
	float radians = ConvertDegreesToRadians(degrees);
	return sinf(radians);
}

float Atan2Degress(float y, float x)
{
	return ConvertRadiansToDegrees(atan2f(y, x));
}

float Acos2Degree(float value)
{
	return ConvertRadiansToDegrees(acosf(value));
}

bool DoAABBsOverlap(const AABB2 & a, const AABB2 & b)
{
	return a.IsPointInside(b.maxs) || a.IsPointInside(b.mins) || b.IsPointInside(a.maxs) || b.IsPointInside(a.mins);
}

bool DoDiscsOverlap(const Disc2 & a, const Disc2 & b)
{
	return (a.radius + b.radius) > GetDistance(a.center, b.center);
}

bool DoDiscsOverlap(const Vector2 & aCenter, float aRadius, const Vector2 & bCenter, float bRadius)
{
	return (aRadius + bRadius) > GetDistance(aCenter, bCenter);
}

bool DoDiscToPointOverlap(const Vector2 & center, float radius, const Vector2 & point)
{
	return radius > GetDistance(center, point);
}

Vector2 GetDiscPushOutOfPointFactor(const Vector2 & center, float radius, const Vector2 & point)
{
	float overLap = radius - GetDistance(point, center);
	float angle = Atan2Degress(center.y - point.y, center.x - point.x);
	return Vector2(CosDegrees(angle) * overLap, SinDegrees(angle) * overLap);
}

float GetDiscOverlapMagnitude(const Vector2 & aCenter, const float aRadius, const Vector2 & bCenter, const float bRadius)
{
	if (DoDiscsOverlap(aCenter, aRadius, bCenter, bRadius))
		return (aRadius + bRadius) - GetDistance(aCenter, bCenter);

	return 0.0f;
}

float ClampFloat(float inValue, float minInclusive, float maxInclusive)
{
	if (inValue > maxInclusive)
		return maxInclusive;
	if (inValue < minInclusive)
		return minInclusive;
	else return inValue;
}

float ClampFloatZeroToOne(float value)
{
	if (value < 0)
		return 0;
	if (value > 1)
		return 1;
	else
		return value;
}

float ClampFloatNegativeOneToOne(float inValue)
{
	if (inValue < -1)
		return -1;
	if (inValue > 1)
		return 1;
	else
		return inValue;
}

float GetFractionInRange(float inValue, float rangeStart, float rangeEnd)
{
	if (rangeEnd > rangeStart)
		return (inValue - rangeStart) / (rangeEnd - rangeStart);
	else
		return (rangeStart - inValue) / (rangeStart - rangeEnd);
}

float Interpolate(float start, float end, float fractionTowardEnd)
{
	return ((end - start) * fractionTowardEnd) + start;
}

int Interpolate(int start, int end, float fractionTowardEnd)
{
	return RoundToNearestInt(((end - start) * fractionTowardEnd) + start);
}

unsigned char Interpolate(unsigned char start, unsigned char end, float fractionTowardEnd)
{
	char output = static_cast<char>(RoundToNearestInt(((end - start) * fractionTowardEnd) + start));
	return output;
}

float MinFloat(float inValue, float minInclusive)
{
	if (inValue < minInclusive)
		return minInclusive;
	else return inValue;
}

float MaxFloat(float inValue, float maxInclusive)
{
	if (inValue > maxInclusive)
		return maxInclusive;
	else return inValue;
}

float RangeMapFloat(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	if (inStart == inEnd)
	{
		return (outStart + outEnd) * 0.5f;
	}

	float inRange = inEnd - inStart;
	float outRange = outEnd - outStart;
	float inFromStart = inValue - inStart;
	float fractionIntoRange = inFromStart / inRange;
	float outRelativeToStart = fractionIntoRange * outRange;
	return outRelativeToStart + outStart;
}

int RoundToNearestInt(float inValue)
{
	if (inValue > 0)
		return static_cast<int>(roundf(inValue));
	else
	{
		// -x.5 case
		if ((roundf(inValue) != inValue) && (roundf(inValue * 2) == (inValue * 2)))
			return static_cast<int>(roundf(inValue)) + 1;
		else
			return static_cast<int>(roundf(inValue));
	}
}

int ClampInt(int inValue, int min, int max)
{
	if (inValue > max)
		return max;
	if (inValue < min)
		return min;
	else return inValue;
}

float GetAngularDisplacement(float startDegrees, float endDegrees)
{
	float angularDisp = endDegrees - startDegrees;
	while (angularDisp > 180)
		angularDisp -= 360;
	while (angularDisp < -180)
		angularDisp += 360;

	return angularDisp;
}

float TurnToward(float currentDegrees, float goalDegrees, float maxTurnDegrees)
{
	if (currentDegrees == goalDegrees)
		return currentDegrees;

	float disp = GetAngularDisplacement(currentDegrees, goalDegrees);

	if (fabsf(disp) < maxTurnDegrees)
		return goalDegrees;

	if (disp > 0)
		return currentDegrees + maxTurnDegrees;
	else if (disp < 0)
		return currentDegrees - maxTurnDegrees;

	return goalDegrees;
}

float DotProduct(const Vector2 & a, const Vector2 & b)
{
	return (a.x * b.x) + (a.y * b.y);
}

bool AreBitsSet(unsigned char bitFlags8, unsigned char flagsToCheck)
{
	return (bitFlags8 & flagsToCheck) == flagsToCheck;
}

bool AreBitsSet(unsigned int bitFlags32, unsigned int flagsToCheck)
{
	return (bitFlags32 & flagsToCheck) == flagsToCheck;
}

void SetBits(unsigned char & bitFlags8, unsigned char flagsToSet)
{
	bitFlags8 |= flagsToSet;
}

void SetBits(unsigned int & bitFlags32, unsigned int flagsToSet)
{
	bitFlags32 |= flagsToSet;
}

void ClearBits(unsigned char & bitFlags8, unsigned char flagToClear)
{
	bitFlags8 &= ~flagToClear;
}

void ClearBits(unsigned int & bitFlags32, unsigned int flagToClear)
{
	bitFlags32 &= ~flagToClear;
}

float SmoothStart2(float t)
{
	return t * t;
}

float SmoothStart3(float t)
{
	return t * t * t;
}

float SmoothStart4(float t)
{
	return t * t * t * t;
}

float SmoothStop2(float t)
{
	return 1 - ((1 -t) * (1 - t));
}

float SmoothStop3(float t)
{
	return 1 - ((1 -t) * (1 - t) * (1 - t));
}

float SmoothStop4(float t)
{
	return 1 - ((1 -t) * (1 - t) * (1 - t) * (1 - t));
}

float SmoothStep3(float t)
{
	return ((1 - t) * SmoothStart3(t)) + (t * SmoothStop3(t));
}
