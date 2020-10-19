#include "staticfunctions.h"
#include "math.h"

bool BoolianFunctions::floatComparison(float a, float b, float accuracy)
{
    return std::fabs(a - b) < accuracy;
}

void BoolianFunctions::boolianSwitch(bool &b)
{
    if(b)
        b = false;
    else
        b = true;
}



