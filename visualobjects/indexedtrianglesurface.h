#ifndef INDEXEDTRIANGLESURFACE_H
#define INDEXEDTRIANGLESURFACE_H

#include "visualobject.h"
#include "triangle.h"
#include "beziercurve.h"
#include "structs.h"


class IndexedTriangleSurface : public VisualObject
{
private:
    std::string mVertexFile;
    std::string mIndexFile;
    float mScale;
    bool mIsLasFile;
    QVector2D mResolution{100.f,100.f};
    std::vector<QVector3D> mLasData;
    SurfaceLimits mLimit;
    QVector2D mTotalSize;
    unsigned int mCol, mRow;

public:
    IndexedTriangleSurface(std::string data, std::string index, float scale = 1.f, bool las = false);
    void run();

    virtual void draw(Shader &shader) override;

    void printDebugInformation();

    void lasOptions(bool las = true, QVector2D size = {0.f, 0.f});

    std::vector<Triangle> getTriangles() {return mTriangles;}

    SurfaceLimits findSurfaceLimit(std::string filename);
    void readConvertedLasFile(std::string filename);
    void assertIndices();

    void readCutsomFile(std::string filename);
    void readCustomIndexFile(std::string filename);
    void writeFile();

    void setDisplayObject(VisualObject *display) {mSimulationObject = display;}


    float barycentricHeightSearch(QVector2D loc);
    const Triangle &getCurrentTriangle() const;

    std::vector<QVector3D> mBarycentricSearchTrace;
    std::vector<unsigned int> mBarycentricSearchTriangleTrace;

protected:
    void calculateSurfaceNormal();
    void calculateVertexNormal();
    VisualObject *mSimulationObject;
    std::vector<Triangle> mTriangles;

    BezierCurve mBezierCurve;

    unsigned int mObjectTriangleIndex{0};
};

#endif // INDEXEDTRIANGLESURFACE_H