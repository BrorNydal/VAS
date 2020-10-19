#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "system.h"

#include <QKeyEvent>

/**
    All code written in InputSystem is currently hardcoded until I can find a better solution.
    I also have three variables which may be removed in the future as a system should only have functions.
    Thou currently all functions need these variables so it makes sence for now.
**/

class InputSystem : public System
{
private:

    //System is not supposed to have functions, but his has until a better solution comes up
    class EntityManager *mEntityManager;
    class InputComponent *mPlayerInputComponent;
    class CameraComponent *mCameraComponent;

public:
    InputSystem();

    static InputSystem &get();

    static void run();
    static void updateScreenSize(int x, int y);

    static void mouseReleaseEvent(QMouseEvent *event);
    static void mousePressEvent(QMouseEvent *event)  ;
    static void mouseMoveEvent(QMouseEvent *event)   ;
    static void wheelEvent(QWheelEvent *event)       ;
    static void keyPressEvent(QKeyEvent *event)      ;
    static void keyReleaseEvent(QKeyEvent *event)    ;

private:
    void run_impl();

    void mouseReleaseEvent_impl  (QMouseEvent *event);
    void mousePressEvent_impl    (QMouseEvent *event);
    void mouseMoveEvent_impl     (QMouseEvent *event);
    void wheelEvent_impl         (QWheelEvent *event);
    void keyPressEvent_impl      (QKeyEvent *event);
    void keyReleaseEvent_impl    (QKeyEvent *event);

    void cameraYaw(class CameraComponent &cam, float delta);
    void cameraPitch(class CameraComponent &cam, float delta);

    void anyEvent(class EntityManager &em, class InputComponent &playerInputComponent);
};

#endif // INPUTSYSTEM_H
