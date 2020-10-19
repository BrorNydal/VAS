#ifndef GUISYSTEM_H
#define GUISYSTEM_H

#include "system.h"

class GuiComponent;

class GuiSystem : public System
{
public:
    GuiSystem();

    static GuiSystem &get();

    static void makeGUI(GuiComponent *gui, const std::string &texture);

    static void render();

private:
    void render_impl();
};

#endif // GUISYSTEM_H
