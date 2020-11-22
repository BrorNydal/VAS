#ifndef INDEXEDTRIANGLESURFACE_H
#define INDEXEDTRIANGLESURFACE_H

#include "visualobject.h"
#include "triangle.h"
#include "beziercurve.h"
#include "structs.h"
#include <QPoint>



class IndexedTriangleSurface : public VisualObject
{
private:
    std::string mVertexFile;
    std::string mIndexFile;
    std::vector<Triangle> mTriangles;
    float mScale;
    bool mIsLasFile;
    QVector2D mResolution{50.f,50.f};
    std::vector<QVector3D> mLasData;
    SurfaceLimits mLimit;
    QVector2D mSquareSize;
    QVector2D mTotalSize;
    unsigned int mCol, mRow;
    float mFrag;
    std::vector<Square> mSquares;

public:
    IndexedTriangleSurface(std::string data, std::string index, float scale = 1.f, bool las = false);
    void run();

    virtual void draw(Shader &shader) override;

    void printDebugInformation();

    void lasOptions(bool las = true, QVector2D triangleSize = {0.f,0.f});

    std::vector<Square> getTriangles() {return mSquares;}

    SurfaceLimits findSurfaceLimit(std::string filename);
    void readConvertedLasFile(std::string filename);
    void assertIndices();
    QPoint getSquare(float x, float y) const;
    Triangle *getTriangle(float x, float y);

    void readCutsomFile(std::string filename);
    void readCustomIndexFile(std::string filename);
    void writeFile();

    void setDisplayObject(VisualObject *display) {mSimulationObject = display;}


    float heightAtLocation(float x, float y);
    float barycentricHeightSearch(QVector2D loc);
    const Triangle &getCurrentTriangle(float x, float y) const;
    const Square &getCurrentSquare(float x, float y) const;

    std::vector<QVector3D> mBarycentricSearchTrace;
    std::vector<unsigned int> mBarycentricSearchTriangleTrace;

protected:
    unsigned vertexIndex(QPoint point) const;
    unsigned vertexIndex(unsigned int x, unsigned int y) const;
    unsigned int row(unsigned int i) const;
    unsigned int col(unsigned int i) const;

    void calculateSurfaceNormal();
    void calculateVertexNormal();
    VisualObject *mSimulationObject;

    BezierCurve mBezierCurve;

    unsigned int mObjectTriangleIndex{0};
    unsigned int mObjectSquareIndex{0};
};

#endif // INDEXEDTRIANGLESURFACE_H
