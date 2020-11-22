#ifndef OBJ_H
#define OBJ_H

#include "visualobject.h"


class OBJ : public VisualObject
{
private:
    std::string mFile;

public:
    OBJ(const std::string &file);
    bool read();

    virtual void draw(Shader &shader) override;
};

#endif // OBJ_H
