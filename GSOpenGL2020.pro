QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    MyMath/vector2d.cpp \
    MyMath/vector3d.cpp \
    MyMath/vector4d.cpp \
    Scenes/Scene.cpp \
    Scenes/Scene1.cpp \
    Scenes/scene2.cpp \
    StaticFunctions.cpp \
    VisualObjects/beziercurve.cpp \
    VisualObjects/grid.cpp \
    VisualObjects/light.cpp \
    VisualObjects/objfile.cpp \
    VisualObjects/octahedronball.cpp \
    VisualObjects/terrain.cpp \
    VisualObjects/xyz.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    triangle.cpp \
    vertex.cpp

HEADERS += \
    GlobalConstants.h \
    MyMath/vector2d.h \
    MyMath/vector3d.h \
    MyMath/vector4d.h \
    Scenes/Scene.h \
    Scenes/Scene1.h \
    Scenes/scene2.h \
    StaticFunctions.h \
    VisualObjects/beziercurve.h \
    VisualObjects/grid.h \
    VisualObjects/light.h \
    VisualObjects/objfile.h \
    VisualObjects/octahedronball.h \
    VisualObjects/terrain.h \
    VisualObjects/visualobject.h \
    VisualObjects/xyz.h \
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
    plainvertex.vert
