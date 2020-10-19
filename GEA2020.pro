QT          += core gui widgets
QT          += core qml

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

mac {
    LIBS += -framework OpenAL
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

INCLUDEPATH += \
    ./ECS \
    ./ECS/Systems \
    ./shaders \
    ./Scenes \
    ./Drawing \
    ./Drawing/Cameras \
    ./VisualObjects \
    ./MyMath \
    ./import \
    ./Vendor \
    ./Vendor/stb_image \
    ./worlds/ \
    #./include \

#win32 {
    #Hvor header filen til bibloteket ligger
#    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    #hvor bibiloteket ligger
#    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
#}


SOURCES += main.cpp \
    Drawing/vertex.cpp \
    ECS/Systems/guisystem.cpp \
    ECS/Systems/inputsystem.cpp \
    ECS/Systems/rendersystem.cpp \
    ECS/Systems/system.cpp \
    ECS/coreengine.cpp \
    ECS/entity.cpp \
    ECS/physicsengine.cpp \
    import/obj.cpp \
    import/terrain.cpp \
    import/texture.cpp \
    import/wave.cpp \
    MyMath/mat4x4.cpp \
    MyMath/vec2.cpp \
    MyMath/vec3.cpp \
    MyMath/vec4.cpp \
    Vendor/stb_image/stb_image.cpp \
    entitymanager.cpp \
    renderwindow.cpp \
    resourcemanager.cpp \
    shader.cpp \
    mainwindow.cpp \
    soundmanager.cpp \
    staticfunctions.cpp \
    wavefilehandler.cpp \
    worlds/dynamicworld.cpp \
    worlds/editorworld.cpp \
    worlds/simulationworld.cpp \
    worlds/world.cpp

HEADERS += \
    Drawing/vertex.h \
    ECS/Systems/guisystem.h \
    ECS/Systems/inputsystem.h \
    ECS/Systems/rendersystem.h \
    ECS/Systems/system.h \
    ECS/components.h \
    ECS/components.h \
    ECS/components.h \
    ECS/coreengine.h \
    ECS/entity.h \
    ECS/physicsengine.h \
    import/obj.h \
    import/terrain.h \
    import/texture.h \
    import/wave.h \
    MyMath/mat4x4.h \
    MyMath/vec2.h \
    MyMath/vec3.h \
    MyMath/vec4.h \
    Vendor/Audio3D/al.h \
    Vendor/Audio3D/alc.h \
    Vendor/Audio3D/alext.h \
    Vendor/Audio3D/efx-creative.h \
    Vendor/Audio3D/efx-presets.h \
    Vendor/Audio3D/efx.h \
    Vendor/stb_image/stb_image.h \
    entitymanager.h \
    enumclasses.h \
    globalconstants.h \
    renderwindow.h \
    resourcemanager.h \
    shader.h \
    mainwindow.h \
    soundmanager.h \
    staticfunctions.h \
    structs.h \
    wavefilehandler.h \
    worlds/dynamicworld.h \
    worlds/editorworld.h \
    worlds/simulationworld.h \
    worlds/world.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    Shaders/guifragment.frag \
    Shaders/guivertex.vert \
    Shaders/phongfragment.frag \
    Shaders/phongvertex.vert \
    Shaders/plainfragment.frag \
    Shaders/plainvertex.vert \ \
    Shaders/skyboxfragment.frag \
    Shaders/skyboxvertex.vert \
    Shaders/texturefragment.frag \
    Shaders/texturevertex.vert
