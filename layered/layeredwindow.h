#ifndef LAYERED_LAYEREDWINDOW_H
#define LAYERED_LAYEREDWINDOW_H

#include <windows.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "layeredwindowinfo.h"
#include "layeredbitmap.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class LayeredWindow
{
public:
    LayeredWindow();
    ~LayeredWindow();

    // Register the window class and call methods for instantiating drawing resources
    HRESULT initialize();

    // Process and dispatch messages
    void runMessageLoop();

    bool isInitialized() const;

    // The windows procedure.
    static LRESULT CALLBACK WndProc(
            HWND hWnd,
            UINT message,
            WPARAM wParam,
            LPARAM lParam
            );

protected:
    HRESULT initWindow();
    HRESULT initGL();
    bool createOpenGLContext();
    void render();

protected:
    virtual void resizeGL(UINT width, UINT height);
    virtual void onInit();
    virtual void onRender();
    virtual void onHotkey(DWORD lParam);
    virtual void onQuit();
    virtual void onResize(int width, int height);
    virtual void onMove(int x, int y);
    virtual void onTimer(UINT timerId);

private:
    bool initOpenGLExtensions();
    bool initPBuffer();
    bool resizePBuffer(UINT width, UINT height);
    void releasePBuffer();

protected:
    HWND hWnd;
    HDC hDC;
    RECT windowPos;
    LayeredWindowInfo *m_info;
    LayeredBitmap *m_bitmap;
    bool initialized;
    int pixelformat;

    glm::mat4 projectionMatrix;
    glm::mat4 modelViewMatrix;

private:

};

#endif // LAYERED_LAYEREDWINDOW_H

