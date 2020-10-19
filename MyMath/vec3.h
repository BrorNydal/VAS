#pragma once

#include <iostream>
#include <vector>
#include <QVector3D>

class vec3
{
public://variables

//three floats creating a 3d-vector
	float x, y, z;

public://functions 
	vec3();
	vec3(float X, float Y, float Z);
	~vec3();

	//static functions
	static const vec3 ZeroVector() { return vec3(0.f, 0.f, 0.f); }
    static unsigned int VectorSize() { return 3; }
    static vec3 averageVector(std::vector<vec3> vs);

	//operators
	friend std::ostream& operator<< (std::ostream&, const vec3&);

	//vec3
	const vec3 operator = (const vec3& v);
	vec3 operator - (const vec3& v) const;
	vec3 operator / (float c) const;
	vec3 operator * (float c) const;
	vec3 operator + (const vec3& v) const;
	vec3 operator ^ (const vec3& v) const; // cross product	
	vec3 operator - () const;

	void operator += (vec3 v);
	void operator -= (vec3 v);

	//float
	float operator * (const vec3& v) const;	// dot product

	//bool
	bool operator == (const vec3& v) const;
	bool operator != (const vec3& v) const;

    //cast
    operator QVector3D() const {return QVector3D(x,y,z);}

	//functons
	float length() const;
	void normalize();
	vec3 normalized() const;
	static vec3 lerp(const vec3 &from, const vec3 &to, float delta);

	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
};

inline std::ostream& operator<< (std::ostream& os, const vec3& v) {
	os << "(" << v.x << "," << v.y << "," << v.z << ")";
	os << std::endl;

	return os;
}
