#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "bspline.h"

class RollingBall : public OctahedronBall
{
private:
    PhysicsProperties mPhysicsProperties;
public:
    RollingBall(float radius = 1.f, int rekursions = 3);

    PhysicsProperties &getPhysicsProperties();
};

#endif // ROLLINGBALL_H
