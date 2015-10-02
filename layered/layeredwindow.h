#ifndef LAYERED_LAYEREDWINDOW_H
#define LAYERED_LAYEREDWINDOW_H

#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/wglext.h>

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

protected:
    virtual void resizeGL(UINT width, UINT height);
    virtual void onInit();
    virtual void onRender();
    virtual void onHotkey(DWORD lParam);
    virtual void onQuit();
    virtual void onResize(UINT x, UINT y, UINT width, UINT height);
    virtual void onTimer(UINT timerId);

private:
    bool initOpenGLExtensions();
    bool initPBuffer();
    bool updatePBuffer(UINT width, UINT height);
    void releasePBuffer();

    void render();

protected:
    HWND hWnd;
    HDC hDC;
    HDC g_hPBufferDC;
    HGLRC g_hPBufferRC;
    HPBUFFERARB g_hPBuffer;
    RECT windowPos;
    LayeredWindowInfo *m_info;
    LayeredBitmap *m_bitmap;
    bool initialized;
    int pixelformat;

private:
    PFNWGLDESTROYPBUFFERARBPROC     wglDestroyPbufferARB;
    PFNWGLQUERYPBUFFERARBPROC       wglQueryPbufferARB;
    PFNWGLGETPBUFFERDCARBPROC       wglGetPbufferDCARB;
    PFNWGLCREATEPBUFFERARBPROC      wglCreatePbufferARB;
    PFNWGLRELEASEPBUFFERDCARBPROC   wglReleasePbufferDCARB;

    PFNWGLCHOOSEPIXELFORMATARBPROC          wglChoosePixelFormatARB;
    PFNWGLGETPIXELFORMATATTRIBFVARBPROC     wglGetPixelFormatAttribfvARB;
    PFNWGLGETPIXELFORMATATTRIBIVARBPROC     wglGetPixelFormatAttribivARB;
};

#endif // LAYERED_LAYEREDWINDOW_H

