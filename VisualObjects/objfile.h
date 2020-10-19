#ifndef OBJFILE_H
#define OBJFILE_H

#include "VisualObjects/visualobject.h"

class ObjFile : public VisualObject
{
public:
    ObjFile(std::string fileName);

    void readFile(std::string fileName);

    virtual void init() override;
    virtual void draw() override;

protected:
    std::vector<Vector3D> mVertexNormals;
    std::vector<Vector2D> mTextureCoordinates;
};

#endif // OBJFILE_H
