#include "item.h"


Item::Item()
    :   OBJ("item.obj")
{

}

void Item::draw(Shader &shader)
{
    shader.uniform3f("color", 1.f, 0.f, 1.f);
    VisualObject::draw(shader);
}
