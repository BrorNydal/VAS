#include "inputsystem.h"

#include "ECS/coreengine.h"
#include "physicsengine.h"

#include "vec4.h"

InputSystem::InputSystem()
{
    mEntityManager = Engine::getEditorWorld()->getEntityManager();

    //get player inputcomponent
    mPlayerInputComponent = &mEntityManager->getPlayerInputComponent();

    mCameraComponent = &Engine::EditorCamera();
}

InputSystem &InputSystem::get()
{
    static InputSystem *mInstance = new InputSystem();
    return *mInstance;
}

void InputSystem::run()
{
    get().run_impl();
}

void InputSystem::updateScreenSize(int x, int y)
{
    get().mCameraComponent->screenSize = QVector2D(x,y);
}

void InputSystem::mouseReleaseEvent(QMouseEvent *event)
{
    get().mouseReleaseEvent_impl(event);
}

void InputSystem::mousePressEvent(QMouseEvent *event)
{
    get().mousePressEvent_impl(event);
}

void InputSystem::mouseMoveEvent(QMouseEvent *event)
{
    get().mouseMoveEvent_impl(event);
}

void InputSystem::wheelEvent(QWheelEvent *event)
{
    get().wheelEvent_impl(event);
}

void InputSystem::keyPressEvent(QKeyEvent *event)
{
    get().keyPressEvent_impl(event);
}

void InputSystem::keyReleaseEvent(QKeyEvent *event)
{
    get().keyReleaseEvent_impl(event);
}

void InputSystem::run_impl()
{
    QVector2D cursorPosition = mPlayerInputComponent->cursorPosition;
    QVector2D screenSize = mCameraComponent->screenSize;

    QVector2D cursorNormal = QVector2D((cursorPosition.x() / (screenSize.x() / 2.f)) - 1.f , ((cursorPosition.y() / (screenSize.y() / 2.f)) -1.f) * -1.f);

    bool invertable;
    QMatrix4x4 proInvert = mCameraComponent->projectionMatrix.inverted(&invertable);
    QMatrix4x4 viewInvert = mCameraComponent->viewMatrix.inverted();

    if(invertable == false)
        qDebug() << "pro not invertable";

    QVector4D rayStart(cursorNormal.x(), cursorNormal.y(), -1.f, 1.f);

    QVector4D rayEye = proInvert * rayStart;
    rayEye.setZ(-1.f);
    rayEye.setW(0.f);

    QVector4D rayWorld4d = viewInvert * rayEye;
    QVector3D rayWorld = QVector3D(rayWorld4d.x(), rayWorld4d.y(), rayWorld4d.z());

    mPlayerInputComponent->cursorRayDirection = rayWorld.normalized();


    //rotate if any mousebutton is clicked
    //just a temporary functionality as it's a little buggy
    if(mPlayerInputComponent->mouse[Qt::MouseButton::RightButton] == true)
    {
        cameraYaw(*mCameraComponent, mPlayerInputComponent->cursorPositionDelta.x());
        cameraPitch(*mCameraComponent, mPlayerInputComponent->cursorPositionDelta.y());
    }

    if(mPlayerInputComponent->keyboard[Qt::Key_W] == true)
    {
        mCameraComponent->worldPosition -= mCameraComponent->forward;
    }
    if(mPlayerInputComponent->keyboard[Qt::Key_S] == true)
    {
        mCameraComponent->worldPosition += mCameraComponent->forward;
    }
    if(mPlayerInputComponent->keyboard[Qt::Key_A] == true)
    {
        mCameraComponent->worldPosition += mCameraComponent->right  ;
    }
    if(mPlayerInputComponent->keyboard[Qt::Key_D] == true)
    {
        mCameraComponent->worldPosition -= mCameraComponent->right  ;
    }
    if(mPlayerInputComponent->keyboard[Qt::Key_Shift] == true)
    {
        mCameraComponent->worldPosition += mCameraComponent->up  ;
    }
    if(mPlayerInputComponent->keyboard[Qt::Key_Control] == true)
    {
        mCameraComponent->worldPosition -= mCameraComponent->up  ;
    }
}

void InputSystem::mouseReleaseEvent_impl(QMouseEvent *event)
{
    mPlayerInputComponent->mouse[event->button()] = false;
}

void InputSystem::mousePressEvent_impl(QMouseEvent *event)
{
    mPlayerInputComponent->mouse[event->button()] = true;

    //gui should say what the click event on gui should be, dont know how to implement it quite yet...

    if(mPlayerInputComponent->mouse[Qt::MouseButton::LeftButton] == true)
    {
        QVector2D cursorPosition = mPlayerInputComponent->cursorPosition;
        QVector2D screenSize = mCameraComponent->screenSize;

        bool anyHit = false;
        for(unsigned int i = 0; i < mEntityManager->getEntities().size(); i++)
        {
            if(mEntityManager->getEntities().at(i).getComponent(EComponentType::collider) != nullptr)
            {
                if(static_cast<CollisionComponent*>(mEntityManager->getEntities().at(i).getComponent(EComponentType::collider))->colliderType == EColliderType::sphere)
                {
                    bool hit =
                    PhysicsEngine::lineToSphere(mCameraComponent->finalPosition,
                                                                        mPlayerInputComponent->cursorRayDirection,
                                                                        *static_cast<SphereCollider*>(mEntityManager->getEntities().at(i).getComponent(EComponentType::collider)),
                                                                        *static_cast<TransformComponent*>(mEntityManager->getEntities().at(i).getComponent(EComponentType::transform)));

                    if(hit == true)
                    {
                        anyHit = true;
                        Engine::selectEntity(&mEntityManager->getEntities().at(i));
                        qDebug() << "Hit!";
                    }
                }
            }

            if(anyHit == false)
                Engine::selectEntity(nullptr);
        }

        QVector2D pos;
        QVector2D scale;

//        for(auto map = mEntityManager->getGuiMap().begin(); map != mEntityManager->getGuiMap().end(); map++)
//        {
//            GuiComponent &gui = ResourceManager::getGui(map->second);

//            pos = gui.position;
//            scale = gui.scale;

//            pos.setX( (((pos.x() + 1) / 2.f) * screenSize.x()) );     //Get value between 0 and 1, then multiply with screensize for screen location
//            pos.setY( (((1.f - ((pos.y() + 1.f) / 2.f)) * screenSize.y())) );     //Get value between 0 and 1 (reversed), then multiply with screensize for screen location

//            scale.x = ((scale.x/2.f) * screenSize.x);
//            scale.y = ((scale.y/2.f) * screenSize.y);

//            if((event->windowPos().x() <= pos.x + scale.x && event->windowPos().x() >= pos.x - scale.x) &&
//               (event->windowPos().y() <= pos.y + scale.y && event->windowPos().y() >= pos.y - scale.y))
//            {
//                Engine::getWorld()->spawnObject("Cube");
//                std::cout << "CUBE SPAWN! " << std::endl;
//            }
//        }
    }
}

void InputSystem::mouseMoveEvent_impl(QMouseEvent *event)
{
    //get how much the cursor has moved since last frame
    mPlayerInputComponent->cursorPositionDelta = QVector2D(event->x() - mPlayerInputComponent->cursorPosition.x(), event->y() - mPlayerInputComponent->cursorPosition.y());

    //set cursor position
    mPlayerInputComponent->cursorPosition = QVector2D(event->x(), event->y());
}

void InputSystem::wheelEvent_impl(QWheelEvent *event)
{
    if(event->delta() > 0 && mCameraComponent->offset > mCameraComponent->minCameraOffset)
    {
        mCameraComponent->offset -= event->delta() * mCameraComponent->cameraZoomScale;
    }
    else if(event->delta() < 0 && mCameraComponent->offset < mCameraComponent->maxCameraOffset)
    {
        mCameraComponent->offset -= event->delta() * mCameraComponent->cameraZoomScale;
    }
}

void InputSystem::keyPressEvent_impl(QKeyEvent *event)
{
    mPlayerInputComponent->keyboard[event->key()] = true;

    if(mPlayerInputComponent->keyboard[Qt::Key_E] == true)
        Engine::getEditorWorld()->spawnObject("Enemy.obj");
}

void InputSystem::keyReleaseEvent_impl(QKeyEvent *event)
{
    mPlayerInputComponent->keyboard[event->key()] = false;
}

void InputSystem::cameraYaw(CameraComponent &cam, float delta)
{
    cam.relativePosition = cam.relativePosition + (cam.right * delta);
}

void InputSystem::cameraPitch(CameraComponent &cam, float delta)
{
    cam.relativePosition = cam.relativePosition + (cam.up * delta);
}

void InputSystem::anyEvent(EntityManager &em, InputComponent &playerInputComponent)
{
    //em = Engine::getWorld()->getEntityManager();
}
