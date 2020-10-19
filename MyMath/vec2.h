#pragma once

#include <iostream>

class vec3;

class vec2
{
public://variables

//two floats creating the vector
	float x, y;

public:
	vec2();
	vec2(float X, float Y);
	~vec2();	

	//static functions
	static const vec2 ZeroVector() { return vec2(0, 0); }
    static unsigned int VectorSize() { return 2; }

	//operators
	friend std::ostream& operator<< (std::ostream&, const vec2&);

	const vec2& operator = (const vec2& v);
	vec2 operator - (const vec2& v) const;
	vec2 operator + (const vec2& v) const;
	vec2 operator / (float c) const;
	vec2 operator * (float c) const;
	float operator ^ (const vec2& v) const;  // Cross product
	float operator * (const vec2& v) const;
	void operator += (vec2 v);
	void operator -= (vec2 v);

	//functions
	float length() const;
	void normalize();
	vec2 normalized() const;
	vec3 barycentricCoordinates(const vec2 &v1, const vec2 &v2, const vec2 &v3);
	static vec2 lerp(const vec2 &from, const vec2 &to, float delta);
	
};

inline std::ostream& operator<< (std::ostream& os, const vec2& v) {
	os << "(" << v.x << "," << v.y << ")";
	os << std::endl;

	return os;
}
