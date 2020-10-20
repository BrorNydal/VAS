QT          += core gui widgets

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    MyMath/matrix4x4.cpp \
    MyMath/mymathfunctions.cpp \
    MyMath/vector2d.cpp \
    MyMath/vector3d.cpp \
    MyMath/vector4d.cpp \
    Scenes/Scene.cpp \
    Scenes/Scene1.cpp \
    Scenes/scene2.cpp \
    VisualObjects/beziercurve.cpp \
    VisualObjects/contourline.cpp \
    VisualObjects/grid.cpp \
    VisualObjects/indexedtrianglesurface.cpp \
    VisualObjects/light.cpp \
    VisualObjects/linedraw.cpp \
    VisualObjects/octahedronball.cpp \
    VisualObjects/trianglesurface.cpp \
    VisualObjects/xyz.cpp \
    renderwindow.cpp \
    shader.cpp \
    mainwindow.cpp \
    triangle.cpp \
    vertex.cpp

HEADERS += \
    MyMath/matrix4x4.h \
    MyMath/mymathfunctions.h \
    MyMath/vector2d.h \
    MyMath/vector3d.h \
    MyMath/vector4d.h \
    Scenes/Scene.h \
    Scenes/Scene1.h \
    Scenes/scene2.h \
    VisualObjects/beziercurve.h \
    VisualObjects/contourline.h \
    VisualObjects/grid.h \
    VisualObjects/indexedtrianglesurface.h \
    VisualObjects/light.h \
    VisualObjects/linedraw.h \
    VisualObjects/octahedronball.h \
    VisualObjects/trianglesurface.h \
    VisualObjects/visualobject.h \
    VisualObjects/xyz.h \
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
    plainvertex.vert
