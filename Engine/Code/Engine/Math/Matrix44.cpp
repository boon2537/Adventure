#include "Engine/Math/Matrix44.hpp"
#include "Engine/Math/MathUtils.hpp"

Matrix44::Matrix44(const float * sixteenValuesBasisMajor)
{
	Ix = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Iy = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Iz = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Iw = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Jx = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Jy = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Jz = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Jw = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Kx = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Ky = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Kz = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Kw = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Tx = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Ty = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Tz = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Tw = *sixteenValuesBasisMajor;
}

Matrix44::Matrix44(const Vector2 & iBasis, const Vector2 & jBasis, const Vector2 & translation)
{
	Ix = iBasis.x;
	Iy = iBasis.y;
	Iz = 0;
	Iw = 0;

	Jx = jBasis.x;
	Jy = jBasis.y;
	Jz = 0;
	Jw = 0;

	Kx = 0;
	Ky = 0;
	Kz = 1;
	Kw = 0;

	Tx = translation.x;
	Ty = translation.y;
	Tz = 0;
	Tw = 1;
}

Vector2 Matrix44::TransformPosition2D(const Vector2 & position2D)
{
	return Vector2((position2D.x * Ix) + (position2D.y * Jx) + Tx, (position2D.x * Iy) + (position2D.y * Jy) + Ty);
}

Vector2 Matrix44::TransformDisplacement2D(const Vector2 & displacement2D)
{
	return Vector2((displacement2D.x * Ix) + (displacement2D.y * Jx), (displacement2D.x * Iy) + (displacement2D.y * Jy));
}

void Matrix44::SetIdentity()
{
	Ix = 1;
	Iy = 0;
	Iz = 0;
	Iw = 0;

	Jx = 0;
	Jy = 1;
	Jz = 0;
	Jw = 0;

	Kx = 0;
	Ky = 0;
	Kz = 1;
	Kw = 0;

	Tx = 0;
	Ty = 0;
	Tz = 0;
	Tw = 1;
}

void Matrix44::SetValues(const float * sixteenValuesBasisMajor)
{
	Ix = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Iy = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Iz = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Iw = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Jx = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Jy = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Jz = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Jw = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Kx = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Ky = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Kz = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Kw = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Tx = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Ty = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Tz = *sixteenValuesBasisMajor; sixteenValuesBasisMajor++;
	Tw = *sixteenValuesBasisMajor;
}

void Matrix44::Append(const Matrix44 & matrixToAppend)
{	
	float oldIx = Ix;
	float oldIy = Iy;
	float oldIz = Iz;
	float oldIw = Iw;
	float oldJx = Jx;
	float oldJy = Jy;
	float oldJz = Jz;
	float oldJw = Jw;
	float oldKx = Kx;
	float oldKy = Ky;
	float oldKz = Kz;
	float oldKw = Kw;
	float oldTx = Tx;
	float oldTy = Ty;
	float oldTz = Tz;
	float oldTw = Tw;

	Ix = (oldIx * matrixToAppend.Ix) + (oldJx * matrixToAppend.Iy) + (oldKx * matrixToAppend.Iz) + (oldTx * matrixToAppend.Iw);
	Iy = (oldIy * matrixToAppend.Ix) + (oldJy * matrixToAppend.Iy) + (oldKy * matrixToAppend.Iz) + (oldTy * matrixToAppend.Iw);
	Iz = (oldIz * matrixToAppend.Ix) + (oldJz * matrixToAppend.Iy) + (oldKz * matrixToAppend.Iz) + (oldTz * matrixToAppend.Iw);
	Iw = (oldIw * matrixToAppend.Ix) + (oldJw * matrixToAppend.Iy) + (oldKw * matrixToAppend.Iz) + (oldTw * matrixToAppend.Iw);
	Jx = (oldIx * matrixToAppend.Jx) + (oldJx * matrixToAppend.Jy) + (oldKx * matrixToAppend.Jz) + (oldTx * matrixToAppend.Jw);
	Jy = (oldIy * matrixToAppend.Jx) + (oldJy * matrixToAppend.Jy) + (oldKy * matrixToAppend.Jz) + (oldTy * matrixToAppend.Jw);
	Jz = (oldIz * matrixToAppend.Jx) + (oldJz * matrixToAppend.Jy) + (oldKz * matrixToAppend.Jz) + (oldTz * matrixToAppend.Jw);
	Jw = (oldIw * matrixToAppend.Jx) + (oldJw * matrixToAppend.Jy) + (oldKw * matrixToAppend.Jz) + (oldTw * matrixToAppend.Jw);
	Kx = (oldIx * matrixToAppend.Kx) + (oldJx * matrixToAppend.Ky) + (oldKx * matrixToAppend.Kz) + (oldTx * matrixToAppend.Kw);
	Ky = (oldIy * matrixToAppend.Kx) + (oldJy * matrixToAppend.Ky) + (oldKy * matrixToAppend.Kz) + (oldTy * matrixToAppend.Kw);
	Kz = (oldIz * matrixToAppend.Kx) + (oldJz * matrixToAppend.Ky) + (oldKz * matrixToAppend.Kz) + (oldTz * matrixToAppend.Kw);
	Kw = (oldIw * matrixToAppend.Kx) + (oldJw * matrixToAppend.Ky) + (oldKw * matrixToAppend.Kz) + (oldTw * matrixToAppend.Kw);
	Tx = (oldIx * matrixToAppend.Tx) + (oldJx * matrixToAppend.Ty) + (oldKx * matrixToAppend.Tz) + (oldTx * matrixToAppend.Tw);
	Ty = (oldIy * matrixToAppend.Tx) + (oldJy * matrixToAppend.Ty) + (oldKy * matrixToAppend.Tz) + (oldTy * matrixToAppend.Tw);
	Tz = (oldIz * matrixToAppend.Tx) + (oldJz * matrixToAppend.Ty) + (oldKz * matrixToAppend.Tz) + (oldTz * matrixToAppend.Tw);
	Tw = (oldIw * matrixToAppend.Tx) + (oldJw * matrixToAppend.Ty) + (oldKw * matrixToAppend.Tz) + (oldTw * matrixToAppend.Tw);
}

void Matrix44::RotateDegrees2D(float rotationDegreesAboutZ)
{
	Append(MakeRotationDegrees2D(rotationDegreesAboutZ));
}

void Matrix44::Translate2D(const Vector2 & translation)
{
	Append(MakeTranslation2D(translation));
}

void Matrix44::ScaleUniform2D(float scaleXY)
{
	Append(MakeScaleUniform2D(scaleXY));
}

void Matrix44::Scale2D(float scaleX, float scaleY)
{
	Append(MakeScale2D(scaleX, scaleY));
}

Matrix44 Matrix44::MakeRotationDegrees2D(float rotationDegreesAboutZ)
{
	return Matrix44(Vector2(CosDegrees(rotationDegreesAboutZ), SinDegrees(rotationDegreesAboutZ)),
		Vector2(-1 * SinDegrees(rotationDegreesAboutZ), CosDegrees(rotationDegreesAboutZ)));
}

Matrix44 Matrix44::MakeTranslation2D(const Vector2 & translation)
{
	return Matrix44(Vector2(1, 0), Vector2(0, 1), translation);
}

Matrix44 Matrix44::MakeScaleUniform2D(float scaleXY)
{
	return Matrix44(Vector2(scaleXY, 0), Vector2(0, scaleXY));
}

Matrix44 Matrix44::MakeScale2D(float scaleX, float scaleY)
{
	return Matrix44(Vector2(scaleX, 0), Vector2(0, scaleY));
}

Matrix44 Matrix44::MakeOrtho2D(const Vector2 & bottomLeft, const Vector2 & topRight)
{
	float Ix = 2 / (topRight.x - bottomLeft.x);
	float Jy = 2 / (topRight.y - bottomLeft.y);
	float Tx = -1 * (topRight.x + bottomLeft.x) / (topRight.x - bottomLeft.x);
	float Ty = -1 * (topRight.y + bottomLeft.y) / (topRight.y - bottomLeft.y);

	float output[16] =
	{ Ix,	0,	0,	0,
	  0,	Jy,	0,	0,
	  0,	0,	-2,	0,
	  Tx,	Ty,	-1,	1 };

	return Matrix44(output);
}
