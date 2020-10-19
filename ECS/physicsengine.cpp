#include "physicsengine.h"

#include <cmath>

PhysicsEngine::PhysicsEngine()
{

}

void PhysicsEngine::applyPhysics(TransformComponent &transform, float deltaTime)
{
    transform.velocity.setZ(transform.velocity.z() - mGravity);
}

bool PhysicsEngine::sphereToSphere(const SphereCollider &sphere1, const TransformComponent &tc1, const SphereCollider &sphere2, const TransformComponent &tc2)
{
    if((tc2.location - tc1.location).length() < sphere1.radius + sphere2.radius)
        return true;

    return false;
}

bool PhysicsEngine::lineToSphere(const QVector3D &lineOrigin, const QVector3D &lineDirection, const SphereCollider &sphere, const TransformComponent &tc)
{
    //variables not needed, optimalize later. just for clearer code atm.

    QVector3D rayOrigin = lineOrigin;
    QVector3D rayDir = lineDirection.normalized();

    QVector3D sphereOrigin = tc.location + sphere.relativeTransform.location;
    float sphereRadius = sphere.radius;

    float t = QVector3D::dotProduct(sphereOrigin-rayOrigin, rayDir);
    QVector3D nearestPoint = rayOrigin + rayDir * t;

    float y = (sphereOrigin-nearestPoint).length();

    //if the ray hits the sphere
    if(y < sphereRadius)
    {
        float x = sqrtf(sphereRadius*sphereRadius - y*y);

        //a line hits the sphere in two locations. this is the delta along the ray in the two locations.
        float t1 = t-x;
        float t2 = t+x;

        return true;
    }

    return false;
}
