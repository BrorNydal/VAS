#ifndef ITEM_H
#define ITEM_H

#include "obj.h"

class Item : public OBJ
{
public:
    Item();
    virtual void draw(Shader &shader) override;

};

#endif // ITEM_H
