#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

class QOpenGLContext;
class Shader;
class MainWindow;

// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;


    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;

private slots:
    void render();

private:
    void init();

    QOpenGLContext *mContext;
    bool mInitialized;

    float mFPS = 0.f;
    bool mWireFrame = false; //check old project for how to do this

    QTimer *mRenderTimer;           //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS



    MainWindow *mMainWindow;        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

protected:
    //The QWindow that we inherit from has these functions to capture
    //mouse and keyboard. Uncomment to use (you also have to make the definitions of these functions

    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

};

#endif // RENDERWINDOW_H