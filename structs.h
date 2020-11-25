#ifndef STRUCTS_H
#define STRUCTS_H

#include <QVector3D>
#include <QVector2D>

enum class EShader {
    plain,
    phong
};

struct Triangle {
    int mNeighbours[3] = {-1,-1,-1};
    int mIndices[3] =    {-1, -1, -1};

    QVector3D mSurfaceNormal{0.f,0.f,1.f};
    float mSurfaceArea{1.f};
};

struct Square {
    bool onUpper = false;
    Triangle upper;
    Triangle lower;
};

struct Sphere {
    float radius{1.f};
};

//simple direction light
struct LightProperties {
    LightProperties(float a = 0.06f, float d = 0.7f, float s = 0.3f){
        for(unsigned int i = 0; i < 3; i++)
        {
            ambient[i] = a;
            diffuse[i] = d;
            specular[i] = s;
        }
    }

    QVector3D direction{0.f,0.f,-1.f};
    QVector3D ambient {0.2f,0.2f,0.2f};
    QVector3D diffuse {0.3f,0.3f,0.3f};
    QVector3D specular{0.2f,0.2f,0.2f};
};

struct PhysicsProperties {
    QVector3D acceleration{0.f, 0.f, 0.f}; //increase in velocity per second
    QVector3D velocity{0.f, 0.f, 0.f}; //change of location
    float mass{1.f};
    bool physic{true};
};

struct Buffers {
    unsigned int indexCount{0};
    unsigned int vertexCount{0};
    unsigned int vao{0};
    unsigned int vbo{0};
    unsigned int eab{0};
};

struct Transform {
    QVector3D location{0,0,0};
    QVector3D rotation{0,0,0};
    QVector3D scale{1,1,1};
};

//Contains the minimum and maximum x, y and z value for a triangle surface
struct SurfaceLimits {
    QVector2D x;
    QVector2D y;
    QVector2D z;

    float centralizedOffsetX() const {
        return -(x.y() + x.x()) / 2.f;
    }
    float centralizedOffsetY() const {
        return -(y.y() + y.x()) / 2.f;
    }
    float centralizedOffsetZ() const {
        return -(z.y() + z.x()) / 2.f;
    }

    float offsetMinX() const {
        return -x.x();
    }
    float offsetMinY() const {
        return -y.x();
    }
    float offsetMinZ() const {
        return -z.x();
    }

    float offsetMaxX() const {
        return -x.y();
    }
    float offsetMaxY() const {
        return -y.y();
    }
    float offsetMaxZ() const {
        return -z.y();
    }
};

struct CollisionResult {
    bool collision;
    float distance;
};

#endif // STRUCTS_H
