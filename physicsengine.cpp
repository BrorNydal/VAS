#include "physicsengine.h"

#include <QDebug>
#include <QMatrix4x4>
#include <cmath>

PhysicsEngine::PhysicsEngine()
{

}

void PhysicsEngine::ballMovement(float deltaTime, Transform &transform, PhysicsProperties &pprop, const QVector3D &surfaceNormal)
{
    //is not optimalized, just for clarity in whats going on
    if(surfaceNormal.z() < 0.f)
        qDebug() << "[Warning] negative z on surface normal.";

    //qDebug() << surfaceNormal;
    QVector3D acceleration; // = Sum of forces divided by mass

    //find forces:
    QVector3D gravitation{0.f,0.f,-mGravity};
    QVector3D normalForce = surfaceNormal;
    normalForce.normalize();

    //qDebug() << "NF :" << normalForce << "," << normalForce.length();
    acceleration = (mGravity * QVector3D(normalForce.x() * normalForce.z(), normalForce.y() * normalForce.z(), (normalForce.z() * normalForce.z()) - 1)) / pprop.mass;
    acceleration = acceleration * deltaTime;
    //qDebug() << "ACCEL :" << acceleration;

    acceleration.setZ(0.f);
    pprop.acceleration = acceleration;
    pprop.velocity += acceleration; //acceleration is change of velocity
//    qDebug() << "Acceleration :" << acceleration;
//    qDebug() << "Velocity :" << pprop.velocity;
    transform.location += pprop.velocity; //velocity is change of location
}

CollisionResult PhysicsEngine::CheckCollision(const Sphere &s1, Transform &t1, const Sphere &s2, Transform &t2)
{
    CollisionResult result;

    float dist = (t2.location - t1.location).length();
    result.distance = dist;

    if(dist < s1.radius + s2.radius)
    {
        result.collision = true;
        qDebug() << "(physicsengine) collision!";
    }
    else
        result.collision = false;

    return result;
}

void PhysicsEngine::handleCollision(const Sphere &s1, PhysicsProperties &pp1, Transform &t1, const Sphere &s2, PhysicsProperties &pp2, Transform &t2)
{
    if(pp1.physic && pp2.physic)
    {
        QVector3D to2 = (t2.location - t1.location);
        float dist = to2.length() - s1.radius - s2.radius;
        qDebug() << "Distance :" << dist;

        to2.normalize();
        QVector3D to1 = to2 * -1.f;

        t2.location -= to2 * dist/2.f;
        t1.location += to2 * dist / 2.f;

        //pp1.velocity = {0.f, 0.f, 0.f};
        //pp2.velocity = {0.f, 0.f, 0.f};

        float m1, m2; //magnitudes of vectors
        m1 = pp1.velocity.length();
        m2 = pp2.velocity.length();

        if(pp1.velocity != QVector3D(0.f,0.f,0.f))
        {
            QVector3D v1 = pp1.velocity.normalized();

            float angle1 = acosf((v1.x() * to1.x() + v1.y() * to1.y()) / (sqrtf(v1.x() * v1.x() + v1.y() * v1.y()) * sqrtf(to1.x() * to1.x() + to1.y() * to1.y())));

            qDebug() << "Angle :" << angle1;

            QMatrix4x4 yawMat;
            yawMat.setToIdentity();

            yawMat.rotate(angle1, QVector3D(0.f,0.f,1.f));

            QVector3D result = (yawMat * to2).normalized();
            QVector3D old = pp1.velocity;
            pp1.velocity += pp2.velocity - pp1.velocity + (-result) * m1; //(result * -1.f * m1)
            pp2.velocity += old - pp2.velocity + (result) * m2; //result * m2
        }
        else if(pp2.velocity != QVector3D(0.f, 0.f, 0.f))
        {
            QVector3D v1 = pp2.velocity.normalized();

            float angle1 = acosf((v1.x() * to2.x() + v1.y() * to2.y()) / (sqrtf(v1.x() * v1.x() + v1.y() * v1.y()) * sqrtf(to2.x() * to2.x() + to2.y() * to2.y())));

            qDebug() << "Angle :" << angle1;

            QMatrix4x4 yawMat;
            yawMat.setToIdentity();

            yawMat.rotate(angle1, QVector3D(0.f,0.f,1.f));

            QVector3D result = (yawMat * to1).normalized();
            pp1.velocity += pp2.velocity + (result) * m1;
            pp2.velocity += -1.f * result * m2;
        }

    }
    else if(pp1.physic && !pp2.physic)
    {

    }
    else
    {

    }
}
