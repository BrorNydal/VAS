QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

INCLUDEPATH += \
    ./scenes \
    ./visualobjects \

SOURCES += main.cpp \
    camera.cpp \
    mymathfunctions.cpp \
    physicsengine.cpp \
    scenes/scene.cpp \
    scenes/scene1.cpp \
    scenes/scene2.cpp \
    scenes/scene3.cpp \
    visualobjects/beziercurve.cpp \
    visualobjects/bspline.cpp \
    visualobjects/enemy.cpp \
    visualobjects/grid.cpp \
    visualobjects/indexedtrianglesurface.cpp \
    visualobjects/item.cpp \
    visualobjects/light.cpp \
    visualobjects/obj.cpp \
    visualobjects/octahedronball.cpp \
    visualobjects/rollingball.cpp \
    visualobjects/visualobject.cpp \
    visualobjects/xyz.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    triangle.cpp \
    vertex.cpp

HEADERS += \
    camera.h \
    mymathfunctions.h \
    physicsengine.h \
    scenes/scene.h \
    scenes/scene1.h \
    scenes/scene2.h \
    scenes/scene3.h \
    staticfunctions.h \
    structs.h \
    visualobjects/beziercurve.h \
    visualobjects/bspline.h \
    visualobjects/enemy.h \
    visualobjects/grid.h \
    visualobjects/indexedtrianglesurface.h \
    visualobjects/item.h \
    visualobjects/light.h \
    visualobjects/obj.h \
    visualobjects/octahedronball.h \
    visualobjects/rollingball.h \
    visualobjects/visualobject.h \
    visualobjects/xyz.h \
    globalconstants.h \
    renderwindow.h \
    shader.h \
    mainwindow.h \
    triangle.h \
    vertex.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    PhongFragment.frag \
    PhongVertex.vert \
    plainfragment.frag \
    plainvertex.vert \
    terrainfragment.frag \
    terrainvertex.vert
