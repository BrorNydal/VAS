#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include"components.h"
#include "entity.h"

class PhysicsEngine
{
private:
    float mGravity = 9.8f;

public:
    PhysicsEngine();

    void applyPhysics(TransformComponent &transform, float deltaTime);

    //static functions, collision testing
    static bool sphereToSphere(const SphereCollider &sphere1, const TransformComponent &tc1, const SphereCollider &sphere2, const TransformComponent &tc2);
    static bool lineToSphere(const QVector3D &lineOrigin, const QVector3D &lineDirection, const SphereCollider &sphere, const TransformComponent &tc);

};

#endif // PHYSICSENGINE_H
