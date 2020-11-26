#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octahedronball.h"
#include "bspline.h"
class Camera;

class RollingBall : public OctahedronBall
{
private:
    PhysicsProperties mPhysicsProperties;
    Camera *mCamera;

public:
    RollingBall(float radius = 1.f, int rekursions = 3);

    PhysicsProperties &getPhysicsProperties();
    void setCamera(Camera *cam);
};

#endif // ROLLINGBALL_H
