#include "physicsengine.h"

#include <QDebug>

PhysicsEngine::PhysicsEngine()
{

}

void PhysicsEngine::ballMovement(float deltaTime, Transform &transform, PhysicsProperties &pprop, const QVector3D &surfaceNormal)
{
    //is not optimalized, just for clarity in whats going on


    QVector3D acceleration; // = Sum of forces divided by mass

    //find forces:
    QVector3D gravitation{0.f,0.f,-mGravity};
    QVector3D normalForce = surfaceNormal;
    normalForce.normalize();

    //qDebug() << "NF :" << normalForce << "," << normalForce.length();
    acceleration = (mGravity * QVector3D(normalForce.x() * normalForce.z(), normalForce.y() * normalForce.z(), (normalForce.z() * normalForce.z()) - 1)) / pprop.mass;
    acceleration *= deltaTime;
    //qDebug() << "ACCEL :" << acceleration;

    acceleration.setZ(0.f);
    pprop.acceleration = acceleration;
    pprop.velocity += acceleration; //acceleration is change of velocity
//    qDebug() << "Acceleration :" << acceleration;
//    qDebug() << "Velocity :" << pprop.velocity;
    transform.location += pprop.velocity; //velocity is change of location
}
