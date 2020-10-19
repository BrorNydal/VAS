#ifndef STRUCTS_H
#define STRUCTS_H

#include "MyMath/vector2d.h"
#include "MyMath/vector3d.h"
#include <vector>

struct Material {
    Vector3D mAmbient;
    Vector3D mDiffuse;
    Vector3D mSpeccular;
    float mShininess;

    Material()
        :   mAmbient(Vector3D(0.4f, 0.4f, 0.4f)), mDiffuse(Vector3D(0.4f, 0.4f, 0.4f)), mSpeccular(Vector3D(0.4f, 0.4f, 0.4f)),
            mShininess(64.f)
    {

    }
};

struct LightProperties {
    Vector3D mAmbient;
    Vector3D mDiffuse;
    Vector3D mSpeccular;

    float mConstant;
    float mLinear;
    float mQuadratic;

    LightProperties()
        :   mAmbient(0.4f,0.4f,0.4f), mDiffuse(0.4f, 0.4f, 0.4f), mSpeccular(1.f,1.f,1.f),
            mConstant(1.f), mLinear(0.0014f), mQuadratic(0.000003f)
    {

    }
};

struct MovementSettings {
    float mMovementSpeed;
    float limiter = 100.f;
    void changeMovementSpeed(float speed){mMovementSpeed = speed / limiter;}
    MovementSettings()
    {
        mMovementSpeed = 10.f / limiter;
    }
};

struct MovementController {
    float moveForward;
    float moveRight;
    float turnRight;

    float movementSpeed;
    float turnSpeed;

    MovementController()
        :   moveForward(0.f), moveRight(0.f), turnRight(0.f), movementSpeed(0.2f), turnSpeed(0.1f)
    {

    }
};

struct CameraSettings {
    float mZoomScale;
    float mPitchScale;
    float mYawScale;

    Vector2D mZoomClamp;

    float mCameraSpeed;

    CameraSettings()
        :   mZoomScale(0.4f), mPitchScale(0.6f), mYawScale(0.6f), mZoomClamp(3.f, 15.f), mCameraSpeed(0.8f)
    {

    }
};

struct Triangle {
    unsigned int ID;

    std::vector<unsigned int> mNeighbours;
    std::vector<unsigned int> mIndices;

    Vector3D mSurfaceNormal;
    float mSurfaceArea;

    Triangle(unsigned int id) {ID = id;}
};

#endif // STRUCTS_H
