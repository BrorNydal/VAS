#include "MyMath/vec2.h"

//custom
#include "MyMath/vec3.h"

//other
#include <math.h>
#include <cmath>

vec2::vec2()
{
	x = 0;
	y = 0;
}

vec2::vec2(float X, float Y)
	:	x(X), y(Y)
{

}

vec2::~vec2()
{
}

const vec2 &vec2::operator =(const vec2 &v)
{
	x = v.x;
	y = v.y;

	return *this;
}

vec2 vec2::operator -(const vec2 &v) const
{
	return vec2(x - v.x, y - v.y);
}

vec2 vec2::operator+(const vec2 & v) const
{
	return vec2(x+v.x, y+v.y);
}

vec2 vec2::operator /(float c) const
{
	return vec2(x / c, y / c);
}

vec2 vec2::operator*(float c) const
{
	return vec2(x*c, y*c);
}

float vec2::operator ^(const vec2 &v) const
{
	return (x * v.y) - (y * v.x);
}

float vec2::operator *(const vec2 &v) const
{
	return (x * v.x) + (y * v.y);
}

void vec2::operator+=(vec2 v)
{
	*this = vec2(x + v.x, y + v.y);
}

void vec2::operator-=(vec2 v)
{
	*this = vec2(x - v.x, y - v.y);
}

float vec2::length() const
{
	return sqrtf((x * x) + (y * y));
}

void vec2::normalize()
{
	if (length() != 0.f)
		*this = *this / length();
}

vec2 vec2::normalized() const
{
	float l = length();
	return vec2(x/l,y/l);
}

vec3 vec2::barycentricCoordinates(const vec2 &v1, const vec2 &v2, const vec2 &v3)
{
	vec2 v12 = v2 - v1;   //Vec2
	vec2 v13 = v3 - v1;   //Vec2

	vec3 n = vec3(v12.x, v12.y, 0.f) ^ vec3(v13.x, v13.y, 0.f);
	float areal123 = n.length();

	vec3 baryc; //return value

	//u
	vec2 p = v2 - *this;    //Vec2
	vec2 q = v3 - *this;    //Vec2
	n = vec3(p.x, p.y, 0.f) ^ vec3(q.x, q.y, 0.f);
	baryc.x = n.z / areal123;

	//v
	p = v3 - *this;
	q = v1 - *this;
	n = vec3(p.x, p.y, 0.f) ^ vec3(q.x, q.y, 0.f);
	baryc.y = n.z / areal123;

	//w
	p = v1 - *this;
	q = v2 - *this;
	n = vec3(p.x, p.y, 0.f) ^ vec3(q.x, q.y, 0.f);
	baryc.z = n.z / areal123;

	return baryc;
}

vec2 vec2::lerp(const vec2 & from, const vec2 & to, float delta)
{
	return to * delta + from * (1 - delta);
}
