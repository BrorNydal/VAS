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

    /**
     * Checks if two objects collide.
     * Checks collision of two objects,
     * a combination of RollingBall and an enemy or
     * two RollingBall's.
     * @param s1
     * @param t1
     * @param s2
     * @param t2
     * @return true at collision, false when not collliding.
     */
    static CollisionResult CheckCollision(const Sphere &s1, Transform &t1, const Sphere &s2, Transform &t2);

    /**
     * Handles collision between two Objcts.
     * Handles collision between RollingBall and an enemy or
     * two RollingBall's.
     * @param s1 sphere first object.
     * @param pp1 physicsproperty first object.
     * @param t1 transform first object.
     * @param s2 sphere second object.
     * @param pp2 physicsproperties second object.
     * @param t2 transform second object.
     */
    void handleCollision(const Sphere &s1, PhysicsProperties &pp1, Transform &t1,
                         const Sphere &s2, PhysicsProperties &pp2, Transform &t2);
};

#endif // PHYSICSENGINE_H
