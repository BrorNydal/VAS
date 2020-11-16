#ifndef CONTOURLINE_H
#define CONTOURLINE_H

#include "VisualObjects/visualobject.h"

struct Edge{
    QVector3D mFrom;
    QVector3D mTo;
    QVector3D mBetween;

    bool operator == (Edge e) {return mBetween == e.mBetween || mBetween == (e.mBetween * -1);}
    bool operator != (Edge e) {return mBetween == e.mBetween || mBetween == (e.mBetween * -1);}

    Edge(QVector3D from, QVector3D to){
        mFrom = from; mTo = to;
        mBetween = to - from;
    }
};

class ContourLine : public VisualObject
{
public:
    ContourLine(VisualObject *triangleSurface, float height = 1.5, bool indexed = true);

    void createContourLine();
    void newHeight(float height);

    void initDrawLines();

protected:
    std::vector<Edge> mEdges;

    VisualObject *mSurface;
    float mHeight;
    const bool mIndexedTerrain;


};

#endif // CONTOURLINE_H
