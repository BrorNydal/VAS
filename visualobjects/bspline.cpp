#include "bspline.h"

#include <math.h>

BSplineCurve::BSplineCurve()
{

}

BSplineCurve::BSplineCurve(std::vector<float> knots, std::vector<QVector3D> controlpoints, int degree)
    :   t(knots), b(controlpoints), n(knots.size()), d(degree)
{
    mDrawMode = GL_LINE_STRIP;

    createCurve();
//    for(unsigned int k = 0; k < 20; k++)
//        mIndices.pop_back();
}

void BSplineCurve::setKnotsAndControlPoints(std::vector<float> knots, std::vector<QVector3D> points)
{
    t = knots;
    b = points;
    n = knots.size();
}

// Parametre inn:
// x - en parameterverdi på skjøtvektor
// my - et tall slik at t[my] <= x < t[my+1]
// Returverdi: et punkt på splinekurven
// b,n,d,t er objektvariable i klassen BSplineCurve

QVector3D BSplineCurve::evaluateBSpline(int my, float x)
{
    QVector3D a[20]; // forutsetter da at n+d+1 <= 20

    for (int j=0; j<=d; j++)
        a[d-j] = b[my-j];

    for (int k=d; k>0; k--) {
        int j = my-k;

        for (int i=0; i<k; i++) {
            j++;
            float w = (x-t[j])/(t[j+k]-t[j]);
            a[i] = a[i] * (1-w) + a[i+1] * w;
        }
    }

    qDebug() << "evaluate bspline :" << a[0];
    return a[0];
}


QVector3D BSplineCurve::getLocationOnSpline(unsigned int &index, float delta, bool &end)
{
    qDebug() << "size :" << mVertices.size();
    if(index + 1 < mVertices.size())
    {
        end = false;
        QVector3D result = delta * mVertices[index + 1] + (1 - delta) * mVertices[index];
        return result * getScale();
    }
    else{
        end = true;
        index = 0;
        return mVertices[0];
    }
}

void BSplineCurve::draw(Shader &shader)
{
    shader.uniform3f("color", 10.f, 10.f, 0.f);
    VisualObject::draw(shader);
}

void BSplineCurve::setNewValues(std::vector<float> knots, std::vector<QVector3D> controlpoints, int degree)
{
    t = knots;
    b = controlpoints;
    n = controlpoints.size();
    d = degree;

    createCurve();
}

void BSplineCurve::createCurve()
{
    mVertices.clear();
    mIndices.clear();

    float acc = 0.01f;

    move(QVector3D(0.f,0.f,0.02f));

    //unsigned int it = 0;
//    for(auto &cp : controlpoints)
//    {
//        mVertices.push_back(Vertex(cp.x(), cp.y(), cp.z()));

//        if(it < controlpoints.size() - 2)
//            mIndices.push_back(it);
//        if(it+1 < controlpoints.size())
//            mIndices.push_back(it+1);

//        it++;
//    }

    qDebug() << "SIZE:" << mVertices.size();

    //first and second loop index
    unsigned int first = 0;
    unsigned int second = mVertices.size();

    while(first < t.size()) {

        float delta = t[first];//controlpoints[t[first]].x();

        while(delta < t[first+1])
        {
            qDebug() << "delta :" << delta;
            qDebug() << "knot :" << t[first] << t[first+1];

            QVector3D v = evaluateBSpline(first, delta);

            mVertices.push_back(Vertex(v.x(), v.y(), v.z()));

            //Indices
            if(delta + acc < t[first+1])
            {
                mIndices.push_back(second);
                mIndices.push_back(second + 1);
            }

            second++;
            delta+=acc;
        }

        first++;
    }

    initializeBuffers_impl(mIndices.data(), mVertices.data(), mIndices.size(), mVertices.size(), mBuffers, mDrawMode);
}

void BSplineCurve::operator =(const BSplineCurve &bs)
{
    t = bs.t;
    b = bs.b;
    n = bs.n;
    d = bs.d;
}
