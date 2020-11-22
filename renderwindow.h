#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include "Scenes/Scene.h"

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

    unsigned int mSceneIndex = 0;
    std::vector<Scene*> mScenes;
    void nextScene();

    QTimer *mRenderTimer;           //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the calculated FPS
    clock_t mDeltaTimer;
    float mDeltaTime;
    float mLastFrame = 0.f;

    MainWindow *mMainWindow;        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void checkForGLerrors();

    void calculateFramerate();

    void startOpenGLDebugger();

    std::map<Qt::MouseButton, bool> mMouseInput;
    std::map<int, bool> mKeyInput;

    double mTPF;
protected:
    //The QWindow that we inherit from has these functions to capture
    //mouse and keyboard. Uncomment to use (you also have to make the definitions of these functions
    //
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
};

#endif // RENDERWINDOW_H
