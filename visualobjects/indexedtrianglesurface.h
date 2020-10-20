#ifndef INDEXEDTRIANGLESURFACE_H
#define INDEXEDTRIANGLESURFACE_H

#include "VisualObjects/visualobject.h"
#include "triangle.h"
#include "beziercurve.h"

class IndexedTriangleSurface : public VisualObject
{
public:
    IndexedTriangleSurface(std::string data, std::string index);

    virtual void init() override;
    virtual void draw() override;

    void printDebugInformation();

    std::vector<Triangle> getTriangles() {return mTriangles;}

    //Oppgave 5.2.11

    void readDataFile(std::string fileName);
    void readIndexFile(std::string fileName);
    void writeFile();

    void setDisplayObject(VisualObject *display) {mSimulationObject = display;}

    //Seraches for the given coordinates and saves locations searched to mBarycentricSearchTrace
    void barycentricSearchPath(float x, float y);
    void barycentricSearchPath(Vector2D coordinates);
    void barycentricHeightSearch(VisualObject *object, unsigned int initialTriangle = 0);

    unsigned int mTriangleIndex = 0;
    std::vector<Vector3D> mBarycentricSearchTrace;
    std::vector<unsigned int> mBarycentricSearchTriangleTrace;
protected:
    void calculateSurfaceNormal();
    void calculateVertexNormal();
    VisualObject *mSimulationObject;
    std::vector<Triangle> mTriangles;

    BezierCurve mBezierCurve;

    unsigned int mObjectTriangleIndex;
    unsigned int mIndex = 0;
};

#endif // INDEXEDTRIANGLESURFACE_H
