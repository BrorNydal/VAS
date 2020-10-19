#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "enumclasses.h"

#include "globalconstants.h"
#include "vec2.h"
#include "vec3.h"
#include <QVector3D>
#include "texture.h"
#include <QMatrix4x4>
#include <QKeyEvent>


struct Component
{
    EComponentType type() const {return componentType;}

protected:
    EComponentType componentType;
};

struct TransformComponent : public Component {
    TransformComponent() : location(vec3::ZeroVector()), rotation(vec3::ZeroVector()), scale(vec3(1,1,1))
    {
        componentType = EComponentType::transform;
        movementLimitation = EMovementLimitType::fixed;
    }

    EMovementLimitType movementLimitation;

    QVector3D location;
    QVector3D rotation;
    QVector3D scale;

    QVector3D velocity;

    TransformComponent operator + (const TransformComponent &tc) const
    {
        TransformComponent t;
        t.location = location + tc.location;
        t.rotation = rotation + tc.rotation;
        t.scale = scale + tc.scale;
        return t;
    }
};

struct CollisionComponent : public Component{
    CollisionComponent()
    {
        componentType = EComponentType::collider;
        colliderType = EColliderType::unspecified;
    }

    TransformComponent relativeTransform;

    EColliderType colliderType;
};

struct SphereCollider : public CollisionComponent
{
    SphereCollider(float rad = 1.f) : radius(rad)
    {
        colliderType = EColliderType::sphere;
    }

    float radius;
};

struct PlaneCollider : public CollisionComponent
{
    PlaneCollider(QVector3D bl, QVector3D br, QVector3D ul, QVector3D ur)
        :   bottomLeft(bl), bottomRight(br), upperLeft(ul), upperRight(ur)
    {
        colliderType = EColliderType::plane;
    }

    QVector3D bottomLeft{0,0,0};
    QVector3D bottomRight{0,0,0};
    QVector3D upperLeft{0,0,0};
    QVector3D upperRight{0,0,0};
};

struct TerrainComponent : public Component
{
    TerrainComponent(const std::string &tg = "Terrain") : tag(tg)
    {
        componentType = EComponentType::terrain;
    }

    std::string tag;

    unsigned int mRow{0}, mCol{0};
    int indexCount{0};
    int vertexCount{0};

    unsigned int vao{0};
    unsigned int vbo{0};
    unsigned int eab{0};
};

struct MeshComponent : public Component
{
    MeshComponent(const std::string &tg = "Mesh") : tag(tg), texture(new Texture())
    {
        componentType = EComponentType::mesh;
    }

    std::string tag;
    Texture *texture;

    int indexCount = 0;
    unsigned int vao{0};
    unsigned int vbo{0};
    unsigned int eab{0};

    TransformComponent relativeTransform;

    bool operator == (const MeshComponent &mc) const
    {
        return vao == mc.vao && indexCount == mc.indexCount;
    }
};

struct CameraComponent : public Component
{
    CameraComponent()
        :   cameraZoomScale(0.006f), maxCameraOffset(20.f), minCameraOffset(5.f)
    {
        componentType = EComponentType::camera;

        viewMatrix.setToIdentity();
        projectionMatrix.setToIdentity();
        projectionMatrix.frustum(-1, 1,-1, 1, 1, -1);
    }

    QVector2D screenSize;

    float cameraZoomScale{0.006f};
    float maxCameraOffset{20.f};
    float minCameraOffset{5.f};
    float offset{10.f};

    QVector3D forward;
    QVector3D right;
    QVector3D up;

    QMatrix4x4 viewMatrix;;
    QMatrix4x4 projectionMatrix;

    QMatrix4x4 pitchMatrix;
    QMatrix4x4 yawMatrix;

    QVector3D worldPosition     {0.f,0.f,0.f};
    QVector3D relativePosition  {0.f,-1.f,0.f};
    QVector3D finalPosition     {0.f,0.f,0.f};
};

struct InputComponent : public Component
{
    InputComponent() : cursorPosition(QVector2D(0,0)), cursorPositionDelta(QVector2D(0,0)), cursorRayDirection(QVector3D(0,0,0))
    {
        componentType = EComponentType::input;
    }

    std::map<Qt::MouseButton, bool> mouse;
    std::map<int, bool> keyboard;

    QVector2D cursorPosition;
    QVector2D cursorPositionDelta;

    QVector3D cursorRayDirection;
};

struct TextureComponent : public Component
{
    TextureComponent()
    {
        componentType = EComponentType::texture;
    }

    unsigned int ID;
    unsigned char* data;
    unsigned int target;

    int width;
    int height;
    int channelCount;
};

struct GuiComponent : public Component
{
    GuiComponent(QVector2D pos = QVector2D(0,0), QVector2D scl = QVector2D(1,1))
            :   texture(nullptr), position(pos), scale(scl)
    {
        componentType = EComponentType::gui;
    }

    QMatrix4x4 mat4;

    TextureComponent *texture;

    unsigned int size{0};
    unsigned int vao{0};
    unsigned int vbo{0};
    unsigned int eab{0};

    QVector2D position;
    QVector2D scale;
};


#endif // COMPONENTS_H
