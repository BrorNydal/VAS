#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "VisualObjects/visualobject.h"
#include "VisualObjects/linedraw.h"
#include "triangle.h"

class TriangleSurface : public VisualObject
{
protected:
    std::vector<OldTriangle> mTriangles;

public:
    TriangleSurface();
    TriangleSurface(std::string fileName);

    virtual void init() override;
    virtual void draw() override;

    std::vector<OldTriangle> getTriangles(){return mTriangles;}

    //Oppgave_5_2_10
    void readFile(std::string fileName);
    void writeFile();    

    void setSimulationObject(VisualObject *display) {mSimulationObject = display;}
public:
    //Finds which triangle is adjacent to eachother and gives a pointer to it
    void findTriangleAdjacency();

    //Seraches for the given coordinates and saves locations searched to mBarycentricSearchTrace
    void barycentricSearchPath(float x, float y);
    void barycentricSearchPath(Vector2D loc);

    std::vector<Vector3D> mBarycentricSearchTrace;
protected:
    VisualObject * mSimulationObject;
    float mDelta;
    float mSpeed;
    unsigned int mLerpIndex;
    unsigned int mTriangleIndex;
    unsigned int mCurrentTriangleIndex{0};
    void lerpObject(VisualObject *object);
    void findHeight();
    bool findNextTriangle(unsigned int index);

protected:
    void calculateNormals();

    LineDraw mLineDraw;
};

#endif // TRIANGLESURFACE_H
