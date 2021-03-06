#include "AglVector3.h"
#include "AglMatrix.h"
#include "AglVector4.h"

//Constructors
AglVector3::AglVector3(float p_x, float p_y, float p_z) : x(p_x), y(p_y), z(p_z)
{
}
AglVector3::AglVector3() : x(0), y(0), z(0)
{
}

//Destructor
AglVector3::~AglVector3()
{
}

//Operators
float& AglVector3::operator[](const int& pIndex)
{
	if (pIndex == 0)
		return x;
	else if (pIndex == 1)
		return y;
	return z;
}
const float AglVector3::operator[](const int& pIndex) const
{
	if (pIndex == 0)
		return x;
	else if (pIndex == 1)
		return y;
	return z;
}
AglVector3 AglVector3::operator+(const AglVector3& pOther) const
{
	return AglVector3(x + pOther.x,
					  y + pOther.y,
					  z + pOther.z);
}
AglVector3& AglVector3::operator+=(const AglVector3& pOther)
{
	x += pOther.x;
	y += pOther.y;
	z += pOther.z;
	return (*this);
}
AglVector3 AglVector3::operator-(const AglVector3& pOther) const
{
	return AglVector3(x - pOther.x,
					  y - pOther.y,
					  z - pOther.z);
}
AglVector3& AglVector3::operator-=(const AglVector3& pOther)
{
	x -= pOther.x;
	y -= pOther.y;
	z -= pOther.z;
	return (*this);
}
AglVector3 AglVector3::operator*(const AglVector3& pOther) const
{
	return AglVector3(x * pOther.x,
					  y * pOther.y,
					  z * pOther.z);
}
AglVector3& AglVector3::operator*=(const AglVector3& pOther)
{
	x *= pOther.x;
	y *= pOther.y;
	z *= pOther.z;
	return (*this);
}

AglVector3 AglVector3::operator*(const float& pFactor) const
{
	return AglVector3(x * pFactor,
					  y * pFactor,
					  z * pFactor);
}
AglVector3& AglVector3::operator*=(const float& pFactor)
{
	x *= pFactor;
	y *= pFactor;
	z *= pFactor;
	return (*this);
}
AglVector3 AglVector3::operator/(const float& pFactor) const
{
	float f = 1.0f / pFactor;
	return AglVector3(x * f,
					  y * f,
					  z * f);
}
AglVector3& AglVector3::operator/=(const float& pFactor)
{
	float f = 1.0f / pFactor;
	x *= f;
	y *= f;
	z *= f;
	return (*this);
}
AglVector3 AglVector3::operator-() const 
{
	return AglVector3(-x, -y, -z);
}

bool AglVector3::operator==(const AglVector3& p_other) const
{
	float epsilon = 0.0001f;
	if (fabs(x - p_other.x) < epsilon &&
		fabs(y - p_other.y) < epsilon &&
		fabs(z - p_other.z) < epsilon)
		return true;
	return false;
}

//Member functions
float AglVector3::length()
{
	return sqrt(x*x + y*y + z*z);
}
float AglVector3::lengthSquared()
{
	return x*x + y*y + z*z;
}
void AglVector3::normalize()
{
	float frac = 1.0f / length();
	x *= frac;
	y *= frac;
	z *= frac;
}
void AglVector3::transform(const AglMatrix& pMatrix)
{
	AglVector4 vec = AglVector4(*this, 1);
	vec.transform(pMatrix);
	x = vec.x;
	y = vec.y;
	z = vec.z;
}
void AglVector3::transformNormal(const AglMatrix& pMatrix)
{
	AglVector4 vec = AglVector4(*this, 0);
	vec.transform(pMatrix);
	x = vec.x;
	y = vec.y;
	z = vec.z;
}


//Static functions
float AglVector3::dotProduct(const AglVector3& pV1, const AglVector3& pV2)
{
	return pV1.x * pV2.x + pV1.y * pV2.y + pV1.z * pV2.z;
}
AglVector3 AglVector3::crossProduct(const AglVector3& pV1, const AglVector3& pV2)
{
	AglVector3 cross;
	cross[0] = pV1[1] * pV2[2] - pV1[2] * pV2[1];
	cross[1] = pV1[2] * pV2[0] - pV1[0] * pV2[2];
	cross[2] = pV1[0] * pV2[1] - pV1[1] * pV2[0];
	return cross;
}
float AglVector3::lengthSquared(const AglVector3& pVector)
{
	return dotProduct(pVector, pVector);
}
float AglVector3::length(const AglVector3& pVector)
{
	return sqrt(lengthSquared(pVector));
}
void AglVector3::normalize(AglVector3& pVector)
{
	pVector /= (AglVector3::length(pVector));
}
AglVector3 AglVector3::lerp(const AglVector3& p_v1, const AglVector3& p_v2, float p_t)
{
	return p_v1 * (1.0f - p_t) + p_v2 * p_t;
}

AglVector3 AglVector3::createDirection(const AglVector3& p_from, const AglVector3& p_to)
{
	AglVector3 dir = p_to - p_from;
	dir.normalize();
	return dir;
}

AglVector3 AglVector3::minOf( const AglVector3& p_v1, const AglVector3& p_v2 )
{
	AglVector3 minV;
	if (p_v1.x < p_v2.x) minV.x = p_v1.x;
	else				 minV.x = p_v2.x;
	if (p_v1.y < p_v2.y) minV.y = p_v1.y;
	else				 minV.y = p_v2.y;
	if (p_v1.z < p_v2.z) minV.z = p_v1.z;
	else				 minV.z = p_v2.z;
	return minV;
}

AglVector3 AglVector3::maxOf( const AglVector3& p_v1, const AglVector3& p_v2 )
{
	AglVector3 maxV;
	if (p_v1.x > p_v2.x) maxV.x = p_v1.x;
	else				 maxV.x = p_v2.x;
	if (p_v1.y > p_v2.y) maxV.y = p_v1.y;
	else				 maxV.y = p_v2.y;
	if (p_v1.z > p_v2.z) maxV.z = p_v1.z;
	else				 maxV.z = p_v2.z;
	return maxV;
}

AglVector3 AglVector3::rotateAroundAxis( const AglVector3& p_toRotate,
	const AglVector3& p_axis, const float& p_radians )
{
	AglVector3 rotated = p_toRotate * cosf(p_radians) + crossProduct(p_axis, p_toRotate) * sinf(p_radians) +
		p_axis * dotProduct(p_axis, p_toRotate) * (1.0f - cosf(p_radians));
	return rotated;
}

AglVector3 AglVector3::right()
{
	return AglVector3(1, 0, 0);
}
AglVector3 AglVector3::left()
{
	return AglVector3(-1, 0, 0);
}
AglVector3 AglVector3::up()
{
	return AglVector3(0, 1, 0);
}
AglVector3 AglVector3::down()
{
	return AglVector3(0, -1, 0);
}
AglVector3 AglVector3::forward()
{
	return AglVector3(0, 0, 1);
}
AglVector3 AglVector3::backward()
{
	return AglVector3(0, 0, -1);
}
AglVector3 AglVector3::one()
{
	return AglVector3(1, 1, 1);
}
AglVector3 AglVector3::zero()
{
	return AglVector3(0, 0, 0);
}