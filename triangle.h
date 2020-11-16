#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector3D>
#include "vertex.h"

struct Triangle{
    Triangle(unsigned int id) {ID = id;}
    unsigned int ID;

    QVector3D mSurfaceNormal;
    QVector3D mCentroid;

    unsigned int mIndecies[3] = {0,0,0};
    int mAdjacentTriangles[3] = {-1,-1,-1};
    QVector3D mPosition[3];
};

class OldTriangle
{
private:
    //Unique for each triangle
    unsigned int ID;

protected:
    QVector3D mSurfaceNormal;
    QVector3D mCentroid;
    float mSurfaceArea;

public:
    OldTriangle(unsigned int id){ID = id;}

    unsigned int getID() {return ID;}

    std::vector<OldTriangle*> mAdjentTriangles;
    std::vector<QVector3D> mPositions;

    //Checks if input value is listed in the existing vector
    bool isTriangleListed(OldTriangle* t);

    //Set functions
    void setFaceNormal(QVector3D norm) {mSurfaceNormal = norm; mSurfaceArea = norm.length()/2.f;}
    void setCentroid(QVector3D centroid) {mCentroid = centroid;}

    //Get Functions
    QVector3D getCentroid() {return mCentroid;}
    QVector3D getSurfaceNormal() {return mSurfaceNormal;}

    //Operators
    bool operator == (OldTriangle t) {return ID == t.ID;}
    bool operator != (OldTriangle t) {return ID == t.ID;}
};

#endif // TRIANGLE_H
