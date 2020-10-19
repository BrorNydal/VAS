#ifndef MYMATHFUNCTIONS_H
#define MYMATHFUNCTIONS_H

#include <iostream>
#include <vector>
#include "vec2.h"
#include "vec3.h"

struct BoolianFunctions
{
    static bool floatComparison (float a, float b, float accuracy = 0.05f);
    static void boolianSwitch   (bool &b);
};


struct SortFunctions
{
    template<class T>
    static void swap(T *xp, T *yp)
    {
        T temp = *xp;
        *xp = *yp;
        *yp = temp;
    }

    //Sorting without indexes for each value
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

    //Sorting with indexes for each value
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
