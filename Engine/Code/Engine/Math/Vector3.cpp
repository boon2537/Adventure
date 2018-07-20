#include "Engine/Math/Vector3.hpp"

Vector3::Vector3(const Vector3 & copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

Vector3::Vector3(float initialX, float initialY, float initialZ)
{
	x = initialX;
	y = initialY;
	z = initialZ;
}

const Vector3 Vector3::operator+(const Vector3 & vecToAdd) const
{
	return Vector3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}

const Vector3 Vector3::operator-(const Vector3 & vecToSubtract) const
{
	return Vector3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}

const Vector3 Vector3::operator*(float uniformScale) const
{
	return Vector3(x * uniformScale, y * uniformScale, z * uniformScale);
}

const Vector3 Vector3::operator/(float inverseScale) const
{
	return Vector3(x / inverseScale, y / inverseScale, z / inverseScale);
}

void Vector3::operator+=(const Vector3 & vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}

void Vector3::operator-=(const Vector3 & vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}

void Vector3::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}

void Vector3::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
}

void Vector3::operator=(const Vector3 & copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

bool Vector3::operator==(const Vector3 & compare) const
{
	return x == compare.x && y == compare.y && z == compare.z;
}

bool Vector3::operator!=(const Vector3 & compare) const
{
	return x != compare.x || y != compare.y || z != compare.z;
}
