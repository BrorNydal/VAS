#include "renderwindow.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include <iostream>
#include "scene1.h"
#include "scene2.h"
#include "scene3.h"
#include "shader.h"
#include "mainwindow.h"

/*
 * Scene1 lager første scenen du ser og består av oppgave 5.2.10
 * Scene2 lager andre scenen du ser og består av oppgave 5.2.11
 * Du kan bytte scene ved å trykke "Space"
 * Roter kamera ved tastene 1 til 4, x og z
 *
 *
 *
 *
 */

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

RenderWindow::~RenderWindow()
{

}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Print render version info (what GPU is used):
    //(Have to use cout to see text- qDebug just writes numbers...)
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    std::cout << "The active GPU and API: \n";
    std::cout << "  Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "  Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "  Version: " << glGetString(GL_VERSION) << std::endl;

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f,1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mScenes.push_back(new Scene1());
    mScenes.push_back(new Scene2());
    mScenes.push_back(new Scene3());
}

void RenderWindow::nextScene()
{
    mSceneIndex++;

    if(mSceneIndex >= mScenes.size())
        mSceneIndex=0;

    if(mScenes[mSceneIndex]->hasInitialized() == false)
        mScenes[mSceneIndex]->initializeScene();

}

// Called each frame - doing the rendering
void RenderWindow::render()
{
    mLastFrame = mDeltaTimer;
    mDeltaTimer = clock();
    //qDebug() << "dT" << mDeltaTimer;
    mDeltaTime = (mDeltaTimer - mLastFrame) / CLOCKS_PER_SEC;

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)


    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //qDebug() << "deltatime :" << mDeltaTime;

    float deltaX = 0.f;
    float deltaY = 0.f;
    float deltaZ = 0.f;

    if(mKeyInput[Qt::Key_D])
        deltaX = 1.f;
    else if(mKeyInput[Qt::Key_A])
        deltaX = -1.f;
    if(mKeyInput[Qt::Key_W])
        deltaY = 1.f;
    else if(mKeyInput[Qt::Key_S])
        deltaY = -1.f;
    if(mKeyInput[Qt::Key_Shift])
        deltaZ = 1.f;
    else if(mKeyInput[Qt::Key_Control])
        deltaZ = -1.f;

    mScenes[mSceneIndex]->getCamera().move(QVector3D(deltaX, deltaY, deltaZ));
    mScenes[mSceneIndex]->draw(1/120.f);

    calculateFramerate();

    //using our expanded OpenGL debugger to check if everything is OK.
    checkForGLerrors();

    mContext->swapBuffers(this);
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//The way this is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            mTPF = nsecElapsed/1000000000.f;
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            qDebug() << "This system can not use QOpenGLDebugLogger, so we revert to glGetError()";

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }
    }
}

void RenderWindow::mousePressEvent(QMouseEvent *event){
    mMouseInput[event->button()] = true;
}

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    mMouseInput[event->button()] = false;
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event){

    Camera &cam = mScenes[mSceneIndex]->getCamera();

    float deltaX = event->x() - cam.getCursorLocation().x();
    float deltaY = event->y() - cam.getCursorLocation().y();
    cam.setCursorDelta(QVector2D(deltaX, deltaY));

    cam.setCursorLocation(QVector2D(event->x(), event->y()));

    if(mMouseInput[Qt::MouseButton::RightButton] == true)
    {
        cam.yaw(deltaX);
        cam.pitch(deltaY);
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    mKeyInput[event->key()] = true;

    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        mMainWindow->close();
    }
    if(event->key() == Qt::Key_Space)
    {
        nextScene();
    }
    if(event->key() == Qt::Key_M)
    {
//        if(mScenes[mSceneIndex]->mTurnTable == false)
//            mScenes[mSceneIndex]->mTurnTable = true;
//        else
//            mScenes[mSceneIndex]->mTurnTable = false;
    }
//    if(event->key() == Qt::Key_2)
//    {
//        mScenes[mSceneIndex]->mViewMatrix.rotate(1.f, QVector3D(0.f, 0.f, 1.f));
//    }
//    if(event->key() == Qt::Key_1)
//    {
//        mScenes[mSceneIndex]->mViewMatrix.rotate(-1.f, QVector3D(0.f, 0.f, 1.f));
//    }
//    if(event->key() == Qt::Key_3)
//    {
//        mScenes[mSceneIndex]->mViewMatrix.rotate(1.f, QVector3D(0.f, 1.f, 0.f));
//    }
//    if(event->key() == Qt::Key_4)
//    {
//        mScenes[mSceneIndex]->mViewMatrix.rotate(-1.f, QVector3D(0.f, 1.f, 0.f));
//    }
//    if(event->key() == Qt::Key_Z)
//    {
//        mScenes[mSceneIndex]->mViewMatrix.rotate(1.f, QVector3D(1.f, 0.f, 0.f));
//    }
//    if(event->key() == Qt::Key_X)
//    {
//        mScenes[mSceneIndex]->mViewMatrix.rotate(-1.f, QVector3D(1.f, 0.f, 0.f));
//    }
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event){
    mKeyInput[event->key()] = false;
}
