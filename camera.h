#ifndef CAMERA_H
#define CAMERA_H

#include "structs.h"
#include "shader.h"
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>

class Camera
{
private:

    Transform mTransform;

    const float mFov{77.f};
    const float mAspectRatio{4.f/3.f};
    const float mNearPlane{0.01f};
    const float mFarPlane{3000.f};
    const float mPitchLimit = 87.f;

    QVector2D mCursorDelta{0,0};
    QVector2D mCursorLocation{0,0};

    QVector2D mScreenSize;

    float mMovementSensitivity{1.1f};
    float mRotationSensitivity{0.2f};

    float mCameraZoomScale{0.06f};
    float mMaxCameraOffset{2000.f};
    float mMinCameraOffset{0.5f};
    float mOffset{1.5f};

    QVector3D mForward     {0.f,1.f,0.f};
    QVector3D mRight;
    QVector3D mUp;

    QMatrix4x4 mViewMatrix;;
    QMatrix4x4 mProjectionMatrix;

    const QVector3D mRelativeLocation{0.f,-1.f,0.f};
    QVector3D mLocation{0.f,0.f,0.f};
    QVector3D mFinalLocation{0.f,0.f,0.f};

public:

    Camera();

    void lookAt();
    void render(Shader &shader);

    void setScreenSize(QVector2D ss);
    void setCursorLocation(QVector2D loc);
    void setCursorDelta(QVector2D delta);
    QVector2D getCursorLocation() const;

    void setLocation(QVector3D loc);
    QVector3D getLocation() const;
    void move(QVector3D dir);
    void yaw(float delta);
    void pitch(float delta);

};

#endif // CAMERA_H
