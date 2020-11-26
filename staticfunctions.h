#ifndef STATICFUNCTIONS_H
#define STATICFUNCTIONS_H

#include <QVector3D>
#include <QVector2D>
#include <cmath>

static bool CompareFloats(float a, float b)
{
    return fabs(a - b) < __FLT_EPSILON__;
}

static QVector3D BarycentricCoordinates(const QVector2D &v, const QVector2D &v1, const QVector2D &v2, const QVector2D &v3){
    QVector2D v12 = v2-v1;   //Vec2
    QVector2D v13 = v3-v1;   //Vec2

    QVector3D n =  QVector3D::crossProduct(v12, v13);
    float areal123 = n.length();

    QVector3D baryc; //return value

    //u
    QVector2D p = v2 - v;    //Vec2
    QVector2D q = v3 - v;    //Vec2
    n = QVector3D::crossProduct(p, q);
    baryc.setX(n.z() / areal123);

    //v
    p = v3 - v;
    q = v1 - v;
    n = QVector3D::crossProduct(p, q);
    baryc.setY(n.z() / areal123);

    //w
    p = v1 - v;
    q = v2 - v;
    n = QVector3D::crossProduct(p, q);
    baryc.setZ(n.z() / areal123);

    return baryc;
}

static QVector3D CalculateTriangleNormal(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
{
    return QVector3D::crossProduct(v2 - v1, v3 - v1);
}

struct MySortFunctions
{
    template<class T>
    static void swap(T *xp, T *yp)
    {
        T temp = *xp;
        *xp = *yp;
        *yp = temp;
    }

    template<class T>
    static void selectionSort(T arr[], unsigned int n)
    {
        unsigned int min_idx;

        // One by one move boundary of unsorted subarray
        for (unsigned int i = 0; i < n-1; i++)
        {
            // Find the minimum element in unsorted array
            min_idx = i;
            for (unsigned int j = i+1; j < n; j++)
            {
                if (arr[j] < arr[min_idx])
                    min_idx = j;
            }

            // Swap the found minimum element with the first element
            swap(&arr[min_idx], &arr[i]);
        }
    }

    template<class T>
    static void selectionSort(T arr[], unsigned int indxArr[], unsigned int n)
    {
        unsigned int min_idx;

        // One by one move boundary of unsorted subarray
        for (unsigned int i = 0; i < n-1; i++)
        {
            // Find the minimum element in unsorted array
            min_idx = i;

            for (unsigned int j = i+1; j < n; j++)
            {
                if (arr[j] < arr[min_idx])
                    min_idx = j;
            }

            // Swap the found minimum element with the first element
            swap<T>(&arr[min_idx], &arr[i]);
            // Do the samefor the index-array
            swap(&indxArr[min_idx], &indxArr[i]);
        }
    }
};

struct MyMathFunctions
{
    static QVector3D average3DVector(std::vector<QVector3D> vs)
    {
        QVector3D averageVector;

        for(unsigned int i = 0; i < vs.size(); i++)
            averageVector = averageVector + vs[i];

        return averageVector/ vs.size();
    }

    static bool floatComparison(float a, float b, float accuracy)
    {
        return std::fabs(a - b) < accuracy;
    }
};

#endif // STATICFUNCTIONS_H
