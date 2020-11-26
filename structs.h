#ifndef STRUCTS_H
#define STRUCTS_H

#include <QVector3D>
#include <QVector2D>

enum class EShader {
    plain,
    phong,
    terrain
};

//Triangle with neighbour information available
struct Triangle {
    int mNeighbours[3] = {-1,-1,-1};
    int mIndices[3] =    {-1, -1, -1};

    QVector3D mSurfaceNormal{0.f,0.f,1.f};
    float mSurfaceArea{1.f};
};

//a square consisting of two triangles, and a boolian saying if object is on lower or upper triangle
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
    LightProperties(float a = 0.09f, float d = 0.8f, float s = 0.6f){
        for(unsigned int i = 0; i < 3; i++)
        {
            ambient[i] = a;
            diffuse[i] = d;
            specular[i] = s;
        }
    }

    QVector3D direction{0.f,-0.5f,-0.7f};
    QVector3D ambient {0.2f,0.2f,0.2f};
    QVector3D diffuse {0.3f,0.3f,0.3f};
    QVector3D specular{0.2f,0.2f,0.2f};
};

//struct holding all necessary information for a ball to roll
struct PhysicsProperties {
    QVector3D acceleration{0.f, 0.f, 0.f}; //increase in velocity per second
    QVector3D velocity{0.f, 0.f, 0.f};      //change of location
    float mass{1.f};
    bool physic{true}; //if false, don't apply physics
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

//Return value of a collision-test
struct CollisionResult {
    bool collision;
    float distance;
};

#endif // STRUCTS_H
