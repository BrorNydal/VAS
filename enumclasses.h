#ifndef ENUMCLASSES_H
#define ENUMCLASSES_H

#include <iostream>

//All enum classes starts with E

enum class EShaderType {
    plain = 0,
    texture,
    phong,
    gui,
    skybox
};

enum class EComponentType {
    transform = 0,
    mesh,
    camera,
    input,
    texture,
    gui,
    collider,
    terrain
};

enum class EColliderType {
    unspecified,
    sphere,
    box,
    plane
};

enum class EMovementLimitType {
    fixed,
    dynaimc,
    controlled
};

enum class ETerrainType {
    grid,
    square,
    map
};

#endif // ENUMCLASSES_H
