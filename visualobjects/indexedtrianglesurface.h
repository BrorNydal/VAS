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
    //name of file for vertices
    std::string mVertexFile;

    //name of file for indices, if set to "none" it will assume it's a converted las-file
    std::string mIndexFile;

    //Triangles and Squares. One square is two triangles.
    std::vector<Triangle> mTriangles;
    std::vector<Square> mSquares;

    //Norsk : målestokk
    float mScale;
    bool mIsLasFile;

    //how detailed the terrain is. The heigher values the more triangles.
    QVector2D mResolution{30.f,30.f};
    std::vector<QVector3D> mLasData;
    SurfaceLimits mLimit;
    QVector2D mSquareSize;
    QVector2D mTotalSize;
    unsigned int mCol, mRow;
    float mFrag;


    //Wanted to use QPoint instead of pair of ints, but it was buggy.
    //first : pair of unsigned int represent which vertex on grid; first being x and second y.
    //second : pair of unsigned int and float. first being the number of floats added, second being the total sum of all added floats.
    std::map<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, float>> mHeightData;

    //The radius around vertecies to obtain height data from
    float mHeightAcceptanceRadius;

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
