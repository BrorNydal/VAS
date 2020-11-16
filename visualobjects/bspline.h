#ifndef BSPLINE_H
#define BSPLINE_H

#include "visualobject.h"

#include <QVector3D>
#include <vector>

class BSplineCurve : public VisualObject
{
public:
    BSplineCurve();
    BSplineCurve(std::vector<float> knots, std::vector<QVector3D> controlpoints, int degree = 2);

    void setKnotsAndControlPoints(std::vector<float> knots, std::vector<QVector3D> points);

    QVector3D evaluateBSpline(int my, float x);
    int findKnotInterval(float x);
    QVector3D evaluateBSpline(int degree, int startKnot, float x);

    virtual void draw(Shader &shader) override;
private:
    std::vector<float> t; // knots
    std::vector<QVector3D> b; // controll points
    int n; // n = number of knots
    int d; // d = degree

    QVector3D mColor{1.f,0.f,1.f};
};

#endif // BSPLINE_H
