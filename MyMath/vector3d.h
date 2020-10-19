#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include "vertex.h"
#include <vector>

class Vector3D
{
    static const unsigned int vectorSize = 3;
    //Positions
    float xyz[vectorSize];

public:
    Vector3D();
    Vector3D(float X, float Y, float Z);

    unsigned int getVectorSize() {
        return vectorSize;
    }
    float getXYZ(unsigned int xyzIndex)  {
        if(xyzIndex < vectorSize)
            return xyz[xyzIndex];
        else
            return NULL;
    }
    void setX(float x) {xyz[0] = x;}
    void setY(float y) {xyz[1] = y;}
    void setZ(float z) {xyz[2] = z;}
    float x() const    {return xyz[0];}
    float y() const    {return xyz[1];}
    float z() const    {return xyz[2];}

    Vector3D calculateNormal(Vector3D v1, Vector3D v2);
    Vector3D calculateNormal(Vertex v1, Vertex v2);

    friend std::ostream& operator<< (std::ostream&, const Vector3D&);
    friend std::istream& operator>> (std::istream&, const Vector3D&);

    bool operator == (const Vector3D &v) const;
    const Vector3D& operator = (const Vector3D& v); // Assignment
    Vector3D operator + (const Vector3D& v) const;  // Addition
    Vector3D operator - (const Vector3D& v) const;  // Subtraction
    float operator * (const Vector3D& v) const;     // Dot product
    Vector3D operator ^ (const Vector3D& v) const;  // Cross product
    float length() const;                           // return length
    void normalize();                               // Normalize to unit length
    Vector3D operator * (float c) const;            // Scaling
    Vector3D operator / (float c) const;            // Dividing
};

inline std::ostream& operator<< (std::ostream& os, const Vector3D& v) {
  os << std::fixed;
  os << "(" << v.xyz[0] << ", " << v.xyz[1] << ", " << v.xyz[2] << ") ";
  return os;
}

inline std::istream& operator>> (std::istream& is, const Vector3D& v)   {
    char dum1, dum2, dum3, dum4;
    char x = static_cast<char>(v.xyz[0]);
    char y = static_cast<char>(v.xyz[1]);
    char z = static_cast<char>(v.xyz[2]);
    is >> dum1 >> x >> dum2 >> y >> dum3 >> z >> dum4;
    return is;
}


#endif // VECTOR3D_H
