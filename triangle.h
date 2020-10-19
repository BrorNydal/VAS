#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "MyMath/vector3d.h"
#include "vertex.h"

struct Triangle{
    Triangle(unsigned int id) {ID = id;}
    unsigned int ID;

    Vector3D mSurfaceNormal;
    Vector3D mCentroid;

    std::vector<unsigned int> mIndecies;
    std::vector<int> mAdjacentTriangles;
};

class OldTriangle
{
private:
    //Unique for each triangle
    unsigned int ID;

protected:
    Vector3D mSurfaceNormal;
    Vector3D mCentroid;
    float mSurfaceArea;

public:
    OldTriangle(unsigned int id){ID = id;}

    unsigned int getID() {return ID;}

    std::vector<OldTriangle*> mAdjentTriangles;
    std::vector<Vector3D> mPositions;

    //Checks if input value is listed in the existing vector
    bool isTriangleListed(OldTriangle* t);

    //Set functions
    void setFaceNormal(Vector3D norm) {mSurfaceNormal = norm; mSurfaceArea = norm.length()/2.f;}
    void setCentroid(Vector3D centroid) {mCentroid = centroid;}

    //Get Functions
    Vector3D getCentroid() {return mCentroid;}
    Vector3D getSurfaceNormal() {return mSurfaceNormal;}

    //Operators
    bool operator == (OldTriangle t) {return ID == t.ID;}
    bool operator != (OldTriangle t) {return ID == t.ID;}
};

#endif // TRIANGLE_H
