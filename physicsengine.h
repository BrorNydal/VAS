#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "structs.h"

class PhysicsEngine
{
    float mGravity = 9.81f;

public:
    PhysicsEngine();

    /**
     * Moves a ball accross a surface each frame.
     * @param transform of object.
     * @param physics properties of object.
     * @param surface normal
     */
    void ballMovement(float deltaTime, Transform &transform, PhysicsProperties &pprop, const QVector3D &surfaceNormal);
};

#endif // PHYSICSENGINE_H
