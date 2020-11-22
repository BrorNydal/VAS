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

    QVector3D getLocationOnSpline(unsigned int &index, float delta, bool &end);

    virtual void draw(Shader &shader) override;

    void setNewValues(std::vector<float> knots, std::vector<QVector3D> controlpoints, int degree = 2);
    void createCurve();

    void operator = (const BSplineCurve &bs);

private:
    std::vector<float> t; // knots
    std::vector<QVector3D> b; // controll points
    int n; // n = number of knots
    int d; // d = degree

    QVector3D mColor{1.f,0.f,1.f};
};

#endif // BSPLINE_H
