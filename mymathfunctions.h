#ifndef MYMATHFUNCTIONS_H
#define MYMATHFUNCTIONS_H

#include <iostream>
#include <vector>
#include <QVector3D>

struct MyMathFunctions
{
    static QVector3D centroidOfTriangle(class OldTriangle *triangle);
    static QVector3D centroidOfTriangle(QVector3D v1, QVector3D v2, QVector3D v3);
    static QVector3D average3DVector(std::vector<QVector3D> vs);

    static bool floatComparison(float a, float b, float accuracy = 0.05f);
};


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

#endif // MYMATHFUNCTIONS_H