#include "MyMath/vec3.h"

#include <math.h>
#include <cmath>

//custom
#include "staticfunctions.h"
#include "globalconstants.h"

vec3::vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

vec3::vec3(float X, float Y, float Z)
	:	x(X), y(Y), z(Z)
{
	
}

vec3::~vec3()
{
}

vec3 vec3::averageVector(std::vector<vec3> vs)
{
    vec3 averageVector;

    for(unsigned int i = 0; i < vs.size(); i++)
        averageVector = averageVector + vs[i];

    return averageVector/ vs.size();
}

const vec3 vec3::operator=(const vec3 & v)	{
	x = v.x; y = v.y; z = v.z;
	return *this;
}

vec3 vec3::operator-(const vec3 & v) const
{
	return vec3(x - v.x, y - v.y, z - v.z);
}

vec3 vec3::operator/(float c) const
{
	return vec3(x/c, y/c, z/c);
}

vec3 vec3::operator*(float c) const
{
	return vec3(x*c, y*c, z*c);
}

vec3 vec3::operator+(const vec3 & v) const
{
	return vec3(x + v.x, y + v.y, z + v.z);
}

vec3 vec3::operator^(const vec3 & v) const
{
	return vec3((y * v.z) - (z * v.y),
				(z * v.x) - (x * v.z),
				(x * v.y) - (y * v.x));
}

float vec3::operator*(const vec3 & v) const
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

bool vec3::operator==(const vec3 & v) const
{
    return BoolianFunctions::floatComparison(x, v.x) &&
           BoolianFunctions::floatComparison(y, v.y) &&
           BoolianFunctions::floatComparison(z, v.z);
}

bool vec3::operator!=(const vec3 & v) const
{
    return !BoolianFunctions::floatComparison(x, v.x) ||
           !BoolianFunctions::floatComparison(y, v.y) ||
           !BoolianFunctions::floatComparison(z, v.z);
}

vec3 vec3::operator-() const
{
	return vec3(-x,-y,-z);
}

void vec3::operator+=(vec3 v)
{
	*this = vec3(x+v.x, y+v.y, z+v.z);
}

void vec3::operator-=(vec3 v)
{
	*this = vec3(x-v.x, y-v.y, z-v.z);
}

float vec3::length() const
{
	return sqrtf((x*x) + (y*y) + (z*z));
}

void vec3::normalize()
{
	float l = length();

	if (l != 0.f)
	{
		*this = *this / l;
	}
}

vec3 vec3::normalized() const
{
	float l = length();

	if (l != 0.f)
		return *this/l;

	return *this;
}

vec3 vec3::lerp(const vec3 & from, const vec3 & to, float delta)
{	
	return to * delta + from * (1 - delta);
}

void vec3::rotateX(float angle)
{
	vec3 dir;
    angle = (angle * MathConstants::PIf) / 180.f;

	dir.y = cosf(angle) * y - std::sin(angle) * z;
	dir.z = sinf(angle) * y + std::cos(angle) * z;

	y = dir.y;
	z = dir.z;
}

void vec3::rotateY(float angle)
{
	vec3 dir;
    angle = (angle * MathConstants::PIf) / 180.f;

	dir.x = cosf(angle) * z + sinf(angle) * x;
	dir.z = cosf(angle) * z - sinf(angle) * x;

	x = dir.x;
	z = dir.z;
}

void vec3::rotateZ(float angle)
{
	vec3 dir;
    angle = (angle * MathConstants::PIf) / 180.f;

	dir.x = cosf(angle) * x - sinf(angle) * y;
	dir.y = sinf(angle) * x + cosf(angle) * y;

	x = dir.x;
	y = dir.y;
}
