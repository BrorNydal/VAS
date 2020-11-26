#include "camera.h"

#include "globalconstants.h"


Camera::Camera()
{
    mViewMatrix.setToIdentity();
    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(mFov, mAspectRatio, mNearPlane, mFarPlane);
    mLocation = mForward + mRelativeLocation;
    mFinalLocation = mLocation - mForward * mOffset;
    mViewMatrix.lookAt(mFinalLocation, mFinalLocation + mForward, QVector3D(0,0,1));
}

void Camera::lookAt()
{
    mViewMatrix.setToIdentity();
    mForward = QVector3D(0, 1.f, 0.f);

    float pitch = mTransform.rotation.x();
    float yaw = mTransform.rotation.z();

    QMatrix4x4 yawMatrix;
    yawMatrix.setToIdentity();
    yawMatrix.rotate(yaw, WorldSettings::UP);
    mForward = yawMatrix * mForward;

    mRight = QVector3D::crossProduct(mForward, WorldSettings::UP).normalized();

    QMatrix4x4 pitchMatrix;
    pitchMatrix.setToIdentity();
    pitchMatrix.rotate(pitch, mRight);
    mForward = pitchMatrix * mForward;

    mUp = QVector3D::crossProduct(mRight, mForward).normalized();

    mFinalLocation = mLocation - mForward * mOffset;
    mViewMatrix.lookAt(mFinalLocation, mFinalLocation + mForward, WorldSettings::UP);

//    qDebug() << "location :" << mLocation;
//    qDebug() << "final location :" << mFinalLocation;
//    qDebug() << "forward :" << mForward;
}

void Camera::render(Shader &shader)
{
    shader.uniformMat4x4f("vmatrix", mViewMatrix);
    shader.uniformMat4x4f("pmatrix", mProjectionMatrix);

    if(shader.getType() == EShader::phong)
    {
        shader.uniform3f("viewLocation", mFinalLocation);
    }
}

void Camera::setCursorLocation(QVector2D loc)
{
    mCursorLocation = loc;
}

void Camera::setCursorDelta(QVector2D delta)
{
    mCursorDelta = delta;
}

QVector2D Camera::getCursorLocation() const
{
    return mCursorLocation;
}

void Camera::setLocation(QVector3D loc)
{
    mLocation = loc;
}

QVector3D Camera::getLocation() const
{
    return mLocation;
}

void Camera::move(QVector3D dir)
{
    if(dir.x() != 0.f)
        mLocation += mRight * mMovementSensitivity * dir.x();
    if(dir.y() != 0.f)
        mLocation += mForward * mMovementSensitivity * dir.y();
    if(dir.z() != 0.f)
        mLocation += mUp * mMovementSensitivity * dir.z();
}

void Camera::yaw(float delta)
{
    mTransform.rotation[2] += mRotationSensitivity * delta * -1.f;
}

void Camera::pitch(float delta)
{
    delta *= -1.f;

    if(mTransform.rotation.x() >= mPitchLimit && delta > 0.f)
    {
        mTransform.rotation[0] = mPitchLimit;
        return;
    }
    else if(mTransform.rotation.x() <= -mPitchLimit && delta < 0.f)
    {
        mTransform.rotation[0] = -mPitchLimit;
        return;
    }

    mTransform.rotation[0] += delta * mRotationSensitivity;
}

void Camera::zoom(float delta)
{
    mOffset += delta * mCameraZoomScale;
    if(mOffset < mMinCameraOffset)
        mOffset = mMinCameraOffset;
    else if(mOffset > mMaxCameraOffset)
        mOffset = mMaxCameraOffset;
}

QVector2D Camera::getForwardVectorXY() const
{
    return QVector2D(mForward.x(), mForward.y()).normalized();
}

QVector2D Camera::getRightVectorXY() const
{
    return QVector2D(mRight.x(), mRight.y()).normalized();
}
