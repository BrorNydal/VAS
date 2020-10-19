#include "MyMath/mat4x4.h"

#include <cassert>
#include <iostream>
#include <cmath>

//custom
#include "staticfunctions.h"
#include "globalconstants.h"
#include "MyMath/vec3.h"
#include "MyMath/vec4.h"


mat4x4::mat4x4()
{
	setToIdentity();
}

mat4x4::mat4x4(float val)
{
	for (unsigned int x = 0; x < MatrixDimensions(); x++)
	{
		for (unsigned int y = 0; y < MatrixDimensions(); y++)
		{
			mData[x][y] = val;
		}
	}
}

mat4x4::mat4x4(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
{
	mData[0][0] = a;
	mData[0][1] = b;
	mData[0][2] = c;
	mData[0][3] = d;

	mData[1][0] = e;
	mData[1][1] = f;
	mData[1][2] = g;
	mData[1][3] = h;

	mData[2][0] = i;
	mData[2][1] = j;
	mData[2][2] = k;
	mData[2][3] = l;

	mData[3][0] = m;
	mData[3][1] = n;
	mData[3][2] = o;
	mData[3][3] = p;
}

mat4x4::~mat4x4()
{
}

float& mat4x4::operator()(unsigned int row, unsigned int col)
{
    assert(col <= MatrixDimensions());
    assert(row <= MatrixDimensions());

	return mData[col][row];
}

void mat4x4::frustum(float left, float right, float bottom, float top, float cNear, float cFar)
{
	// Forutsetter a_ij initielt 0
	mData[0][0] = 2 * cNear / (right - left);
	mData[1][1] = 2 * cNear / (top - bottom);
	mData[0][2] = (right + left) / (right - left);
	mData[1][2] = (top + bottom) / (top - bottom);
	mData[2][2] = -(cFar + cNear) / (cFar - cNear);
	mData[2][3] = -2 * cFar*cNear / (cFar - cNear);
	mData[3][2] = -1.0f;
	mData[3][3] = 0.0f;
}

void mat4x4::perspective(float fovy, float aspectRatio, float cNear, float cFar)
{
	float top = cNear * static_cast<float>(std::tan(fovy));
	float right = top / aspectRatio;
	mData[0][0] = cNear / right;
	mData[1][1] = cNear / top;
	mData[2][2] = -(cFar + cNear) / (cFar - cNear);
	mData[2][3] = -2 * cFar*cNear / (cFar - cNear);
	mData[3][2] = -1;
}

void mat4x4::lookAt(const vec3 &eye, const vec3 &at, const vec3 &worldUp)
{
	setToIdentity();

	/*std::cout << "eye:" << eye << std::endl;
	std::cout << "at:" << at << std::endl;
	std::cout << "worldUp:" << worldUp << std::endl;*/

	if (at - eye == vec3::ZeroVector()) return;

	vec3 fwd =	(at - eye).normalized();    //forward
	vec3 rgt =	(fwd^worldUp).normalized(); //right
	vec3 up =	(rgt^fwd).normalized();     //up

	if (rgt == vec3::ZeroVector() || up == vec3::ZeroVector()) {
		rgt = vec3(1, 0, 0 );
		up  = vec3(0, 0, -1);
	}

	/*std::cout << "fwd:" << fwd << std::endl;
	std::cout << "rgt:" << rgt << std::endl;
	std::cout << "up:" << up << std::endl;*/

	*this =
		mat4x4(
			rgt.x,		up.x,		-fwd.x,		0.f,
			rgt.y,		up.y,		-fwd.y,		0.f,
			rgt.z,		up.z,		-fwd.z,		0.f,
			-rgt * eye, -up * eye,	fwd*eye,	1.f
		);
}

void mat4x4::setToIdentity()
{
	for (unsigned int i = 0; i < MatrixDimensions(); i++)
	{
		for (unsigned int j = 0; j < MatrixDimensions(); j++)
		{
			if (i == j)
				mData[i][j] = 1;
			else
				mData[i][j] = 0;
		}
	}
}

mat4x4 mat4x4::operator=(mat4x4 m)
{
	for (unsigned int i = 0; i < MatrixDimensions(); i++)
		for (unsigned int j = 0; j < MatrixDimensions(); j++)
			mData[i][j] = m(i, j);

	return *this;
}

mat4x4 mat4x4::operator *(float c)
{
	for (unsigned int i = 0; i < MatrixDimensions(); i++)
		for (unsigned int j = 0; j < MatrixDimensions(); j++)
			mData[i][j] = mData[i][j] * c;

	return *this;
}

vec4 mat4x4::operator *(const vec4 &v)
{
	float temp[4] = { 0, 0, 0, 0 };
	float tempv[4] = { v.x, v.y, v.z, v.w };	

    for (unsigned int i = 0; i < MatrixDimensions(); i++)
        for (unsigned int j = 0; j < MatrixDimensions(); j++)
				temp[i] += mData[i][j] * tempv[j];

	return vec4(temp[0],temp[1],temp[2],temp[3]);
}

mat4x4 mat4x4::operator *(mat4x4 m)
{
	mat4x4 matrix = mat4x4(0);

	for (unsigned int i = 0; i < MatrixDimensions(); i++)
	{
		for (unsigned int j = 0; j < MatrixDimensions(); j++)
		{
			for (unsigned int k = 0; k < MatrixDimensions(); k++)
				matrix(i, j) += mData[i][k] * m(k, j);
		}
	}

	return matrix;
}

void mat4x4::scale(float sX, float sY, float sZ)
{
	scale(vec3(sX, sY, sZ));
}

void mat4x4::scale(const vec3 &v)
{
	mat4x4 m;
	m.setToIdentity();

	m(0,0) = v.x;
	m(1,1) = v.y;
	m(2,2) = v.z;
	m(3,3) = 1;

	*this = *this * m;
}

void mat4x4::rotate(float angle, const vec3& v)
{
    angle = angle * MathConstants::PIf / 180.f;
	float c = cosf(angle);
	float s = sinf(angle);

	mat4x4 m;
	m.setToIdentity();

	if (v.x > 0)
	{
		m(1, 1) = c;
		m(1, 2) = -s;
		m(2, 1) = s;
		m(2, 2) = c;
	}
	else if (v.y > 0)
	{
		m(0, 0) = c;
		m(0, 2) = -s;
		m(2, 0) = s;
		m(2, 2) = c;
	}
	else if (v.z > 0)
	{
		m(0, 0) = c;
		m(0, 1) = -s;
		m(1, 0) = s;
		m(1, 1) = c;
	}
	else
		std::cout << "No Rotation" << std::endl;

	*this = *this * m;
}

void mat4x4::rotate(float angle, float x, float y, float z)
{
	rotate(angle, vec3(x, y, z));
}

void mat4x4::pitch(float angle)
{
}

void mat4x4::yaw(float angle)
{
}

void mat4x4::roll(float angle)
{
}

void mat4x4::translate(float tX, float tY, float tZ)
{
	translate(vec3(tX, tY, tZ));
}

void mat4x4::translate(const vec3 &t)
{
	mat4x4 m;
	m.setToIdentity();

	m(0,3) = t.x;
	m(1,3) = t.y;
	m(2,3) = t.z;

	*this = (*this) *(m);
}

vec3 mat4x4::getTranslation() const { 
	return vec3(mData[0][3], mData[1][3], mData[2][3]); 
}
void mat4x4::setTranslation(vec3 newTranslation)
{
	mData[0][3] = newTranslation.x;
	mData[1][3] = newTranslation.y;
	mData[2][3] = newTranslation.z;
}
vec3 mat4x4::getForwardVector() const
{
	return vec3(mData[0][2], mData[1][2], mData[2][2]);
}
vec3 mat4x4::getRightVector() const
{
	return vec3(mData[0][1], mData[1][1], mData[2][1]);
}
vec3 mat4x4::getUpVector() const
{
	return vec3(mData[0][0], mData[1][0], mData[2][0]);
}
