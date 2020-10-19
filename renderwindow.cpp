#include "renderwindow.h"
#include <QTimer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <chrono>

#include <iostream>
#include "ECS/coreengine.h"
#include "mainwindow.h"
#include "inputsystem.h"


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
    auto fullInitStart = std::chrono::high_resolution_clock::now();

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

    if(FolderPath::PrintInfo == true)
    {
        qDebug() << "";
        qDebug() << "The active GPU and API: \n";
        qDebug() << "  Vendor: " << glGetString(GL_VENDOR);
        qDebug() << "  Renderer: " << glGetString(GL_RENDERER);
        qDebug() << "  Version: " << glGetString(GL_VERSION);

        qDebug() << "Window Size : " << width() << "x" << height();
    }

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:

    //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //checks if one plane is in front of the other
    glEnable(GL_DEPTH_TEST);

    //draws only front side of planes
    glEnable(GL_CULL_FACE);

    //gray color used in glClear GL_COLOR_BUFFER_BIT
    glClearColor(0.43f, 0.43f, 0.43f,1.0f);

    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto fullInitFinish = std::chrono::high_resolution_clock::now();

    if(FolderPath::PrintTimerInfo == true)
    {
        qDebug() << "Full initialize timer:"   << (fullInitFinish - fullInitStart).count()  << " seconds";
    }

    qDebug() << "(renderwindow) renderwindow initialized!";

    Engine::initialize(mMainWindow, this);
}

// Called each frame - doing the rendering
void RenderWindow::render()
{
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Engine::tick(mFPS);

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
            mFPS = nsecElapsed/1000000.f;
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

void RenderWindow::mouseReleaseEvent(QMouseEvent *event)
{
    InputSystem::mouseReleaseEvent(event);
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    InputSystem::mousePressEvent(event);
}

void RenderWindow::mouseMoveEvent(QMouseEvent *event)
{
    InputSystem::updateScreenSize(width(), height());
    InputSystem::mouseMoveEvent(event);
}

void RenderWindow::wheelEvent(QWheelEvent *event)
{
    InputSystem::wheelEvent(event);
}

void RenderWindow::keyReleaseEvent(QKeyEvent *event)
{
    InputSystem::keyReleaseEvent(event);
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    InputSystem::keyPressEvent(event);

    if (event->key() == Qt::Key_Escape) //Shuts down whole program
    {
        mMainWindow->close();
    }
}
