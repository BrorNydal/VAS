#include "rollingball.h"

RollingBall::RollingBall(float radius, int rekursions)
    :   OctahedronBall(radius, rekursions)

{

}

PhysicsProperties &RollingBall::getPhysicsProperties()
{
    return mPhysicsProperties;
}
