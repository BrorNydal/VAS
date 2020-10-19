#pragma once

#include <iostream>
#include <QVector4D>

class vec4
{
public://variables
	float x, y, z, w;

public:
	vec4();
	vec4(float X, float Y, float Z, float W);
	~vec4();

	//static functions
	static const vec4 ZeroVector() { return vec4(0.f, 0.f, 0.f, 0.f); }
    static unsigned int VectorSize() { return 4; }

	//operators
	friend std::ostream& operator<< (std::ostream&, const vec4&);

	//vec3
	const vec4 operator = (const vec4& v);
	vec4 operator - (const vec4& v) const;
	vec4 operator / (float c) const;
	vec4 operator * (float c) const;
	vec4 operator + (const vec4& v) const;
	vec4 operator ^ (const vec4& v) const; // cross product	
	vec4 operator - () const;

	//float
	float operator * (const vec4& v) const;	// dot product

	//bool
	bool operator == (const vec4& v) const;
	bool operator != (const vec4& v) const;

    //cast
    operator QVector4D() const {return QVector4D(x,y,z,w);}
    vec4 operator = (const QVector4D &v);

	//functons
	float length() const;
	void normalize();
	vec4 normalized() const;
	static vec4 lerp(const vec4 &from, const vec4 &to, float delta);

};

inline std::ostream& operator<< (std::ostream& os, const vec4& v) {
	os << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
	os << std::endl;

	return os;
}
