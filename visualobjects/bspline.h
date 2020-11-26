#ifndef BSPLINE_H
#define BSPLINE_H

#include "visualobject.h"

#include <QVector3D>
#include <vector>

class BSplineCurve : public VisualObject
{
public:
    BSplineCurve();

    QVector3D evaluateBSpline(int my, float x);

    QVector3D getLocationOnSpline(unsigned int &index, float delta, bool &end);
    float lengthOfCurrentStep(unsigned int index);

    virtual void draw(Shader &shader) override;

    /**
     * Sets new values for bspline and creates curve.
     * @param knots
     * @param controlpoints
     * @param degree
     */
    void setNewValues(std::vector<float> knots, std::vector<QVector3D> controlpoints, int degree = 2);
    void createCurve();

    void operator = (const BSplineCurve &bs);

private:
    std::vector<float> t; // knots
    std::vector<QVector3D> b; // controll points
    int n; // n = number of knots
    int d; // d = degree

    float xmax{0.f}, ymax{0.f};
    float xmin{0.f}, ymin{0.f};

    QVector3D mColor{1.f,0.f,1.f};
    bool mIncludeCP{false};
};

#endif // BSPLINE_H
