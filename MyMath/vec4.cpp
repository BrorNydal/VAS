#include "vec4.h"

#include <math.h>
#include <cmath>

//custom 
#include "staticfunctions.h"


vec4::vec4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

vec4::vec4(float X, float Y, float Z, float W)
	:	x(X), y(Y), z(Z), w(W)
{

}


vec4::~vec4()
{
}

const vec4 vec4::operator=(const vec4 & v)
{
	x = v.x; y = v.y; z = v.z; w = v.w;
	return *this;
}

vec4 vec4::operator-(const vec4 & v) const
{
	return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

vec4 vec4::operator/(float c) const
{
	return vec4(x / c, y / c, z / c, w / c);
}

vec4 vec4::operator*(float c) const
{
	return vec4(x * c, y * c, z * c, w * c);
}

vec4 vec4::operator+(const vec4 & v) const
{
	return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

vec4 vec4::operator^(const vec4 & v) const
{
	return vec4();
}

vec4 vec4::operator-() const
{
	return vec4(-x,-y,-z,-w);
}

float vec4::operator*(const vec4 & v) const
{
	return (x * v.x) + (y * v.y) + (z * v.z) + (w * v.w);
}

bool vec4::operator==(const vec4 & v) const
{
    return BoolianFunctions::floatComparison(x, v.x) &&
           BoolianFunctions::floatComparison(y, v.y) &&
           BoolianFunctions::floatComparison(z, v.z) &&
           BoolianFunctions::floatComparison(w, v.w);
}

bool vec4::operator!=(const vec4 & v) const
{
    return !BoolianFunctions::floatComparison(x, v.x) ||
           !BoolianFunctions::floatComparison(y, v.y) ||
           !BoolianFunctions::floatComparison(z, v.z) ||
            !BoolianFunctions::floatComparison(w, v.w);
}

vec4 vec4::operator =(const QVector4D &v)
{
    return vec4(v.x(),v.y(),v.z(),v.w());
}

float vec4::length() const
{
	return sqrtf((x*x)+(y*y)+(z*z)+(w*w));
}

void vec4::normalize()
{
	float l = length();

	if(l != 0)
		*this = *this / l;
}

vec4 vec4::normalized() const
{
	float l = length();

	if (l != 0)
		return *this / l;

	return *this;
}

vec4 vec4::lerp(const vec4 & from, const vec4 & to, float delta)
{
	return to * delta + from * (1 - delta);
}
