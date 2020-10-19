#include "triangle.h"


bool OldTriangle::isTriangleListed(OldTriangle *t)
{
    for(unsigned int i = 0; i < mAdjentTriangles.size(); i++)
        if(t == mAdjentTriangles[i])
            return true;
    return false;

}
