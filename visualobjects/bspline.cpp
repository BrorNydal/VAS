#include "bspline.h"

#include <math.h>
#include "staticfunctions.h"


BSplineCurve::BSplineCurve()
{
    mDrawMode = GL_LINES;
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

    return a[0];
}


QVector3D BSplineCurve::getLocationOnSpline(unsigned int &index, float delta, bool &end)
{
    unsigned int csize{0};

    if(mIncludeCP)
        csize = b.size();

    if(index + 1 + csize < mVertices.size())
    {
        end = false;
        QVector3D result = delta * mVertices[index + 1 + csize] + (1.f - delta) * mVertices[index + csize];
        return getLocation() + result * getScale();
    }
    else{
        end = true;

        if(index + csize < mVertices.size())
            return 1.f * mVertices[index + csize] + (1.f - 1.f) * mVertices[index-1 + csize];
        else
            return QVector3D(0.f,0.f,0.f);
    }
}

float BSplineCurve::lengthOfCurrentStep(unsigned int index)
{
    if(index + 1 < mVertices.size())
    {
        QVector3D result = (QVector3D)mVertices[index+1] - (QVector3D)mVertices[index];
        return result.length();
    }
    else
    {
        return 1.f;
    }
}

void BSplineCurve::draw(Shader &shader)
{
    shader.uniform3f("color", 10.f, 10.f, 0.f);
    VisualObject::draw(shader);
}

void BSplineCurve::setNewValues(std::vector<float> knots, std::vector<QVector3D> controlpoints, int degree)
{
    mVertices.clear();
    mIndices.clear();
    t.clear();
    b.clear();

    t = knots;
    b = controlpoints;
    n = controlpoints.size();
    d = degree;

    xmax = b[0].x(); xmin = b[0].x();
    ymax = b[0].y(); ymin = b[0].y();

    for(auto cp : b)
    {
        if(cp.x() < xmin) xmin = cp.x();
        if(cp.x() > xmax) xmax = cp.x();
        if(cp.y() < ymin) ymin = cp.y();
        if(cp.y() > ymax) ymax = cp.y();
    }

    xmax -= xmin;
    ymax -= ymin;

    for(auto &cp : b)
    {
        cp[0] -= xmin;

        if(!CompareFloats(xmax, 0.f))
            cp[0] /= xmax;

        cp[1] -= ymin;

        if(!CompareFloats(ymax, 0.f))
            cp[1] /= ymax;
    }

    createCurve();    
}

void BSplineCurve::createCurve()
{
    float acc = 0.01f;

    setLocation(QVector3D(0.f,0.f,0.f)); //reset location
    move(QVector3D(xmin,ymin,0.02f));   //move to location
    setScale(QVector3D(xmax, ymax, 1.f)); //scale back

    unsigned int it = 0;
    mIncludeCP = true;
    for(auto &cp : b)
    {
        mVertices.push_back(Vertex(cp.x(), cp.y(), cp.z()));

        if(it+1 < b.size())
        {
            mIndices.push_back(it);
            mIndices.push_back(it+1);
        }

        it++;
    }

//    xmax -= xmin;
//    ymax -= ymin;

    //first and second loop index
    unsigned int first = 0;
    unsigned int second = mVertices.size();

    while(first < t.size() - 1) {

        float delta = t[first];//controlpoints[t[first]].x();

        while(delta < t[first+1])
        {
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

    //for(unsigned int i = 0; i < 3; i++)
    //    mVertices.pop_back();

    initializeBuffers_impl(mIndices.data(), mVertices.data(), mIndices.size(), mVertices.size(), mBuffers, mDrawMode);
}

void BSplineCurve::operator =(const BSplineCurve &bs)
{
    t = bs.t;
    b = bs.b;
    n = bs.n;
    d = bs.d;
}
